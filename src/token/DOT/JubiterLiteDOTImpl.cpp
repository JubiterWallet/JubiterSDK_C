#include "JUB_SDK_COMM.h"

#include "token/DOT/JubiterLiteDOTImpl.h"
#include <TrezorCrypto/bip32.h>
#include <Polkadot/Signer.h>
//#include <Polkadot/Transaction.h>

namespace jub {
namespace token {


JUB_RV JubiterLiteDOTImpl::SelectApplet() {

    return JubiterLiteImpl::SelectApplet();
}


//MISC functions
JUB_RV JubiterLiteDOTImpl::SetCoin() {

    return JUBR_OK;
}


JUB_RV JubiterLiteDOTImpl::GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) {

    TW::Data publicKey;
    JUB_VERIFY_RV(JubiterLiteImpl::GetCompPubKey(_getSignType(_curve_name), (JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX, path, publicKey));

    return _getAddress(publicKey, address);
}


JUB_RV JubiterLiteDOTImpl::GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) {

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


JUB_RV JubiterLiteDOTImpl::_encodeRSV(const std::vector<JUB_BYTE>& vRSV, std::vector<JUB_BYTE>& signature) {

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


JUB_RV JubiterLiteDOTImpl::SignTX(const uint64_t& nonce,
                                  const uint64_t& gprice,
                                  const uint64_t& glimit,
                                  const std::string& to,
                                  const uint64_t& value,
                                  const std::string& input,
                                  const std::string& path,
                                  std::vector<JUB_BYTE>& vSignatureRaw) {

//    try {
//        std::string from;
//        JUB_VERIFY_RV(GetAddress(path, 0, from));
//
//        TW::Polkadot::Transaction tx(TW::Polkadot::Address(to),
//                                     TW::Polkadot::Address(from),
//                                     nonce,
//                                     value,
//                                     gprice,
//                                     glimit);
//
////        tx.setPreImage(vUnsignedRaw);
////
//        std::vector<std::string> vInputPath;
//        vInputPath.push_back(path);
//
//        TW::Hash::Hasher halfHasher;
//        JUB_BYTE halfHasherType = _getHalfHasher(get_curve_by_name(_curve_name)->hasher_sign, halfHasher);
//
//        TW::Data half = TW::Hash::blake2b(tx.cid(), 32);
//
//        std::vector<TW::Data> vPreImageHash;
//        vPreImageHash.push_back(half);
//
//        std::vector<TW::Data> vRSV;
//        JUB_VERIFY_RV(JubiterLiteImpl::SignTX(vInputPath.size(),
//                                              vInputPath,
//                                              _getSignType(_curve_name),
//                                              halfHasherType,
//                                              vPreImageHash,
//                                              vRSV));
//
//        for (const auto& rsv : vRSV) {
//            TW::Data sign;
//            JUB_VERIFY_RV(_encodeRSV(rsv, sign));
//            vSignatureRaw = sign;
//        }
//    }
//    catch (...) {
//        return JUBR_ERROR_ARGS;
//    }

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
