// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Bitcoin/TransactionInput.h"
#include "../Hcash/OutPoint.h"
#include "Amount.h"

namespace TW::Hcash {

// JuBiter-defined
/// Hcash transaction input.
class TransactionInput :
    public TW::Bitcoin::TransactionInput {
public:
    /// Reference to the previous transaction's output.
    TW::Hcash::OutPoint previousOutput;

    /// Transaction amount.
    TW::Bitcoin::Amount value;

    /// Reference to the previous transaction's block height
    uint32_t blockHeight = 0;

    /// Reference to the previous transaction's block index
    uint32_t blockIndex = 0xffffffff;

    TransactionInput() : TW::Bitcoin::TransactionInput() {
        //previousOutput = static_cast<TW::Hcash::OutPoint>;
        //static_cast<TW::Hcash::OutPoint&>(previousOutput);
    }

    /// Initializes a transaction input with a previous output, a script and a
    /// sequence number.
    TransactionInput(TW::Hcash::OutPoint previousOutput, TW::Bitcoin::Script script, uint32_t sequence)
        : TW::Bitcoin::TransactionInput(previousOutput, script, sequence) {}
    TransactionInput(TW::Hcash::OutPoint previousOutput, uint32_t sequence)
        : TW::Bitcoin::TransactionInput(previousOutput, TW::Bitcoin::Script(), sequence) {}

    /// Encodes the transaction into the provided buffer.
    virtual void encode(Data& data) const override;

    /// Encodes the witness data into the provided buffer.
    virtual void encodeWitness(Data& data) const override;

    /// Decodes the provided buffer into the transactionInput.
    virtual bool decode(const Data& data) override;

    /// Decodes the provided buffer into the witness data
    virtual bool decodeWitness(const Data& data) override;

    virtual size_t size() override;

    virtual size_t sizeWitness() override;
}; // class TransactionInput end

} // namespace TW::Hcash

// JuBiter-defined
/// Wrapper for C interface.
struct TWHcashTransactionInput {
    TW::Hcash::TransactionInput impl;
};
