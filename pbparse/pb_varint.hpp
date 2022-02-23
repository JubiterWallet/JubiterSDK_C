//
//  pb_varint.hpp
//  JubSDK
//
//  Created by Pan Min on 2020/10/16.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef pb_varint_hpp
#define pb_varint_hpp

#include <stdio.h>
#include <stdint.h>
#include <vector>

#include "pb_buf.hpp"


using namespace ::google::protobuf::internal;


//Type    Meaning             Used For
//0       Varint              int32, int64, uint32, uint64, sint32, sint64, bool, enum
typedef struct pb_varint : pb_buf {

public:
    pb_varint();
    pb_varint(const int field_number,
              const WireFormatLite::FieldType& type,
              const int64_t v);
    pb_varint(const int field_number,
              const WireFormatLite::FieldType& type,
              const uint64_t v);
    pb_varint(const int field_number,
              const WireFormatLite::FieldType& type,
              const int32_t v);
    pb_varint(const int field_number,
              const WireFormatLite::FieldType& type,
              const uint32_t v);

    bool empty() const;
    bool isValid() const;

    virtual size_t size() const override;
    virtual size_t sizeTag() const override;
    virtual size_t sizeValue() const override;

private:
    bool encodeTag(const int field_number,
                   const WireFormatLite::FieldType& type);
} pb_varint;


#endif /* pb_varint_hpp */
