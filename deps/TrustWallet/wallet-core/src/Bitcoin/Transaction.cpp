// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "../BinaryCoding.h"
#include "../Hash.h"
#include "Amount.h"
#include "Data.h"
#include "SegwitAddress.h"
#include "SigHashType.h"

#include "SignatureVersion.h"
#include "TWBitcoinSigHashType.h"
#include <cstdint>
#include <iterator>
#include <vector>

using namespace TW;
using namespace TW::Bitcoin;

// JuBiter-defined
Data Transaction::getPreImage(const Script &scriptCode, size_t index, uint32_t hashType) const {
    assert(index < inputs.size());

    auto data = std::vector<uint8_t>{};

    // Version
    encode32LE(version, data);

    // Input
    encodeVarInt(inputs.size(), data);
    for (size_t i = 0; i < inputs.size(); ++i) {
        // The input being signed (replacing the scriptSig with scriptCode + amount)
        // The prevout may already be contained in hashPrevout, and the nSequence
        // may already be contain in hashSequence.
        reinterpret_cast<const TW::Bitcoin::OutPoint &>(inputs[i]->previousOutput).encode(data);
        if (i == index) {
            scriptCode.encode(data);
        } else {
            scriptCode.encodeZero(data);
        }
        encode32LE(inputs[index]->sequence, data);
    }

    // Outputs
    encodeVarInt(outputs.size(), data);
    for (auto &output : outputs) {
        output->encode(data);
    }

    // Locktime
    encode32LE(lockTime, data);

    // Sighash type
    encode32LE(hashType, data);

    return data;
}

Data Transaction::getPreImage(const Script &scriptCode, size_t index, uint32_t hashType, uint64_t amount) const {
    assert(index < inputs.size());

    Data data;

    // Version
    encode32LE(version, data);

    // Input prevouts (none/all, depending on flags)
    if ((hashType & TWBitcoinSigHashTypeAnyoneCanPay) == 0) {
        auto hashPrevouts = getPrevoutHash();
        std::copy(std::begin(hashPrevouts), std::end(hashPrevouts), std::back_inserter(data));
    } else {
        std::fill_n(back_inserter(data), 32, 0);
    }

    // Input nSequence (none/all, depending on flags)
    if ((hashType & TWBitcoinSigHashTypeAnyoneCanPay) == 0 && !hashTypeIsSingle((enum TWBitcoinSigHashType)hashType) &&
        !hashTypeIsNone((enum TWBitcoinSigHashType)hashType)) {
        auto hashSequence = getSequenceHash();
        std::copy(std::begin(hashSequence), std::end(hashSequence), std::back_inserter(data));
    } else {
        std::fill_n(back_inserter(data), 32, 0);
    }

    // The input being signed (replacing the scriptSig with scriptCode + amount)
    // The prevout may already be contained in hashPrevout, and the nSequence
    // may already be contain in hashSequence.
    reinterpret_cast<const TW::Bitcoin::OutPoint &>(inputs[index]->previousOutput).encode(data);
    scriptCode.encode(data);

    encode64LE(amount, data);
    encode32LE(inputs[index]->sequence, data);

    // Outputs (none/one/all, depending on flags)
    if (!hashTypeIsSingle((enum TWBitcoinSigHashType)hashType) &&
        !hashTypeIsNone((enum TWBitcoinSigHashType)hashType)) {
        auto hashOutputs = getOutputsHash();
        copy(begin(hashOutputs), end(hashOutputs), back_inserter(data));
    } else if (hashTypeIsSingle((enum TWBitcoinSigHashType)hashType) && index < outputs.size()) {
        Data outputData;
        outputs[index]->encode(outputData);
        auto hashOutputs = TW::Hash::hash(hasher, outputData);
        copy(begin(hashOutputs), end(hashOutputs), back_inserter(data));
    } else {
        fill_n(back_inserter(data), 32, 0);
    }

    // Locktime
    encode32LE(lockTime, data);

    // Sighash type
    encode32LE(hashType, data);

    return data;
}

