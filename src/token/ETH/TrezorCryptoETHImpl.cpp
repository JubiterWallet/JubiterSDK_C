#include <token/ETH/TrezorCryptoETHImpl.h>

#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/curves.h>
#include <TrezorCrypto/sha3.h>

#include <libETH/RLP.h>
#include <libETH/AddressChecksum.h>

#include <HDKey/HDKey.hpp>
#include <utility/util.h>

namespace jub {
namespace token {


JUB_RV TrezorCryptoETHImpl::SelectApplet() {

    return JUBR_OK;
}


JUB_RV TrezorCryptoETHImpl::GetAppletVersion(std::string& version) {

    return JUBR_OK;
}


JUB_RV TrezorCryptoETHImpl::GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) {

    //tag used by hardware,this imp not use.
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, SECP256K1_NAME, TWHDVersion::TWHDVersionXPUB, TWHDVersion::TWHDVersionXPRV, &hdkey, &parentFingerprint));

    JUB_BYTE ethKeyHash[20] = { 0, };
    if (1 == hdnode_get_ethereum_pubkeyhash(&hdkey, ethKeyHash)) {
        uchar_vector _address(ethKeyHash, ethKeyHash + 20);
        address = jub::eth::checksumed(_address.getHex(), jub::eth::eip55);
        return JUBR_OK;
    }

    return JUBR_ERROR;
}


JUB_RV TrezorCryptoETHImpl::GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) {

    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, SECP256K1_NAME, TWHDVersion::TWHDVersionXPUB, TWHDVersion::TWHDVersionXPRV, &hdkey, &parentFingerprint));

    //    typedef enum {
    //        HEX = 0x00,
    //        XPUB = 0x01
    //    } JUB_ENUM_PUB_FORMAT;
    if (0x00 == format) {//hex
        uchar_vector pk(hdkey.public_key, hdkey.public_key + 33);
        pubkey = pk.getHex();
        pubkey = ETH_PRDFIX + pubkey;
    }
    else { //xpub
        JUB_CHAR _pk[200] = { 0, };
        if (0 == hdnode_serialize_public(&hdkey, parentFingerprint, TWHDVersion::TWHDVersionXPUB, _pk, sizeof(_pk) / sizeof(JUB_CHAR))) {
            return JUBR_ERROR;
        }
        pubkey = std::string(_pk);
    }

    return JUBR_OK;
}


JUB_RV TrezorCryptoETHImpl::SignTX(const bool bERC20,
                                   const std::vector<JUB_BYTE>& vNonce,
                                   const std::vector<JUB_BYTE>& vGasPrice,
                                   const std::vector<JUB_BYTE>& vGasLimit,
                                   const std::vector<JUB_BYTE>& vTo,
                                   const std::vector<JUB_BYTE>& vValue,
                                   const std::vector<JUB_BYTE>& vData,
                                   const std::vector<JUB_BYTE>& vPath,
                                   const std::vector<JUB_BYTE>& vChainID,
                                   std::vector<JUB_BYTE>& vRaw) {

    auto encoded = abcd::DataChunk();
    abcd::append(encoded, jub::eth::RLP::encode(vNonce));
    abcd::append(encoded, jub::eth::RLP::encode(vGasPrice));
    abcd::append(encoded, jub::eth::RLP::encode(vGasLimit));
    abcd::append(encoded, jub::eth::RLP::encode(vTo));
    abcd::append(encoded, jub::eth::RLP::encode(vValue));
    abcd::append(encoded, jub::eth::RLP::encode(vData));
    abcd::append(encoded, jub::eth::RLP::encode(vChainID));
    abcd::append(encoded, jub::eth::RLP::encode(abcd::DataChunk{ 0 }));
    abcd::append(encoded, jub::eth::RLP::encode(abcd::DataChunk{ 0 }));
    encoded = jub::eth::RLP::encodeList(encoded);

    uchar_vector rlps = encoded;
    std::string rlprlp = rlps.getHex();

    JUB_BYTE digest[32] = { 0, };
    keccak_256(&encoded[0], encoded.size(), digest);

    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    std::string path(&vPath[0], &vPath[0] + vPath.size());
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, SECP256K1_NAME, TWHDVersion::TWHDVersionXPUB, TWHDVersion::TWHDVersionXPRV, &hdkey, &parentFingerprint));

    JUB_BYTE sig[64] = { 0, };
    JUB_BYTE by = 0;
    if (0 != hdnode_sign_digest(&hdkey, digest, sig, &by, nullptr)) {
        return JUBR_ERROR;
    }
    abcd::DataChunk r(sig, sig + 32);
    abcd::DataChunk s(sig + 32, sig + 64);;
    abcd::DataChunk v(1, vChainID[0]*2 + 35 + by);

    abcd::append(vRaw, jub::eth::RLP::encode(vNonce));
    abcd::append(vRaw, jub::eth::RLP::encode(vGasPrice));
    abcd::append(vRaw, jub::eth::RLP::encode(vGasLimit));
    abcd::append(vRaw, jub::eth::RLP::encode(vTo));
    abcd::append(vRaw, jub::eth::RLP::encode(vValue));
    abcd::append(vRaw, jub::eth::RLP::encode(vData));
    abcd::append(vRaw, jub::eth::RLP::encode(v));
    abcd::append(vRaw, jub::eth::RLP::encode(r));
    abcd::append(vRaw, jub::eth::RLP::encode(s));
    vRaw = jub::eth::RLP::encodeList(vRaw);

    return JUBR_OK;
}


JUB_RV TrezorCryptoETHImpl::SetERC20ETHToken(const std::string& tokenName,
                                             const JUB_UINT16 unitDP,
                                             const std::string& contractAddress) {

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
