// Copyright © 2017-2021 JuBiter Wallet.
//
// This file is part of JuBiter. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Bitcoin/Script.h"
#include "../NervosCKB/WitnessArgs.h"
#include "../NervosCKB/TransactionCellDep.h"
#include "../NervosCKB/TransactionCellInput.h"
#include "../NervosCKB/TransactionCellOutput.h"
#include "../Hash.h"
#include "../Data.h"

#include <nlohmann/json.hpp>

#include <vector>
#include <map>

namespace TW::NervosCKB {


struct Transaction {
    static size_t TX_ITEM_COUNT;

    /// Transaction data format version (note, this is signed)
    int32_t version = 1;

    /// A list of 1 or more transaction celldeps or sources for coins
    std::vector<TransactionCellDep*> cellDeps;

    /// A list of 1 or more transaction inputs or sources for coins
    std::vector<TransactionInput*> inputs;

    std::vector<WitnessArgs> witnesses;

    /// A list of 1 or more transaction outputs or destinations for coins
    std::vector<TransactionOutput*> outputs;

    TW::Hash::Hasher hasher = TW::Hash::sha256d;

    Transaction(TW::Hash::Hasher hasher = TW::Hash::sha256d)
        : version(0), cellDeps(), inputs(), outputs(), hasher(hasher) {}
    Transaction(int32_t version, TW::Hash::Hasher hasher = TW::Hash::sha256d)
        : version(version), cellDeps(), inputs(), outputs(), hasher(hasher) {}

    ~Transaction() {
        for (size_t i=0; i<cellDeps.size(); ++i) {
            if (cellDeps[i]) {
                delete cellDeps[i]; cellDeps[i] = nullptr;
            }
        }
        cellDeps.clear();

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
    bool empty() const { return cellDeps.empty() && inputs.empty() && outputs.empty(); }

    bool isValid() const {
        return !empty();
    }

    /// Returns the transaction's transaction hash.
    Data hash() const;

    virtual void encodeVersion(Data& data) const;
    virtual void decodeVersion(const Data& data, int& index);

    /// Group inputs and return the indexes by the input group.
    virtual std::multimap<int, std::string> computeInputGroups() const;
//    virtual std::vector<Data> witnessGroups() const;
    virtual std::vector<WitnessArgs> witnessGroups() const;
//    virtual std::vector<WitnessArgs> witnessArgs() const;

    Data getPreImage(size_t index) const;

    /// Serializes the transaction's deps into the provided buffer.
    virtual Data serializeDeps() const;
    virtual bool deserializeDeps(const Data& data);

    /// Serializes the transaction's inputs into the provided buffer.
    virtual Data serializeInputs() const;
    virtual bool deserializeInputs(const Data& data);

    /// Serializes the transaction's outputs into the provided buffer.
    virtual Data serializeOutputs() const;
    virtual bool deserializeOutputs(const Data& data, size_t& outputCount);

    /// Serializes the transaction's outputs data into the provided buffer.
    virtual Data serializeOutputsData() const;
    virtual bool deserializeOutputsData(const Data& data, const size_t& outputCount);

    /// Encodes the transaction into the provided buffer.
    virtual void serialize(Data& data) const;
    virtual bool deserialize(const Data& data);

    nlohmann::json serialize() const noexcept;
};

} // namespace TW::NervosCKB

/// Wrapper for C interface.
struct TWNervosCKBTransaction {
    TW::NervosCKB::Transaction impl;
};
