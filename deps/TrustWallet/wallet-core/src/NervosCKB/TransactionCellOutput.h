// Copyright © 2017-2020 JuBiter Wallet.
//
// This file is part of JuBiter. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../NervosCKB/Script.h"
#include "../NervosCKB/Serialization.hpp"
#include "../NervosCKB/Deserialization.hpp"

#include <memory>

namespace TW::NervosCKB {

/// Bitcoin transaction output.
struct TransactionOutput {
    static size_t TXOUTPUT_ITEM_COUNT;

    /// Transaction amount.
    uint64_t capacity;

    /// Usually contains the public key as a Bitcoin script setting up
    /// conditions to claim this output.
    Data lock;

    ///
    Data type;

    ///
    Data data;

    /// Initializes an empty transaction output.
    TransactionOutput() = default;

    /// Initializes a transaction output with a value and a script.
    TransactionOutput(uint64_t capacity, Data lock, Data type, Data data) : capacity(capacity), lock(std::move(lock)), type(std::move(type)), data(std::move(data)) {}

    ~TransactionOutput() {
        capacity = 0;
        if (!lock.empty()) {
            lock.clear();
        }
        if (!type.empty()) {
            type.clear();
        }
        if (!data.empty()) {
            data.clear();
        }
    }

    bool isValid() const {
        return !lock.empty();
    }

    ///
    virtual size_t size();

    static uint8_t capacitySize() {
        return sizeof(capacity)/sizeof(uint8_t);
    }
    virtual Data serializeCapacity() const;
    virtual bool deserializeCapacity(const Data& data);

    static uint8_t lockScriptSize() {
        return sizeTable({TW::Hash::sha256Size, 1, (4+TW::Hash::sha1Size)});
    }
    virtual Data serializeLockScript() const;
    virtual bool deserializeLockScript(const Data& data);

    virtual Data serializeTypeScript() const;
    virtual bool deserializeTypeScript(const Data& data);

    /// Serializes the output into the provided buffer.
    virtual Data serialize() const;
    virtual bool deserialize(const Data& data);

    static std::vector<TW::Data> serialize(const TransactionOutput& output);
};

} // namespace TW::NervosCKB

/// Wrapper for C interface.
struct TWNervosCKBTransactionOutput {
    TW::NervosCKB::TransactionOutput impl;
};