Data Transaction::getSigMsg(const Script &scriptCode, size_t index, uint32_t hashType, uint64_t amount, uint8_t extByte,
                            Data annex) const {
    // see: https://github.com/bitcoin/bips/blob/master/bip-0341.mediawiki#signature-validation-rules
    Data data;

    // Control:
    // hash_type(1)
    data.push_back((uint8_t)(hashType & 0xff));

    // Transaction data:
    // nVersion(4)
    encodeVersion(data);

    // nLockTime(4)
    encode32LE(lockTime, data);

    auto append = [&data](const Data &&v) { TW::append(data, v); };

    // If the hash_type & 0x80 does not equal SIGHASH_ANYONECANPAY:
    if ((hashType & TWBitcoinSigHashTypeAnyoneCanPay) != TWBitcoinSigHashTypeAnyoneCanPay) {
        // sha_prevouts(32)
        append(getPrevoutHash());
        // sha_amounts(32)
        append(getAmountsHash());
        // sha_scriptpubkeys(32)
        append(getSpendScriptHash());
        // sha_sequences(32)
        append(getSequenceHash());
    }

    // If hash_type & 3 does not equal SIGHASH_NONE or SIGHASH_SINGLE:
    if (!hashTypeIsNone((TWBitcoinSigHashType)hashType) || !hashTypeIsSingle((TWBitcoinSigHashType)hashType)) {
        // sha_outpus(32)
        append(getOutputsHash());
    }

    // Data about this input:
    // spend_type(1):
    //   equal to (ext_flag * 2) + annex_present, where annex_present is 0 if no annex is present, or 1 otherwise
    data.push_back((uint8_t)(extByte * 2 + (!annex.empty() ? 1 : 0)));

    // If hash_type & 0x80 equals SIGHASH_ANYONECANPAY:
    if ((hashType & 0x80) == TWBitcoinSigHashTypeAnyoneCanPay) {
        // outpoint (36): the COutPoint of this input (32-byte hash + 4-byte little-endian).
        inputs[index]->previousOutput.encode(data);
        // amount (8)
        encode64LE(amount, data);
        // scriptPubKey (35)
        inputs[index]->script.encode(data);
        // nSequence (4)
        encode32LE(inputs[index]->sequence, data);
    } else {
        encode32LE(index, data);
    }

    // If an annex is present (the lowest bit of spend_type is set):
    if (!annex.empty()) {
        Data annexEncoded;
        encodeVarInt(annex.size(), annexEncoded);
        std::copy(annex.begin(), annex.end(), std::back_inserter(annexEncoded));
        // sha_annex (32): the SHA256 of (compact_size(size of annex) || annex), where annex includes the mandatory 0x50
        // prefix.
        append(TW::Hash::sha256(annexEncoded));
    }

    // Data about this output:
    // If hash_type & 3 equals SIGHASH_SINGLE:
    if (hashTypeIsSingle((TWBitcoinSigHashType)hashType)) {
        Data outputData;
        outputs[index]->encode(outputData);
        append(TW::Hash::sha256(outputData));
    }

    return data;
}

Data Transaction::getPrevoutHash() const {
    Data data;
    for (auto &input : inputs) {
        auto &outpoint = reinterpret_cast<const TW::Bitcoin::OutPoint &>(input->previousOutput);
        outpoint.encode(data);
    }
    auto hash = TW::Hash::hash(hasher, data);
    return hash;
}

Data Transaction::getSequenceHash() const {
    Data data;
    for (auto &input : inputs) {
        encode32LE(input->sequence, data);
    }
    auto hash = TW::Hash::hash(hasher, data);
    return hash;
}

Data Transaction::getOutputsHash() const {
    Data data;
    for (auto &output : outputs) {
        output->encode(data);
    }
    auto hash = TW::Hash::hash(hasher, data);
    return hash;
}

Data Transaction::getAmountsHash() const {
    Data data;
    for (auto &output : outputs) {
        encode64LE(output->value, data);
    }
    auto hash = TW::Hash::hash(hasher, data);
    return hash;
}

Data Transaction::getSpendScriptHash() const {
    Data data;
    for (auto &output : outputs) {
        output->script.encode(data);
    }
    auto hash = TW::Hash::hash(hasher, data);
    return hash;
}

