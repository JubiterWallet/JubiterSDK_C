//
//  pb_length_delimited.hpp
//  JubSDK
//
//  Created by Pan Min on 2020/10/16.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef pb_length_delimited_hpp
#define pb_length_delimited_hpp

#include <stdint.h>
#include <stdio.h>
#include <vector>

#include "pb_buf.hpp"
#include <string>

using namespace ::google::protobuf::internal;

// Type    Meaning             Used For
// 2       Length-delimited    string, bytes, embedded messages, packed repeated fields
typedef struct pb_length_delimited : pb_buf {

public:
    pb_length_delimited();
    pb_length_delimited(const int field_number, const WireFormatLite::FieldType &type, const std::string &v);
    pb_length_delimited(const int field_number, const WireFormatLite::FieldType &type, const std::vector<uint8_t> &v);

    bool empty() const;
    bool isValid() const;
    bool has() const;

    virtual size_t size() const override;
    virtual size_t sizeTag() const override;
    size_t sizeLength() const;
    virtual size_t sizeValue() const override;

private:
    bool encodeTag(const int field_number, const WireFormatLite::FieldType &type);

    bool encodeLength();

    bool encodeValue(const std::string &v, std::vector<uint8_t> &enc);
    bool encodeValue(const std::vector<uint8_t> &v, std::vector<uint8_t> &enc);
} pb_length_delimited;

#endif /* pb_length_delimited_hpp */
