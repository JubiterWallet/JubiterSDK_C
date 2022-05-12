#include "JUB_SDK_COMM.h"
#include "token/ETH/JubiterLiteETHImpl.h"
#include <TrezorCrypto/bip32.h>
#include <Ethereum/Signer.h>

namespace jub {
namespace token {


#define SWITCH_TO_ETH_APP                       \
do {				                            \
    JUB_VERIFY_RV(_SelectApp(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE)));\
} while (0);                                    \


JUB_RV JubiterLiteETHImpl::SelectApplet() {

    SWITCH_TO_ETH_APP;
    return JUBR_OK;
}


JUB_RV JubiterLiteETHImpl::GetAppletVersion(stVersion& version) {

    return JubiterLiteImpl::GetAppletVersion(version);
}


JUB_RV JubiterLiteETHImpl::GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) {

    TW::Data publicKey;
    JUB_VERIFY_RV(JubiterLiteImpl::GetCompPubKey(_getSignType(_curve_name),
                                                 (JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX,
                                                 path,
                                                 publicKey));

    return _getAddress(publicKey, address);
}


JUB_RV JubiterLiteETHImpl::GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) {

    //path = "m/44'/60'/0'";
    std::string btcXpub;
    JUB_VERIFY_RV(JubiterLiteImpl::GetHDNode(_getSignType(_curve_name), 0x00, path, btcXpub));

//    typedef enum {
//        HEX = 0x00,
//        XPUB = 0x01
//    } JUB_ENUM_PUB_FORMAT;
    if (0x00 == format) {//hex
        TW::Data publicKey;
        JUB_VERIFY_RV(_getPubkeyFromXpub(btcXpub, publicKey));

        uchar_vector vPublicKey(publicKey);
        pubkey = vPublicKey.getHex();
        pubkey = ETH_PRDFIX + pubkey;
    }
    else { //xpub
        pubkey = btcXpub;
    }

    return JUBR_OK;
}


JUB_RV JubiterLiteETHImpl::_encodeRSV(const std::vector<JUB_BYTE>& vRSV,
                                      const std::vector<JUB_BYTE>& vChainID,
                                      TW::Data& r, TW::Data& s, TW::Data& v) {
    TW::Ethereum::Signature signature = TW::Ethereum::Signer::signatureDataToStruct(vRSV,
                                                                                    false, // TW::Ethereum::TransactionNonTyped::usesReplayProtection() is handled by the hardware
                                                                                    vChainID);
    if (!signature.isValid()) {
        return JUBR_SIGN_FAILED;
    }

    r = signature.r;
    s = signature.s;
    v = signature.v;

    return JUBR_OK;
}


