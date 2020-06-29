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

    if (value.empty()) {
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
