// Copyright © 2018-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Bitcoin/Script.h"
#include "../Bitcoin/Transaction.h"
#include "../Bitcoin/TransactionInput.h"
#include "../Bitcoin/TransactionOutput.h"
#include "../Hash.h"

#include <vector>

namespace TW::Dash {

// JuBiter-defined
struct Transaction :
    public TW::Bitcoin::Transaction {
    Transaction() : TW::Bitcoin::Transaction() {};
    Transaction(int32_t version, uint32_t lockTime, TW::Hash::Hasher hasher = TW::Hash::sha256d) :
    TW::Bitcoin::Transaction(version, lockTime, hasher) {};
    virtual ~Transaction() = default;

    /// Transaction data format version (note, this is signed)
    int16_t version = 1;

    /// Transaction data format type (note, this is signed)
    int16_t type = 0;

    // JuBiter-defined
    void encodeVersion(Data& data) const override;
    // JuBiter-defined
    void decodeVersion(const Data& data, int& index) override;
}; // struct Transaction end

} // namespace TW::Dash

// JuBiter-defined
/// Wrapper for C interface.
struct TWBitcoinDashTransaction {
    TW::Dash::Transaction impl;
}; // struct TWBitcoinDashTransaction end
