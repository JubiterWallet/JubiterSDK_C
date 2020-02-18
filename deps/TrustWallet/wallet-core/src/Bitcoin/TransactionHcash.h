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

    virtual std::vector<uint8_t> getPreImage(const Script& scriptCode, size_t index, uint32_t hashType,
                                  uint64_t amount) const override;
    std::vector<uint8_t> txSerializeNoWitness() const;
    std::vector<uint8_t> txSerializeOnlyWitness(const Script& signScript, size_t index) const;
    std::vector<uint8_t> txSerializeWitnessSigning(const Script& pkScript, size_t index) const;
    std::vector<uint8_t> txSerializeWitnessValueSigning(const Script& pkScript, size_t index) const;
    std::vector<uint8_t> getTxSerializeNoWitnessHash() const;
    std::vector<uint8_t> getTxSerializeWitnessSigningHash(const Script& pkScript, size_t index) const;
    std::vector<uint8_t> getTxSerializeWitnessValueSigningHash(const Script& pkScript, size_t index) const;

    /// Encodes the transaction into the provided buffer.
    virtual void encode(bool witness, std::vector<uint8_t>& data) const override;

    /// Decodes the provided buffer into the transaction.
    virtual bool decode(bool witness, const std::vector<uint8_t>& data) override;

}; // struct HcashTransaction end

} // namespace TW::Bitcoin

/// Wrapper for C interface.
struct TWHcashTransaction {
    TW::Bitcoin::HcashTransaction impl;
}; // struct TWHcashTransaction end
