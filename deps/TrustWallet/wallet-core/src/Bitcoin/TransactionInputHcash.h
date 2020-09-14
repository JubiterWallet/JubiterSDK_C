// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TransactionInput.h"
#include "OutPointHcash.h"
#include "Amount.h"

namespace TW::Bitcoin {

// JuBiter-defined
/// Hcash transaction input.
class HcashTransactionInput :
    public TransactionInput {
public:
    /// Reference to the previous transaction's output.
    HcashOutPoint previousOutput;

    /// Transaction amount.
    Amount value;

    /// Reference to the previous transaction's block height
    uint32_t blockHeight = 0;

    /// Reference to the previous transaction's block index
    uint32_t blockIndex = 0xffffffff;

    HcashTransactionInput() : TransactionInput() {
        //previousOutput = static_cast<TW::Bitcoin::HcashOutPoint>;
        //static_cast<TW::Bitcoin::HcashOutPoint&>(previousOutput);
    }

    /// Initializes a transaction input with a previous output, a script and a
    /// sequence number.
    HcashTransactionInput(HcashOutPoint previousOutput, Script script, uint32_t sequence)
        : TransactionInput(previousOutput, script, sequence) {}
    HcashTransactionInput(HcashOutPoint previousOutput, uint32_t sequence)
        : TransactionInput(previousOutput, Script(), sequence) {}

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
}; // class HcashTransactionInput end

} // namespace TW::Bitcoin

// JuBiter-defined
/// Wrapper for C interface.
struct TWHcashTransactionInput {
    TW::Bitcoin::HcashTransactionInput impl;
};
