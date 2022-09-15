#include "token/CKB/JubiterLiteCKBImpl.h"
#include "JUB_SDK_COMM.h"
#include "PublicKey.h"
#include "TWPublicKeyType.h"
#include <string>
#include <vector>

//#include <TrezorCrypto/base58.h>

namespace jub {
namespace token {

JUB_RV JubiterLiteCKBImpl::SelectApplet() { return JubiterLiteImpl::SelectApplet(); }

JUB_RV JubiterLiteCKBImpl::SetUnit(const JUB_ENUM_BTC_UNIT_TYPE &unit) { return JUBR_OK; }

JUB_RV JubiterLiteCKBImpl::SetCoin(const JUB_ENUM_COINTYPE_BTC &type) { return JUBR_OK; }

JUB_RV JubiterLiteCKBImpl::GetAddress(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE &type,
                                      const std::string &path, const JUB_UINT16 tag, std::string &address,
                                      const TWCoinType &coinNet) {

    TW::Data publicKey;
    JUB_VERIFY_RV(
        JubiterLiteImpl::GetCompPubKey(_getSignType(_curve_name), (JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX, path, publicKey));

    return _getSegwitAddress(publicKey, address);
}

JUB_RV JubiterLiteCKBImpl::GetHDNode(const JUB_ENUM_BTC_TRANS_TYPE &type, const std::string &path, std::string &pubkey,
                                     const TWCoinType &coinNet) {

    std::string xpub;
    JUB_VERIFY_RV(JubiterLiteImpl::GetHDNode(_getSignType(_curve_name), 0x00, path, xpub));

    auto pk = TW::PublicKey::FromXpub(xpub, _curve_name);

    pubkey = uchar_vector(pk.bytes).getHex();

    return JUBR_OK;
}

JUB_RV JubiterLiteCKBImpl::_SignTX(const std::vector<std::string> &vInputPath, TW::NervosCKB::Transaction &tx,
                                   const TWCoinType &coinNet) {

    TW::Hash::Hasher halfHasher;
    JUB_BYTE halfHasherType = _getHalfHasher(get_curve_by_name(_curve_name)->hasher_sign, halfHasher);

    std::vector<TW::Data> vPreImageHash;
    auto witness = tx.witnessGroups();
    for (size_t index = 0; index < witness.size(); ++index) {
        if (!witness[index].isZero()) {
            Data preImage       = tx.getPreImage(index);
            const auto begin    = reinterpret_cast<const uint8_t *>(preImage.data());
            TW::Data halfDigest = halfHasher(begin, preImage.size());

            vPreImageHash.push_back(halfDigest);
        } else {
            vPreImageHash.push_back({});
        }
    }

    std::vector<TW::Data> vRSV;
    JUB_VERIFY_RV(JubiterLiteImpl::SignTX(vInputPath.size(), vInputPath, _getSignType(_curve_name), halfHasherType,
                                          vPreImageHash, vRSV));

    return JUBR_OK;
}

JUB_RV JubiterLiteCKBImpl::_VerifyTX(const std::vector<std::string> &vInputPath, const TW::NervosCKB::Transaction &tx,
                                     const std::vector<uchar_vector> &vSignatureRaw, const TWCoinType &coinNet) {

    JUB_RV rv = JUBR_OK;

    TW::NervosCKB::Transaction signedTx(tx);

    auto witness = signedTx.witnessGroups();
    if (vSignatureRaw.size() != witness.size()) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::vector<TW::PublicKey> vInputPublicKey;
    for (size_t index = 0; index < witness.size(); ++index) {
        TW::Data publicKey;
        JUB_VERIFY_RV(JubiterLiteImpl::GetCompPubKey(_getSignType(_curve_name), JUB_ENUM_BTC_TRANS_TYPE::p2pkh,
                                                     vInputPath[index], publicKey));

        vInputPublicKey.push_back(TW::PublicKey(publicKey, _publicKeyType));

        if (!witness[index].isZero()) {
            TW::NervosCKB::WitnessArgs arg = witness[index];
            arg.lock                       = vSignatureRaw[index];
            signedTx.witnesses.push_back(arg);
        } else {
            signedTx.witnesses.push_back(TW::NervosCKB::WitnessArgs(0x00));
        }
    }

    JUB_VERIFY_RV(_verifyTX(signedTx, vInputPublicKey, coinNet));

    return rv;
}

} // namespace token
} // namespace jub
