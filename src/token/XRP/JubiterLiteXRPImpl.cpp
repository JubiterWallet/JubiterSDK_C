#include "token/XRP/JubiterLiteXRPImpl.h"
#include "JUB_SDK_COMM.h"
#include "mSIGNA/stdutils/uchar_vector.h"
#include <Ripple/Transaction.h>
#include <TrezorCrypto/bip32.h>
#include <cstring>
#include <string>
#include <vector>

namespace jub {
namespace token {

JUB_RV JubiterLiteXRPImpl::SelectApplet() { return JubiterLiteImpl::SelectApplet(); }

JUB_RV JubiterLiteXRPImpl::GetAppletVersion(stVersion &version) { return JubiterLiteImpl::GetAppletVersion(version); }

// MISC functions
JUB_RV JubiterLiteXRPImpl::SetCoin() { return JUBR_OK; }

JUB_RV JubiterLiteXRPImpl::GetAddress(const std::string &path, const JUB_UINT16 tag, std::string &address) {

    TW::Data publicKey;
    JUB_VERIFY_RV(
        JubiterLiteImpl::GetCompPubKey(_getSignType(_curve_name), (JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX, path, publicKey));

    return _getAddress(publicKey, address);
}

JUB_RV JubiterLiteXRPImpl::GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey) {

    std::string xpub;
    JUB_VERIFY_RV(JubiterLiteImpl::GetHDNode(_getSignType(_curve_name), 0x00, path, xpub));

    //    typedef enum class {
    //        HEX = 0x00,
    //        XRP = 0x01
    //    } JUB_ENUM_PUB_FORMAT;
    if (0x00 == format) { // hex
        auto pk = TW::PublicKey::FromXpub(xpub, _curve_name);
        pubkey  = uchar_vector{pk.bytes}.getHex();
        pubkey  = ETH_PRDFIX + pubkey;
    } else { // xpub
        pubkey = xpub;
    }

    return JUBR_OK;
}

JUB_RV JubiterLiteXRPImpl::_encodeRSV(const std::vector<JUB_BYTE> &vRSV, std::vector<JUB_BYTE> &signature) {

    uint8_t *sig = new uint8_t[vRSV.size() + 1];
    memset(sig, 0x0, vRSV.size() + 1);
    std::copy(std::begin(vRSV), std::end(vRSV), sig);

    // signature DER encode
    std::array<uint8_t, 73> resultBytes;
    size_t size = ecdsa_sig_to_der(sig, resultBytes.data());
    std::copy(resultBytes.begin(), resultBytes.begin() + size, std::back_inserter(signature));

    delete[] sig;
    sig = nullptr;

    return JUBR_OK;
}

JUB_RV JubiterLiteXRPImpl::SignTX(const std::vector<JUB_BYTE> &vPath, std::vector<JUB_BYTE> &vUnsignedRaw,
                                  std::vector<uchar_vector> &vSignatureRaw) {

    try {
        TW::Ripple::Transaction tx;
        tx.setPreImage(vUnsignedRaw);

        std::string path(vPath.begin(), vPath.end());
        std::vector<std::string> vInputPath;
        vInputPath.push_back(path);

        TW::Data publicKey;
        JUB_VERIFY_RV(JubiterLiteImpl::GetCompPubKey(_getSignType(_curve_name), (JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX,
                                                     path, publicKey));

        tx.pub_key.insert(tx.pub_key.end(), publicKey.begin(), publicKey.end());
        tx.serialize();
        vUnsignedRaw.clear();
        vUnsignedRaw = tx.getPreImage();

        TW::Hash::Hasher halfHasher;
        JUB_BYTE halfHasherType = _getHalfHasher(get_curve_by_name(_curve_name)->hasher_sign, halfHasher);

        const auto begin    = reinterpret_cast<const uint8_t *>(vUnsignedRaw.data());
        TW::Data halfDigest = halfHasher(begin, vUnsignedRaw.size());
        TW::Data half;
        half.insert(half.begin(), halfDigest.begin(), halfDigest.begin() + 32);

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

} // namespace token
} // namespace jub
