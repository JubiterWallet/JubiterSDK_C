// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransactionHcash.h"
#include "../BinaryCoding.h"
#include "../Hash.h"

#include <TrustWalletCore/TWHcash.h>
#include "SigHashTypeHcash.h"

#include <cassert>


using namespace TW;
using namespace TW::Bitcoin;

// JuBiter-defined
Data HcashTransaction::getPreImage(const Script& scriptCode, size_t index, uint32_t hashType, uint64_t amount) const {
    assert(index < inputs.size());

    auto data = std::vector<uint8_t>{};

    if (!hashTypeIsAll((enum TWHcashSigHashType)hashType)) {
        return data;
    }

    // HashType
    encode32LE(hashType, data);

    switch (hashType) {
    case TWHcashSigHashAll:
    {
        // TxSerializeNoWitnessHash
        auto hashTxSerializeNoWitness = getTxSerializeNoWitnessHash();
        copy(begin(hashTxSerializeNoWitness), end(hashTxSerializeNoWitness), back_inserter(data));

        //only TxSerializeWitnessSigning
        auto hashTxSerializeWitnessSigning = getTxSerializeWitnessSigningHash(scriptCode, index);
        copy(begin(hashTxSerializeWitnessSigning), end(hashTxSerializeWitnessSigning), back_inserter(data));
        break;
    }
    default:
        break;
    }

    return data;
}

// JuBiter-defined
Data HcashTransaction::txSerializeNoWitness() const {
    auto data = std::vector<uint8_t>{};

    // Version
    encode32LE((version|TWHcashSerializeNoWitness<<16), data);

    //not support many inputs
    encodeVarInt(inputs.size(), data);
    for (auto& input : inputs) {
        input->encode(data);
    }

    encodeVarInt(outputs.size(), data);
    for (auto& output : outputs) {
        output->encode(data);
    }

    encode32LE(lockTime, data);

    encode32LE(expiry, data);

    return data;
}

// JuBiter-defined
Data HcashTransaction::txSerializeOnlyWitness(const Script& signScript, size_t index) const {
    auto data = std::vector<uint8_t>{};

    // Version
    encode32LE((version|TWHcashSerializeOnlyWitness<<16), data);

    //not support many inputs
    encodeVarInt(inputs.size(), data);
    for (size_t i=0; i<inputs.size(); ++i) {
        encode64LE(dynamic_cast<HcashTransactionInput*>(inputs[i])->value, data);
        encode32BE(dynamic_cast<HcashTransactionInput*>(inputs[i])->blockHeight, data);
        encode32BE(dynamic_cast<HcashTransactionInput*>(inputs[i])->blockIndex, data);
        if (i == index) {
            signScript.encode(data);
        }
        else {
            signScript.encodeZero(data);
        }
    }

    return data;
}

// JuBiter-defined
Data HcashTransaction::txSerializeWitnessSigning(const Script& pkScript, size_t index) const {
    auto data = std::vector<uint8_t>{};

    // Version
    encode32LE((version|TWHcashSerializeWitnessSigning<<16), data);

    encodeVarInt(inputs.size(), data);
    for (size_t i=0; i<inputs.size(); ++i) {
        if (i == index) {
            pkScript.encode(data);
        }
        else {
            pkScript.encodeZero(data);
        }
    }

    return data;
}

// JuBiter-defined
Data HcashTransaction::txSerializeWitnessValueSigning(const Script& pkScript, size_t index) const {
    auto data = std::vector<uint8_t>{};

    // Version
    encode32LE((version|TWHcashSerializeWitnessValueSigning<<16), data);

    encodeVarInt(inputs.size(), data);
    for (size_t i=0; i<inputs.size(); ++i) {
        encode64BE(dynamic_cast<HcashTransactionInput*>(inputs[i])->value, data);
        if (i == index) {
            pkScript.encode(data);
        }
        else {
            pkScript.encodeZero(data);
        }
    }

    return data;
}

// JuBiter-defined
Data HcashTransaction::getTxSerializeNoWitnessHash() const {
    auto data = txSerializeNoWitness();
    auto hash = TW::Hash::hash(hasher, data);
    return hash;
}

