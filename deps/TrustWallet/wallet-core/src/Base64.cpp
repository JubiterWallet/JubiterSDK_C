// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base64.h"
#include <string>

//#include <boost/algorithm/string.hpp>
//#include <boost/archive/iterators/base64_from_binary.hpp>
//#include <boost/archive/iterators/binary_from_base64.hpp>
//#include <boost/archive/iterators/transform_width.hpp>

namespace TW::Base64 {

using namespace TW;
using namespace std;

static const uint8_t from_base64[] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  62, 255,  62, 255,  63,
     52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255, 255, 255, 255, 255,
    255,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
     15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255,  63,
    255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
     41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51, 255, 255, 255, 255, 255};

static const char to_base64[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

Data decode(const string& val) {
//    using namespace boost::archive::iterators;
//    using It = transform_width<binary_from_base64<string::const_iterator>, 8, 6>;
//    return boost::algorithm::trim_right_copy_if(Data(It(begin(val)), It(end(val))),
//                                                [](char c) { return c == '\0'; });
    std::string encoded_string = val;

    // Make sure string length is a multiple of 4
    while ((encoded_string.size() % 4) != 0)
        encoded_string.push_back('=');

    size_t encoded_size = encoded_string.size();
    Data ret;
    ret.reserve(3*encoded_size/4);
    ret.clear();

    for (size_t i=0; i<encoded_size; i += 4) {
        // Get values for each group of four base 64 characters
        uint8_t b4[4] = {0x00,};
        b4[0] = (encoded_string[i+0] <= 'z') ? from_base64[encoded_string[i+0]] : 0xff;
        b4[1] = (encoded_string[i+1] <= 'z') ? from_base64[encoded_string[i+1]] : 0xff;
        b4[2] = (encoded_string[i+2] <= 'z') ? from_base64[encoded_string[i+2]] : 0xff;
        b4[3] = (encoded_string[i+3] <= 'z') ? from_base64[encoded_string[i+3]] : 0xff;

        // Transform into a group of three bytes
        uint8_t b3[3] = {0x00,};
        b3[0] = ((b4[0] & 0x3f) << 2) + ((b4[1] & 0x30) >> 4);
        b3[1] = ((b4[1] & 0x0f) << 4) + ((b4[2] & 0x3c) >> 2);
        b3[2] = ((b4[2] & 0x03) << 6) + ((b4[3] & 0x3f) >> 0);

        // Add the byte to the return value if it isn't part of an '=' character (indicated by 0xff)
        if (b4[1] != 0xff) ret.push_back(b3[0]);
        if (b4[2] != 0xff) ret.push_back(b3[1]);
        if (b4[3] != 0xff) ret.push_back(b3[2]);
    }

    return ret;
}

std::string encode(const uint8_t* buf, size_t bufLen) {
    // Calculate how many bytes that needs to be added to get a multiple of 3
    size_t missing = 0;
    size_t ret_size = bufLen;
    while ((ret_size % 3) != 0) {
        ++ret_size;
        ++missing;
    }

    // Expand the return string size to a multiple of 4
    ret_size = 4*ret_size/3;

    std::string ret;
    ret.reserve(ret_size);

    for (unsigned int i=0; i<ret_size/4; ++i) {
        // Read a group of three bytes (avoid buffer overrun by replacing with 0)
        size_t index = i*3;
        char b3[3];
        b3[0] = (index+0 < bufLen) ? buf[index+0] : 0;
        b3[1] = (index+1 < bufLen) ? buf[index+1] : 0;
        b3[2] = (index+2 < bufLen) ? buf[index+2] : 0;

        // Transform into four base 64 characters
        char b4[4];
        b4[0] =                            ((b3[0] & 0xfc) >> 2);
        b4[1] = ((b3[0] & 0x03) << 4) +    ((b3[1] & 0xf0) >> 4);
        b4[2] = ((b3[1] & 0x0f) << 2) +    ((b3[2] & 0xc0) >> 6);
        b4[3] = ((b3[2] & 0x3f) << 0);

        // Add the base 64 characters to the return value
        ret.push_back(to_base64[b4[0]]);
        ret.push_back(to_base64[b4[1]]);
        ret.push_back(to_base64[b4[2]]);
        ret.push_back(to_base64[b4[3]]);
    }

    // Replace data that is invalid (always as many as there are missing bytes)
    for (size_t i=0; i<missing; ++i)
        ret[ret_size - i - 1] = '=';

    return ret;
}

string encode(const Data& val) {
//    using namespace boost::archive::iterators;
//    using It = base64_from_binary<transform_width<Data::const_iterator, 6, 8>>;
//    auto encoded = string(It(begin(val)), It(end(val)));
//    return encoded.append((3 - val.size() % 3) % 3, '=');
    if (val.empty())
        return ""; // Avoid dereferencing buf if it's empty
    return encode(&val[0], val.size());
}

/// Convert from Base64Url format to regular
void convertFromBase64Url(string& b) {
    // '-' and '_' (Base64URL format) are changed to '+' and '/'
    // in-place replace
    size_t n = b.length();
    char* start = (char*)b.data();
    char* end = start + n;
    for(auto p = start; p < end; ++p) {
        if (*p == '-') { *p = '+'; }
        else if (*p == '_') { *p = '/'; }
    }
}

/// Convert from regular format to Base64Url
void convertToBase64Url(string& b) {
    // '+' and '/' are changed to '-' and '_' (Base64URL format)
    // in-place replace
    size_t n = b.length();
    char* start = (char*)b.data();
    char* end = start + n;
    for(auto p = start; p < end; ++p) {
        if (*p == '+') { *p = '-'; }
        else if (*p == '/') { *p = '_'; }
    }
}

Data decodeBase64Url(const string& val) {
    Data bytes;
    try {
        return decode(val);
    } catch (const exception& ex) {
        // 2nd try: Base64URL format (replaced by '-' and '_' by '+' and '/' )
        string base64Url = val;
        convertFromBase64Url(base64Url);
        return decode(base64Url);
    }
}

//string encodeBase64Url(const Data& val) {
//    using namespace boost::archive::iterators;
//    using It = base64_from_binary<transform_width<Data::const_iterator, 6, 8>>;
//    auto encoded = string(It(begin(val)), It(end(val)));
//    encoded.append((3 - val.size() % 3) % 3, '=');
//    convertToBase64Url(encoded);
//    return encoded;
//}
//
} // namespace TW::Base64
