// Copyright © 2018-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Script.h"
#include "Transaction.h"
#include "TransactionInput.h"
#include "TransactionOutput.h"
#include "../Hash.h"

#include <TrustWalletCore/TWBitcoin.h>
#include <vector>

namespace TW::Bitcoin {

// JuBiter-defined
struct DashTransaction :
    public Transaction {
    DashTransaction() : Transaction() {};
    DashTransaction(int32_t version, uint32_t lockTime, TW::Hash::Hasher hasher = TW::Hash::sha256d) :
    Transaction(version, lockTime, hasher) {};
    virtual ~DashTransaction() = default;

    /// Transaction data format version (note, this is signed)
    int16_t version = 1;

    /// Transaction data format type (note, this is signed)
    int16_t type = 0;

    // JuBiter-defined
    void encodeVersion(std::vector<uint8_t>& data) const override;
    // JuBiter-defined
    void decodeVersion(const std::vector<uint8_t>& data, int& index) override;
}; // struct DashTransaction end

} // namespace TW::Bitcoin

// JuBiter-defined
/// Wrapper for C interface.
struct TWBitcoinDashTransaction {
    TW::Bitcoin::DashTransaction impl;
}; // struct TWBitcoinDashTransaction end
