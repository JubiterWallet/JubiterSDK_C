// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TransactionOutput.h"

//#include <memory>

namespace TW::Bitcoin {

// JuBiter-defined
/// Hcash transaction output.
struct HcashTransactionOutput :
     public TransactionOutput {
    /// Transaction version.
    int16_t version = 0;

    /// Initializes an empty transaction output.
    HcashTransactionOutput() : TransactionOutput() {};

    /// Initializes a transaction output with a value and a script.
    HcashTransactionOutput(Amount value, int16_t version, Script script) : version(version), TransactionOutput(value, script) {}

    /// Encodes the output into the provided buffer.
    virtual void encode(std::vector<uint8_t>& data) const override;

    /// Decodes the provided buffer into the output.
    virtual bool decode(const std::vector<uint8_t>& data) override;

    ///
    virtual size_t size() override;
}; // struct HcashTransactionOutput end

} // namespace TW::Bitcoin

/// Wrapper for C interface.
struct TWHcashTransactionOutput {
    TW::Bitcoin::HcashTransactionOutput impl;
};
