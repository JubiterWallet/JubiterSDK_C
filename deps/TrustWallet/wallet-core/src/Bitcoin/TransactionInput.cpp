// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransactionInput.h"

#include "../BinaryCoding.h"

using namespace TW::Bitcoin;

void TransactionInput::encode(Data& data) const {
    auto& outpoint = reinterpret_cast<const TW::Bitcoin::OutPoint&>(previousOutput);
    outpoint.encode(data);
    script.encode(data);
    encode32LE(sequence, data);
}

void TransactionInput::encodeWitness(Data& data) const {
    encodeVarInt(scriptWitness.size(), data);
    for (auto& item : scriptWitness) {
        encodeVarInt(item.size(), data);
        std::copy(std::begin(item), std::end(item), std::back_inserter(data));
    }
}

// JuBiter-defined
/// Decodes the provided buffer into the transactionInput.
bool TransactionInput::decode(const Data& data) {
    size_t index = 0;
    if (!previousOutput.decode(data)) {
        return false;
    }
    index += previousOutput.size();

    Data temp(std::begin(data)+index, std::end(data));
    if (!script.decode(temp)) {
        return false;
    }
    index += script.size();

    sequence = decode32LE(&data[index]);

    return true;
}

// JuBiter-defined
/// Decodes the provided buffer into the witness data.
bool TransactionInput::decodeWitness(const Data& data) {
    size_t index = 0;

    size_t size = 0;
    size_t witnessCnt = decodeVarInt(data, size);
    index += size;

    for (size_t i=0; i<witnessCnt; ++i) {
        Data temp(std::begin(data)+index, std::end(data));

        size_t subIndex = 0;
        size_t witnessSize = decodeVarInt(temp, size);
        subIndex += size;
        Data witness;
        std::copy(std::begin(temp)+subIndex, std::begin(temp)+subIndex+witnessSize, std::back_inserter(witness));
        scriptWitness.push_back(witness);
        subIndex += witnessSize;
        index += subIndex;
    }

    return true;
}

// JuBiter-defined
size_t TransactionInput::size() {
    return (previousOutput.size() + script.size() + sizeof(sequence)/sizeof(uint8_t));
}
