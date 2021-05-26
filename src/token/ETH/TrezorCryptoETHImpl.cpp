#include "token/ETH/TrezorCryptoETHImpl.h"
#include <Ethereum/Signer.h>
#include "HDKey/HDKey.hpp"

namespace jub {
namespace token {


JUB_RV TrezorCryptoETHImpl::SelectApplet() {

    return JUBR_OK;
}


JUB_RV TrezorCryptoETHImpl::GetAppletVersion(stVersion& version) {

    return JUBR_OK;
}


JUB_RV TrezorCryptoETHImpl::GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) {

    //tag used by hardware,this imp not use.
    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint));

    uchar_vector vPublicKey(hdkey.public_key, sizeof(hdkey.public_key)/sizeof(uint8_t));
    TW::Data publicKey(vPublicKey);

    return _getAddress(publicKey, address);
}


JUB_RV TrezorCryptoETHImpl::GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) {

    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
     JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint));

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


JUB_RV TrezorCryptoETHImpl::SignTx(const std::vector<JUB_BYTE>& vNonce,
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
    JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint));

    TW::Ethereum::Transaction tx(vNonce,
                                 vGasPrice,
                                 vGasLimit,
                                 TW::Ethereum::Address(vTo),
                                 vValue,
                                 vInput);
    TW::Ethereum::Signer signer(vChainID);
    signer.sign(TW::PrivateKey(TW::Data(uchar_vector(hdkey.private_key, TW::PrivateKey::size))),
                tx);

    vRaw = TW::Ethereum::RLP::encode(tx);

    return JUBR_OK;
}


JUB_RV TrezorCryptoETHImpl::SignTX(const int erc,
                                   const std::vector<JUB_BYTE>& vNonce,
                                   const std::vector<JUB_BYTE>& vGasPrice,
                                   const std::vector<JUB_BYTE>& vGasLimit,
                                   const std::vector<JUB_BYTE>& vTo,
                                   const std::vector<JUB_BYTE>& vValue,
                                   const std::vector<JUB_BYTE>& vInput,
                                   const std::vector<JUB_BYTE>& vPath,
                                   const std::vector<JUB_BYTE>& vChainID,
                                   std::vector<JUB_BYTE>& vRaw) {

    return SignTx(vNonce,
                  vGasPrice,
                  vGasLimit,
                  vTo,
                  vValue,
                  vInput,
                  vPath,
                  vChainID,
                  vRaw);
}


JUB_RV TrezorCryptoETHImpl::VerifyTX(const std::vector<JUB_BYTE>& vChainID,
                                     const std::string& path,
                                     const std::vector<JUB_BYTE>& vSigedTrans) {

    uint32_t hdVersionPub = TWCoinType2HDVersionPublic(_coin);
    uint32_t hdVersionPrv = TWCoinType2HDVersionPrivate(_coin);

    std::string xpub;
    JUB_VERIFY_RV(GetHDNode((JUB_BYTE)JUB_ENUM_PUB_FORMAT::XPUB, path, xpub));

    TW::Data publicKey;
    JUB_VERIFY_RV(_getPubkeyFromXpub(xpub, publicKey,
                                     hdVersionPub, hdVersionPrv));

    // verify signature
    return VerifyTx(vChainID,
                    vSigedTrans,
                    publicKey);
}


JUB_RV TrezorCryptoETHImpl::SetERC20ETHToken(const std::string& tokenName,
                                             const JUB_UINT16 unitDP,
                                             const std::string& contractAddress) {

    return JUBR_OK;
}


JUB_RV TrezorCryptoETHImpl::SetERC721ETHToken(const std::string& tokenName,
                                              const std::string& contractAddress) {

    return JUBR_OK;
}


JUB_RV TrezorCryptoETHImpl::SignContract(const JUB_BYTE inputType,
                                         const std::vector<JUB_BYTE>& vNonce,
                                         const std::vector<JUB_BYTE>& vGasPrice,
                                         const std::vector<JUB_BYTE>& vGasLimit,
                                         const std::vector<JUB_BYTE>& vTo,
                                         const std::vector<JUB_BYTE>& vValue,
                                         const std::vector<JUB_BYTE>& vInput,
                                         const std::vector<JUB_BYTE>& vPath,
                                         const std::vector<JUB_BYTE>& vChainID,
                                         std::vector<JUB_BYTE>& vRaw) {

    return SignTx(vNonce,
                  vGasPrice,
                  vGasLimit,
                  vTo,
                  vValue,
                  vInput,
                  vPath,
                  vChainID,
                  vRaw);
}


JUB_RV TrezorCryptoETHImpl::SignBytestring(const std::vector<JUB_BYTE>& vTypedData,
                                           const std::vector<JUB_BYTE>& vPath,
                                           const std::vector<JUB_BYTE>& vChainID,
                                           std::vector<JUB_BYTE>& signatureRaw) {

    HDNode hdkey;
    JUB_UINT32 parentFingerprint;
    std::string path(&vPath[0], &vPath[0] + vPath.size());
    JUB_VERIFY_RV(_HdnodeCkd(path, &hdkey, &parentFingerprint));

    TW::Ethereum::Signer signer(vChainID);
    signer.sign(TW::PrivateKey(TW::Data(uchar_vector(hdkey.private_key, TW::PrivateKey::size))),
                vTypedData, signatureRaw);

    return JUBR_OK;
}


JUB_RV TrezorCryptoETHImpl::VerifyBytestring(const std::vector<JUB_BYTE>& vChainID,
                                             const std::string& path,
                                             const std::vector<JUB_BYTE>& vTypedData,
                                             const std::vector<JUB_BYTE>& vSignature) {

    uint32_t hdVersionPub = TWCoinType2HDVersionPublic(_coin);
    uint32_t hdVersionPrv = TWCoinType2HDVersionPrivate(_coin);

    std::string xpub;
    JUB_VERIFY_RV(GetHDNode((JUB_BYTE)JUB_ENUM_PUB_FORMAT::XPUB, path, xpub));

    TW::Data publicKey;
    JUB_VERIFY_RV(_getPubkeyFromXpub(xpub, publicKey,
                                     hdVersionPub, hdVersionPrv));

    // verify signature
    return JubiterBaseETHImpl::VerifyBytestring(vChainID,
                                                vTypedData,
                                                vSignature,
                                                publicKey);
}


} // namespace token end
} // namespace jub end
