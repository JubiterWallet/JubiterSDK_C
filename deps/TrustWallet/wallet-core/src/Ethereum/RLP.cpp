// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "RLP.h"

#include "../Data.h"
#include "Address.h"
#include "BinaryCoding.h"
#include "mSIGNA/stdutils/uchar_vector.h"

//#include <tuple>

using namespace TW;
using namespace TW::Ethereum;

//Data RLP::encode(const uint256_t& value) noexcept {
//    using boost::multiprecision::cpp_int;
//
//    Data bytes;
//    export_bits(value, std::back_inserter(bytes), 8);
//
//    if (bytes.empty() || (bytes.size() == 1 && bytes[0] == 0)) {
//        return {0x80};
//    }
//
//    return encode(bytes);
//}
//
Data RLP::encodeList(const Data& encoded) noexcept {
    auto result = encodeHeader(encoded.size(), 0xc0, 0xf7);
    result.reserve(result.size() + encoded.size());
    result.insert(result.end(), encoded.begin(), encoded.end());
    return result;
}

// JuBiter-defined
Data RLP::decodeList(const Data& decoded) noexcept {
    uint8_t offset = 0;
    return removeHeader(decoded, 0xc0, 0xf7, offset);
}

Data RLP::encode(const Transaction& transaction) noexcept {
    auto encoded = Data();
    append(encoded, encode(transaction.nonce));
    append(encoded, encode(transaction.gasPrice));
    append(encoded, encode(transaction.gasLimit));
    append(encoded, encode(transaction.to.bytes));
    append(encoded, encode(transaction.amount));
    append(encoded, encode(transaction.payload));
    append(encoded, encode(transaction.v));
    append(encoded, encode(transaction.r));
    append(encoded, encode(transaction.s));
    return encodeList(encoded);
}

// JuBiter-defined
/// Decodes a transaction.
Transaction RLP::decode(const Data& encoded) noexcept {

    Data toDecode = decodeList(encoded);

    Transaction transaction;

    uint8_t offset = 0;
    uint8_t headerSize = 0;
    transaction.nonce    = decode(toDecode, headerSize);
    offset += headerSize;
    offset += transaction.nonce.size();

    uchar_vector rGasPrice(toDecode.begin()+offset, toDecode.end());
    transaction.gasPrice = decode(rGasPrice, headerSize);
    offset += headerSize;
    offset += transaction.gasPrice.size();

    uchar_vector rGasLimit(toDecode.begin()+offset, toDecode.end());
    transaction.gasLimit = decode(rGasLimit, headerSize);
    offset += headerSize;
    offset += transaction.gasLimit.size();

    uchar_vector rTo(toDecode.begin()+offset, toDecode.end());
    transaction.to.bytes = decode<TW::Ethereum::Address::size>(TW::Data(rTo), headerSize);
    offset += headerSize;
    offset += transaction.to.bytes.size();

    uchar_vector rAmount(toDecode.begin()+offset, toDecode.end());
    transaction.amount = decode(rAmount, headerSize);
    offset += headerSize;
    offset += transaction.amount.size();

    uchar_vector rPayload(toDecode.begin()+offset, toDecode.end());
    transaction.payload = decode(rPayload, headerSize);
    offset += headerSize;
    offset += transaction.payload.size();

    uchar_vector rV(toDecode.begin()+offset, toDecode.end());
    transaction.v = decode(rV, headerSize);
    offset += headerSize;
    offset += transaction.v.size();

    uchar_vector rR(toDecode.begin()+offset, toDecode.end());
    transaction.r = decode(rR, headerSize);
    offset += headerSize;
    offset += transaction.r.size();

    uchar_vector rS(toDecode.begin()+offset, toDecode.end());
    transaction.s = decode(rS, headerSize);
    offset += headerSize;
    offset += transaction.s.size();

    if (offset != toDecode.size()) {
        return Transaction();
    }

    return transaction;
}

Data RLP::encode(const Data& data) noexcept {
    if (data.empty() || (data.size() == 1 && data[0] == 0)) {
        return {0x80};
    }

    if (data.size() == 1 && data[0] <= 0x7f) {
        // Fits in single byte, no header
        return data;
    }

    auto encoded = encodeHeader(data.size(), 0x80, 0xb7);
    encoded.insert(encoded.end(), data.begin(), data.end());
    return encoded;
}

Data RLP::encodeHeader(uint64_t size, uint8_t smallTag, uint8_t largeTag) noexcept {
    if (size < 56) {
        return {static_cast<uint8_t>(smallTag + size)};
    }

    const auto sizeData = putint(size);

    auto header = Data();
    header.reserve(1 + sizeData.size());
    header.push_back(largeTag + static_cast<uint8_t>(sizeData.size()));
    header.insert(header.end(), sizeData.begin(), sizeData.end());
    return header;
}

// JuBiter-defined
Data RLP::decode(const Data& data, uint8_t& offset) noexcept {
    if (data.empty() || (data[0] == 0x80)) {
        offset = 0;
        return {0};
    }

    if (data[0] <= 0x7f) {
        // Fits in single byte, no header
        offset = 0;
        return {data[0]};
    }

    return removeHeader(data, 0x80, 0xb7, offset);
}

// JuBiter-defined
Data RLP::removeHeader(const Data& header, const uint8_t smallTag, const uint8_t largeTag, uint8_t& headerSize) noexcept {

    auto size = decodeHeader(header, smallTag, largeTag, headerSize);
    if (0 == size) {
        return header;
    }

    Data decoded;
    decoded.insert(decoded.end(), header.begin()+headerSize, header.begin()+headerSize+size);
    return decoded;
}

