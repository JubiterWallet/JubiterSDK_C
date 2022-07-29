// Copyright © 2017-2022 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Tron.h"
#include "TransactionContract.hpp"


namespace TW::Tron {


/// Transaction signing output.
//message raw {
//  bytes ref_block_bytes = 1;
//  int64 ref_block_num = 3;
//  bytes ref_block_hash = 4;
//  int64 expiration = 8;
//  repeated authority auths = 9;
//  // transaction note
//  bytes data = 10;
//  //only support size = 1, repeated list here for extension
//  repeated Contract contract = 11;
//  // scripts not used
//  bytes scripts = 12;
//  int64 timestamp = 14;
//  int64 fee_limit = 18;
//}
class TransactionRaw {

public:
    TransactionRaw() {}
    TransactionRaw(const Data& ref_block_bytes,
                   const int64_t ref_block_num,
                   const Data& ref_block_hash,
                   const int64_t expiration,
                   std::vector<Authority>& authority,
                   const Data& data,
                   std::vector<TransactionContract>& contracts,
                   const Data& scripts,
                   const int64_t timestamp,
                   const int64_t fee_limit)
        : ref_block_bytes(ref_block_bytes)
        , ref_block_num(ref_block_num)
        , ref_block_hash(ref_block_hash)
        , expiration(expiration)
        , authority(authority)
        , data(data)
        , contracts(contracts)
        , scripts(scripts)
        , timestamp(timestamp)
        , fee_limit(fee_limit)
    {}
    ~TransactionRaw() {
        clear();
    }

    void clear() {
        ref_block_bytes.clear();
        ref_block_num = 0;
        ref_block_hash.clear();
        expiration = 0;
        authority.clear();
        data.clear();
        contracts.clear();
        scripts.clear();
        timestamp = 0;
        fee_limit = 0;
    }

    // JuBiter-defined
    void from_internal(const ::protocol::Transaction_raw& raw);
    // JuBiter-defined
    ::protocol::Transaction_raw to_internal() const;

    // JuBiter-defined
    void deserialize(const Data& o);
    Data serialize();

    virtual size_t contractOffset(const size_t offset);

    virtual TW::Data feeLimitSize() const;
    virtual TW::Data feeLimitOffset(const size_t offset) const;

private:
    virtual size_t contractValueIndex(const size_t index, const size_t offset);

    virtual bool calculateOffset();
    virtual bool calculateOffset(TW::Data raw);

    virtual pb_length_delimited getRefBlockBytes() const;
    virtual pb_varint getRefBlockNum() const;
    virtual pb_length_delimited getRefBlockHash() const;
    virtual pb_varint getExpiration() const;
    virtual pb_length_delimited getAuths() const;
    virtual pb_length_delimited getData() const;
    virtual TransactionContract getContract(const size_t index);
    virtual pb_length_delimited getContract();

    virtual TW::Data serializeContract(const size_t index);
    virtual TW::Data serializeContract();

    virtual pb_length_delimited getScripts() const;
    virtual pb_varint getTimestamp() const;
    virtual pb_varint getFeeLimit() const;

    virtual size_t feeLimitIndex(const size_t offset) const;

public:
    // Transaction identifier.
    Data ref_block_bytes;
    int64_t ref_block_num;              // optional
    Data ref_block_hash;
    int64_t expiration;
    std::vector<Authority> authority;   // optional, repeated
    Data data;
    std::vector<TransactionContract> contracts; // repeated
    Data scripts;       // optional
    int64_t timestamp;
    int64_t fee_limit;  // optional

private:
    size_t contrIndex = 0;

    size_t feeLimSize = 0;
    size_t feeLimIndex = 0;
};  // class TransactionRaw end


// JuBiter-defined
// https://github.com/tronprotocol/tips/issues/104
class BytestringRaw {

public:
    BytestringRaw() {}
    BytestringRaw(const Data& payload)
        : payload(payload)
    {}
    ~BytestringRaw() {
        clear();
    }

    void clear() {
        payload.clear();
    }

    Data serialize() const;

public:
    // bytestring
    Data payload;
};  // class BytestringRaw end
} // namespace TW::Tron
