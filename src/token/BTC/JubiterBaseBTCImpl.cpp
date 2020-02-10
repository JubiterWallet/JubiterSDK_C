#include <token/BTC/JubiterBaseBTCImpl.h>


namespace jub {
namespace token {


TW::Hash::Hasher JubiterBaseBTCImpl::_getHasherForPublicKey() {
    switch (_publicKeyType) {
        case TWPublicKeyTypeSECP256k1:
        case TWPublicKeyTypeSECP256k1Extended:
        case TWPublicKeyTypeNIST256p1:
        case TWPublicKeyTypeNIST256p1Extended:
        case TWPublicKeyTypeED25519:
        case TWPublicKeyTypeED25519Blake2b:
            return TW::Hash::sha256ripemd;
    }
}


JUB_RV JubiterBaseBTCImpl::_serializeUnsignedTx(const uint32_t coin,
                                                const std::vector<INPUT_BTC>& vInputs,
                                                const std::vector<OUTPUT_BTC>& vOutputs,
                                                TW::Bitcoin::Transaction& tx) {

    // inputs
    for (const auto& input:vInputs) {
        uchar_vector preHash(input.preHash);
        preHash.reverse();
        TW::Bitcoin::OutPoint outpoint(preHash, input.preIndex);
        TW::Bitcoin::Script script;// null
        TW::Bitcoin::TransactionInput txInput(outpoint, script, input.nSequence);
        tx.inputs.push_back(txInput);
    }

    // outputs
    for (const auto& output:vOutputs) {
        TW::Bitcoin::Script scriptPubkey;
        JUB_UINT64 amount = 0;
        switch (output.type) {
            case JUB_ENUM_SCRIPT_BTC_TYPE::RETURN0:
            {
                TW::Data return0(output.return0.dataLen);
                std::memcpy(&return0[0], output.return0.data, output.return0.dataLen);

                scriptPubkey = TW::Bitcoin::Script::buildReturn0(return0);
                amount = output.return0.amount;
                break;
            }
            case JUB_ENUM_SCRIPT_BTC_TYPE::QRC20:
            {
                TW::Data qrc20(output.qrc20.dataLen);
                std::memcpy(&qrc20[0], output.qrc20.data, output.qrc20.dataLen);

                scriptPubkey = TW::Bitcoin::Script(qrc20.begin(), qrc20.end());
                break;
            }
            default:
            {
                scriptPubkey = TW::Bitcoin::Script::buildForAddress(std::string(output.stdOutput.address), (TWCoinType)coin);
                amount = output.stdOutput.amount;
                break;
            }
        }
        TW::Bitcoin::TransactionOutput txOutput(TW::Bitcoin::Amount(amount), scriptPubkey);
        tx.outputs.push_back(txOutput);
    }

    return JUBR_OK;
}


JUB_RV JubiterBaseBTCImpl::SerializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE& type,
                                               const std::vector<INPUT_BTC>& vInputs,
                                               const std::vector<OUTPUT_BTC>& vOutputs,
                                               const JUB_UINT32 lockTime,
                                               uchar_vector& unsignedRaw) {

    JUB_RV rv = JUBR_ERROR;

    bool witness = false;
    if (p2sh_p2wpkh == type) {
        witness = true;
    }

    TW::Bitcoin::Transaction tx(lockTime);
    rv = _serializeUnsignedTx(_coin,
                              vInputs,
                              vOutputs,
                              tx);
    if (JUBR_OK != rv) {
        return rv;
    }

    tx.encode(witness, unsignedRaw);

//    uchar_vector uRaw;
//    rv = jub::btc::serializeUnsignedTx(type,
//                                       vInputs,
//                                       vOutputs,
//                                       lockTime,
//                                       uRaw);
    return JUBR_OK;
}


JUB_RV JubiterBaseBTCImpl::_verifyPayToPublicKeyHashScriptSig(const TWCoinType& coin,
                                                              const TW::Bitcoin::Transaction& tx,
                                                              const size_t index, const TWBitcoinSigHashType& hashType, const uint64_t amount,
                                                              const TW::Data& signature,
                                                              const TW::PublicKey publicKey,
                                                              bool witness) {

    JUB_RV rv = JUBR_OK;

    // script code - scriptPubKey
    uint8_t prefix = TWCoinTypeP2pkhPrefix(coin);
    TW::Bitcoin::Address addr(publicKey, prefix);
    TW::Bitcoin::Script scriptCode = TW::Bitcoin::Script::buildForAddress(addr.string(), coin);

    TW::Data preImage;
    if (!witness) {
        preImage = tx.getPreImage(scriptCode, index, hashType);
    }
    else {
        preImage = tx.getPreImage(scriptCode, index, hashType, amount);
    }
    const auto begin = reinterpret_cast<const uint8_t*>(preImage.data());
    TW::Data digest = tx.hasher(begin, begin+preImage.size());
    if (!publicKey.verifyAsDER(signature, digest)) {
        rv = JUBR_ERROR;
    }

    return rv;
}


JUB_RV JubiterBaseBTCImpl::_verifyTx(const TWCoinType& coin,
                                     const TW::Bitcoin::Transaction& tx,
                                     const TWBitcoinSigHashType& hashType,
                                     const std::vector<JUB_UINT64>& vInputAmount,
                                     const std::vector<TW::PublicKey>& vInputPublicKey) {

    JUB_RV rv = JUBR_OK;

    for (size_t index=0; index<tx.inputs.size(); ++index) {
        rv = JUBR_ERROR;

        TW::Bitcoin::Script witnessProgram;
        if (!witnessProgram.decode(tx.inputs[index].script.bytes)) {
            rv = JUBR_ERROR;
            break;
        }

        if (witnessProgram.isWitnessProgram()) {
            TW::Data signature;
            TW::Data publicKey;
            // P2WPKH
            if (witnessProgram.isPayToWitnessScriptHash()) {
                if (2 != tx.inputs[index].scriptWitness.size()) {
                    rv = JUBR_ERROR;
                    break;
                }
                signature = tx.inputs[index].scriptWitness[0];
                publicKey = tx.inputs[index].scriptWitness[1];
                if (vInputPublicKey[index].bytes != publicKey) {
                    continue;
                }

                rv = _verifyPayToPublicKeyHashScriptSig(coin,
                                                        tx,
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
//                else if (witnessProgram.isPayToWitnessxxx) {
//
//                }
            else {
                rv = JUBR_ERROR;
                continue;
            }
        }
        else {
            TW::Data signature;
            TW::Data publicKey;
            // P2PKH
            if (tx.inputs[index].script.matchPayToPublicKeyHashScriptSig(signature, publicKey)) {
                if (vInputPublicKey[index].bytes != publicKey) {
                    continue;
                }

                rv = _verifyPayToPublicKeyHashScriptSig(coin,
                                                        tx,
                                                        index, hashType, vInputAmount[index],
                                                        signature,
                                                        vInputPublicKey[index]);
                if (JUBR_OK != rv) {
                    break;
                }
                else {
                    rv = JUBR_OK;
                }
            }
//                else if (signedTx.inputs[index].script.matchxxx) {
//
//                }
            else {
                rv = JUBR_ERROR;
                continue;
            }
        }
    }

    return rv;
}


JUB_RV JubiterBaseBTCImpl::VerifyTx(const bool witness,
                                    const uchar_vector& signedRaw,
                                    const std::vector<JUB_UINT64>& vInputAmount,
                                    const std::vector<TW::Data>& vInputPublicKey) {

    JUB_RV rv = JUBR_ARGUMENTS_BAD;

    try {
        TW::Bitcoin::Transaction tx;
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


JUB_RV JubiterBaseBTCImpl::_serializeTx(const uint32_t coin,
                                        const JUB_ENUM_BTC_TRANS_TYPE& type,
                                        const std::vector<INPUT_BTC>& vInputs,
                                        const std::vector<OUTPUT_BTC>& vOutputs,
                                        const JUB_UINT32 lockTime,
                                        const std::vector<uchar_vector>& vSignatureRaw,
                                        uchar_vector& signedRaw) {

    // JuBiter-not-finished
    return JUBR_IMPL_NOT_SUPPORT;
}

} // namespace token end
} // namespace jub end
