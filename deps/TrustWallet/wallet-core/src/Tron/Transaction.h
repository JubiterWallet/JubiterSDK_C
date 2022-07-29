// Copyright © 2017-2022 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <nlohmann/json.hpp>

#include "Tron/TransactionRaw.h"

namespace TW::Tron {

// JuBiter-defined
class TransactionBase {
public:
    Data signature;

    TransactionBase() {
        signature.clear();
    }
    TransactionBase(const Data& signature)
        : signature(signature) {}
};  // class TransactionBase end


/// Transaction point for implementation of Tron coin.
//message Transaction {
//  message Contract {
//    ...
//  }
//
//  message Result {
//    ...
//  }
//
//  message raw {
//    ...
//  }
//
//  raw raw_data = 1;
//  // only support size = 1, repeated list here for muti-sig extension
//  repeated bytes signature = 2;
//  repeated Result ret = 5;
//}
class Transaction : public TransactionBase {
public:
    TransactionRaw raw_data;
    Data txID;

    // JuBiter-defined
    Transaction() {
        raw_data.clear();
        txID.clear();
    }
    // JuBiter-defined
    Transaction(const Data& raw, const Data& signature)
        : TransactionBase(signature) {
        raw_data.deserialize(raw);
        txID.clear();
    }

    // JuBiter-defined
    ::protocol::Transaction to_internal();

    nlohmann::json serialize() noexcept;
};  // class Transaction end


// JuBiter-defined
class Bytestring : public TransactionBase {
public:
    BytestringRaw raw_data;

    Bytestring() {
        raw_data.clear();
    }
    Bytestring(const Data& raw)
        :raw_data(raw) {}
    Bytestring(const Data& raw, const Data& signature)
        : TransactionBase(signature)
        , raw_data(raw) {}

    Data to_internal() const;

    nlohmann::json serialize() noexcept;
};  // class Bytestring end
} // namespace TW::Tron
