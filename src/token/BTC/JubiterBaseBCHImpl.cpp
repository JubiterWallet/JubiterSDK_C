#include "token/BTC/JubiterBaseBCHImpl.h"
#include <Bitcoin/CashAddress.h>


namespace jub {
namespace token {


JUB_RV JubiterBaseBCHImpl::_getAddress(const TW::Data& publicKey, std::string& address, const TWCoinType& coinNet) {

    try {
        TW::Bitcoin::CashAddress addr(TW::PublicKey(TW::Data(publicKey), _publicKeyType));
        address = addr.string();
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


JUB_RV JubiterBaseBCHImpl::CheckAddress(const std::string& address, const TWCoinType& coinNet) {

    return !(TW::Bitcoin::CashAddress::isValid(address));
}


JUB_RV JubiterBaseBCHImpl::_verifyTx(const JUB_ENUM_BTC_TRANS_TYPE& type,
                                     const uchar_vector& signedRaw,
                                     const std::vector<JUB_UINT64>& vInputAmount,
                                     const std::vector<TW::Data>& vInputPublicKey,
                                     const TWCoinType& coinNet) {

    JUB_RV rv = JUBR_ARGUMENTS_BAD;

    auto witness = type == p2sh_p2wpkh || type == p2wpkh;

    try {
        TW::Bitcoin::Transaction tx;
        if (!tx.decode(witness, signedRaw)) {
            return rv;
        }

        std::vector<TW::PublicKey> vInputPubkey;
        for(const auto& inputPublicKey:vInputPublicKey) {
            vInputPubkey.push_back(TW::PublicKey(TW::Data(inputPublicKey), _publicKeyType));
        }

        return JubiterBaseBCHImpl::_verifyTx(type,
                                             (coinNet?coinNet:_coin),
                                             &tx,
                                             _hashType,
                                             vInputAmount,
                                             vInputPubkey);
    }
    catch (...) {
        rv = JUBR_ERROR;
    }

    return rv;
}


JUB_RV JubiterBaseBCHImpl::_verifyTx(const JUB_ENUM_BTC_TRANS_TYPE& type,
                                     const TWCoinType& coin,
                                     const TW::Bitcoin::Transaction* tx,
                                     const uint32_t& hashType,
                                     const std::vector<JUB_UINT64>& vInputAmount,
                                     const std::vector<TW::PublicKey>& vInputPublicKey) {

    JUB_RV rv = JUBR_OK;

    for (size_t index=0; index<tx->inputs.size(); ++index) {
        rv = JUBR_ERROR;

        TW::Bitcoin::Script script = tx->inputs[index]->script;
        if (script.empty()) {
            rv = JUBR_ARGUMENTS_BAD;
            break;
        }

        // P2WPKH
        TW::Data signature;
        TW::Data publicKey;
        if (script.matchPayToPublicKeyHashScriptSig(signature, publicKey)) {
            if (vInputPublicKey[index].bytes != publicKey) {
                continue;
            }

            // preimage using SegWit
            rv = _verifyPayToPublicKeyHashScriptSig(coin,
                                                    *tx,
                                                    index, hashType, vInputAmount[index],
                                                    signature,
                                                    vInputPublicKey[index], true);
            if (JUBR_OK != rv) {
                break;
            }
            else {
                rv = JUBR_OK;
            }
        }
//        else if (signedTx.inputs[index].script.matchxxx) {
//
//        }
        else {
            rv = JUBR_ERROR;
            continue;
        }
    }

    return rv;
}


} // namespace token end
} // namespace jub end
