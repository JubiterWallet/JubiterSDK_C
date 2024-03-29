// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <algorithm>

/*
 * Nullability qualifiers: currently only supported by Clang.
 */
#if !(defined(__clang__) /*&& __has_feature(nullability)*/)
#define    _Nonnull
#define    _Nullable
#define    _Null_unspecified
#define    __NULLABILITY_PRAGMA_PUSH
#define    __NULLABILITY_PRAGMA_POP
#else
#define    __NULLABILITY_PRAGMA_PUSH _Pragma("clang diagnostic push")    \
    _Pragma("clang diagnostic ignored \"-Wnullability-completeness\"")
#define    __NULLABILITY_PRAGMA_POP _Pragma("clang diagnostic pop")
#endif

namespace TW {

/// Encodes a 16-bit little-endian value into the provided buffer.
inline void encode16LE(uint16_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(val));
    data.push_back(static_cast<uint8_t>(val >> 8));
}

/// Decodes a 16-bit little-endian value from the provided buffer.
inline uint16_t decode16LE(const uint8_t* _Nonnull src) {
    return static_cast<uint16_t>((src[0]) | ((uint16_t)(src[1]) << 8));
}

// JuBiter-defined
/// Encodes a 24-bit little-endian value into the provided buffer.
inline void encode24LE(uint32_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(val));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
}

// JuBiter-defined
/// Decodes a 24-bit little-endian value from the provided buffer.
inline uint32_t decode24LE(const uint8_t* _Nonnull src) {
    // clang-format off
    return static_cast<uint32_t>(src[0])
        | (static_cast<uint32_t>(src[1]) << 8)
        | (static_cast<uint32_t>(src[2]) << 16);
    // clang-format on
}

/// Encodes a 32-bit little-endian value into the provided buffer.
inline void encode32LE(uint32_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(val));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 24)));
}

/// Decodes a 32-bit little-endian value from the provided buffer.
inline uint32_t decode32LE(const uint8_t* _Nonnull src) {
    // clang-format off
    return static_cast<uint32_t>(src[0])
        | (static_cast<uint32_t>(src[1]) << 8)
        | (static_cast<uint32_t>(src[2]) << 16)
        | (static_cast<uint32_t>(src[3]) << 24);
    // clang-format on
}

// JuBiter-defined
/// Encodes a 40-bit little-endian value into the provided buffer.
inline void encode40LE(uint64_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(val));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 24)));
    data.push_back(static_cast<uint8_t>((val >> 32)));
}

// JuBiter-defined
/// Decodes a 40-bit little-endian value from the provided buffer.
inline uint64_t decode40LE(const uint8_t* _Nonnull src) {
    // clang-format off
    return static_cast<uint64_t>(src[0])
        | (static_cast<uint64_t>(src[1]) << 8)
        | (static_cast<uint64_t>(src[2]) << 16)
        | (static_cast<uint64_t>(src[3]) << 24)
        | (static_cast<uint64_t>(src[4]) << 32);
    // clang-format on
}

// JuBiter-defined
/// Encodes a 48-bit little-endian value into the provided buffer.
inline void encode48LE(uint64_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(val));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 24)));
    data.push_back(static_cast<uint8_t>((val >> 32)));
    data.push_back(static_cast<uint8_t>((val >> 40)));
}

// JuBiter-defined
/// Decodes a 48-bit little-endian value from the provided buffer.
inline uint64_t decode48LE(const uint8_t* _Nonnull src) {
    // clang-format off
    return static_cast<uint64_t>(src[0])
        | (static_cast<uint64_t>(src[1]) << 8)
        | (static_cast<uint64_t>(src[2]) << 16)
        | (static_cast<uint64_t>(src[3]) << 24)
        | (static_cast<uint64_t>(src[4]) << 32)
        | (static_cast<uint64_t>(src[5]) << 40);
    // clang-format on
}

// JuBiter-defined
/// Encodes a 56-bit little-endian value into the provided buffer.
inline void encode56LE(uint64_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(val));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 24)));
    data.push_back(static_cast<uint8_t>((val >> 32)));
    data.push_back(static_cast<uint8_t>((val >> 40)));
    data.push_back(static_cast<uint8_t>((val >> 48)));
}

// JuBiter-defined
/// Decodes a 56-bit little-endian value from the provided buffer.
inline uint64_t decode56LE(const uint8_t* _Nonnull src) {
    // clang-format off
    return static_cast<uint64_t>(src[0])
        | (static_cast<uint64_t>(src[1]) << 8)
        | (static_cast<uint64_t>(src[2]) << 16)
        | (static_cast<uint64_t>(src[3]) << 24)
        | (static_cast<uint64_t>(src[4]) << 32)
        | (static_cast<uint64_t>(src[5]) << 40)
        | (static_cast<uint64_t>(src[6]) << 48);
    // clang-format on
}

