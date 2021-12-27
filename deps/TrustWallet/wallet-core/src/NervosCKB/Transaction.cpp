// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../NervosCKB/Transaction.h"
#include "../NervosCKB/Serialization.hpp"
#include "../NervosCKB/Deserialization.hpp"
#include <TrezorCrypto/blake256.h>

#include "mSIGNA/stdutils/uchar_vector.h"
#include <utility>
#include <string>
#include <ctime>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdio>

using namespace TW;
using namespace TW::NervosCKB;

extern const uint8_t personal_ckb[16];

size_t Transaction::TX_ITEM_COUNT = 6;

/// Returns the transaction's transaction hash.
Data Transaction::hash() const {

    Data hex;
    serialize(hex);

    TW::Data blake(HASHER_DIGEST_LENGTH);
    Hasher hasher;
    hasher_InitParam(&hasher, HasherType::HASHER_BLAKE2B_PERSONAL,
                     personal_ckb, sizeof(personal_ckb)/sizeof(uint8_t));
    if (0 != hasher_InitParam_Raw(&hasher,
                                  &hex[0], (uint32_t)hex.size(),
                                  &blake[0])) {
        return {};
    }

    return blake;
}


void Transaction::encodeVersion(Data& data) const {

    encode32LE(version, data);
}


void Transaction::decodeVersion(const Data& data, int& index) {

    version = decode32LE(&data[0]);
    index += (sizeof(uint32_t)/sizeof(uint8_t));
}


/// Group inputs and return the indexes by the input group.
std::multimap<int, std::string> Transaction::computeInputGroups() const {

    //TransactionInput index, lock hash
    std::multimap<int, std::string> table;
    for (size_t idx=0; idx<inputs.size(); ++idx) {
        table.insert(std::make_pair(idx, uchar_vector(inputs[idx]->lockHash()).getHex()));
    }

    return table;
}


std::vector<WitnessArgs> Transaction::witnessGroups() const {

    std::vector<WitnessArgs> groups;
    groups.push_back(WitnessArgs());

    std::string last = "";
    for (const auto& group : computeInputGroups()) {
        if ("" == last) {
            last = group.second;
            continue;
        }
        if (last == group.second) {
            groups.push_back(WitnessArgs(0x00));
        }
        else {
            groups.push_back(WitnessArgs());
        }
        last = group.second;
    }

    return groups;
}


Data Transaction::getPreImage(size_t index) const {

    Data emptyWitnessBytes = WitnessArgs().serialize();
    Data emptyWitnessSize = encode64LE(emptyWitnessBytes.size());

    Data transactionHash = hash();

    Data s;
    s.insert(std::end(s), std::begin(transactionHash),   std::end(transactionHash));
    s.insert(std::end(s), std::begin(emptyWitnessSize),  std::end(emptyWitnessSize));
    s.insert(std::end(s), std::begin(emptyWitnessBytes), std::end(emptyWitnessBytes));

    std::vector<WitnessArgs> witnesses = witnessGroups();
    for (size_t i=index+1; i<witnesses.size(); ++i) {
        if (!witnesses[i].isZero()) {
            break;
        }
        Data witness = witnesses[i].serialize();
        s.insert(std::end(s), std::begin(witness), std::end(witness));
    }

    return s;
}


/// Serializes the transaction's deps into the provided buffer.
Data Transaction::serializeDeps() const {

    std::vector<Data> table;
    for (const auto& dep : cellDeps) {
        table.push_back(dep->encode());
    }

    return serializeFixVector(table);
}


bool Transaction::deserializeDeps(const Data& data) {

    bool b = true;

    auto table = deserializeFixVector(data);
    for (int i=0; i<table.size(); ++i) {
        TransactionCellDep dep;
        if (!dep.decode(table[i])) {
            b = false;
            break;
        }
        TransactionCellDep *txDep = new TransactionCellDep(dep);
        cellDeps.push_back(txDep);
    }

    return b;
}


/// Serializes the transaction's inputs into the provided buffer.
Data Transaction::serializeInputs() const {

    std::vector<Data> table;
    for (const auto& input : inputs) {
        table.push_back(input->encode());
    }

    return serializeFixVector(table);
}


bool Transaction::deserializeInputs(const Data& data) {

    bool b = true;

    auto table = deserializeFixVector(data);
    for (int i=0; i<table.size(); ++i) {
        TransactionInput input;
        if (!input.decode(table[i])) {
            b = false;
            break;
        }
        TransactionInput *txInput = new TransactionInput(input);
        inputs.push_back(txInput);
    }

    return b;
}


/// Serializes the transaction's outputs into the provided buffer.
Data Transaction::serializeOutputs() const {

    std::vector<Data> table;
    for (const auto& output : outputs) {
        table.push_back(output->serialize());
    }

    return serializeDynVector(table);
}


bool Transaction::deserializeOutputs(const Data& data, size_t& outputCount) {

    bool b = true;

    auto table = deserializeTableByItemLength(
        data,
        sizeTable({
            TransactionOutput::capacitySize(),
            TransactionOutput::lockScriptSize(),
            0}) // type script, unless Nervos DAO Script is used, this item is 0 bytes
    );
    if (0 == table.size()) {
        return false;
    }
    outputCount = table.size();
    for (int i=0; i<table.size(); ++i) {
        TransactionOutput output;
        if (!output.deserialize(table[i])) {
            b = false;
            break;
        }
        TransactionOutput *txOutput = new TransactionOutput(output);
        outputs.push_back(txOutput);
    }

    return b;
}


/// Serializes the transaction's outputs data into the provided buffer.
Data Transaction::serializeOutputsData() const {

    std::vector<Data> table;
    for (const auto& output : outputs) {
        if (0 == output->data.size()) {
            table.push_back(serializeFixVector({}));
        }
        else {
            table.push_back(serializeFixVector({output->data}));
        }
    }

    return serializeDynVector(table);
}


