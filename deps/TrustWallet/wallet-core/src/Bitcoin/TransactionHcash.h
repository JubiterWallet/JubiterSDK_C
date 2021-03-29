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

#include <vector>

namespace TW::Bitcoin {

// JuBiter-defined
struct HcashTransaction :
     public Transaction {

    uint32_t expiry = 0;

    HcashTransaction(Hash::Hasher hasher = static_cast<Hash::HasherSimpleType>(Hash::blake256)) : Transaction(hasher) {}
    HcashTransaction(int32_t version,
                     uint32_t lockTime,
                     uint32_t expiry = 0,
                     Hash::Hasher hasher = static_cast<Hash::HasherSimpleType>(Hash::blake256)) : expiry(expiry),
    Transaction(version, lockTime, hasher) {}
    virtual ~HcashTransaction() = default;

    virtual Data getPreImage(const Script& scriptCode, size_t index, uint32_t hashType,
                                  uint64_t amount) const override;
    Data txSerializeNoWitness() const;
    Data txSerializeOnlyWitness(const Script& signScript, size_t index) const;
    Data txSerializeWitnessSigning(const Script& pkScript, size_t index) const;
    Data txSerializeWitnessValueSigning(const Script& pkScript, size_t index) const;
    Data getTxSerializeNoWitnessHash() const;
    Data getTxSerializeWitnessSigningHash(const Script& pkScript, size_t index) const;
    Data getTxSerializeWitnessValueSigningHash(const Script& pkScript, size_t index) const;

    // JuBiter-defined
    virtual void encodeVersion(Data& data) const override;
    // JuBiter-defined
    virtual void decodeVersion(const Data& data, int& index) override;

    /// Encodes the transaction into the provided buffer.
    virtual void encode(bool witness, Data& data) const override;

    /// Decodes the provided buffer into the transaction.
    virtual bool decode(bool witness, const Data& data) override;

}; // struct HcashTransaction end

} // namespace TW::Bitcoin

/// Wrapper for C interface.
struct TWHcashTransaction {
    TW::Bitcoin::HcashTransaction impl;
}; // struct TWHcashTransaction end