JUB_RV JubiterLiteETHImpl::SignTx(const std::vector<JUB_BYTE>& vNonce,
                                  const std::vector<JUB_BYTE>& vGasPrice,
                                  const std::vector<JUB_BYTE>& vGasLimit,
                                  const std::vector<JUB_BYTE>& vTo,
                                  const std::vector<JUB_BYTE>& vValue,
                                  const std::vector<JUB_BYTE>& vInput,
                                  const std::vector<JUB_BYTE>& vPath,
                                  const std::vector<JUB_BYTE>& vChainID,
                                  std::vector<JUB_BYTE>& vRaw) {

    try {
        TW::Ethereum::TransactionNonTyped tx(vNonce,
                                             vGasPrice,
                                             vGasLimit,
                                             TW::Ethereum::Address(vTo),
                                             vValue,
                                             vInput);
        TW::Data half = tx.preHash(vChainID);

        std::string path(vPath.begin(), vPath.end());
        std::vector<std::string> vInputPath;
        vInputPath.push_back(path);

        TW::Data publicKey;
        JUB_VERIFY_RV(JubiterLiteImpl::GetCompPubKey(_getSignType(_curve_name),
                                                     (JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX,
                                                     path,
                                                     publicKey));

        TW::Hash::Hasher halfHasher;
        JUB_BYTE halfHasherType = _getHalfHasher(get_curve_by_name(_curve_name)->hasher_sign, halfHasher);

        std::vector<TW::Data> vPreImageHash;
        vPreImageHash.push_back(half);

        std::vector<TW::Data> vRSV;
        JUB_VERIFY_RV(JubiterLiteImpl::SignTX(vInputPath.size(),
                                              vInputPath,
                                              _getSignType(_curve_name),
                                              halfHasherType,
                                              vPreImageHash,
                                              vRSV));

        std::vector<TW::Ethereum::Signature> signatures;
        for (const auto& rsv : vRSV) {
            TW::Ethereum::Signature signature;
            JUB_VERIFY_RV(_encodeRSV(rsv, vChainID,
                                     signature.r, signature.s, signature.v));
//            if (!signer.verify(TW::PublicKey(publicKey, TWPublicKeyType::TWPublicKeyTypeSECP256k1),
//                               tx, signature)) {
//                return JUBR_VERIFY_SIGN_FAILED;
//            }
            signatures.push_back(signature);
        }

        vRaw = tx.encoded(signatures[0], vChainID);
    }
    catch (...) {
        return JUBR_ERROR_ARGS;
    }

    return JUBR_OK;
}


JUB_RV JubiterLiteETHImpl::SignTX(const int erc,
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


JUB_RV JubiterLiteETHImpl::SignTX(const int erc,
                                  const std::vector<JUB_BYTE>& vNonce,
                                  const std::vector<JUB_BYTE>& vGasPrice,
                                  const std::vector<JUB_BYTE>& vGasLimit,
                                  const std::vector<JUB_BYTE>& vTo,
                                  const std::vector<JUB_BYTE>& vValue,
                                  const std::vector<JUB_BYTE>& vInput,
                                  const std::string& accessListInJSON,
                                  const std::vector<JUB_BYTE>& vPath,
                                  const std::vector<JUB_BYTE>& vChainID,
                                  std::vector<JUB_BYTE>& vRaw) {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterLiteETHImpl::SignTX(const int erc,
                                  const std::vector<JUB_BYTE>& vNonce,
                                  const std::vector<JUB_BYTE>& vGasLimit,
                                  const std::vector<JUB_BYTE>& vMaxPriorityFeePerGas,
                                  const std::vector<JUB_BYTE>& vMaxFeePerGas,
                                  const std::vector<JUB_BYTE>& vDestination,
                                  const std::vector<JUB_BYTE>& vValue,
                                  const std::vector<JUB_BYTE>& vData,
                                  const std::string& accessListInJSON,
                                  const std::vector<JUB_BYTE>& vPath,
                                  const std::vector<JUB_BYTE>& vChainID,
                                  std::vector<JUB_BYTE>& vRaw) {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterLiteETHImpl::VerifyTX(const std::vector<JUB_BYTE>& vChainID,
                                    const std::string& path,
                                    const std::vector<JUB_BYTE>& vSigedTrans) {

    TW::Data publicKey;
    JUB_VERIFY_RV(JubiterLiteImpl::GetCompPubKey(_getSignType(_curve_name),
                                                 JUB_ENUM_BTC_TRANS_TYPE::p2pkh,
                                                 path,
                                                 publicKey));

    // verify signature
    return VerifyTx(vChainID,
                    vSigedTrans,
                    publicKey);
}


JUB_RV JubiterLiteETHImpl::SetERC20ETHTokens(const ERC20_TOKEN_INFO tokens[], const JUB_UINT16 iCount) {

    return JUBR_OK;
}


JUB_RV JubiterLiteETHImpl::SetERC20ETHToken(const std::string& tokenName, const JUB_UINT16 unitDP, const std::string& contractAddress) {

    return JUBR_OK;
}


JUB_RV JubiterLiteETHImpl::SetERC721ETHToken(const std::string& tokenName,
                                             const std::string& contractAddress) {

    return JUBR_OK;
}


JUB_RV JubiterLiteETHImpl::SignContract(const JUB_BYTE inputType,
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


JUB_RV JubiterLiteETHImpl::SignContractHash(const JUB_BYTE inputType,
                                            const std::vector<JUB_BYTE>& vNonce,
                                            const std::vector<JUB_BYTE>& vGasPrice,
                                            const std::vector<JUB_BYTE>& vGasLimit,
                                            const std::vector<JUB_BYTE>& vTo,
                                            const std::vector<JUB_BYTE>& vValue,
                                            const std::vector<JUB_BYTE>& vInput,
                                            const std::vector<JUB_BYTE>& vPath,
                                            const std::vector<JUB_BYTE>& vChainID,
                                            std::vector<JUB_BYTE>& vRaw) {

    return SignContract(inputType,
                        vNonce,
                        vGasPrice,
                        vGasLimit,
                        vTo,
                        vValue,
                        vInput,
                        vPath,
                        vChainID,
                        vRaw);
}


JUB_RV JubiterLiteETHImpl::SignBytestring(const std::vector<JUB_BYTE>& vData,
                                          const std::vector<JUB_BYTE>& vPath,
                                          const std::vector<JUB_BYTE>& vChainID,
                                          std::vector<JUB_BYTE>& vSignature) {

    try {
        TW::Ethereum::TransactionPersonal tx(vData);
        TW::Data half = tx.preHash();

        std::string path(vPath.begin(), vPath.end());
        std::vector<std::string> vInputPath;
        vInputPath.push_back(path);

        TW::Data publicKey;
        JUB_VERIFY_RV(JubiterLiteImpl::GetCompPubKey(_getSignType(_curve_name),
                                                     (JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX,
                                                     path,
                                                     publicKey));

        TW::Hash::Hasher halfHasher;
        JUB_BYTE halfHasherType = _getHalfHasher(get_curve_by_name(_curve_name)->hasher_sign, halfHasher);

        std::vector<TW::Data> vPreImageHash;
        vPreImageHash.push_back(half);

        std::vector<TW::Data> vRSV;
        JUB_VERIFY_RV(JubiterLiteImpl::SignTX(vInputPath.size(),
                                              vInputPath,
                                              _getSignType(_curve_name),
                                              halfHasherType,
                                              vPreImageHash,
                                              vRSV));

        for (const auto& rsv : vRSV) {
            TW::Ethereum::Signature signature;
            JUB_VERIFY_RV(_encodeRSV(rsv,
                                     {},    // `v` is not depend chainId
                                     signature.r, signature.s, signature.v));

            vSignature.resize(signature.r.size()+signature.s.size()+signature.v.size());
            std::copy(std::begin(signature.r), std::end(signature.r), std::begin(vSignature));
            std::copy(std::begin(signature.s), std::end(signature.s), std::begin(vSignature)+signature.r.size());
            std::copy(std::begin(signature.v), std::end(signature.v), std::begin(vSignature)+signature.r.size()+signature.s.size());
        }
    }
    catch (...) {
        return JUBR_ERROR_ARGS;
    }

    return JUBR_OK;
}


JUB_RV JubiterLiteETHImpl::VerifyBytestring(const std::string& path,
                                            const std::vector<JUB_BYTE>& vData,
                                            const std::vector<JUB_BYTE>& vSignature) {

    TW::Data publicKey;
    JUB_VERIFY_RV(JubiterLiteImpl::GetCompPubKey(_getSignType(_curve_name),
                                                 JUB_ENUM_BTC_TRANS_TYPE::p2pkh,
                                                 path,
                                                 publicKey));

    // verify signature
    return JubiterBaseETHImpl::VerifyBytestring(vData,
                                                vSignature,
                                                publicKey);
}


JUB_RV JubiterLiteETHImpl::SignTypedData(const bool& bMetamaskV4Compat,
                                         const std::string& typedDataInJSON,
                                         const std::vector<JUB_BYTE>& vPath,
                                         const std::vector<JUB_BYTE>& vChainID,
                                         std::vector<JUB_BYTE>& signatureRaw) {
    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterLiteETHImpl::VerifyTypedData(const bool& bMetamaskV4Compat,
                                           const std::string& path,
                                           const std::string& typedDataInJSON,
                                           const std::vector<JUB_BYTE>& vSignature) {
    return JUBR_IMPL_NOT_SUPPORT;
}


} // namespace token end
} // namespace jub end
