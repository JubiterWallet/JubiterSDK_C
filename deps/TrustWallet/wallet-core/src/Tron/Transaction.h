// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <nlohmann/json.hpp>

#include "Tron/TransactionRaw.h"

namespace TW::Tron {


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
class Transaction {
public:
    TransactionRaw raw_data;
    Data signature;
    Data txID;

    // JuBiter-defined
    Transaction() {
        raw_data.clear();
        signature.clear();
        txID.clear();
    }
    // JuBiter-defined
    Transaction(const Data& raw, const Data& signature)
        : signature(signature) {
        raw_data.deserialize(raw);
        txID.clear();
    }

    // JuBiter-defined
    ::protocol::Transaction to_internal();

    nlohmann::json serialize() const noexcept;
};  // class Transaction end


} // namespace TW::Tron
