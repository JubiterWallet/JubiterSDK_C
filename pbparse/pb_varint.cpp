//
//  pb_varint.cpp
//  JubSDK
//
//  Created by Pan Min on 2020/10/16.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include <stdio.h>
#include "pb_varint.hpp"


pb_varint::pb_varint() {

    clear();
}


pb_varint::pb_varint(const int field_number,
                     const WireFormatLite::FieldType& type,
                     const int64_t v) {

    clear();

    if (WireFormatLite::FieldType::TYPE_INT64 != type) {
        return;
    }

    if (!encodeTag(field_number, type)) {
        return;
    }
    fieldType = type;
    if (!pb_basevarint::encode(v, value)) {
        return;
    }
}


pb_varint::pb_varint(const int field_number,
                     const WireFormatLite::FieldType& type,
                     const uint64_t v) {

    clear();

    if (WireFormatLite::FieldType::TYPE_UINT64 != type) {
        return;
    }

    if (!encodeTag(field_number, type)) {
        return;
    }
    fieldType = type;
    if (!pb_basevarint::encode(v, value)) {
        return;
    }
}


pb_varint::pb_varint(const int field_number,
                     const WireFormatLite::FieldType& type,
                     const int32_t v) {

    clear();

    if (WireFormatLite::FieldType::TYPE_INT32  != type) {
        return;
    }

    if (!encodeTag(field_number, type)) {
        return;
    }
    fieldType = type;
    if (!pb_basevarint::encode(v, value)) {
        return;
    }
}


pb_varint::pb_varint(const int field_number,
                     const WireFormatLite::FieldType& type,
                     const uint32_t v) {

    clear();

    if (WireFormatLite::FieldType::TYPE_UINT32  != type) {
        return;
    }

    if (!encodeTag(field_number, type)) {
        return;
    }
    fieldType = type;
    if (!pb_basevarint::encode(v, value)) {
        return;
    }
}


bool pb_varint::empty() {

    return (0 == size()) ? true : false;
}


bool pb_varint::isValid() {

    if (0 < size()) {
        return tag.isValid();
    }

    return true;
}


size_t pb_varint::size() {

    return sizeTag()
        +  sizeValue();
}


size_t pb_varint::sizeTag() {

    if (0 < sizeValue()) {
        return tag.size(fieldType);
    }

    return 0;
}


size_t pb_varint::sizeValue() {

    return value.size();
}


bool pb_varint::encodeTag(const int field_number,
                          const WireFormatLite::FieldType& type) {

    WireFormatLite::WireType wire_type;
    switch (type) {
    case WireFormatLite::FieldType::TYPE_INT64:
    case WireFormatLite::FieldType::TYPE_UINT64:
    case WireFormatLite::FieldType::TYPE_INT32:
    case WireFormatLite::FieldType::TYPE_BOOL:
    case WireFormatLite::FieldType::TYPE_UINT32:
    case WireFormatLite::FieldType::TYPE_SINT32:
    case WireFormatLite::FieldType::TYPE_SINT64:
        wire_type = WireFormatLite::WireType::WIRETYPE_VARINT;
        break;
    default:
        return false;
    }

    tag = pb_tag(field_number, wire_type);

    return true;
}
