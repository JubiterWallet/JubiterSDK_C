//
//  pb_tag.hpp
//  JubSDK
//
//  Created by Pan Min on 2020/10/16.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef pb_tag_hpp
#define pb_tag_hpp

#include <stdio.h>
#include <vector>

#include <google/protobuf/wire_format_lite.h>
#include "pb_basevarint.hpp"

//enum WireType {
//  WIRETYPE_VARINT = 0,
//  WIRETYPE_FIXED64 = 1,
//  WIRETYPE_LENGTH_DELIMITED = 2,
//  WIRETYPE_START_GROUP = 3,
//  WIRETYPE_END_GROUP = 4,
//  WIRETYPE_FIXED32 = 5,
//};
//
//// Lite alternative to FieldDescriptor::Type.  Must be kept in sync.
//enum FieldType {
//  TYPE_DOUBLE = 1,
//  TYPE_FLOAT = 2,
//  TYPE_INT64 = 3,
//  TYPE_UINT64 = 4,
//  TYPE_INT32 = 5,
//  TYPE_FIXED64 = 6,
//  TYPE_FIXED32 = 7,
//  TYPE_BOOL = 8,
//  TYPE_STRING = 9,
//  TYPE_GROUP = 10,
//  TYPE_MESSAGE = 11,
//  TYPE_BYTES = 12,
//  TYPE_UINT32 = 13,
//  TYPE_ENUM = 14,
//  TYPE_SFIXED32 = 15,
//  TYPE_SFIXED64 = 16,
//  TYPE_SINT32 = 17,
//  TYPE_SINT64 = 18,
//  MAX_FIELD_TYPE = 18,
//};

using namespace ::google::protobuf::internal;


typedef struct pb_tag {

public:
    pb_tag() {
        clear();
    }
    pb_tag(const int field_number,
           const WireFormatLite::WireType& wire_type)
        : field_number(field_number)
        , wire_type(wire_type)
    {}
    pb_tag(uint32_t tag) {
        fromTag(tag);
    }

    void clear() {
        field_number = 0;
         wire_type   = WireFormatLite::WireType::WIRETYPE_VARINT;
    }

    bool isValid() const {
        return !(0 == field_number);
    }

    size_t size(const WireFormatLite::FieldType& type) const {
        return WireFormatLite::TagSize(field_number, type);
    }

    void fromTag(uint32_t tag) {
        field_number = WireFormatLite::GetTagFieldNumber(tag);
         wire_type   = WireFormatLite::GetTagWireType(tag);
    }

    uint32_t toTag() {
        return WireFormatLite::MakeTag(field_number, wire_type);
    }

    bool toTag(std::vector<uint8_t>& enc) {
        return pb_basevarint::encode(toTag(), enc);
    }

private:
    int field_number;
    WireFormatLite::WireType wire_type;
} pb_tag;


#endif /* pb_tag_hpp */
