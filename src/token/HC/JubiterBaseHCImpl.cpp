#include <token/HC/JubiterBaseHCImpl.h>
#include "Bitcoin/Script.h"


namespace jub {
namespace token {


JUB_RV JubiterBaseHCImpl::SerializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE& type,
                                              const std::vector<INPUT_BTC>& vInputs,
                                              const std::vector<OUTPUT_BTC>& vOutputs,
                                              const JUB_UINT32 lockTime,
                                              uchar_vector& unsignedRaw) {

    JUB_RV rv = JUBR_ERROR;

    bool witness = false;
    if (p2sh_p2wpkh == type) {
        witness = true;
    }

    TW::Bitcoin::HcashTransaction tx(lockTime);
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


JUB_RV JubiterBaseHCImpl::_verifyTx(const TWCoinType& coin,
                                    const TW::Bitcoin::Transaction* tx,
                                    const uint32_t& hashType,
                                    const std::vector<JUB_UINT64>& vInputAmount,
                                    const std::vector<TW::PublicKey>& vInputPublicKey) {

    JUB_RV rv = JUBR_OK;

    for (size_t index=0; index<tx->inputs.size(); ++index) {
        rv = JUBR_ERROR;

        TW::Data signature;
        TW::Data publicKey;
        if (!TW::Bitcoin::Script::parseWitnessStackToPayToWitnessScriptHash(tx->inputs[index]->scriptWitness,
                                                                            signature, publicKey)) {
            rv = JUBR_ERROR;
            break;
        }

        if (vInputPublicKey[index].bytes != publicKey) {
            continue;
        }

        rv = _verifyPayToPublicKeyHashScriptSig(coin,
                                                *tx,
                                                index, hashType, vInputAmount[index],
                                                signature,
                                                vInputPublicKey[index],
                                                true);
        if (JUBR_OK != rv) {
            break;
        }
        else {
            rv = JUBR_OK;
        }
    }

    return rv;
}


JUB_RV JubiterBaseHCImpl::VerifyTx(const bool witness,
                                   const uchar_vector& signedRaw,
                                   const std::vector<JUB_UINT64>& vInputAmount,
                                   const std::vector<TW::Data>& vInputPublicKey) {

    JUB_RV rv = JUBR_ARGUMENTS_BAD;

    try {
        TW::Bitcoin::HcashTransaction tx;
        if (!tx.decode(!witness, signedRaw)) {
            return rv;
        }

        std::vector<TW::PublicKey> vInputPubkey;
        for(const auto& inputPublicKey:vInputPublicKey) {
            vInputPubkey.push_back(TW::PublicKey(TW::Data(inputPublicKey), _publicKeyType));
        }

        return _verifyTx(_coin,
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


JUB_RV JubiterBaseHCImpl::_serializeTx(bool witness,
                                       const std::vector<JUB_UINT64>& vInputAmount,
                                       const std::vector<TW::Data>& vInputPublicKey,
                                       const std::vector<uchar_vector>& vSignatureRaw,
                                       TW::Bitcoin::Transaction* tx,
                                       uchar_vector& signedRaw) {

    for (size_t index=0; index<tx->inputs.size(); ++index) {
        dynamic_cast<TW::Bitcoin::HcashTransactionInput*>(tx->inputs[index])->value = TW::Bitcoin::Amount(vInputAmount[index]);

        TW::Bitcoin::Script script;
        if (!witness) {
            // P2PKH
            tx->inputs[index]->script = TW::Bitcoin::Script::buildPayToPublicKeyHashScriptSig(vSignatureRaw[index], vInputPublicKey[index]);
        }
        else {
            // P2WPKH
            TW::PublicKey twpk = TW::PublicKey(vInputPublicKey[index], _publicKeyType);

            TW::Data scriptPubkey;
            TW::Bitcoin::Script::buildPayToWitnessPubkeyHash(twpk.hash(TW::Data())).encode(scriptPubkey);
            tx->inputs[index]->script.bytes = scriptPubkey;

            tx->inputs[index]->scriptWitness = TW::Bitcoin::Script::buildPayToPublicKeyHashScriptSigWitness(vSignatureRaw[index], vInputPublicKey[index]);
        }
    }

    tx->encode(witness, signedRaw);
    if (0 >= signedRaw.size()) {
        return JUBR_ERROR;
    }

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
