#include <token/BTC/JubiterBaseDashImpl.h>


namespace jub {
namespace token {


JUB_RV JubiterBaseDashImpl::SerializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE& type,
                                                const std::vector<INPUT_BTC>& vInputs,
                                                const std::vector<OUTPUT_BTC>& vOutputs,
                                                const JUB_UINT32 lockTime,
                                                uchar_vector& unsignedRaw) {

    JUB_RV rv = JUBR_ERROR;

    bool witness = false;
    if (p2sh_p2wpkh == type) {
        witness = true;
    }

    TW::Bitcoin::DashTransaction tx(lockTime);
    rv = _serializeUnsignedTx(_coin,
                              vInputs,
                              vOutputs,
                              tx);
    if (JUBR_OK != rv) {
        return rv;
    }

    tx.encode(witness, unsignedRaw);

    return JUBR_OK;
}

JUB_RV JubiterBaseDashImpl::VerifyTx(const bool witness,
                                     const uchar_vector& signedRaw,
                                     const std::vector<JUB_UINT64>& vInputAmount,
                                     const std::vector<TW::Data>& vInputPublicKey) {

    JUB_RV rv = JUBR_ARGUMENTS_BAD;

    try {
        TW::Bitcoin::DashTransaction tx;
        if (!tx.decode(witness, signedRaw)) {
            return rv;
        }

        std::vector<TW::PublicKey> vInputPubkey;
        for(const auto& inputPublicKey:vInputPublicKey) {
            vInputPubkey.push_back(TW::PublicKey(TW::Data(inputPublicKey), _publicKeyType));
        }

        return _verifyTx(_coin,
                         tx,
                         _hashType,
                         vInputAmount,
                         vInputPubkey);
    }
    catch (...) {
        rv = JUBR_ERROR;
    }

    return rv;
}

} // namespace token end
} // namespace jub end
