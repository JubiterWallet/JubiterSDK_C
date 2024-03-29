// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Hcash/TransactionInput.h"
#include "../BinaryCoding.h"

using namespace TW::Hcash;

// JuBiter-defined
void TransactionInput::encode(Data& data) const {
    TransactionInput::previousOutput.encode(data);
    encode32LE(sequence, data);
}

// JuBiter-defined
void TransactionInput::encodeWitness(Data& data) const {
    if (0 == scriptWitness.size()) {
        return;
    }

    Data witnesses;
    for (auto& item : scriptWitness) {
        encodeVarInt(item.size(), witnesses);
        std::copy(std::begin(item), std::end(item), std::back_inserter(witnesses));
    }

    encodeVarInt(witnesses.size(), data);
    std::copy(std::begin(witnesses), std::end(witnesses), std::back_inserter(data));
}

// JuBiter-defined
/// Decodes the provided buffer into the transactionInput.
bool TransactionInput::decode(const Data& data) {
    size_t index = 0;

    if (!previousOutput.decode(data)) {
        return false;
    }
    index += previousOutput.size();

    sequence = decode32LE(&data[index]);

    return true;
}

// JuBiter-defined
/// Decodes the provided buffer into the witness data.
bool TransactionInput::decodeWitness(const Data& data) {
    size_t index = 0;

    value = decode64LE(&data[index]);
    index += (sizeof(value)/sizeof(uint8_t));

    blockHeight = decode32LE(&data[index]);
    index += (sizeof(blockHeight)/sizeof(uint8_t));

    blockIndex = decode32LE(&data[index]);
    index += (sizeof(blockIndex)/sizeof(uint8_t));

    size_t size = 0;
    Data temp(std::begin(data)+index, std::end(data));
    size_t witnessSize = decodeVarInt(temp, size);
    if (0 == witnessSize) {
        return true;
    }
    index += size;

    size_t witnessCnt = 2;
    for (size_t i=0; i<witnessCnt; ++i) {
        Data tempWitness(std::begin(data)+index, std::end(data));

        size_t subIndex = 0;
        size_t witnessItemSize = decodeVarInt(tempWitness, size);
        subIndex += size;
        Data witness;
        std::copy(std::begin(tempWitness)+subIndex, std::begin(tempWitness)+subIndex+witnessItemSize, std::back_inserter(witness));
        scriptWitness.push_back(witness);
        subIndex += witnessItemSize;
        index += subIndex;

        witnessSize -= (witnessItemSize+size);
    }
    if (0 != witnessSize) {
        return false;
    }

    return true;
}

// JuBiter-defined
size_t TransactionInput::size() {
    return (previousOutput.size() + sizeof(sequence)/sizeof(uint8_t));
}

// JuBiter-defined
size_t TransactionInput::sizeWitness() {
    Data script;
    encodeWitness(script);
    return (sizeof(value)/sizeof(uint8_t) +
            sizeof(blockHeight)/sizeof(uint8_t) +
            sizeof(blockIndex)/sizeof(uint8_t) +
            script.size());
}
