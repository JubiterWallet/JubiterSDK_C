// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <array>
#include <algorithm>

namespace TW {

using byte = std::uint8_t;
using Data = std::vector<byte>;

inline void pad_left(Data& data, const uint32_t size) {
    data.insert(data.begin(), size - data.size(), 0);
}

inline Data data(const std::string& data) {
    return std::vector<byte>(data.begin(), data.end());
}

inline Data data(const byte* data, size_t size) {
    return std::vector<byte>(data, data + size);
}

inline void append(Data& data, const Data& suffix) {
    data.insert(data.end(), suffix.begin(), suffix.end());
}

inline void append(Data& data, const byte suffix) {
    data.push_back(suffix);
}

/// Return a part (subdata) of the requested size of the input data.
Data subData(const Data& data, size_t index, size_t length);

/// Determines if a byte array has a specific prefix.
template <typename T>
inline bool has_prefix(const Data& data, T& prefix) {
    // using c++11 instead of c++14
//    return std::equal(prefix.begin(), prefix.end(), data.begin(), data.begin() + std::min(data.size(), prefix.size()));
//    Data vPrefix(data.begin() + std::min(data.size(), prefix.size()));
    Data vData(data.begin(), data.begin() + std::min(data.size(), prefix.size()));
    return (prefix == vData);
}

// JuBiter-defined
inline std::vector<std::size_t> find_all_indexes(const Data& haystack, const Data& needle) {
    std::vector<std::size_t> indexes{};
    auto it{haystack.begin()};
    while ((it = std::search(it, haystack.end(), needle.begin(), needle.end())) != haystack.end())
        indexes.push_back(std::distance(haystack.begin(), it++));
    return indexes;
}

} // namespace TW
