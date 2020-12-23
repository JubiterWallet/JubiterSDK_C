#include "JUB_SDK_COMM.h"
#include "token/TRX/JubiterLiteTRXImpl.h"
#include <TrezorCrypto/bip32.h>
#include <Tron/Signer.h>
#include <Tron/Transaction.h>

namespace jub {
namespace token {


JUB_RV JubiterLiteTRXImpl::SelectApplet() {

    return JubiterLiteImpl::SelectApplet();
}


JUB_RV JubiterLiteTRXImpl::GetAppletVersion(std::string &version) {

    uchar_vector appID(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE));
    JUB_VERIFY_RV(JubiterBladeToken::GetAppletVersion(CharPtr2HexStr(appID), version));

    return JUBR_OK;
}


//MISC functions
JUB_RV JubiterLiteTRXImpl::SetCoin() {

    return JUBR_OK;
}


JUB_RV JubiterLiteTRXImpl::GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) {

    TW::Data publicKey;
    JUB_VERIFY_RV(JubiterLiteImpl::GetCompPubKey(_getSignType(_curve_name), (JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX, path, publicKey));

    return _getAddress(publicKey, address);
}


JUB_RV JubiterLiteTRXImpl::GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) {

    std::string btcXpub;
    JUB_VERIFY_RV(JubiterLiteImpl::GetHDNode(_getSignType(_curve_name), 0x00, path, btcXpub));

    //    typedef enum class JubPubFormat {
    //        HEX = 0x00,
    //        XPUB = 0x01
    //    } JUB_ENUM_PUB_FORMAT;
    if (0x00 == format) {//hex
        TW::Data publicKey;
        JUB_VERIFY_RV(_getPubkeyFromXpub(btcXpub, publicKey));

        uchar_vector vPublicKey(publicKey);
        pubkey = vPublicKey.getHex();
    }
    else { //xpub
        pubkey = btcXpub;
    }

    return JUBR_OK;
}


JUB_RV JubiterLiteTRXImpl::_encodeRSV(const std::vector<JUB_BYTE>& vRSV, std::vector<JUB_BYTE>& signature) {

    // RSV: 32 + 32 + 1
    curve_point R;
    uint8_t r[32] = {0x00,};
    memcpy(r, &vRSV[0], 32);
    bn_read_be(r, &R.x);
    uint8_t s[32] = {0x00,};
    memcpy(s, &vRSV[32], 32);
    bn_read_be(s, &R.y);
    uint8_t by = vRSV[64];
    by += 31;

    std::copy(std::begin(vRSV), std::end(vRSV), std::back_inserter(signature));
    signature.push_back(by);

    return JUBR_OK;
}


JUB_RV JubiterLiteTRXImpl::SignTX(const std::vector<JUB_BYTE>& vPath,
                                  const std::vector<JUB_BYTE>& vRaw,
                                  std::vector<uchar_vector>& vSignatureRaw) {

    try {
        TW::Tron::Transaction tx;
        tx.raw_data.deserialize(vRaw);

        std::string path(vPath.begin(), vPath.end());
        std::vector<std::string> vInputPath;
        vInputPath.push_back(path);

        TW::Hash::Hasher halfHasher;
        JUB_BYTE halfHasherType = _getHalfHasher(get_curve_by_name(_curve_name)->hasher_sign, halfHasher);

        TW::Tron::Signer signer;
        TW::Data half = signer.hash(tx);

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
            TW::Data sign;
            JUB_VERIFY_RV(_encodeRSV(rsv, sign));
            vSignatureRaw.push_back(sign);
        }
    }
    catch (...) {
        return JUBR_ERROR_ARGS;
    }

    return JUBR_OK;
}


JUB_RV JubiterLiteTRXImpl::SetTRC20Token(const std::string& tokenName,
                                         const JUB_UINT16 unitDP,
                                         const std::string& contractAddress) {

    return JUBR_OK;
}
} // namespace token end
} // namespace jub end
