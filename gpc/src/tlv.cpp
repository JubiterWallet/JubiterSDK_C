//
//  tlv.cpp
//  JubSDK
//
//  Created by Pan Min on 2020/6/24.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include <stdio.h>
#include "tlv.hpp"


/// struct gpc_tlv_buf
std::vector<unsigned char> tlv_buf::encode() {

    std::vector<unsigned char> out;

    if (   0 == tag
        && value.empty()
        ) {
        return out;
    }
    if (tag&0xFF00) {
        out.push_back((tag&0xFF00)>>8);
        out.push_back( tag&0x00FF);
    }
    else {
        out.push_back(tag);
    }
    out.push_back(value.size());
    out.insert(out.end(), value.begin(), value.end());

    return out;
}


size_t tlv_buf::encode(std::vector<unsigned char>& code) {

    code = encode();
    return code.size();
}


// push_back '00' if value is empty.
std::vector<unsigned char> tlv_buf::encodeTBAV() {

    std::vector<unsigned char> out;

    if (value.empty()) {
        out.push_back(value.size());
        return out;
    }
    out.push_back(value.size());
    out.insert(out.end(), value.begin(), value.end());

    return out;
}


// push_back nothing if value is empty.
std::vector<unsigned char> tlv_buf::encodeLV() {

    std::vector<unsigned char> out;

    if (value.empty()) {
        return out;
    }
    out.push_back(value.size());
    out.insert(out.end(), value.begin(), value.end());

    return out;
}


size_t tlv_buf::encodeLV(std::vector<unsigned char>& lv) {

    lv = encodeLV();
    return lv.size();
}


std::vector<unsigned char> tlv_buf::encodeV() {

    std::vector<unsigned char> out;

    if (value.empty()) {
        return out;
    }
    out.insert(out.end(), value.begin(), value.end());

    return out;
}


size_t tlv_buf::encodeV(std::vector<unsigned char>& v) {

    v = encodeV();
    return v.size();
}


size_t tlv_buf::decodeLV(const std::vector<unsigned char>& lv, std::vector<unsigned char>& v) {

    if (lv.size() != (lv[0]+1)) {
        return 0;
    }

    v.insert(v.end(), lv.begin()+1, lv.end());

    return v.size();
}


size_t tlv_buf::decodeTV(const int tag, const std::vector<unsigned char>& tv,
                         std::vector<unsigned char>& v) {

    std::vector<unsigned char> vTag;

    if (tag&0xFF00) {
        vTag.push_back((tag&0xFF00)>>8);
        vTag.push_back( tag&0x00FF);
    }
    else {
        vTag.push_back(tag);
    }

    if (!std::equal(tv.begin(), tv.begin()+vTag.size(), std::begin(vTag))) {
        return 0;
    }

    v.clear();
    v.insert(v.end(), tv.begin()+vTag.size(), tv.end());

    return (tv.size()-vTag.size());
}