/// Encodes a 64-bit little-endian value into the provided buffer.
inline void encode64LE(uint64_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(val));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 24)));
    data.push_back(static_cast<uint8_t>((val >> 32)));
    data.push_back(static_cast<uint8_t>((val >> 40)));
    data.push_back(static_cast<uint8_t>((val >> 48)));
    data.push_back(static_cast<uint8_t>((val >> 56)));
}

inline std::vector<uint8_t> encode64LE(uint64_t val) {
    std::vector<uint8_t> data;
    encode64LE(val, data);
    return data;
}

/// Decodes a 64-bit little-endian value from the provided buffer.
inline uint64_t decode64LE(const uint8_t* _Nonnull src) {
    // clang-format off
    return static_cast<uint64_t>(src[0])
        | (static_cast<uint64_t>(src[1]) << 8)
        | (static_cast<uint64_t>(src[2]) << 16)
        | (static_cast<uint64_t>(src[3]) << 24)
        | (static_cast<uint64_t>(src[4]) << 32)
        | (static_cast<uint64_t>(src[5]) << 40)
        | (static_cast<uint64_t>(src[6]) << 48)
        | (static_cast<uint64_t>(src[7]) << 56);
    // clang-format on
}

/// Returns the number of bytes it would take to serialize the provided value
/// as a variable-length integer (varint).
inline std::size_t varIntSize(std::size_t value) {
    // The value is small enough to be represented by itself.
    if (value < 0xfd) {
        return 1;
    }

    // Discriminant 1 byte plus 2 bytes for the uint16.
    if (value <= UINT16_MAX) {
        return 3;
    }

    // Discriminant 1 byte plus 4 bytes for the uint32.
    if (value <= UINT32_MAX) {
        return 5;
    }

    // Discriminant 1 byte plus 8 bytes for the uint64.
    return 9;
}

/// Encodes a value as a variable-length integer.
///
/// A variable-length integer (varint) is an encoding for integers up to a max
/// value of 2^64-1 that uses a variable number of bytes depending on the value
/// being encoded. It produces fewer bytes for smaller numbers as opposed to a
/// fixed-size encoding.
///
/// @returns the number of bytes written.
inline std::size_t encodeVarInt(std::size_t size, std::vector<uint8_t>& data) {
    if (size < 0xfd) {
        data.push_back(static_cast<uint8_t>(size));
        return 1;
    }

    if (size <= UINT16_MAX) {
        data.push_back(0xfd);
        encode16LE((uint16_t)size, data);
        return 3;
    }

    if (size <= UINT32_MAX) {
        data.push_back(0xfe);
        encode32LE((uint32_t)size, data);
        return 5;
    }

    data.push_back(0xff);
    encode64LE((uint64_t)size, data);
    return 9;
}

// JuBiter-defined
/// Decodes a variable-length integer  as a value.
///
/// @returns the number of bytes.
inline std::size_t decodeVarInt(const std::vector<uint8_t>& data, std::size_t& size) {
    uint8_t tag = data[0];

    switch (tag) {
        case 0xfd:
            size = 3;
            return decode16LE(&data[1]);
        case 0xfe:
            size = 5;
            return decode32LE(&data[1]);
        case 0xff:
            size = 9;
            return decode64LE(&data[1]);
        default:
            size = 1;
            return tag;
    }
}

/// Encodes a 16-bit big-endian value into the provided buffer.
inline void encode16BE(uint16_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(val >> 8));
    data.push_back(static_cast<uint8_t>(val));
}

/// Decodes a 16-bit big-endian value from the provided buffer.
inline uint16_t decode16BE(const uint8_t* _Nonnull src) {
    return static_cast<uint16_t>((src[1]) | ((uint16_t)(src[0]) << 8));
}

// JuBiter-defined
/// Encodes a 24-bit big-endian value into the provided buffer.
inline void encode24BE(uint32_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>(val));
}

// JuBiter-defined
/// Decodes a 24-bit big-endian value from the provided buffer.
inline uint32_t decode24BE(const uint8_t* _Nonnull src) {
    // clang-format off
    return static_cast<uint32_t>(src[2])
        | (static_cast<uint32_t>(src[1]) << 8)
        | (static_cast<uint32_t>(src[0]) << 16);
    // clang-format on
}

/// Encodes a 32-bit big-endian value into the provided buffer.
inline void encode32BE(uint32_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>((val >> 24)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>(val));
}

