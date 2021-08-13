// Copyright © 2017-2021 JuBiter Wallet.
//
// This file is part of JuBiter. The full JuBiter copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.


#ifndef Serialization_h
#define Serialization_h

#include "../Data.h"
#include "../BinaryCoding.h"

#include <vector>
#include <set>
#include <string>

using namespace std;
using namespace TW;

const int     offsetSize = 4;   // 4 bytes
const int fullLengthSize = 4;   // 4 bytes


inline size_t sizeFixVector(const std::vector<Data>& fixVector) {

    size_t fixVectorLength = sizeof(uint32_t)/sizeof(uint8_t);
    for (const auto& item : fixVector) {
        fixVectorLength += item.size();
    }

    return fixVectorLength;
}


inline Data serializeFixVector(const std::vector<Data>& fixVector) {

    Data res;
    encode32LE((uint32_t)fixVector.size(), res);

    for (const auto& item : fixVector) {
        std::copy(std::begin(item), std::end(item), std::back_inserter(res));
    }

    return res;
}


inline std::vector<uint32_t> getOffsets(const Data& elmLengths) {

    uint32_t headerLength = (uint32_t)(fullLengthSize + offsetSize*elmLengths.size());
    std::vector<uint32_t> offsets;
    offsets.push_back(headerLength);
    for (int idx=0; idx<elmLengths.size(); ++idx) {
        if (idx) {
            offsets.push_back(offsets[offsets.size()-1]+elmLengths[idx-1]);
        }
    }

    return offsets;
}


inline size_t sizeFullLength(const Data& offsetArray) {

    return sizeof(uint32_t)/sizeof(uint8_t)
        + (sizeof(uint32_t)/sizeof(uint8_t))*offsetArray.size();
}


inline Data serializeFullLength(const Data& offsetArray) {

    uint32_t headerLength = fullLengthSize + offsetSize*(uint32_t)offsetArray.size();

    uint32_t bodyLength = 0;
    for (const auto& offset : offsetArray) {
        bodyLength += offset;
    }
    uint32_t fullLength = headerLength + bodyLength;

    std::vector<uint32_t> offsets = getOffsets(offsetArray);

    Data data;
    // full length
    encode32LE(fullLength, data);
    for (const auto& offset : offsets) {
        Data encode;
        encode32LE(offset, encode);
        std::copy(std::begin(encode), std::end(encode), std::back_inserter(data));
    }

    return data;
}


inline Data serializeStruct(const std::vector<Data>& st) {

    Data res;
    for (const auto& item : st) {
        std::copy(std::begin(item), std::end(item), std::back_inserter(res));
    }

    return res;
}


inline Data serializeDynVector(const std::vector<Data>& dynVec) {

    if (dynVec.empty()) {
        return {};
    }

    Data offsetArray, body;
    for (const auto& item : dynVec) {
        offsetArray.push_back(item.size());
        std::copy(std::begin(item), std::end(item), std::back_inserter(body));
    }

    Data data = serializeFullLength(offsetArray);
    std::copy(std::begin(body), std::end(body), std::back_inserter(data));

    return data;
}


inline size_t sizeTable(const std::vector<Data>& table) {

    size_t bodyLength = 0;
    Data offsetArray;
    for (const auto& item : table) {
        offsetArray.push_back(item.size());
        bodyLength += item.size();
    }

    return sizeFullLength(offsetArray) + bodyLength;
}


inline Data serializeTable(const std::vector<Data>& table) {

    if (table.empty()) {
        return {};
    }

    Data offsetArray, body;
    for (const auto& item : table) {
        offsetArray.push_back(item.size());
        std::copy(std::begin(item), std::end(item), std::back_inserter(body));
    }

    Data data = serializeFullLength(offsetArray);
    std::copy(std::begin(body), std::end(body), std::back_inserter(data));

    return data;
}


#endif /* Serialization_h */
