#include "JUB_SDK_COMM.h"
#include "token/BTC/JubiterLiteBCHImpl.h"

namespace jub {
namespace token {

//JUB_RV JubiterLiteBCHImpl::SignTX(const JUB_BYTE addrFmt,
//                                  const JUB_ENUM_BTC_TRANS_TYPE& type,
//                                  const JUB_UINT16 inputCount,
//                                  const std::vector<JUB_UINT64>& vInputAmount,
//                                  const std::vector<std::string>& vInputPath,
//                                  const std::vector<JUB_UINT16>& vChangeIndex,
//                                  const std::vector<std::string>& vChangePath,
//                                  const std::vector<JUB_BYTE>& vUnsigedTrans,
//                                  std::vector<JUB_BYTE>& vRaw) {
//
//    bool witness = false;
//    if (p2sh_p2wpkh == type) {
//        witness = true;
//    }
//
//    TW::Bitcoin::Transaction tx;
//    if (!tx.decode(witness, vUnsigedTrans)) {
//        return JUBR_ARGUMENTS_BAD;
//    }
//
//    std::vector<TW::Data> vInputPublicKey;
//    std::vector<uchar_vector> vSignatureRaw;
//    JUB_VERIFY_RV(_SignTx(!witness,
//                          type,
//                          vInputAmount,
//                          vInputPath,
//                          vChangeIndex,
//                          vChangePath,
//                          tx,
//                          vInputPublicKey,
//                          vSignatureRaw));
//
//    uchar_vector signedRaw;
//    JUB_VERIFY_RV(_serializeTx(witness,
//                               vInputAmount,
//                               vInputPublicKey,
//                               vSignatureRaw,
//                               &tx,
//                               signedRaw));
//
//    vRaw = signedRaw;
//
//    return JUBR_OK;
//}
JUB_RV JubiterLiteBCHImpl::_SignTx(bool witness,
                                   const JUB_ENUM_BTC_TRANS_TYPE& type,
                                   const std::vector<JUB_UINT64>& vInputAmount,
                                   const std::vector<std::string>& vInputPath,
                                   const std::vector<JUB_UINT16>& vChangeIndex,
                                   const std::vector<std::string>& vChangePath,
                                   const TW::Bitcoin::Transaction& tx,
                                   std::vector<TW::Data>& vInputPublicKey,
                                   std::vector<uchar_vector>& vSignatureRaw) {

    TW::Hash::Hasher halfHasher;
    JUB_BYTE halfHasherType = _getHalfHasher(get_curve_by_name(_curve_name)->hasher_sign, halfHasher);

    std::vector<TW::Data> vPreImageHash;
    for (size_t index=0; index<tx.inputs.size(); ++index) {
        TW::Data publicKey;
        JUB_VERIFY_RV(JubiterLiteImpl::GetCompPubKey(_getSignType(_curve_name),
                                                     type,
                                                     vInputPath[index],
                                                     publicKey));

        TW::PublicKey twpk = TW::PublicKey(publicKey, _publicKeyType);

        // script code - scriptPubKey
        uint8_t prefix = TWCoinTypeP2pkhPrefix(_coin);
        TW::Bitcoin::Address addr(twpk, prefix);
        TW::Bitcoin::Script scriptCode = TW::Bitcoin::Script::buildForAddress(addr.string(), _coin);
        if (scriptCode.empty()) {
            return JUBR_ARGUMENTS_BAD;
        }

        TW::Data preImage = tx.getPreImage(scriptCode, index, _hashType, vInputAmount[index]);

        const auto begin = reinterpret_cast<const uint8_t*>(preImage.data());
        TW::Data halfDigest = halfHasher(begin, preImage.size());
        vPreImageHash.push_back(halfDigest);

        vInputPublicKey.push_back(publicKey);
    }

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
        TW::Data signature = pushAll(sign);
        vSignatureRaw.push_back(signature);
    }

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
