// Copyright © 2017-2021 JuBiter Wallet.
//
// This file is part of JuBiter. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Bitcoin/OutPoint.h"
#include "../NervosCKB/Script.h"
#include "../Data.h"

#include <vector>

namespace TW::NervosCKB {

/// Bitcoin transaction input.
class TransactionInput {
  public:
    /// Reference to the previous transaction's output.
    TW::Bitcoin::OutPoint previousOutput;

    /// Transaction version as defined by the sender.
    ///
    /// Intended for "replacement" of transactions when information is updated
    /// before inclusion into a block.
    uint64_t since;

    Data lock;

    TransactionInput() {}
    /// Initializes a transaction input with a previous output, a script and a
    /// sequence number.
    TransactionInput(TW::Bitcoin::OutPoint previousOutput, uint64_t since, Data lock)
        : previousOutput(std::move(previousOutput)), since(since), lock(std::move(lock)) {}

   ~TransactionInput() {
       since = 0;
   }

    bool isValid() const {
        return previousOutput.isValid();
    }

    virtual size_t size();

    virtual Data lockHash() const;

    /// Encodes the transaction into the provided buffer.
    virtual void encode(Data& data) const;
    virtual Data encode() const;

    /// Decodes the provided buffer into the transactionInput.
    virtual bool decode(const Data& data);

    /// Serializes the input into the provided buffer.
    virtual Data serialize() const;

    static std::vector<TW::Data> serialize(const TransactionInput& input);
};

} // namespace TW::NervosCKB

/// Wrapper for C interface.
struct TWNervosCKBTransactionInput {
    TW::NervosCKB::TransactionInput impl;
};
