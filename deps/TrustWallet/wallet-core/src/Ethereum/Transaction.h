// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
//#include "../uint256.h"

namespace TW::Ethereum {

class Transaction {
  public:
//    uint256_t nonce;
//    uint256_t gasPrice;
//    uint256_t gasLimit;
    Data nonce;
    Data gasPrice;
    Data gasLimit;
    Address to;
//    uint256_t amount;
    Data amount;
    std::vector<uint8_t> payload;

    // Signature values
//    uint256_t v = uint256_t();
//    uint256_t r = uint256_t();
//    uint256_t s = uint256_t();
    Data v;
    Data r;
    Data s;

    // JuBiter-defined
    Transaction() {}

//    Transaction(uint256_t nonce, uint256_t gasPrice, uint256_t gasLimit, Address to, uint256_t amount, Data payload)
    Transaction(Data nonce, Data gasPrice, Data gasLimit, Address to, Data amount, Data payload)
        : nonce(std::move(nonce))
        , gasPrice(std::move(gasPrice))
        , gasLimit(std::move(gasLimit))
        , to(std::move(to))
        , amount(std::move(amount))
        , payload(std::move(payload)){}

    // JuBiter-defined
    bool isValid() {
        if (   0 == nonce.size()
            || 0 == gasPrice.size()
            || 0 == gasLimit.size()
            || !Address::isValid(to.string())
            || 0 == amount.size()
            || 0 == payload.size()
            ) {
            return false;
        }

        return true;
    }
};

} // namespace TW::Ethereum
