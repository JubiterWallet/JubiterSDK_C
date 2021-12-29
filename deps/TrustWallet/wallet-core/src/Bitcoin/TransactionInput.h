// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "OutPoint.h"
#include "Script.h"
#include "TransactionOutput.h"

#include <cstdint>
#include <memory>
#include <vector>

namespace TW::Bitcoin {

/// Bitcoin transaction input.
class TransactionInput {
  public:
    /// Reference to the previous transaction's output.
    OutPoint previousOutput;

    /// Transaction version as defined by the sender.
    ///
    /// Intended for "replacement" of transactions when information is updated
    /// before inclusion into a block.
    uint32_t sequence;

    /// Computational Script for confirming transaction authorization.
    Script script;

    /// Witness stack.
    std::vector<Data> scriptWitness;

    /// which output we spending
    std::unique_ptr<TransactionOutput> output;

    // JuBiter-defined
    TransactionInput() {}
    ~TransactionInput() {
        if (!scriptWitness.empty()) {
            scriptWitness.clear();
        }
    }

    /// Initializes a transaction input with a previous output, a script and a
    /// sequence number.
    TransactionInput(OutPoint previousOutput, Script script, uint32_t sequence)
        : previousOutput(std::move(previousOutput)), sequence(sequence), script(std::move(script)) {}

    void spending(uint64_t amount, Script script);

    /// Encodes the transaction into the provided buffer.
    virtual void encode(Data &data) const;

    /// Encodes the witness data into the provided buffer.
    virtual void encodeWitness(Data &data) const;

    // JuBiter-defined
    /// Decodes the provided buffer into the transactionInput.
    virtual bool decode(const Data &data);

    // JuBiter-defined
    /// Decodes the provided buffer into the witness data
    virtual bool decodeWitness(const Data &data);

    // JuBiter-defined
    virtual size_t size();

    // JuBiter-defined
    virtual size_t sizeWitness();
};

} // namespace TW::Bitcoin

/// Wrapper for C interface.
struct TWBitcoinTransactionInput {
    TW::Bitcoin::TransactionInput impl;
};
