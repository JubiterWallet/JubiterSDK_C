// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Bitcoin/TransactionOutput.h"

//#include <memory>

namespace TW::Hcash {

// JuBiter-defined
/// Hcash transaction output.
struct TransactionOutput :
    public TW::Bitcoin::TransactionOutput {
    /// Transaction version.
    int16_t version = 0;

    /// Initializes an empty transaction output.
    TransactionOutput() : TW::Bitcoin::TransactionOutput() {};

    /// Initializes a transaction output with a value and a script.
    TransactionOutput(TW::Bitcoin::Amount value, int16_t version, TW::Bitcoin::Script script) : version(version), TW::Bitcoin::TransactionOutput(value, script) {}

    /// Encodes the output into the provided buffer.
    virtual void encode(std::vector<uint8_t>& data) const override;

    /// Decodes the provided buffer into the output.
    virtual bool decode(const std::vector<uint8_t>& data) override;

    ///
    virtual size_t size() override;
}; // struct TransactionOutput end

} // namespace TW::Hcash

/// Wrapper for C interface.
struct TWHcashTransactionOutput {
    TW::Hcash::TransactionOutput impl;
};
