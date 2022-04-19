//
//  pb_length_delimited.cpp
//  JubSDK
//
//  Created by Pan Min on 2020/10/16.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include "pb_length_delimited.hpp"
#include <stdio.h>
#include <string>
#include <vector>

pb_length_delimited::pb_length_delimited() {

    clear();
}


pb_length_delimited::pb_length_delimited(const int field_number, const WireFormatLite::FieldType &type,
                                         const std::string &v) {

    clear();

    if (!encodeTag(field_number, type)) {
        return;
    }
    fieldType = type;
    if (!encodeValue(v, value)) {
        return;
    }
    if (!encodeLength()) {
        return;
    }
}


pb_length_delimited::pb_length_delimited(const int field_number, const WireFormatLite::FieldType &type,
                                         const std::vector<uint8_t> &v) {

    clear();

    if (!encodeTag(field_number, type)) {
        return;
    }
    fieldType = type;
    if (!encodeValue(v, value)) {
        return;
    }
    if (!encodeLength()) {
        return;
    }
}


bool pb_length_delimited::empty() const {

    return (0 == size()) ? true : false;
}


bool pb_length_delimited::isValid() const {

    if (0 < size()) {
        return tag.isValid() && !(0 == length.size()) && !(0 == value.size());
    }

    return true;
}


bool pb_length_delimited::has() const {

    return !(0 == length.size());
}


size_t pb_length_delimited::size() const {

    return sizeTag() + sizeLength() + sizeValue();
}


size_t pb_length_delimited::sizeTag() const {

    if (0 < sizeValue()) {
        return tag.size(fieldType);
    }

    return 0;
}


size_t pb_length_delimited::sizeLength() const {

    return length.size();
}


size_t pb_length_delimited::sizeValue() const {

    return value.size();
}


bool pb_length_delimited::encodeTag(const int field_number, const WireFormatLite::FieldType &type) {

    WireFormatLite::WireType wire_type;
    switch (type) {
        case WireFormatLite::FieldType::TYPE_STRING:
        case WireFormatLite::FieldType::TYPE_BYTES:
        case WireFormatLite::FieldType::TYPE_MESSAGE: {
            wire_type = WireFormatLite::WireType::WIRETYPE_LENGTH_DELIMITED;
        } break;
        default: {
            return false;
        }
    }

    tag = pb_tag(field_number, wire_type);

    return true;
}


bool pb_length_delimited::encodeLength() {

    if (0 < length.size()) {
        length.clear();
        length.resize(0);
    }
    uint8_t *target = &length[0];
    size_t sz = value.size();
    while (sz >= 0x80) {
        length.resize(length.size() + 1);
        if (nullptr == target) {
            target = &length[0];
        } else {
            target = &length[length.size() - 1];
        }
        *target = static_cast<uint8_t>(sz | 0x80);
        sz >>= 7;
        ++target;
    }
    length.resize(length.size() + 1);
    target = &length[length.size() - 1];
    *target = static_cast<uint8_t>(sz);

    return true;
}


bool pb_length_delimited::encodeValue(const std::string &v, std::vector<uint8_t> &enc) {

    enc.resize(v.size());
    std::copy(std::begin(v), std::end(v), std::begin(enc));

    return true;
}


bool pb_length_delimited::encodeValue(const std::vector<uint8_t> &v, std::vector<uint8_t> &enc) {

    enc.resize(v.size());
    std::copy(std::begin(v), std::end(v), std::begin(enc));

    return true;
}
