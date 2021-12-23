#include "token/BTC/JubiterBaseBTCImpl.h"
#include "Data.h"
#include "Hash.h"
#include "JUB_SDK_BTC.h"
#include "JUB_SDK_COMM.h"
#include "PublicKey.h"
#include "Script.h"
#include "TWPublicKeyType.h"
#include "Transaction.h"
#include "TrezorCrypto/bignum.h"
#include "TrezorCrypto/ecdsa.h"
#include "TrezorCrypto/secp256k1.h"
#include <Bitcoin/Address.h>
#include <Bitcoin/SegwitAddress.h>
#include <TrezorCrypto/base58.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

namespace jub {
namespace token {

TW::Data JubiterBaseBTCImpl::pushAll(const TW::Data &results) {
    auto data = TW::Data{};

    data.insert(data.end(), results.begin(), results.begin() + results.size());

    switch (_hashType) {
    case TWBitcoinSigHashTypeAll:
    case TWBitcoinSigHashTypeForkBCH:
        data.push_back(_hashType);
        break;
    default:
        break;
    } // switch (_hashType) end

    return data;
}

JUB_RV JubiterBaseBTCImpl::_getAddress(const TW::Data &publicKey, std::string &address, const TWCoinType &coinNet) {
    try {
        TW::Bitcoin::Address addr(TW::PublicKey(publicKey, _publicKeyType),
                                  TWCoinTypeP2pkhPrefix((coinNet ? coinNet : _coin)));
        address = addr.string();
    } catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}

JUB_RV JubiterBaseBTCImpl::CheckAddress(const std::string &address, const TWCoinType &coinNet) {

    // check legacy address
    std::vector<TW::Data> prefixs;
    prefixs.push_back({TWCoinTypeP2pkhPrefix((coinNet ? coinNet : _coin))});
    prefixs.push_back({TWCoinTypeP2shPrefix((coinNet ? coinNet : _coin))});
    JUB_RV rvLegacy = !(TW::Bitcoin::Address::isValid(address, prefixs));
    // check segwit address
    JUB_RV rvSegwit =
        !(TW::Bitcoin::SegwitAddress::isValid(address, std::string(stringForHRP(TWCoinTypeHRP(_coin, coinNet)))));

    return rvLegacy & rvSegwit;
}

JUB_RV JubiterBaseBTCImpl::_getTaprootAddress(const TW::Data &publicKey, std::string &address,
                                              const TWCoinType &coinNet) {
    try {
        // 32 bytes output key
        // https://github.com/bitcoin/bips/blob/master/bip-0086.mediawiki#address-derivation
        auto pub = TW::PublicKey(publicKey, TWPublicKeyTypeSECP256k1).p2trPublicKey();

        TW::Data prog(pub.bytes.begin() + 1, pub.bytes.begin() + 33);
        TW::Bitcoin::SegwitAddress segwitAddr(std::string(stringForHRP(TWCoinTypeHRP(_coin, coinNet))), 1, prog);
        address = segwitAddr.string();
    } catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}

JUB_RV JubiterBaseBTCImpl::_getSegwitAddress(const TW::Data &publicKey, std::string &address,
                                             const TWCoinType &coinNet) {
    try {
        // keyhash
        TW::Bitcoin::SegwitAddress segwitAddr(TW::PublicKey(publicKey, _publicKeyType), OpCode::OP_0,
                                              std::string(stringForHRP(TWCoinTypeHRP(_coin, coinNet))));

        address = segwitAddr.string();
    } catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}

JUB_RV JubiterBaseBTCImpl::_getNestedSegwitAddress(const TW::Data &publicKey, std::string &address,
                                                   const TWCoinType &coinNet) {
    try {
        // keyhash
        TW::Bitcoin::SegwitAddress segwitAddr(TW::PublicKey(publicKey, _publicKeyType), OpCode::OP_0,
                                              std::string(stringForHRP(TWCoinTypeHRP(_coin, coinNet))));

        // redeemScript
        TW::Bitcoin::Script redeemScript = TW::Bitcoin::Script::buildPayToWitnessProgram(0, segwitAddr.witnessProgram);
        if (redeemScript.empty()) {
            return JUBR_ARGUMENTS_BAD;
        }
        TW::Data hRedeemScript = TW::Hash::sha256ripemd(&redeemScript.bytes[0], redeemScript.bytes.size());

        // address
        TW::Data bytes;
        bytes.insert(bytes.end(), TWCoinTypeP2shPrefix((coinNet ? coinNet : _coin)));
        bytes.insert(bytes.end(), hRedeemScript.begin(), hRedeemScript.end());

        address = TW::Base58::bitcoin.encodeCheck(bytes);
    } catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}

JUB_RV JubiterBaseBTCImpl::_unsignedTx(const uint32_t coin, const std::vector<INPUT_BTC> &vInputs,
                                       const std::vector<OUTPUT_BTC> &vOutputs, TW::Bitcoin::Transaction &tx) {
    JUB_RV rv = JUBR_OK;

    // inputs
    for (const auto &input : vInputs) {
        uchar_vector preHash(input.preHash);
        preHash.reverse();
        TW::Bitcoin::OutPoint outpoint(preHash, input.preIndex);
        TW::Bitcoin::TransactionInput *txInput = new TW::Bitcoin::TransactionInput();
        txInput->previousOutput                = outpoint;
        txInput->script                        = TW::Bitcoin::Script(); // null
        txInput->sequence                      = input.nSequence;
        tx.inputs.push_back(txInput);
    }

    // outputs
    for (const auto &output : vOutputs) {
        TW::Bitcoin::Script scriptPubkey;
        JUB_UINT64 amount = 0;
        switch (output.type) {
        case JUB_ENUM_SCRIPT_BTC_TYPE::RETURN0: {
            TW::Data return0(output.return0.dataLen);
            std::memcpy(&return0[0], output.return0.data, output.return0.dataLen);

            // Check if it is omni protocol
            scriptPubkey = TW::Bitcoin::Script::buildReturn0(return0, uchar_vector("6f6d6e69"));
            amount       = output.return0.amount;
            break;
        } // case JUB_ENUM_SCRIPT_BTC_TYPE::RETURN0 end
        case JUB_ENUM_SCRIPT_BTC_TYPE::QRC20: {
            TW::Data qrc20(output.qrc20.dataLen);
            std::memcpy(&qrc20[0], output.qrc20.data, output.qrc20.dataLen);

            scriptPubkey = TW::Bitcoin::Script(qrc20.begin(), qrc20.end());
            break;
        } // case JUB_ENUM_SCRIPT_BTC_TYPE::QRC20 end
        default: {
            scriptPubkey =
                TW::Bitcoin::Script::buildForAddress(std::string(output.stdOutput.address), (TWCoinType)coin);
            amount = output.stdOutput.amount;
            break;
        } // default
        } // switch (output.type) end
        if (scriptPubkey.empty()) {
            rv = JUBR_ARGUMENTS_BAD;
        }
        if (0 != rv) {
            break;
        }
        TW::Bitcoin::TransactionOutput *ptxOutput = new TW::Bitcoin::TransactionOutput();
        ptxOutput->value                          = TW::Bitcoin::Amount(amount);
        ptxOutput->script                         = scriptPubkey;
        tx.outputs.push_back(ptxOutput);
    } // for (const auto& output:vOutputs) end

    return rv;
}

JUB_RV JubiterBaseBTCImpl::SerializeUnsignedTx(const JUB_ENUM_BTC_TRANS_TYPE &type, const JUB_UINT32 version,
                                               const std::vector<INPUT_BTC> &vInputs,
                                               const std::vector<OUTPUT_BTC> &vOutputs, const JUB_UINT32 lockTime,
                                               uchar_vector &unsignedRaw, const TWCoinType &coinNet) {

    auto witness = type == p2sh_p2wpkh || type == p2wpkh || type == p2tr;

    TW::Bitcoin::Transaction tx(version, lockTime);
    JUB_VERIFY_RV(_unsignedTx((coinNet ? coinNet : _coin), vInputs, vOutputs, tx));

    tx.encode(witness, unsignedRaw);

    return JUBR_OK;
}

JUB_RV JubiterBaseBTCImpl::_scriptPubKey(const TWCoinType &coin, const TW::PublicKey publicKey,
                                         TW::Bitcoin::Script &script) {

    // scriptPubKey
    uint8_t prefix = TWCoinTypeP2pkhPrefix(coin);
    TW::Bitcoin::Address addr(publicKey, prefix);
    script = TW::Bitcoin::Script::buildForAddress(addr.string(), coin);
    if (script.empty()) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}

JUB_RV JubiterBaseBTCImpl::_scriptCode(const TWCoinType &coin, const TW::PublicKey publicKey,
                                       TW::Bitcoin::Script &scriptCode) {

    return _scriptPubKey(coin, publicKey, scriptCode);
}

JUB_RV JubiterBaseBTCImpl::_verifyPayToPublicKeyHashScriptSig(const TWCoinType &coin,
                                                              const TW::Bitcoin::Transaction &tx, const size_t index,
                                                              const uint32_t &hashType, const uint64_t amount,
                                                              const TW::Data &signature, const TW::PublicKey publicKey,
                                                              bool witness) {

    JUB_RV rv = JUBR_OK;

    // script code - scriptPubKey
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

JUB_RV JubiterBaseBTCImpl::_verifyPayToWitnessPublicKeyHashScriptSig(const TWCoinType &coin,
                                                                     const TW::Bitcoin::Transaction &tx,
                                                                     const size_t index, const uint32_t &hashType,
                                                                     const uint64_t amount, const TW::Data &signature,
                                                                     const TW::PublicKey publicKey) {
    auto program    = publicKey.hash({});
    auto scriptCode = TW::Bitcoin::Script::buildPayToPublicKeyHash(program);
    TW::Data preImage;
    preImage = tx.getPreImage(scriptCode, index, hashType, amount);

    const auto begin = reinterpret_cast<const uint8_t *>(preImage.data());
    TW::Data digest  = tx.hasher(begin, preImage.size());
    if (!publicKey.verifyAsDER(signature, digest)) {
        return JUBR_VERIFY_SIGN_FAILED;
    }

    return JUBR_OK;
}

JUB_RV JubiterBaseBTCImpl::_verifyPayToTaprootKeyPathSpending(const TWCoinType &coin,
                                                              const TW::Bitcoin::Transaction &tx, const size_t index,
                                                              const uint32_t &hashType, const uint64_t amount,
                                                              const TW::Data &signature,
                                                              const TW::PublicKey publicKey) {
    // notused
    (void)hashType;

    // https://github.com/bitcoin/bips/blob/master/bip-0341.mediawiki#taproot-key-path-spending-signature-validation
    auto program    = TW::Data(publicKey.bytes.begin() + 1, publicKey.bytes.begin() + 33);
    auto scriptCode = TW::Bitcoin::Script::buildPayToTaprootKeyPathSpending(program);
    TW::Data preImage;
    uint8_t hash_type = signature.size() == 64 ? 0 : signature.at(64);
    if (hash_type == 0 && signature.size() == 65) {
        return JUBR_VERIFY_SIGN_FAILED;
    }
    // key path spending not have annex
    preImage = tx.getSigMsg(scriptCode, index, hashType, amount, 1, {});
    preImage.insert(preImage.begin(), hash_type);
    TW::Data digest = TW::Hash::hash_TapSigHash(preImage);
    if (!publicKey.verifySchnorr(signature, digest)) {
        return JUBR_VERIFY_SIGN_FAILED;
    }

    return JUBR_OK;
}

JUB_RV JubiterBaseBTCImpl::_verifyTx(const TWCoinType &coin, const TW::Bitcoin::Transaction *tx,
                                     const uint32_t &hashType, const std::vector<JUB_UINT64> &vInputAmount,
                                     const std::vector<TW::PublicKey> &vInputPublicKey) {

    JUB_RV rv = JUBR_OK;
    for (size_t index = 0; index < tx->inputs.size(); ++index) {
        rv = JUBR_ERROR;

        auto &input   = tx->inputs[index];
        auto &script  = input->script;
        auto &witness = input->scriptWitness;

        // support native witness transaction
        if (script.empty() && witness.empty()) {
            return JUBR_ARGUMENTS_BAD;
        }

        // native witness transaction
        if (script.empty()) {
            if (witness.size() == 2) {
                // p2wpkh
                TW::Data signature;
                TW::Data publicKey;
                // https://github.com/bitcoin/bips/blob/master/bip-0141.mediawiki#witness-program
                // The witness must consist of exactly 2 items (≤ 520 bytes each).
                // The first one a signature, and the second one a public key
                TW::Bitcoin::Script::parseWitnessStackToPayToWitnessScriptHash(witness, signature, publicKey);
                // P2WPKH
                if (publicKey.size() != 33 || vInputPublicKey[index].bytes != publicKey) {
                    return JUBR_ERROR;
                }
                rv = _verifyPayToWitnessPublicKeyHashScriptSig(coin, *tx, index, hashType, vInputAmount[index],
                                                               signature, vInputPublicKey[index]);
            } else if (witness.size() == 1) {
                // P2TR
                // https://github.com/bitcoin/bips/blob/master/bip-0341.mediawiki#constructing-and-spending-taproot-outputs
                // Spending using the key path A Taproot output can be spent with the secret key corresponding to the
                // internal_pubkey. To do so, a witness stack consists of a single element: a BIP340 signature on the
                // signature hash as defined above
                auto &signature = witness.front();
                rv = _verifyPayToTaprootKeyPathSpending(coin, *tx, index, hashType, vInputAmount[index], signature,
                                                        vInputPublicKey[index]);
            } else {
                return JUBR_ARGUMENTS_BAD;
            }

            if (rv) {
                break;
            }
            continue;
        }

        TW::Bitcoin::Script witnessProgram;
        if (!witnessProgram.decode(script.bytes)) {
            rv = JUBR_ERROR;
            break;
        }

        if (witnessProgram.isWitnessProgram()) {
            TW::Data signature;
            TW::Data publicKey;
            // P2SH-P2WPKH
            if (witnessProgram.isPayToWitnessScriptHash()) {
                if (!TW::Bitcoin::Script::parseWitnessStackToPayToWitnessScriptHash(witness, signature, publicKey)) {
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
            } else {
                rv = JUBR_ERROR;
                continue;
            }
        } else {
            TW::Data signature;
            TW::Data publicKey;
            // P2PKH
            if (script.matchPayToPublicKeyHashScriptSig(signature, publicKey)) {
                if (vInputPublicKey[index].bytes != publicKey) {
                    continue;
                }

                rv = _verifyPayToPublicKeyHashScriptSig(coin, *tx, index, hashType, vInputAmount[index], signature,
                                                        vInputPublicKey[index]);
                if (JUBR_OK != rv) {
                    break;
                } else {
                    rv = JUBR_OK;
                }
            }
//            else if (signedTx.inputs[index].script.matchxxx) {
//
//            }
            else {
                rv = JUBR_ERROR;
                continue;
            }
        }
    }

    return rv;
}

JUB_RV JubiterBaseBTCImpl::_verifyTx(JUB_ENUM_BTC_TRANS_TYPE type, const uchar_vector &signedRaw,
                                     const std::vector<JUB_UINT64> &vInputAmount,
                                     const std::vector<TW::Data> &vInputPublicKey, const TWCoinType &coinNet) {

    JUB_RV rv = JUBR_ARGUMENTS_BAD;

    auto witness = type == p2sh_p2wpkh || type == p2wpkh || type == p2tr;

    // 目前来看我们组交易，在一个交易里的所有input都是同一类型
    TW::Hash::Hasher hasher =
        type == p2tr ? static_cast<TW::Hash::HasherSimpleType>(TW::Hash::sha256) : TW::Hash::sha256d;

    try {
        TW::Bitcoin::Transaction tx(hasher);
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

JUB_RV JubiterBaseBTCImpl::_serializeTx(JUB_ENUM_BTC_TRANS_TYPE type, const std::vector<JUB_UINT64> &vInputAmount,
                                        const std::vector<TW::Data> &vInputPublicKey,
                                        const std::vector<uchar_vector> &vSignatureRaw, TW::Bitcoin::Transaction *tx,
                                        uchar_vector &signedRaw) {
    JUB_RV rv = JUBR_OK;

    auto witness = type == p2sh_p2wpkh || type == p2wpkh || type == p2tr;

    using TW::Bitcoin::Script;
    for (size_t index = 0; index < tx->inputs.size(); ++index) {
        TW::PublicKey twpk = TW::PublicKey(vInputPublicKey[index], _publicKeyType);
        if (type == p2pkh) {
            auto script = Script::buildPayToPublicKeyHashScriptSig(vSignatureRaw[index], vInputPublicKey[index]);
            tx->inputs[index]->script = script;
        } else if (type == p2wpkh) {
            tx->inputs[index]->scriptWitness =
                Script::buildPayToPublicKeyHashScriptSigWitness(vSignatureRaw[index], vInputPublicKey[index]);
        } else if (type == p2sh_p2wpkh) {
            auto script = Script::buildPayToWitnessPubkeyHash(twpk.hash({}));

            tx->inputs[index]->script = script;
            tx->inputs[index]->scriptWitness =
                Script::buildPayToPublicKeyHashScriptSigWitness(vSignatureRaw[index], vInputPublicKey[index]);
        } else if (type == p2tr) {
            auto pk = vInputPublicKey[index];

            tx->inputs[index]->scriptWitness = Script::buildPayToTaprootKeyPathSpendingWitness(vSignatureRaw[index]);
        }
    }
    if (JUBR_OK != rv) {
        return rv;
    }

    tx->encode(witness, signedRaw);
    if (0 >= signedRaw.size()) {
        return JUBR_ERROR;
    }

    return JUBR_OK;
}

} // namespace token
} // namespace jub
