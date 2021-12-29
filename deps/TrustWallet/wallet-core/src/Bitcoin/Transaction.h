// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../Hash.h"
#include "Script.h"
#include "TWHcashSigHashType.h"
#include "TransactionInput.h"
#include "TransactionOutput.h"
#include <TrustWalletCore/TWBitcoinSigHashType.h>

#include "SignatureVersion.h"
#include <cstddef>
#include <cstdint>
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
    std::vector<TransactionInput *> inputs;

    /// A list of 1 or more transaction outputs or destinations for coins
    std::vector<TransactionOutput *> outputs;

    TW::Hash::Hasher hasher = TW::Hash::sha256d;

    Transaction(TW::Hash::Hasher hasher = TW::Hash::sha256d) : hasher(hasher) {}

    Transaction(int32_t version, uint32_t lockTime, TW::Hash::Hasher hasher = TW::Hash::sha256d)
        : version(version), lockTime(lockTime), inputs(), outputs(), hasher(hasher) {}

    // JuBiter-defined
    virtual ~Transaction() {
        for (size_t i = 0; i < inputs.size(); ++i) {
            if (inputs[i]) {
                delete inputs[i];
                inputs[i] = nullptr;
            }
        }
        inputs.clear();

        for (size_t i = 0; i < outputs.size(); ++i) {
            if (outputs[i]) {
                delete outputs[i];
                outputs[i] = nullptr;
            }
        }
        outputs.clear();
    }

    /// Whether the transaction is empty.
    bool empty() const { return inputs.empty() && outputs.empty(); }

    /// Generates the signature pre-image.
    // JuBiter-defined
    Data getPreImage(const Script &scriptCode, size_t index, uint32_t hashType) const;
    virtual Data getPreImage(const Script &scriptCode, size_t index, uint32_t hashType, uint64_t amount) const;
    Data getSigMsg(size_t index, uint32_t hashType, uint8_t extByte, Data annex) const;
    Data getPrevoutHash() const;
    Data getSequenceHash() const;
    Data getOutputsHash() const;
    Data getSpendAmountsHash() const;
    Data getSpendScriptHash() const;

    // JuBiter-defined
    virtual void encodeVersion(Data &data) const;
    // JuBiter-defined
    virtual void decodeVersion(const Data &data, int &index);

    /// Encodes the transaction into the provided buffer.
    virtual void encode(bool witness, Data &data) const;

    // JuBiter-defined
    /// Decodes the provided buffer into the transaction.
    virtual bool decode(bool witness, const Data &data);

    /// Generates the signature hash for this transaction.
    Data getSignatureHash(const Script &scriptCode, size_t index, uint32_t hashType, uint64_t amount,
                          enum SignatureVersion version) const;

    void serializeInput(size_t subindex, const Script &, size_t index, uint32_t hashType, Data &data) const;

    //    /// Converts to Protobuf model
    //    Proto::Transaction proto() const;

  private:
    /// Generates the signature hash for Witness version 0 scripts.
    Data getSignatureHashWitnessV0(const Script &scriptCode, size_t index, uint32_t hashType, uint64_t amount) const;

    /// Generates the signature hash for for scripts other than witness scripts.
    Data getSignatureHashBase(const Script &scriptCode, size_t index, uint32_t hashType) const;
};

} // namespace TW::Bitcoin

/// Wrapper for C interface.
struct TWBitcoinTransaction {
    TW::Bitcoin::Transaction impl;
};
