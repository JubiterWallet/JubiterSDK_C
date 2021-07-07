// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Hcash/Transaction.h"
#include "../BinaryCoding.h"
#include "../Hash.h"

#include <TrustWalletCore/TWHcash.h>
#include "../Hcash/SigHashType.h"

#include <cassert>


using namespace TW;
using namespace TW::Hcash;

// JuBiter-defined
Data Transaction::getPreImage(const TW::Bitcoin::Script& scriptCode, size_t index, uint32_t hashType, uint64_t amount) const {
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
Data Transaction::txSerializeNoWitness() const {
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
Data Transaction::txSerializeOnlyWitness(const TW::Bitcoin::Script& signScript, size_t index) const {
    auto data = std::vector<uint8_t>{};

    // Version
    encode32LE((version|TWHcashSerializeOnlyWitness<<16), data);

    //not support many inputs
    encodeVarInt(inputs.size(), data);
    for (size_t i=0; i<inputs.size(); ++i) {
        encode64LE(dynamic_cast<TW::Hcash::TransactionInput*>(inputs[i])->value, data);
        encode32BE(dynamic_cast<TW::Hcash::TransactionInput*>(inputs[i])->blockHeight, data);
        encode32BE(dynamic_cast<TW::Hcash::TransactionInput*>(inputs[i])->blockIndex, data);
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
Data Transaction::txSerializeWitnessSigning(const TW::Bitcoin::Script& pkScript, size_t index) const {
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
Data Transaction::txSerializeWitnessValueSigning(const TW::Bitcoin::Script& pkScript, size_t index) const {
    auto data = std::vector<uint8_t>{};

    // Version
    encode32LE((version|TWHcashSerializeWitnessValueSigning<<16), data);

    encodeVarInt(inputs.size(), data);
    for (size_t i=0; i<inputs.size(); ++i) {
        encode64BE(dynamic_cast<TW::Hcash::TransactionInput*>(inputs[i])->value, data);
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
Data Transaction::getTxSerializeNoWitnessHash() const {
    auto data = txSerializeNoWitness();
    auto hash = TW::Hash::hash(hasher, data);
    return hash;
}

// JuBiter-defined
Data Transaction::getTxSerializeWitnessSigningHash(const TW::Bitcoin::Script& pkScript, size_t index) const {
    auto data = txSerializeWitnessSigning(pkScript, index);
    auto hash = TW::Hash::hash(hasher, data);
    return hash;
}

// JuBiter-defined
Data Transaction::getTxSerializeWitnessValueSigningHash(const TW::Bitcoin::Script& pkScript, size_t index) const {
    auto data = txSerializeWitnessValueSigning(pkScript, index);
    auto hash = TW::Hash::hash(hasher, data);
    return hash;
}

// JuBiter-defined
void Transaction::encodeVersion(Data& data) const {
    encode32LE((version|(TWHcashSerializeFull<<16)), data);
}

// JuBiter-defined
void Transaction::decodeVersion(const Data& data, int& index) {
    //Cos 0 == (TWHcashSerializeFull<<16)
    version = decode32LE(&data[0]);
    index += (sizeof(uint32_t)/sizeof(uint8_t));
}

// JuBiter-defined
/// Encodes the transaction into the provided buffer.
void Transaction::encode(bool witness, Data& data) const {

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
            encode64LE(dynamic_cast<TW::Hcash::TransactionInput*>(inputs[i])->value, data);
            encode32BE(dynamic_cast<TW::Hcash::TransactionInput*>(inputs[i])->blockHeight, data);
            encode32BE(dynamic_cast<TW::Hcash::TransactionInput*>(inputs[i])->blockIndex, data);
            inputs[i]->encodeWitness(data);
        }
    }
}

// JuBiter-defined
/// Decodes the provided buffer into the transaction.
bool Transaction::decode(bool witness, const Data& data) {

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
        TW::Hcash::TransactionInput *input = new TW::Hcash::TransactionInput();
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
        TW::Hcash::TransactionOutput *output = new TW::Hcash::TransactionOutput();
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
