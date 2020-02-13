// Copyright © 2018-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Script.h"
#include "Transaction.h"
#include "TransactionInputHcash.h"
#include "TransactionOutputHcash.h"
#include "../Hash.h"

#include <TrustWalletCore/TWBitcoin.h>
#include <vector>

namespace TW::Bitcoin {

// JuBiter-defined
struct HcashTransaction :
     public Transaction {

    uint32_t expiry = 0;

    /// A list of 1 or more transaction inputs or sources for coins
    std::vector<HcashTransactionInput> inputs;

    /// A list of 1 or more transaction outputs or destinations for coins
    std::vector<HcashTransactionOutput> outputs;

    HcashTransaction(TW::Hash::Hasher hasher = [](const byte* begin, const byte* end) mutable -> TW::Data {
       return TW::Hash::blake256(begin, end);
    }) : Transaction(hasher) {};
    HcashTransaction(uint32_t lockTime,
                     uint32_t expiry = 0,
                     TW::Hash::Hasher hasher = [](const byte* begin, const byte* end) mutable -> TW::Data {
                        return TW::Hash::blake256(begin, end);
                     }) : expiry(expiry),
    Transaction(lockTime, hasher) {};
    virtual ~HcashTransaction() = default;

    /// Whether the transaction is empty.
    virtual bool empty() const override { return inputs.empty() && outputs.empty(); }

    virtual size_t scopeInputCount() const override { return inputs.size(); }

    virtual TransactionInput scopeInput(size_t index) const override { return HcashTransaction::inputs[index]; }

    virtual std::vector<uint8_t> getPreImage(const Script& scriptCode, size_t index, uint32_t hashType,
                                  uint64_t amount) const override;
    std::vector<uint8_t> txSerializeNoWitness() const;
    std::vector<uint8_t> txSerializeWitnessSigning() const;
    std::vector<uint8_t> getTxSerializeNoWitnessHash() const;
    std::vector<uint8_t> getTxSerializeWitnessSigningHash() const;

    /// Encodes the transaction into the provided buffer.
    virtual void encode(bool witness, std::vector<uint8_t>& data) const override;

    /// Decodes the provided buffer into the transaction.
    virtual bool decode(bool witness, const std::vector<uint8_t>& data) override;

}; // struct HcashTransaction end

} // namespace TW::Bitcoin

/// Wrapper for C interface.
struct TWBitcoinHcashTransaction {
    TW::Bitcoin::HcashTransaction impl;
}; // struct TWBitcoinHcashTransaction end
