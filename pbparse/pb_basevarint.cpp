//
//  pb_basevarint.cpp
//  JubSDK
//
//  Created by Pan Min on 2020/10/22.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include <stdio.h>
#include "pb_basevarint.hpp"


size_t pb_basevarint::size(const  int64_t v) {

    int64_t value = v;

    size_t sz = 0;
    while (value >= 0x80) {
      value >>= 7;
        ++ sz;
    }
    sz += 1;

    return sz;
}


size_t pb_basevarint::size(const uint64_t v) {

    return size((int64_t)v);
}


size_t pb_basevarint::size(const  int32_t v) {

    return size((int64_t)v);
}


size_t pb_basevarint::size(const uint32_t v) {

    return size((int64_t)v);
}


bool pb_basevarint::encode(const  int64_t v, std::vector<uint8_t>& enc) {

    return encode((uint64_t)v, enc);
}


bool pb_basevarint::encode(const uint64_t v, std::vector<uint8_t>& enc) {

    uint64_t value = v;
    size_t sz = size(value);
    if (0 == sz) {
        return false;
    }

    enc.resize(sz);

    size_t index = 0;
    while (value >= 0x80) {
      enc[index] = static_cast<uint8_t>(value | 0x80);
      value >>= 7;
        ++ index;
    }
    enc[index] = static_cast<uint8_t>(value);

    return true;
}


bool pb_basevarint::encode(const  int32_t v, std::vector<uint8_t>& enc) {

    return encode((uint64_t)v, enc);
}


bool pb_basevarint::encode(const uint32_t v, std::vector<uint8_t>& enc) {

    return encode((uint64_t)v, enc);
}
