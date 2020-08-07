//
//  tlv.hpp
//  JubSDK
//
//  Created by Pan Min on 2020/6/24.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef tlv_hpp
#define tlv_hpp

#include <stdio.h>
#include <vector>

typedef struct gpc_tlv_buf {

public:
    gpc_tlv_buf() {
        clear();
    }
    gpc_tlv_buf(const int t, const unsigned char& v) :
        tag(t) {
        value.push_back(v);
    }
    gpc_tlv_buf(const int t, const std::vector<unsigned char>& v) :
        tag(t),
        value(v) {}
    gpc_tlv_buf(const int t) :
        tag(t),
        value(0) {}
    gpc_tlv_buf(const std::vector<unsigned char>& v) :
        tag(0x00),
        value(v) {}
    ~gpc_tlv_buf() {
        clear();
    }

    bool operator==(const std::vector<unsigned char>& rhs) const {
        return std::equal(value.begin(), value.end(), rhs.begin());
    }
    bool operator==(const gpc_tlv_buf& rhs) const {
        if (!((*this) == rhs.value)
            ||    tag != rhs.tag
            ) {
            return false;
        }
        else {
            return true;
        }
    }
    bool operator!=(const std::vector<unsigned char>& rhs) const {
        return !((*this) == rhs);
    }
    bool operator!=(const gpc_tlv_buf& rhs) const {
        return !((*this) == rhs);
    }

    gpc_tlv_buf& operator=(const gpc_tlv_buf& rhs) {
        this->tag = rhs.tag;
        this->value.clear();
        this->value.insert(std::end(this->value), std::begin(rhs.value), std::end(rhs.value));
        return *this;
    }

    void clear() {
        tag = 0;
        value.clear();
    }
    bool empty() {
        return value.empty();
    }

    std::vector<unsigned char> encodeV();
    size_t encodeV(std::vector<unsigned char>& v);
    // push_back '00' if value is empty.
    std::vector<unsigned char> encodeTBAV();
    // push_back nothing if value is empty.
    std::vector<unsigned char> encodeLV();
    // only for value=0
    std::vector<unsigned char> encodeT();
    size_t encodeLV(std::vector<unsigned char>& lv);
    std::vector<unsigned char> encode();
    size_t encode(std::vector<unsigned char>& code);

    static size_t decodeLV(const std::vector<unsigned char>& lv, std::vector<unsigned char>& v);
    static size_t decodeTV(const int tag, const std::vector<unsigned char>& tv, std::vector<unsigned char>& v);

public:
    int tag;                /**< TLV type, e.g. TLV_UTF8_STRING. */
    std::vector<unsigned char> value;   /**< TLV data, e.g. in ASCII. */
} tlv_buf;


#endif /* tlv_hpp */
