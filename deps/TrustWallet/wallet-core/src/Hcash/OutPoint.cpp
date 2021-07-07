// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Hcash/OutPoint.h"

#include "../BinaryCoding.h"

using namespace TW::Hcash;

// JuBiter-defined
void OutPoint::encode(std::vector<uint8_t>& data) const {
    TW::Bitcoin::OutPoint::encode(data);
    data.push_back(outputTree);
}

// JuBiter-defined
/// Decodes the out-point into the provided buffer.
bool OutPoint::decode(const std::vector<uint8_t>& data) {
    TW::Bitcoin::OutPoint::decode(data);
    size_t index = TW::Bitcoin::OutPoint::size();
    outputTree = data[index];

    return true;
}

// JuBiter-defined
size_t OutPoint::size() {
    return (sizeof(outputTree)/sizeof(uint8_t)+TW::Bitcoin::OutPoint::size());
}