bool Transaction::deserializeOutputsData(const Data& data, const size_t& outputCount) {

    auto table = deserializeDynVector(data, outputCount);
    for (int i=0; i<table.size(); ++i) {
        outputs[i]->data = table[i];
    }

    return true;
}


/// Encodes the transaction into the provided buffer.
void Transaction::serialize(Data& data) const {

    Data ver;
    encodeVersion(ver);

    std::vector<Data> table;
    table.push_back(ver);
    table.push_back(serializeDeps());
    table.push_back(serializeFixVector({}));    // HeaderDeps
    table.push_back(serializeInputs());
    table.push_back(serializeOutputs());
    table.push_back(serializeOutputsData());

    data = serializeTable(table);
}


bool Transaction::deserialize(const Data& data) {

    bool b = true;

    auto table = deserializeTableByItemCount(data, Transaction::TX_ITEM_COUNT);
    size_t outputCount = 0;
    for (size_t i=0; i<table.size(); ++i) {
        switch (i) {
        case 0:     // Version
        {
            int index = 0;
            decodeVersion(table[i], index);
            break;
        }
        case 1:     // Deps
            b = b && deserializeDeps(table[i]);
            break;
        case 2:     // HeaderDeps
            break;
        case 3:     // Inputs
            b = b && deserializeInputs(table[i]);
            break;
        case 4:     // Outputs
            b = b && deserializeOutputs(table[i], outputCount);
            break;
        case 5:     // OutputsData
            b = b && deserializeOutputsData(table[i], outputCount);
            break;
        default:
            b = b && false;
            break;
        }
        if (!b) {
            break;
        }
    }

    return b;
}


nlohmann::json Transaction::serialize() const noexcept {

    auto lambdaIntegerInHex = [](const unsigned long& i) -> std::string {
        char str[12] = {0x00,};
        sprintf(str, "0x%lx", i);
        return str;
    };

    auto lambdaUint64InHex = [](const uint64_t& i) -> std::string {
        std::string str = uchar_vector(encodeBENoZero(i)).getHex();
        str.erase(0, min(str.find_first_not_of('0'), str.size()-1));
        return "0x"+str;
    };

    auto lambdaBytesInHex = [](const Data& data) -> std::string {
        return "0x"+uchar_vector(data).getHex();
    };

    auto lambdaArrayInHex = [](const std::array<byte, 32>& data) -> std::string {
        return "0x"+uchar_vector(std::begin(data), std::end(data)).getHex();
    };

    auto lambdaDepType2String = [](const uint8_t& type) -> std::string {
        return (DepType::DEP_GROUP == type ? "depGroup" : "codeGroup");
    };

    auto lambdaScriptHashType2String = [](const uint8_t& type) -> std::string {
        return (ScriptHashType::TYPE == type ? "type" : "data");
    };

    // add everything to the json object
    nlohmann::json jsonTx;
    jsonTx["version"] = version ? lambdaUint64InHex(version) : "0x0";

    nlohmann::json jsonDeps;
    for (const auto& dep : cellDeps) {
        nlohmann::json cellDep;
        cellDep["outPoint"] = {
            {"txHash",  lambdaArrayInHex(dep->outPoint.hash)},
            {"index", lambdaIntegerInHex(dep->outPoint.index)}
        };
        cellDep["depType"] = lambdaDepType2String(dep->depType);

        jsonDeps.push_back(cellDep);
    }
    jsonTx["cellDeps"] = jsonDeps;

    jsonTx["headerDeps"] = nlohmann::json::array();

    nlohmann::json jsonInputs;
    for (const auto& input : inputs) {
        nlohmann::json cellInput;
        cellInput["previousOutput"] = {
            {"txHash",  lambdaArrayInHex(input->previousOutput.hash)},
            {"index", lambdaIntegerInHex(input->previousOutput.index)},
        };
        cellInput["since"] = lambdaIntegerInHex(input->since);

        jsonInputs.push_back(cellInput);
    }
    jsonTx["inputs"] = jsonInputs;

    nlohmann::json jsonOutputs;
    for (const auto& output : outputs) {
        Data codeHash;
        uint8_t type = 0;
        Data args;
        if (!Script::parseScript(output->lock, codeHash, type, args)) {
            continue;
        }

        nlohmann::json cellOutput;
        cellOutput["lock"] = {
            {"codeHash", lambdaBytesInHex(codeHash)},
            {"hashType", lambdaScriptHashType2String(type)},
            {"args",     lambdaBytesInHex(args)}
        };

        if (0 < output->type.size()) {
            cellOutput["type"] = {
                {"codeHash", lambdaBytesInHex(codeHash)},
                {"hashType", lambdaScriptHashType2String(type)},
                {"args",     lambdaBytesInHex(args)}
            };
        }

        cellOutput["capacity"] = lambdaUint64InHex(output->capacity);

        jsonOutputs.push_back(cellOutput);
    }
    jsonTx["outputs"] = jsonOutputs;

    nlohmann::json jsonWitnesses;
    for (const auto& witness : witnesses) {
        std::string str = "0x";
        if (!witness.isZero()) {
            str += uchar_vector(witness.serialize()).getHex();
        }
        jsonWitnesses.push_back(str);
    }
    jsonTx["witnesses"] = jsonWitnesses;

    nlohmann::json jsonOutputsData;
    for (const auto& output : outputs) {
        std::string str = "0x";
        if (0 < output->data.size()) {
            str += uchar_vector(output->data).getHex();
        }
        jsonOutputsData.push_back(str);
    }
    jsonTx["outputsData"] = jsonOutputsData;

    return jsonTx;
}
