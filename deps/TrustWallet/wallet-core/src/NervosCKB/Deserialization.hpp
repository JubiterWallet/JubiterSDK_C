// Copyright © 2017-2021 JuBiter Wallet.
//
// This file is part of JuBiter. The full JuBiter copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.


#ifndef Deserialization_h
#define Deserialization_h

#include "../Data.h"
#include "../BinaryCoding.h"
#include "Serialization.hpp"

#include <vector>
#include <set>
#include <string>

using namespace std;
using namespace TW;


inline std::vector<Data> deserializeFixVector(const Data& fixVector) {

    uint32_t fixVectorSize = decode32LE(&fixVector[0]);
    if (0 == fixVectorSize) {
        return {};
    }

    size_t restLength = fixVector.size()-4;
    if (restLength%fixVectorSize) {
        return {};
    }

    size_t itemLength = restLength/fixVectorSize;
    std::vector<Data> result;
    for (size_t i=0; i<fixVectorSize; ++i) {
        Data item(itemLength);
        std::copy(&fixVector[4+itemLength*i], &fixVector[4+itemLength*i+itemLength], std::begin(item));
        result.push_back(item);
    }

    return result;
}


inline std::vector<uint32_t> getSizes(const uint32_t& fullLength, const std::vector<uint32_t>& elmOffsets) {

    std::vector<uint32_t> sizes;
    for (size_t idx=1; idx<=elmOffsets.size(); ++idx) {
        if (elmOffsets.size() == idx) {
            sizes.push_back(fullLength-elmOffsets[idx-1]);
        }
        else {
            sizes.push_back(elmOffsets[idx]-elmOffsets[idx-1]);
        }
    }

    return sizes;
}


inline std::vector<uint32_t> deserializeFullLength(const Data& vFullLength, const size_t& itemCount) {

    // skip full length
    uint32_t fullLength = decode32LE(&vFullLength[0]);
    uint32_t offset = fullLengthSize;

    std::vector<uint32_t> offsetArray;
    for (size_t i=0; i<itemCount; ++i) {
        Data encode;
        uint32_t itemSize = decode32LE(&vFullLength[offset+i*offsetSize]);
        offsetArray.push_back(itemSize);
    }

    return getSizes(fullLength, offsetArray);
}


inline std::vector<Data> deserializeDynVector(const Data& dynVec, const size_t& itemCount) {

    if (dynVec.empty()) {
        return {};
    }

    uint32_t headerLength = fullLengthSize + offsetSize*(uint32_t)itemCount;

    auto vFullLength = deserializeFullLength(dynVec, itemCount);

    uint32_t offset = headerLength;
    std::vector<Data> bodys;
    for (size_t i=0; i<itemCount; ++i) {
        if (0 < i) {
            offset += vFullLength[i-1];
        }
        Data body;
        std::copy(std::begin(dynVec)+offset, std::begin(dynVec)+offset+vFullLength[i], std::back_inserter(body));
        bodys.push_back(body);
    }

    return bodys;
}


// Prerequisite: The number of subterms is known, but the length of each subterm is unknown and inconsistent.
inline std::vector<Data> deserializeTableByItemCount(const Data& data, const size_t& itemCount) {

    if (data.empty()) {
        return {};
    }

    auto vFullLength = deserializeFullLength(data, itemCount);
    if (vFullLength.empty()) {
        return {};
    }

    uint32_t fullLength = decode32LE(&data[0]);

    Data table;
    std::copy(std::begin(data), std::begin(data)+fullLength, std::back_inserter(table));

    uint32_t headerLength = fullLengthSize + offsetSize*(uint32_t)itemCount;

    uint32_t offset = headerLength;
    std::vector<Data> bodys;
    for (size_t i=0; i<itemCount; ++i) {
        if (0 < i) {
            offset += vFullLength[i-1];
        }
        Data body;
        std::copy(std::begin(table)+offset, std::begin(table)+offset+vFullLength[i], std::back_inserter(body));
        bodys.push_back(body);
    }

    return bodys;
}


// Prerequisites: Subterm length is known and consistent, but number of subterms is unknown.
inline std::vector<Data> deserializeTableByItemLength(const Data& data, const size_t& itemLength) {

    if (data.empty()) {
        return {};
    }

    uint32_t fullLength = decode32LE(&data[0]);

    Data table;
    std::copy(std::begin(data), std::begin(data)+fullLength, std::back_inserter(table));

    size_t restLength = table.size();

    std::vector<Data> bodys;
    for (size_t i=0; i<table.size()/itemLength; ++i) {
        if (restLength < itemLength) {
            break;
        }
        Data body;
        std::copy(std::end(table)-itemLength*(i+1), std::end(table)-itemLength*i, std::back_inserter(body));
        uint32_t bodyLength = decode32LE(&body[0]);
        if (bodyLength != itemLength) {
            break;
        }
        bodys.push_back(body);
        restLength -= itemLength;
    }

    return bodys;
}


#endif /* Deserialization_h */
