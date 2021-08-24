// Copyright © 2017-2020 JuBiter Wallet.
//
// This file is part of JuBiter. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../NervosCKB/TransactionCellOutput.h"
#include "../NervosCKB/TransactionCellDep.h"
#include "../BinaryCoding.h"

using namespace TW;
using namespace TW::NervosCKB;

size_t TransactionOutput::TXOUTPUT_ITEM_COUNT = 3;

size_t TransactionOutput::size() {

    return ((sizeof(capacity)/sizeof(uint8_t))
          + sizeTable(Script::serializeScript(lock))
          + sizeTable(Script::serializeScript(type))
          +     (sizeof(type)/sizeof(uint8_t)));
}


Data TransactionOutput::serializeCapacity() const {

    Data data;
    encode64LE(capacity, data);

    return data;
}


bool TransactionOutput::deserializeCapacity(const Data& data) {

    capacity = decode64LE(&data[0]);

    return true;
}


Data TransactionOutput::serializeLockScript() const {

    return serializeTable(Script::serializeScript(lock));
}


bool TransactionOutput::deserializeLockScript(const Data& data) {

    auto table = deserializeTableByItemCount(data, Script::LOCK_SCRIPT_ITEM_COUNT);
    for (size_t i=0; i<table.size(); ++i) {
        std::copy(std::begin(table[i]), std::end(table[i]), std::back_inserter(lock));
    }

    return true;
}


Data TransactionOutput::serializeTypeScript() const {

    if (0 < type.size()) {
        return serializeTable(Script::serializeScript(type));
    }

    return {};
}


bool TransactionOutput::deserializeTypeScript(const Data& data) {

    bool b = true;

    if (0 >= data.size()) {
        type = {};
    }

    return b;
}


Data TransactionOutput::serialize() const {

    return serializeTable(TransactionOutput::serialize(*this));
}


bool TransactionOutput::deserialize(const Data& data) {

    bool b = true;

    auto table = deserializeTableByItemCount(data, TransactionOutput::TXOUTPUT_ITEM_COUNT);
    for (size_t i=0; i<table.size(); ++i) {
        switch (i) {
        case 0:
            b = b && deserializeCapacity(table[i]);
            break;
        case 1:
            b = b && deserializeLockScript(table[i]);
            break;
        case 2:
            b = b && deserializeTypeScript(table[i]);
            break;
        default:
            b = b && false;
            break;
        }
    }

    return b;
}


std::vector<TW::Data> TransactionOutput::serialize(const TransactionOutput& output) {

    if (!output.isValid()) {
        return {};
    }

    std::vector<TW::Data> table;
    table.push_back(output.serializeCapacity());
    table.push_back(output.serializeLockScript());
    table.push_back(output.serializeTypeScript());

    return table;
}
