// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransactionOutput.h"

#include "../BinaryCoding.h"

using namespace TW::Bitcoin;

void TransactionOutput::encode(std::vector<uint8_t>& data) const {
    encode64LE(value, data);
    script.encode(data);
}

// JuBiter-defined
/// Decodes the provided buffer into the output.
bool TransactionOutput::decode(const std::vector<uint8_t>& data) {

    size_t index = 0;
    value = decode64LE(&data[0]);
    index += (sizeof(value)/sizeof(uint8_t));

    std::vector<uint8_t> temp(std::begin(data)+index, std::end(data));
    return script.decode(temp);
}

// JuBiter-defined
size_t TransactionOutput::size() {
    return ((sizeof(value)/sizeof(uint8_t)) + script.size());
}
