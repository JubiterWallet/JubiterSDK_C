// Copyright © 2018-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Bitcoin/Script.h"
#include "../Bitcoin/Transaction.h"
#include "../Hcash/TransactionInput.h"
#include "../Hcash/TransactionOutput.h"
#include "../Hash.h"

#include <vector>

namespace TW::Hcash {

// JuBiter-defined
struct Transaction :
    public TW::Bitcoin::Transaction {

    uint32_t expiry = 0;

    Transaction(Hash::Hasher hasher = static_cast<Hash::HasherSimpleType>(Hash::blake256)) : TW::Bitcoin::Transaction(hasher) {}
    Transaction(int32_t version,
                uint32_t lockTime,
                uint32_t expiry = 0,
                Hash::Hasher hasher = static_cast<Hash::HasherSimpleType>(Hash::blake256)) : expiry(expiry),
    TW::Bitcoin::Transaction(version, lockTime, hasher) {}
    virtual ~Transaction() = default;

    virtual Data getPreImage(const TW::Bitcoin::Script& scriptCode, size_t index, uint32_t hashType,
                                  uint64_t amount) const override;
    Data txSerializeNoWitness() const;
    Data txSerializeOnlyWitness(const TW::Bitcoin::Script& signScript, size_t index) const;
    Data txSerializeWitnessSigning(const TW::Bitcoin::Script& pkScript, size_t index) const;
    Data txSerializeWitnessValueSigning(const TW::Bitcoin::Script& pkScript, size_t index) const;
    Data getTxSerializeNoWitnessHash() const;
    Data getTxSerializeWitnessSigningHash(const TW::Bitcoin::Script& pkScript, size_t index) const;
    Data getTxSerializeWitnessValueSigningHash(const TW::Bitcoin::Script& pkScript, size_t index) const;

    // JuBiter-defined
    virtual void encodeVersion(Data& data) const override;
    // JuBiter-defined
    virtual void decodeVersion(const Data& data, int& index) override;

    /// Encodes the transaction into the provided buffer.
    virtual void encode(bool witness, Data& data) const override;

    /// Decodes the provided buffer into the transaction.
    virtual bool decode(bool witness, const Data& data) override;

}; // struct Transaction end

} // namespace TW::Hcash

/// Wrapper for C interface.
struct TWHcashTransaction {
    TW::Hcash::Transaction impl;
}; // struct TWHcashTransaction end
