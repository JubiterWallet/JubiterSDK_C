// Copyright © 2017-2020 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <utility>

#include "Address.h"
#include "../Cbor.h"
//#include "../uint256.h"
#include "../BinaryCoding.h"

namespace TW::Filecoin {

class Transaction {
  public:
    // Recipient address
    Address to;
    // Sender address
    Address from;
    // Transaction nonce
    uint64_t nonce;
    // Transaction value
//    uint256_t value;
    uint64_t value;
    // Miner fee
//    uint256_t gasPrice;
    uint64_t gasPrice;
    int64_t gasLimit;
    // Transaction type; 0 for simple transfers
    uint64_t method;
    // Transaction data; empty for simple transfers
    Data params;

//    Transaction(Address to, Address from, uint64_t nonce, uint256_t value, uint256_t gprice,
//                uint256_t glimit)
    Transaction(Address to, Address from, uint64_t nonce, uint64_t value, uint64_t gprice,
                uint64_t glimit)
        : to(std::move(to))
        , from(std::move(from))
        , nonce(nonce)
        , value(std::move(value))
        , gasPrice(std::move(gprice))
        , gasLimit(std::move(glimit))
        , method(0) {}

  public:
    // JuBiter-defined
    static Data getCidPrefix();
    // JuBiter-defined
    Data getNonce() const;
    // JuBiter-defined
    Data getValue() const;
    // JuBiter-defined
    Data getGasPrice() const;
    // JuBiter-defined
    Data getGasLimit() const;

    // message returns the CBOR encoding of the Filecoin Message to be signed.
    Cbor::Encode message() const;

    // cid returns the raw Filecoin message CID (excluding the signature).
    Data cid() const;

    // serialize returns the CBOR encoding of the Filecoin SignedMessage.
    Data serialize(Data& signature) const;
};

} // namespace TW::Filecoin
