#include <token/ETH/JubiterNFCETHImpl.h>
#include <TrezorCrypto/bip32.h>
#include <Ethereum/Signer.h>

namespace jub {
namespace token {


#define SWITCH_TO_ETH_APP                       \
do {				                            \
    JUB_VERIFY_RV(_SelectApp(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE)));\
} while (0);                                    \


JUB_RV JubiterNFCETHImpl::SelectApplet() {

    SWITCH_TO_ETH_APP;
    return JUBR_OK;
}


JUB_RV JubiterNFCETHImpl::GetAppletVersion(std::string &version) {

    uchar_vector appID(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE));
    JUB_VERIFY_RV(GetAppletVersionBlade(CharPtr2HexStr(appID), version));

    return JUBR_OK;
}


JUB_RV JubiterNFCETHImpl::GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) {

    TW::Data publicKey;
    JUB_VERIFY_RV(JubiterNFCImpl::GetCompPubKey((JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX, path, publicKey));

    return _getAddress(publicKey, address);
}


JUB_RV JubiterNFCETHImpl::GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) {

    //path = "m/44'/60'/0'";
    std::string btcXpub;
    JUB_VERIFY_RV(JubiterNFCImpl::GetHDNode(0x00, path, btcXpub));

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


JUB_RV JubiterNFCETHImpl::_encodeRSV(const std::vector<JUB_BYTE>& vRSV,
                                     const std::vector<JUB_BYTE>& vChainID,
                                     TW::Data& r, TW::Data& s, TW::Data& v) {

    auto tuple = TW::Ethereum::Signer::values(TW::Data(vChainID), TW::Data(vRSV));

    r = std::get<0>(tuple);
    s = std::get<1>(tuple);
    v = std::get<2>(tuple);

    return JUBR_OK;
}


JUB_RV JubiterNFCETHImpl::SignTX(const bool bERC20,
                                 const std::vector<JUB_BYTE>& vNonce,
                                 const std::vector<JUB_BYTE>& vGasPrice,
                                 const std::vector<JUB_BYTE>& vGasLimit,
                                 const std::vector<JUB_BYTE>& vTo,
                                 const std::vector<JUB_BYTE>& vValue,
                                 const std::vector<JUB_BYTE>& vInput,
                                 const std::vector<JUB_BYTE>& vPath,
                                 const std::vector<JUB_BYTE>& vChainID,
                                 std::vector<JUB_BYTE>& vRaw) {

    try {
        TW::Ethereum::Transaction tx(vNonce,
                                     vGasPrice,
                                     vGasLimit,
                                     TW::Ethereum::Address(vTo),
                                     vValue,
                                     vInput);
        TW::Ethereum::Signer signer(vChainID);
        TW::Data half = signer.hash(tx);

        std::string path(vPath.begin(), vPath.end());
        std::vector<std::string> vInputPath;
        vInputPath.push_back(path);

        TW::Data publicKey;
        JUB_VERIFY_RV(JubiterNFCImpl::GetCompPubKey((JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX, path, publicKey));

        TW::Hash::Hasher halfHasher;
        JUB_BYTE halfHasherType = _getHalfHasher(get_curve_by_name(_curve_name)->hasher_sign, halfHasher);

        std::vector<TW::Data> vPreImageHash;
        vPreImageHash.push_back(half);

        std::vector<TW::Data> vRSV;
        JUB_VERIFY_RV(JubiterNFCImpl::SignTX(vInputPath.size(),
                                             vInputPath,
                                             _getSignType(_curve_name),
                                             halfHasherType,
                                             vPreImageHash,
                                             vRSV));

        for (const auto& rsv : vRSV) {
            JUB_VERIFY_RV(_encodeRSV(rsv, vChainID,
                                     tx.r, tx.s, tx.v));
//            if (!signer.verify(TW::PublicKey(publicKey, TWPublicKeyType::TWPublicKeyTypeSECP256k1),
//                               tx)) {
//                return JUBR_ERROR;
//            }
        }

        vRaw = TW::Ethereum::RLP::encode(tx);
    }
    catch (...) {
        return JUBR_ERROR_ARGS;
    }

    return JUBR_OK;
}


JUB_RV JubiterNFCETHImpl::VerifyTX(const std::vector<JUB_BYTE>& vChainID,
                                   const std::string& path,
                                   const std::vector<JUB_BYTE>& vSigedTrans) {

    TW::Data publicKey;
    JUB_VERIFY_RV(JubiterNFCImpl::GetCompPubKey(JUB_ENUM_BTC_TRANS_TYPE::p2pkh, path, publicKey));

    // verify signature
    return VerifyTx(vChainID,
                    vSigedTrans,
                    publicKey);
}


JUB_RV JubiterNFCETHImpl::SetERC20ETHToken(const std::string& tokenName, const JUB_UINT16 unitDP, const std::string& contractAddress) {

    return JUBR_IMPL_NOT_SUPPORT;
}


} // namespace token end
} // namespace jub end
