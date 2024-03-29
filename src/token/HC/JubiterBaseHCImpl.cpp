#include "token/HC/JubiterBaseHCImpl.h"
#include "JUB_SDK_BTC.h"
#include <Bitcoin/Script.h>
#include <TrustWalletCore/TWCurve.h>
#include <string>
#include <vector>

namespace jub {
namespace token {

JUB_RV JubiterBaseHCImpl::SerializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE &type, const JUB_UINT32 version,
                                              const std::vector<INPUT_BTC> &vInputs,
                                              const std::vector<OUTPUT_BTC> &vOutputs, const JUB_UINT32 lockTime,
                                              uchar_vector &unsignedRaw, const TWCoinType &coinNet) {

    JUB_RV rv = JUBR_ERROR;

    auto witness = type == p2sh_p2wpkh;

    TW::Hcash::Transaction tx(version, lockTime);
    rv = _unsignedTx(_coin, vInputs, vOutputs, tx);
    if (JUBR_OK != rv) {
        return rv;
    }

    tx.encode(witness, unsignedRaw);

    return JUBR_OK;
}

JUB_RV JubiterBaseHCImpl::_pkScript(const TW::PublicKey publicKey, TW::Bitcoin::Script &script) {

    // pkScript
    script = TW::Bitcoin::Script::buildPayToPublicKeyHash(
        TW::Hash::ripemd(TW::Hash::blake256(publicKey.compressed().bytes)));
    if (script.empty()) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}

JUB_RV JubiterBaseHCImpl::_scriptCode(const TWCoinType &coin, const TW::PublicKey publicKey,
                                      TW::Bitcoin::Script &scriptCode) {

    return _pkScript(publicKey, scriptCode);
}

JUB_RV JubiterBaseHCImpl::_verifyPayToPublicKeyHashScriptSig(const TWCoinType &coin, const TW::Bitcoin::Transaction &tx,
                                                             const size_t index, const uint32_t &hashType,
                                                             const uint64_t amount, const TW::Data &signature,
                                                             const TW::PublicKey publicKey, bool witness) {

    JUB_RV rv = JUBR_ERROR;

    // script code - pkScript
    TW::Bitcoin::Script scriptCode;
    rv = _scriptCode(coin, publicKey, scriptCode);
    if (JUBR_OK != rv) {
        return rv;
    }

    TW::Data preImage;
    if (!witness) {
        preImage = tx.getPreImage(scriptCode, index, hashType);
    } else {
        preImage = tx.getPreImage(scriptCode, index, hashType, amount);
    }
    const auto begin = reinterpret_cast<const uint8_t *>(preImage.data());
    TW::Data digest  = tx.hasher(begin, preImage.size());
    if (!publicKey.verifyAsDER(signature, digest)) {
        rv = JUBR_ERROR;
    }

    return rv;
}

JUB_RV JubiterBaseHCImpl::_verifyTx(const JUB_ENUM_BTC_TRANS_TYPE &type,
                                    const TWCoinType &coin, const TW::Bitcoin::Transaction *tx,
                                    const uint32_t &hashType, const std::vector<JUB_UINT64> &vInputAmount,
                                    const std::vector<TW::PublicKey> &vInputPublicKey) {

    JUB_RV rv = JUBR_OK;

    for (size_t index = 0; index < tx->inputs.size(); ++index) {
        rv = JUBR_ERROR;

        TW::Data signature;
        TW::Data publicKey;
        if (!TW::Bitcoin::Script::parseWitnessStackToPayToWitnessScriptHash(tx->inputs[index]->scriptWitness, signature,
                                                                            publicKey)) {
            rv = JUBR_ERROR;
            break;
        }

        if (vInputPublicKey[index].bytes != publicKey) {
            continue;
        }

        rv = _verifyPayToPublicKeyHashScriptSig(coin, *tx, index, hashType, vInputAmount[index], signature,
                                                vInputPublicKey[index], true);
        if (JUBR_OK != rv) {
            break;
        } else {
            rv = JUBR_OK;
        }
    }

    return rv;
}

JUB_RV JubiterBaseHCImpl::_verifyTx(const JUB_ENUM_BTC_TRANS_TYPE &type, const uchar_vector &signedRaw,
                                    const std::vector<JUB_UINT64> &vInputAmount,
                                    const std::vector<TW::Data> &vInputPublicKey, const TWCoinType &coinNet) {

    JUB_RV rv = JUBR_ARGUMENTS_BAD;

    try {
        TW::Hcash::Transaction tx;
        if (!tx.decode(true, signedRaw)) {
            return rv;
        }

        std::vector<TW::PublicKey> vInputPubkey;
        for (const auto &inputPublicKey : vInputPublicKey) {
            vInputPubkey.push_back(TW::PublicKey(TW::Data(inputPublicKey), _publicKeyType));
        }

        return _verifyTx(type, (coinNet ? coinNet : _coin), &tx, _hashType, vInputAmount, vInputPubkey);
    } catch (...) {
        rv = JUBR_ERROR;
    }

    return rv;
}

JUB_RV JubiterBaseHCImpl::_serializeTx(const JUB_ENUM_BTC_TRANS_TYPE &type, const std::vector<JUB_UINT64> &vInputAmount,
                                       const std::vector<TW::Data> &vInputPublicKey,
                                       const std::vector<uchar_vector> &vSignatureRaw, TW::Bitcoin::Transaction *tx,
                                       uchar_vector &signedRaw) {
    JUB_RV rv = JUBR_OK;

    for (size_t index = 0; index < tx->inputs.size(); ++index) {
        dynamic_cast<TW::Hcash::TransactionInput *>(tx->inputs[index])->value =
            TW::Bitcoin::Amount(vInputAmount[index]);

//        if (!witness) {
            // P2PKH
//            tx->inputs[index]->script =
//                TW::Bitcoin::Script::buildPayToPublicKeyHashScriptSig(vSignatureRaw[index], vInputPublicKey[index]);
//        } else {
//            // P2WPKH
            TW::PublicKey twpk = TW::PublicKey(vInputPublicKey[index], _publicKeyType);

            TW::Data scriptPubkey;
            TW::Bitcoin::Script::buildPayToWitnessPubkeyHash(twpk.hash(TW::Data())).encode(scriptPubkey);
            tx->inputs[index]->script.bytes = scriptPubkey;

            tx->inputs[index]->scriptWitness = TW::Bitcoin::Script::buildPayToPublicKeyHashScriptSigWitness(
                vSignatureRaw[index], vInputPublicKey[index]);
            if (tx->inputs[index]->scriptWitness.empty()) {
                rv = JUBR_ARGUMENTS_BAD;
                break;
            }
//        }
        if (tx->inputs[index]->script.empty()) {
            rv = JUBR_ARGUMENTS_BAD;
            break;
        }
    }
    if (JUBR_OK != rv) {
        return rv;
    }

    tx->encode(true,    // decode Hcash signedTx with witness(signature is in wintessScript)
               signedRaw);
    if (0 >= signedRaw.size()) {
        return JUBR_ERROR;
    }

    return JUBR_OK;
}

JUB_RV JubiterBaseHCImpl::_getAddress(const TW::Data &publicKey, std::string &address, const TWCoinType &coinNet) {

    try {
        TW::Hash::Hasher hasherPubkey;
        TW::Hash::Hasher hasherBase58;
        if (!hasherType2Hasher(get_curve_by_name(_curve_name)->hasher_pubkey, hasherPubkey) ||
            !hasherType2Hasher(get_curve_by_name(_curve_name)->hasher_base58, hasherBase58)) {
            return JUBR_ARGUMENTS_BAD;
        }

        TW::PublicKey twpk = TW::PublicKey(TW::Data(publicKey), _publicKeyType);
        TW::Data bytes     = twpk.hash(TWCoinTypeP2pkhPrefixData(_coin), hasherPubkey);
        address            = TW::Base58::bitcoin.encodeCheck(bytes, hasherBase58);
    } catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}

} // namespace token
} // namespace jub