/// Decodes a 32-bit big-endian value from the provided buffer.
inline uint32_t decode32BE(const uint8_t* _Nonnull src) {
    // clang-format off
    return static_cast<uint32_t>(src[3])
        | (static_cast<uint32_t>(src[2]) << 8)
        | (static_cast<uint32_t>(src[1]) << 16)
        | (static_cast<uint32_t>(src[0]) << 24);
    // clang-format on
}

// JuBiter-defined
/// Encodes a 40-bit big-endian value into the provided buffer.
inline void encode40BE(uint64_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>((val >> 32)));
    data.push_back(static_cast<uint8_t>((val >> 24)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>(val));
}

// JuBiter-defined
/// Decodes a 40-bit big-endian value from the provided buffer.
inline uint64_t decode40BE(const uint8_t* _Nonnull src) {
    // clang-format off
    return static_cast<uint64_t>(src[4])
        | (static_cast<uint64_t>(src[3]) << 8)
        | (static_cast<uint64_t>(src[2]) << 16)
        | (static_cast<uint64_t>(src[1]) << 24)
        | (static_cast<uint64_t>(src[0]) << 32)
;
    // clang-format on
}

// JuBiter-defined
/// Encodes a 48-bit big-endian value into the provided buffer.
inline void encode48BE(uint64_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>((val >> 40)));
    data.push_back(static_cast<uint8_t>((val >> 32)));
    data.push_back(static_cast<uint8_t>((val >> 24)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>(val));
}

// JuBiter-defined
/// Decodes a 48-bit big-endian value from the provided buffer.
inline uint64_t decode48BE(const uint8_t* _Nonnull src) {
    // clang-format off
    return static_cast<uint64_t>(src[5])
        | (static_cast<uint64_t>(src[4]) << 8)
        | (static_cast<uint64_t>(src[3]) << 16)
        | (static_cast<uint64_t>(src[2]) << 24)
        | (static_cast<uint64_t>(src[1]) << 32)
        | (static_cast<uint64_t>(src[0]) << 40);
    // clang-format on
}

// JuBiter-defined
/// Encodes a 56-bit big-endian value into the provided buffer.
inline void encode56BE(uint64_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>((val >> 48)));
    data.push_back(static_cast<uint8_t>((val >> 40)));
    data.push_back(static_cast<uint8_t>((val >> 32)));
    data.push_back(static_cast<uint8_t>((val >> 24)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>(val));
}

// JuBiter-defined
/// Decodes a 56-bit big-endian value from the provided buffer.
inline uint64_t decode56BE(const uint8_t* _Nonnull src) {
    // clang-format off
    return static_cast<uint64_t>(src[6])
        | (static_cast<uint64_t>(src[5]) << 8)
        | (static_cast<uint64_t>(src[4]) << 16)
        | (static_cast<uint64_t>(src[3]) << 24)
        | (static_cast<uint64_t>(src[2]) << 32)
        | (static_cast<uint64_t>(src[1]) << 40)
        | (static_cast<uint64_t>(src[0]) << 48);
    // clang-format on
}

/// Encodes a 64-bit big-endian value into the provided buffer.
inline void encode64BE(uint64_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>((val >> 56)));
    data.push_back(static_cast<uint8_t>((val >> 48)));
    data.push_back(static_cast<uint8_t>((val >> 40)));
    data.push_back(static_cast<uint8_t>((val >> 32)));
    data.push_back(static_cast<uint8_t>((val >> 24)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>(val));
}

/// Decodes a 64-bit big-endian value from the provided buffer.
inline uint64_t decode64BE(const uint8_t* _Nonnull src) {
    // clang-format off
    return static_cast<uint64_t>(src[7])
        | (static_cast<uint64_t>(src[6]) << 8)
        | (static_cast<uint64_t>(src[5]) << 16)
        | (static_cast<uint64_t>(src[4]) << 24)
        | (static_cast<uint64_t>(src[3]) << 32)
        | (static_cast<uint64_t>(src[2]) << 40)
        | (static_cast<uint64_t>(src[1]) << 48)
        | (static_cast<uint64_t>(src[0]) << 56);
    // clang-format on
}

// JuBiter-defined
/// Encodes a 64-bit big-endian value into the provided buffer.
template<typename T>
std::vector<uint8_t> encodeBENoZero(T value) {
    std::vector<uint8_t> data;
    do {
        uint8_t v = value%256;
        // push v
        data.push_back(v);
        value /= 256;
    } while(value > 0);
    std::reverse(data.begin(), data.end());

    return data;
}

template <typename T> T decodeBENoZero(const std::vector<uint8_t> &src) {
    T v = 0;
    for (auto c: src) {
        v <<= 8;
        v += c;
    }
    return v;
}

} // namespace TW
