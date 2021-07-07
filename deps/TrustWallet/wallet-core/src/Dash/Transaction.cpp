// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Dash/Transaction.h"
#include "../BinaryCoding.h"
#include "../Hash.h"

#include <cassert>

using namespace TW;
using namespace TW::Dash;

// JuBiter-defined
void Transaction::encodeVersion(Data& data) const {
    encode16LE(version, data);
    encode16LE(type, data);
}

// JuBiter-defined
void Transaction::decodeVersion(const Data& data, int& index) {
    version = decode16LE(&data[index]);
    index += (sizeof(version)/sizeof(uint8_t));
    type = decode32LE(&data[index]);
    index += (sizeof(type)/sizeof(uint8_t));
}
