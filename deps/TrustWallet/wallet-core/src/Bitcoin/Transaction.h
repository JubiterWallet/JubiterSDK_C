// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Script.h"
#include "TransactionInput.h"
#include "TransactionOutput.h"
#include "../Hash.h"

#include <TrustWalletCore/TWBitcoin.h>
#include <vector>

namespace TW::Bitcoin {

struct Transaction {
    /// Transaction data format version (note, this is signed)
    int32_t version = 1;

    // JuBiter-defined
    int8_t marker = 0;

    // JuBiter-defined
    int8_t flag = 1;

    /// The block number or timestamp at which this transaction is unlocked
    ///
    ///     | Value          | Description
    ///     |----------------|------------
    ///     |  0             | Not locked
    ///     | < 500000000    | Block number at which this transaction is unlocked
    ///     | >= 500000000   | UNIX timestamp at which this transaction is unlocked
    ///
    /// If all inputs have final (`0xffffffff`) sequence numbers then `lockTime` is irrelevant. Otherwise, the
    /// transaction may not be added to a block until after `lockTime`.
    uint32_t lockTime = 0;

    /// A list of 1 or more transaction inputs or sources for coins
    std::vector<TransactionInput*> inputs;

    /// A list of 1 or more transaction outputs or destinations for coins
    std::vector<TransactionOutput*> outputs;

    TW::Hash::Hasher hasher = TW::Hash::sha256d;

    Transaction(TW::Hash::Hasher hasher = TW::Hash::sha256d) : hasher(hasher) {}

    Transaction(int32_t version, uint32_t lockTime, TW::Hash::Hasher hasher = TW::Hash::sha256d)
        : version(version), lockTime(lockTime), inputs(), outputs(), hasher(hasher) {}

    // JuBiter-defined
    ~Transaction() {
        for (size_t i=0; i<inputs.size(); ++i) {
            if (inputs[i]) {
                delete inputs[i]; inputs[i] = nullptr;
            }
        }
        inputs.clear();

        for (size_t i=0; i<outputs.size(); ++i) {
            if (outputs[i]) {
                delete outputs[i]; outputs[i] = nullptr;
            }
        }
        outputs.clear();
    }

    /// Whether the transaction is empty.
    bool empty() const { return inputs.empty() && outputs.empty(); }

    /// Generates the signature pre-image.
    // JuBiter-defined
    std::vector<uint8_t> getPreImage(const Script& scriptCode, size_t index,
                                     uint32_t hashType) const;
    virtual std::vector<uint8_t> getPreImage(const Script& scriptCode, size_t index, uint32_t hashType,
                                     uint64_t amount) const;
    std::vector<uint8_t> getPrevoutHash() const;
    std::vector<uint8_t> getSequenceHash() const;
    std::vector<uint8_t> getOutputsHash() const;

    // JuBiter-defined
    virtual void encodeVersion(std::vector<uint8_t>& data) const;
    // JuBiter-defined
    virtual void decodeVersion(const std::vector<uint8_t>& data, int& index);

    /// Encodes the transaction into the provided buffer.
    virtual void encode(bool witness, std::vector<uint8_t>& data) const;

    // JuBiter-defined
    /// Decodes the provided buffer into the transaction.
    virtual bool decode(bool witness, const std::vector<uint8_t>& data);

    /// Generates the signature hash for this transaction.
    std::vector<uint8_t> getSignatureHash(const Script& scriptCode, size_t index, uint32_t hashType,
                                          uint64_t amount, TWBitcoinSignatureVersion version) const;

    void serializeInput(size_t subindex, const Script&, size_t index, uint32_t hashType,
                        std::vector<uint8_t>& data) const;

//    /// Converts to Protobuf model
//    Proto::Transaction proto() const;

  private:
    /// Generates the signature hash for Witness version 0 scripts.
    std::vector<uint8_t> getSignatureHashWitnessV0(const Script& scriptCode, size_t index,
                                                   uint32_t hashType, uint64_t amount) const;

    /// Generates the signature hash for for scripts other than witness scripts.
    std::vector<uint8_t> getSignatureHashBase(const Script& scriptCode, size_t index,
                                              uint32_t hashType) const;
};

} // namespace TW::Bitcoin

/// Wrapper for C interface.
struct TWBitcoinTransaction {
    TW::Bitcoin::Transaction impl;
};
