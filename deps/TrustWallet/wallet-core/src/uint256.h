// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

//#include <boost/lexical_cast.hpp>
//#include <boost/multiprecision/cpp_int.hpp>
#include <uint256_t/uint256_t.h>

namespace TW {

//using int256_t = boost::multiprecision::int256_t;
//using uint256_t = boost::multiprecision::uint256_t;

// JuBiter-defined
inline uint256_t load(const Data& data) {
    if (data.empty()) {
        return uint256_t(0);
    }
    uint256_t result;

//    std::copy(std::begin(data), std::end(data),std::back_inserter(result));
    return result;
}

/// Loads a `uint256_t` from a collection of bytes.
/// The rightmost bytes are taken from data
//inline uint256_t load(const Data& data) {
//    using boost::multiprecision::cpp_int;
//    if (data.empty()) {
//        return uint256_t(0);
//    }
//    uint256_t result;
//    import_bits(result, data.begin(), data.end());
//    return result;
//}

/// Loads a `uint256_t` from a collection of bytes.
/// The leftmost offset bytes are skipped, and the next 32 bytes are taken.  At least 32 (+offset)
/// bytes are needed.
//inline uint256_t loadWithOffset(const Data& data, size_t offset) {
//    using boost::multiprecision::cpp_int;
//    if (data.empty() || (data.size() < (256 / 8 + offset))) {
//        // not enough bytes in data
//        return uint256_t(0);
//    }
//    uint256_t result;
//    import_bits(result, data.begin() + offset, data.begin() + offset + 256 / 8);
//    return result;
//}

// JuBiter-defined
inline uint256_t load(const std::string& data, uint8_t base=10) {
//    using boost::multiprecision::cpp_int;
    if (data.empty()) {
        return uint256_t(0);
    }
    
    return uint256_t(data, base);
}

/// Loads a `uint256_t` from Protobuf bytes (which are wrongly represented as
/// std::string).
//inline uint256_t load(const std::string& data) {
//    using boost::multiprecision::cpp_int;
//    if (data.empty()) {
//        return uint256_t(0);
//    }
//    uint256_t result;
//    import_bits(result, reinterpret_cast<const byte*>(data.data()),
//                reinterpret_cast<const byte*>(data.data() + data.size()));
//    return result;
//}

// JuBiter-defined
/// Stores a `uint256_t` as a collection of bytes.
inline Data store(const uint256_t& v) {
//    using boost::multiprecision::cpp_int;
//    Data bytes;
//    bytes.reserve(32);
//    export_bits(v, std::back_inserter(bytes), 8);
    Data bytes = v.export_bits();
    return bytes;
}

// Append a uint256_t value as a big-endian byte array into the provided buffer, and limit
// the array size by digit/8.
inline void encode256BE(Data& data, const uint256_t& value, uint32_t digit) {
    Data bytes = store(value);
    Data buff(digit / 8);

    for (int i = 0; i < (int)bytes.size(); ++i) {
        int start = (int)buff.size() - (int)bytes.size() + i;
        if (start >= 0) {
            buff[start] = bytes[i];
        }
    }
    data.insert(data.end(), buff.begin(), buff.end());
}

/// Return string representation of uint256_t
inline std::string toString(uint256_t value) {
//    return boost::lexical_cast<std::string>(value);
    return value.str();
}

} // namespace TW
