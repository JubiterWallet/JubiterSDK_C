//
//  pb_buf.hpp
//  JubSDK
//
//  Created by Pan Min on 2020/10/16.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef pb_buf_hpp
#define pb_buf_hpp

#include <stdio.h>
#include <vector>

#include "pb_tag.hpp"


using namespace ::google::protobuf::internal;


typedef struct pb_buf {

public:
    pb_buf() {
        clear();
    }

    void clear() {
        tag = 0;
        value.clear();
        length.clear();
    }

    pb_tag getTag() {
        return tag;
    }

    size_t getValueIndex() {
        return (tag.size(fieldType) + length.size());
    }

    size_t getValueIndex(size_t offset) {
        return (offset + getValueIndex());
    }

    std::vector<uint8_t> serialize() {
        std::vector<uint8_t> o;

        size_t szTag = tag.size(fieldType);
        size_t sz = szTag;
        o.resize(sz);
        if (!tag.toTag(o)) {
            o.clear();
            return o;
        }

        size_t szLength = length.size();
        if (0 < szLength) {
            sz += szLength;
            o.resize(sz);
            std::copy(std::begin(length), std::end(length), std::begin(o)+szTag);
        }

        size_t szValue = value.size();
        if (0 < szValue) {
            sz += szValue;
            o.resize(sz);
            std::copy(std::begin(value), std::end(value), std::begin(o)+szTag+szLength);
        }

        return o;
    }

protected:
    pb_tag tag;                /**< TLV type, e.g. (field_number << 3) | wire_type. */
    WireFormatLite::FieldType fieldType;
    std::vector<uint8_t> value;   /**< TLV data, e.g. in protocol-buffer encode. */
    std::vector<uint8_t> length;

    virtual size_t size() = 0;
    virtual size_t sizeTag() = 0;
    virtual size_t sizeValue() = 0;
} pb_buf;


#endif /* pb_buf_hpp */