// JuBiter-defined
Data HcashTransaction::getTxSerializeWitnessSigningHash(const Script& pkScript, size_t index) const {
    auto data = txSerializeWitnessSigning(pkScript, index);
    auto hash = TW::Hash::hash(hasher, data);
    return hash;
}

// JuBiter-defined
Data HcashTransaction::getTxSerializeWitnessValueSigningHash(const Script& pkScript, size_t index) const {
    auto data = txSerializeWitnessValueSigning(pkScript, index);
    auto hash = TW::Hash::hash(hasher, data);
    return hash;
}

// JuBiter-defined
/// Encodes the transaction into the provided buffer.
void HcashTransaction::encode(bool witness, Data& data) const {

    encodeVersion(data);

    encodeVarInt(inputs.size(), data);
    for (auto& input : inputs) {
        input->encode(data);
    }

    encodeVarInt(outputs.size(), data);
    for (auto& output : outputs) {
        output->encode(data);
    }

    encode32LE(lockTime, data);

    encode32LE(expiry, data);

    if (witness) {
        // Use extended format in case witnesses are to be serialized.
        // TxSerializeOnlyWitness: [nInputCount][amount][InputBlockHeight][InputBlockIndex][SignatureScript]
        encodeVarInt(inputs.size(), data);
        for (size_t i=0; i<inputs.size(); ++i) {
            encode64LE(dynamic_cast<HcashTransactionInput*>(inputs[i])->value, data);
            encode32BE(dynamic_cast<HcashTransactionInput*>(inputs[i])->blockHeight, data);
            encode32BE(dynamic_cast<HcashTransactionInput*>(inputs[i])->blockIndex, data);
            inputs[i]->encodeWitness(data);
        }
    }
}

// JuBiter-defined
/// Decodes the provided buffer into the transaction.
bool HcashTransaction::decode(bool witness, const Data& data) {

    bool bSuccess = true;

// TxSerializeNoWitness: [nVersion][nInputCount][inputs][nOutputCount][outputs][nLockTime][Expiry]
// TxSerializeOnlyWitness: [nInputCount][InputBlockHeight][SignatureScript]
    int index = 0;

    // TxSerializeNoWitness
    // [nVersion]
    decodeVersion(data, index);

    // [nInputCount]
    size_t indexInc = 0;
    std::vector<uint8_t> tempInputCount(std::begin(data)+index, std::end(data));
    size_t nInputCount = decodeVarInt(tempInputCount, indexInc);
    index += indexInc;

    // [txInputs]
    for(size_t i=0; i<nInputCount; ++i) {
        indexInc = 0;
        std::vector<uint8_t> tempInput(std::begin(data)+index+indexInc, std::end(data));
        HcashTransactionInput *input = new HcashTransactionInput();
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
    std::vector<uint8_t> tempOutputCount(std::begin(data)+index, std::end(data));
    size_t nOutputCount = decodeVarInt(tempOutputCount, indexInc);
    index += indexInc;

    // [txOutputs]
    for(size_t i=0; i<nOutputCount; ++i) {
        indexInc = 0;
        std::vector<uint8_t> tempOutput(std::begin(data)+index+indexInc, std::end(data));
        HcashTransactionOutput *output = new HcashTransactionOutput();
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

    // [nLockTime]
    lockTime = decode32LE(&data[index]);
    index += sizeof(lockTime)/sizeof(uint8_t);

    // [Expiry]
    expiry = decode32LE(&data[index]);
    index += sizeof(expiry)/sizeof(uint8_t);

    // TxSerializeOnlyWitness
    // [nInputCount][InputBlockHeight][SignatureScript]
    indexInc = 0;
    std::vector<uint8_t> wtempInputCount(std::begin(data)+index, std::end(data));
    size_t wnInputCount = decodeVarInt(wtempInputCount, indexInc);
    if (wnInputCount != nInputCount) {
        bSuccess = false;
        return bSuccess;
    }
    index += indexInc;

    if (witness) {
        for (size_t i=0; i<wnInputCount; ++i) {
            indexInc = 0;

            std::vector<uint8_t> tempWitness(std::begin(data)+index+indexInc, std::end(data));
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

    return !empty();
}
