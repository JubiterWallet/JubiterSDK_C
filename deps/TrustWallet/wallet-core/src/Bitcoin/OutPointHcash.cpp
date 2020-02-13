// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "OutPointHcash.h"

#include "../BinaryCoding.h"

using namespace TW::Bitcoin;

// JuBiter-defined
void HcashOutPoint::encode(std::vector<uint8_t>& data) const {
    OutPoint::encode(data);
    data.push_back(outputTree);
}

// JuBiter-defined
/// Decodes the out-point into the provided buffer.
bool HcashOutPoint::decode(const std::vector<uint8_t>& data) {
    OutPoint::decode(data);
    size_t index = OutPoint::size();
    outputTree = data[index];

    return true;
}

// JuBiter-defined
size_t HcashOutPoint::size() {
    return (sizeof(outputTree)/sizeof(uint8_t)+OutPoint::size());
}
