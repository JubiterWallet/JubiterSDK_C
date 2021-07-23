// Copyright © 2017-2020 JuBiter Wallet.
//
// This file is part of JuBiter. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../NervosCKB/TransactionCellOutput.h"
#include "../NervosCKB/TransactionCellDep.h"
#include "../NervosCKB/Serialization.hpp"
#include "../BinaryCoding.h"

using namespace TW;
using namespace TW::NervosCKB;


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


Data TransactionOutput::serializeLockScript() const {

    return serializeTable(Script::serializeScript(lock));
}


Data TransactionOutput::serializeTypeScript() const {

    if (0 < type.size()) {
        return serializeTable(Script::serializeScript(type));
    }

    return {};
}


Data TransactionOutput::serialize() const {

    return serializeTable(TransactionOutput::serialize(*this));
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
