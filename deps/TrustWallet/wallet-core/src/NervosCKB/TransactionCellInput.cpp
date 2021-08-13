// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../NervosCKB/TransactionCellInput.h"
#include "../NervosCKB/Serialization.hpp"
#include "../BinaryCoding.h"

using namespace TW;
using namespace TW::NervosCKB;


size_t TransactionInput::size() {

    return (sizeof(uint32_t)/sizeof(uint8_t)
            +  sizeof(since)/sizeof(uint8_t)
            + previousOutput.size());
}


Data TransactionInput::lockHash() const {

    TW::Data codeHash;
    uint8_t type = 0;
    TW::Data args;
    if (!TW::NervosCKB::Script::parseScript(lock, codeHash, type, args)) {
        return {};
    }

    return TW::NervosCKB::Script(std::begin(lock), std::end(lock)).hash();
}


void TransactionInput::encode(Data& data) const {

    data = encode();
}


Data TransactionInput::encode() const {

    Data data;
    encode64LE(since, data);
    auto& outpoint = reinterpret_cast<const TW::Bitcoin::OutPoint&>(previousOutput);
    outpoint.encode(data);

    return data;
}


/// Decodes the provided buffer into the transactionInput.
bool TransactionInput::decode(const Data& data) {
    size_t index = 0;

    if (!previousOutput.decode(data)) {
        return false;
    }
    index += previousOutput.size();

    Data temp(std::begin(data)+index, std::end(data));

    since = decode64LE(&data[index]);

    return true;
}


Data TransactionInput::serialize() const {

    return serializeFixVector(TransactionInput::serialize(*this));
}


std::vector<TW::Data> TransactionInput::serialize(const TransactionInput& input) {

    if (!input.isValid()) {
        return {};
    }

    std::vector<TW::Data> table;
    table.push_back(input.encode());

    return table;
}
