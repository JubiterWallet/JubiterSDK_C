#include "token/TRX/JubiterLiteTRXImpl.h"
#include "JUB_SDK_COMM.h"
#include "mSIGNA/stdutils/uchar_vector.h"
#include <TrezorCrypto/bip32.h>
#include <Tron/Signer.h>
#include <Tron/Transaction.h>
#include <cstring>
#include <string>
#include <vector>

namespace jub {
namespace token {

JUB_RV JubiterLiteTRXImpl::SelectApplet() { return JubiterLiteImpl::SelectApplet(); }

JUB_RV JubiterLiteTRXImpl::GetAppletVersion(stVersion &version) { return JubiterLiteImpl::GetAppletVersion(version); }

// MISC functions
JUB_RV JubiterLiteTRXImpl::SetCoin() { return JUBR_OK; }

JUB_RV JubiterLiteTRXImpl::GetAddress(const std::string &path, const JUB_UINT16 tag, std::string &address) {

    TW::Data publicKey;
    JUB_VERIFY_RV(
        JubiterLiteImpl::GetCompPubKey(_getSignType(_curve_name), (JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX, path, publicKey));

    return _getAddress(publicKey, address);
}

JUB_RV JubiterLiteTRXImpl::GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey) {

    std::string xpub;
    JUB_VERIFY_RV(JubiterLiteImpl::GetHDNode(_getSignType(_curve_name), 0x00, path, xpub));

    //    typedef enum class JubPubFormat {
    //        HEX = 0x00,
    //        XPUB = 0x01
    //    } JUB_ENUM_PUB_FORMAT;
    if (0x00 == format) { // hex
        auto pk = TW::PublicKey::FromXpub(xpub, _curve_name);
        pubkey  = uchar_vector{pk.bytes}.getHex();
    } else { // xpub
        pubkey = xpub;
    }

    return JUBR_OK;
}

JUB_RV JubiterLiteTRXImpl::_encodeRSV(const std::vector<JUB_BYTE> &vRSV, std::vector<JUB_BYTE> &signature) {

    // RSV: 32 + 32 + 1
    curve_point R;
    uint8_t r[32] = {
        0x00,
    };
    memcpy(r, &vRSV[0], 32);
    bn_read_be(r, &R.x);
    uint8_t s[32] = {
        0x00,
    };
    memcpy(s, &vRSV[32], 32);
    bn_read_be(s, &R.y);
    uint8_t by = vRSV[64];
    by += 31;

    std::copy(std::begin(vRSV), std::end(vRSV), std::back_inserter(signature));
    signature.push_back(by);

    return JUBR_OK;
}

JUB_RV JubiterLiteTRXImpl::SignTX(const std::vector<JUB_BYTE> &vPath, const std::vector<JUB_BYTE> &vRaw,
                                  std::vector<uchar_vector> &vSignatureRaw) {

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
        JUB_VERIFY_RV(JubiterLiteImpl::SignTX(vInputPath.size(), vInputPath, _getSignType(_curve_name), halfHasherType,
                                              vPreImageHash, vRSV));

        for (const auto &rsv : vRSV) {
            TW::Data sign;
            JUB_VERIFY_RV(_encodeRSV(rsv, sign));
            vSignatureRaw.push_back(sign);
        }
    } catch (...) {
        return JUBR_ERROR_ARGS;
    }

    return JUBR_OK;
}

JUB_RV JubiterLiteTRXImpl::SignBytestring(const std::vector<JUB_BYTE> &vData, const std::vector<JUB_BYTE> &vPath,
                                          std::vector<uchar_vector> &vSignatureRaw) {
    return JUBR_IMPL_NOT_SUPPORT;
}

JUB_RV JubiterLiteTRXImpl::SetTRC20Token(const std::string &tokenName, const JUB_UINT16 unitDP,
                                         const std::string &contractAddress) {

    return JUBR_OK;
}

JUB_RV JubiterLiteTRXImpl::SetTRC721Token(const std::string &tokenName, const std::string &contractAddress) {

    return JUBR_OK;
}
} // namespace token
} // namespace jub
