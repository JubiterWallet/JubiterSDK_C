#include <token/ETH/TrezorCryptoETHImpl.h>

#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/curves.h>
#include <TrezorCrypto/sha3.h>

#include <Ethereum/RLP.h>
#include <Ethereum/Transaction.h>
#include <Ethereum/Signer.h>

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
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, _curve_name, TWHDVersion::TWHDVersionXPUB, TWHDVersion::TWHDVersionXPRV, &hdkey, &parentFingerprint));

    uchar_vector vPublicKey(hdkey.public_key, sizeof(hdkey.public_key)/sizeof(uint8_t));
    TW::Data publicKey(vPublicKey);

    return _getAddress(publicKey, address);
}


JUB_RV TrezorCryptoETHImpl::GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) {

    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, _curve_name, TWHDVersion::TWHDVersionXPUB, TWHDVersion::TWHDVersionXPRV, &hdkey, &parentFingerprint));

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
                                   const std::vector<JUB_BYTE>& vInput,
                                   const std::vector<JUB_BYTE>& vPath,
                                   const std::vector<JUB_BYTE>& vChainID,
                                   std::vector<JUB_BYTE>& vRaw) {

    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    std::string path(&vPath[0], &vPath[0] + vPath.size());
    JUB_VERIFY_RV(hdnode_priv_ckd(_MasterKey_XPRV, path, _curve_name, TWHDVersion::TWHDVersionXPUB, TWHDVersion::TWHDVersionXPRV, &hdkey, &parentFingerprint));

    TW::Ethereum::Transaction tx(vNonce,
                                 vGasPrice,
                                 vGasLimit,
                                 TW::Ethereum::Address(vTo),
                                 vValue,
                                 vInput);
    TW::Ethereum::Signer signer(vChainID);
    signer.sign(TW::PrivateKey(TW::Data(uchar_vector(hdkey.private_key, TW::PrivateKey::size))),
                tx);

    if (!signer.verify(TW::PublicKey(TW::Data(uchar_vector(hdkey.public_key, TW::PublicKey::secp256k1Size)), TWPublicKeyType::TWPublicKeyTypeSECP256k1),
                       tx)) {
        return JUBR_ERROR;
    }

    vRaw = TW::Ethereum::RLP::encode(tx);

    return JUBR_OK;
}


JUB_RV TrezorCryptoETHImpl::SetERC20ETHToken(const std::string& tokenName,
                                             const JUB_UINT16 unitDP,
                                             const std::string& contractAddress) {

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
