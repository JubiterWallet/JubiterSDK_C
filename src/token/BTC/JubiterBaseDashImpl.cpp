#include "token/BTC/JubiterBaseDashImpl.h"
#include "JUB_SDK_BTC.h"
#include <vector>

namespace jub {
namespace token {

JUB_RV JubiterBaseDashImpl::SerializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE &type, const JUB_UINT32 version,
                                                const std::vector<INPUT_BTC> &vInputs,
                                                const std::vector<OUTPUT_BTC> &vOutputs, const JUB_UINT32 lockTime,
                                                uchar_vector &unsignedRaw, const TWCoinType &coinNet) {

    JUB_RV rv = JUBR_ERROR;

    bool witness = false;
    if (p2sh_p2wpkh == type) {
        witness = true;
    }

    TW::Dash::Transaction tx(version, lockTime);
    rv = _unsignedTx(_coin, vInputs, vOutputs, tx);
    if (JUBR_OK != rv) {
        return rv;
    }

    tx.encode(witness, unsignedRaw);

    return JUBR_OK;
}

JUB_RV JubiterBaseDashImpl::_verifyTx(JUB_ENUM_BTC_TRANS_TYPE type, const uchar_vector &signedRaw,
                                      const std::vector<JUB_UINT64> &vInputAmount,
                                      const std::vector<TW::Data> &vInputPublicKey, const TWCoinType &coinNet) {

    JUB_RV rv    = JUBR_ARGUMENTS_BAD;
    auto witness = type == p2sh_p2wpkh || type == p2wpkh;
    try {
        TW::Dash::Transaction tx;
        if (!tx.decode(witness, signedRaw)) {
            return rv;
        }

        std::vector<TW::PublicKey> vInputPubkey;
        for (const auto &inputPublicKey : vInputPublicKey) {
            vInputPubkey.push_back(TW::PublicKey(TW::Data(inputPublicKey), _publicKeyType));
        }

        return JubiterBaseBTCImpl::_verifyTx((coinNet ? coinNet : _coin), &tx, _hashType, vInputAmount, vInputPubkey);
    } catch (...) {
        rv = JUBR_ERROR;
    }

    return rv;
}

} // namespace token
} // namespace jub