// JuBiter-defined
void Transaction::encodeVersion(Data &data) const { encode32LE(version, data); }

// JuBiter-defined
void Transaction::decodeVersion(const Data &data, int &index) {
    version = decode32LE(&data[0]);
    index += (sizeof(uint32_t) / sizeof(uint8_t));
}

void Transaction::encode(bool witness, Data &data) const {
    // JuBiter-modified
    encodeVersion(data);

    if (witness) {
        // Use extended format in case witnesses are to be serialized.
        data.push_back(marker);
        data.push_back(flag);
    }

    encodeVarInt(inputs.size(), data);
    for (auto &input : inputs) {
        input->encode(data);
    }

    encodeVarInt(outputs.size(), data);
    for (auto &output : outputs) {
        output->encode(data);
    }

    // modified by JuBiter for supports encoding preimage
    if (witness) {
        bool bPreimage = false;
        for (auto &input : inputs) {
            if (0 == input->scriptWitness.size()) {
                bPreimage = true;
                continue;
            }
            input->encodeWitness(data);
        }
        if (bPreimage) {
            data.push_back(0);
        }
    }

    encode32LE(lockTime, data); // nLockTime
}

// JuBiter-defined
/// Decodes the provided buffer into the transaction.
bool Transaction::decode(bool witness, const Data &data) {

    bool bSuccess = true;

    //         [nVersion]              [nInputCount][txInputs][nOutputCount][txOutputs]         [nLockTime]
    // SegWit: [nVersion][marker][flag][nInputCount][txins]   [nOutputCount][txouts]   [witness][nLockTime]
    int index = 0;

    // [nVersion]
    decodeVersion(data, index);

    if (witness) {
        // [marker]
        marker = data[index];
        index += (sizeof(marker) / sizeof(int8_t));
        // [flag]
        flag = data[index];
        index += (sizeof(flag) / sizeof(int8_t));
    }

    // [nInputCount]
    size_t indexInc = 0;
    std::vector<uint8_t> tempInputCount(std::begin(data) + index, std::end(data));
    size_t nInputCount = decodeVarInt(tempInputCount, indexInc);
    index += indexInc;

    // [txInputs]
    for (size_t i = 0; i < nInputCount; ++i) {
        indexInc = 0;
        std::vector<uint8_t> tempInput(std::begin(data) + index + indexInc, std::end(data));
        TransactionInput *input = new TransactionInput();
        if (!input->decode(tempInput)) {
            bSuccess = false;
            break;
        }
        inputs.push_back(input);
        indexInc += input->size();
        index += indexInc;
    }
    if (!bSuccess) {
        return bSuccess;
    }

    // [nOutputCount]
    indexInc = 0;
    std::vector<uint8_t> tempOutputCount(std::begin(data) + index, std::end(data));
    size_t nOutputCount = decodeVarInt(tempOutputCount, indexInc);
    index += indexInc;

    // [txOutputs]
    for (size_t i = 0; i < nOutputCount; ++i) {
        indexInc = 0;
        std::vector<uint8_t> tempOutput(std::begin(data) + index + indexInc, std::end(data));
        TransactionOutput *output = new TransactionOutput();
        if (!output->decode(tempOutput)) {
            bSuccess = false;
            break;
        }
        outputs.push_back(output);
        indexInc += output->size();
        index += indexInc;
    }
    if (!bSuccess) {
        return bSuccess;
    }

    // [witness]
    if (witness) {
        for (size_t i = 0; i < nInputCount; ++i) {
            indexInc = 0;

            std::vector<uint8_t> tempWitness(std::begin(data) + index + indexInc, std::end(data));
            if (!inputs[i]->decodeWitness(tempWitness)) {
                bSuccess = false;
                break;
            }

            indexInc += inputs[i]->sizeWitness();
            index += indexInc;
        }
        if (!bSuccess) {
            return bSuccess;
        }
    }

    // [nLockTime]
    lockTime = decode32LE(&data[index]);

    return !empty();
}

