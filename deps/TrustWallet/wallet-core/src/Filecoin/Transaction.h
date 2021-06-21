// Copyright © 2017-2020 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <utility>

#include "Address.h"
#include "../Cbor.h"
#include "../uint256.h"
#include <uint256_t/uint256_t.h>
#include "../BinaryCoding.h"

namespace TW::Filecoin {

class Transaction {
  public:
    // Transaction version
    uint64_t version;
    // Recipient address
    Address to;
    // Sender address
    Address from;
    // Transaction nonce
    uint64_t nonce;
    // Transaction value
    uint256_t value;
    // Fee settings
    int64_t gasLimit;
    uint256_t gasFeeCap;
    uint256_t gasPremium;
    // Transaction type; 0 for simple transfers
    uint64_t method;
    // Transaction data; empty for simple transfers
    Data params;

    Transaction(Address to, Address from, uint64_t nonce, uint256_t value, int64_t gasLimit,
                uint256_t gasFeeCap, uint256_t gasPremium)
        : version(0)
        , to(std::move(to))
        , from(std::move(from))
        , nonce(nonce)
        , value(std::move(value))
        , gasLimit(gasLimit)
        , gasFeeCap(std::move(gasFeeCap))
        , gasPremium(std::move(gasPremium))
        , method(0) {}

  public:
    // JuBiter-defined
    static Data getCidPrefix();
//    // JuBiter-defined
//    Data getNonce() const;
//    // JuBiter-defined
//    Data getValue() const;
//    // JuBiter-defined
//    Data getGasFeeCap() const;
//    // JuBiter-defined
//    Data getGasPremium() const;
//    // JuBiter-defined
//    Data getGasLimit() const;
//
    // message returns the CBOR encoding of the Filecoin Message to be signed.
    Cbor::Encode message() const;

    // cid returns the raw Filecoin message CID (excluding the signature).
    Data cid() const;
    // JuBiter-defined
    static std::string cid(const Data& cid);
    // JuBiter-defined
    static std::string cid(const Transaction& tx);

    // serialize returns json ready for MpoolPush rpc
    std::string serialize(Data& signature) const;
};

} // namespace TW::Filecoin
