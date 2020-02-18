#include <token/BTC/JubiterBaseBCHImpl.h>


namespace jub {
namespace token {


JUB_RV JubiterBaseBCHImpl::_verifyTx(const TWCoinType& coin,
                                     const TW::Bitcoin::Transaction* tx,
                                     const uint32_t& hashType,
                                     const std::vector<JUB_UINT64>& vInputAmount,
                                     const std::vector<TW::PublicKey>& vInputPublicKey) {

    JUB_RV rv = JUBR_OK;

    for (size_t index=0; index<tx->inputs.size(); ++index) {
        rv = JUBR_ERROR;

        TW::Data signature;
        TW::Data publicKey;
        TW::Bitcoin::Script script = tx->inputs[index]->script;
        // P2WPKH
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