Data Transaction::getSignatureHash(const Script &scriptCode, size_t index, uint32_t hashType, uint64_t amount,
                                   enum SignatureVersion version) const {
    switch (version) {
    case BASE:
        return getSignatureHashBase(scriptCode, index, hashType);
    case WITNESS_V0:
        return getSignatureHashWitnessV0(scriptCode, index, hashType, amount);
    }
}

/// Generates the signature hash for Witness version 0 scripts.
Data Transaction::getSignatureHashWitnessV0(const Script &scriptCode, size_t index, uint32_t hashType,
                                            uint64_t amount) const {
    auto preimage = getPreImage(scriptCode, index, hashType, amount);
    auto hash     = TW::Hash::hash(hasher, preimage);
    return hash;
}

/// Generates the signature hash for for scripts other than witness scripts.
Data Transaction::getSignatureHashBase(const Script &scriptCode, size_t index, uint32_t hashType) const {
    assert(index < inputs.size());

    Data data;

    encode32LE(version, data);

    auto serializedInputCount = (hashType & TWBitcoinSigHashTypeAnyoneCanPay) != 0 ? 1 : inputs.size();
    encodeVarInt(serializedInputCount, data);
    for (auto subindex = 0; subindex < serializedInputCount; subindex += 1) {
        serializeInput(subindex, scriptCode, index, hashType, data);
    }

    auto hashNone              = hashTypeIsNone((enum TWBitcoinSigHashType)hashType);
    auto hashSingle            = hashTypeIsSingle((enum TWBitcoinSigHashType)hashType);
    auto serializedOutputCount = hashNone ? 0 : (hashSingle ? index + 1 : outputs.size());
    encodeVarInt(serializedOutputCount, data);
    for (auto subindex = 0; subindex < serializedOutputCount; subindex += 1) {
        if (hashSingle && subindex != index) {
            auto output = TransactionOutput(-1, {});
            output.encode(data);
        } else {
            outputs[subindex]->encode(data);
        }
    }

    // Locktime
    encode32LE(lockTime, data);

    // Sighash type
    encode32LE(hashType, data);

    auto hash = TW::Hash::hash(hasher, data);
    return hash;
}

void Transaction::serializeInput(size_t subindex, const Script &scriptCode, size_t index, uint32_t hashType,
                                 Data &data) const {
    // In case of SIGHASH_ANYONECANPAY, only the input being signed is
    // serialized
    if ((hashType & TWBitcoinSigHashTypeAnyoneCanPay) != 0) {
        subindex = index;
    }

    reinterpret_cast<const TW::Bitcoin::OutPoint &>(inputs[subindex]->previousOutput).encode(data);

    // Serialize the script
    if (subindex != index) {
        encodeVarInt(0, data);
    } else {
        scriptCode.encode(data);
    }

    // Serialize the nSequence
    auto hashNone   = hashTypeIsNone((enum TWBitcoinSigHashType)hashType);
    auto hashSingle = hashTypeIsSingle((enum TWBitcoinSigHashType)hashType);
    if (subindex != index && (hashSingle || hashNone)) {
        encode32LE(0, data);
    } else {
        encode32LE(inputs[subindex]->sequence, data);
    }
}

// Proto::Transaction Transaction::proto() const {
//     auto protoTx = Proto::Transaction();
//     protoTx.set_version(version);
//     protoTx.set_locktime(lockTime);
//
//     for (const auto& input : inputs) {
//         auto protoInput = protoTx.add_inputs();
//         protoInput->mutable_previousoutput()->set_hash(input.previousOutput.hash.data(),
//                                                        input.previousOutput.hash.size());
//         protoInput->mutable_previousoutput()->set_index(input.previousOutput.index);
//         protoInput->set_sequence(input.sequence);
//         protoInput->set_script(input.script.bytes.data(), input.script.bytes.size());
//     }
//
//     for (const auto& output : outputs) {
//         auto protoOutput = protoTx.add_outputs();
//         protoOutput->set_value(output.value);
//         protoOutput->set_script(output.script.bytes.data(), output.script.bytes.size());
//     }
//
//     return protoTx;
// }