// JuBiter-defined
uint64_t RLP::decodeHeader(const Data& header, const uint8_t smallTag, const uint8_t largeTag, uint8_t& headerSize) noexcept {

    uint8_t cnt = header[0] - smallTag;
    if ( 56 > cnt) {
        headerSize = 1;
        return cnt;
    }

    cnt = header[0] - largeTag;
    uint8_t* sizeData = new uint8_t[cnt+1];
    memset(sizeData, 0x00, cnt+1);
    for (size_t i=1; i<=cnt; ++i) {
        sizeData[i-1] = header[i];
    }
    uint64_t size = 0;
    switch (cnt) {
    case 1:
        size = sizeData[0];
        break;
    case 2:
        size = TW::decode16BE(sizeData);
        break;
    case 3:
        size = TW::decode24BE(sizeData);
        break;
    case 4:
        size = TW::decode32BE(sizeData);
        break;
    case 5:
        size = TW::decode40BE(sizeData);
        break;
    case 6:
        size = TW::decode48BE(sizeData);
        break;
    case 7:
        size = TW::decode56BE(sizeData);
        break;
    case 8:
        size = TW::decode64BE(sizeData);
        break;
    default:
        break;
    }

    delete [] sizeData; sizeData = nullptr;

    headerSize = cnt + 1;

    return size;
}

Data RLP::putint(uint64_t i) noexcept {
    // clang-format off
    if (i < (1l << 8))
        return {static_cast<uint8_t>(i)};
    if (i < (1l << 16))
        return {
            static_cast<uint8_t>(i >> 8),
            static_cast<uint8_t>(i),
        };
    if (i < (1l << 24))
        return {
            static_cast<uint8_t>(i >> 16),
            static_cast<uint8_t>(i >> 8),
            static_cast<uint8_t>(i),
        };
    if (i < (1l << 32))
        return {
            static_cast<uint8_t>(i >> 24),
            static_cast<uint8_t>(i >> 16),
            static_cast<uint8_t>(i >> 8),
            static_cast<uint8_t>(i),
        };
    if (i < (1l << 40))
        return {
            static_cast<uint8_t>(i >> 32),
            static_cast<uint8_t>(i >> 24),
            static_cast<uint8_t>(i >> 16),
            static_cast<uint8_t>(i >> 8),
            static_cast<uint8_t>(i),
        };
    if (i < (1l << 48))
        return {
            static_cast<uint8_t>(i >> 40),
            static_cast<uint8_t>(i >> 32),
            static_cast<uint8_t>(i >> 24),
            static_cast<uint8_t>(i >> 16),
            static_cast<uint8_t>(i >> 8),
            static_cast<uint8_t>(i),
        };
    if (i < (1l << 56))
        return {
            static_cast<uint8_t>(i >> 48),
            static_cast<uint8_t>(i >> 40),
            static_cast<uint8_t>(i >> 32),
            static_cast<uint8_t>(i >> 24),
            static_cast<uint8_t>(i >> 16),
            static_cast<uint8_t>(i >> 8),
            static_cast<uint8_t>(i),
        };

    return {
        static_cast<uint8_t>(i >> 56),
        static_cast<uint8_t>(i >> 48),
        static_cast<uint8_t>(i >> 40),
        static_cast<uint8_t>(i >> 32),
        static_cast<uint8_t>(i >> 24),
        static_cast<uint8_t>(i >> 16),
        static_cast<uint8_t>(i >> 8),
        static_cast<uint8_t>(i),
    };
    // clang-format on
}

// JuBiter-defined
uint64_t RLP::getint(Data i) noexcept {

    uint64_t size = 0;

    uchar_vector vSize(i);
    uint64_t s = vSize.read_uint8();
    switch (i.size()) {
    case 1:
    {
        size = s;
        break;
    }
    case 2:
    {
        size += (s << 8);
        s = vSize.read_uint8();
        size += s;
        break;
    }
    case 3:
    {
        size += (s << 16);
        s = vSize.read_uint8();
        size += (s << 8);
        s = vSize.read_uint8();
        size += s;
        break;
    }
    case 4:
    {
        size += (s << 24);
        s = vSize.read_uint8();
        size += (s << 16);
        s = vSize.read_uint8();
        size += (s << 8);
        s = vSize.read_uint8();
        size += s;
        break;
    }
    case 5:
    {
        size += (s << 32);
        s = vSize.read_uint8();
        size += (s << 24);
        s = vSize.read_uint8();
        size += (s << 16);
        s = vSize.read_uint8();
        size += (s << 8);
        s = vSize.read_uint8();
        size += s;
        break;
    }
    case 6:
    {
        size += (s << 40);
        s = vSize.read_uint8();
        size += (s << 32);
        s = vSize.read_uint8();
        size += (s << 24);
        s = vSize.read_uint8();
        size += (s << 16);
        s = vSize.read_uint8();
        size += (s << 8);
        s = vSize.read_uint8();
        size += s;
        break;
    }
    case 7:
    {
        size += (s << 48);
        s = vSize.read_uint8();
        size += (s << 40);
        s = vSize.read_uint8();
        size += (s << 32);
        s = vSize.read_uint8();
        size += (s << 24);
        s = vSize.read_uint8();
        size += (s << 16);
        s = vSize.read_uint8();
        size += (s << 8);
        s = vSize.read_uint8();
        size += s;
        break;
    }
    case 8:
    {
        size += (s << 56);
        s = vSize.read_uint8();
        size += (s << 48);
        s = vSize.read_uint8();
        size += (s << 40);
        s = vSize.read_uint8();
        size += (s << 32);
        s = vSize.read_uint8();
        size += (s << 24);
        s = vSize.read_uint8();
        size += (s << 16);
        s = vSize.read_uint8();
        size += (s << 8);
        s = vSize.read_uint8();
        size += s;
        break;
    }
    default:
        break;
    }

    return size;
}
