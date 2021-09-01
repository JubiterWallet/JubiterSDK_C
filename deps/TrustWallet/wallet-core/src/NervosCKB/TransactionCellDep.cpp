// Copyright © 2017-2021 JuBiter Wallet.
//
// This file is part of JuBiter. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransactionCellDep.h"
#include "../NervosCKB/Serialization.hpp"

using namespace TW;
using namespace TW::NervosCKB;


void TransactionCellDep::encode(Data& data) const {

    data = encode();
}


Data TransactionCellDep::encode() const {

    Data data;
    auto& outpoint = reinterpret_cast<const TW::Bitcoin::OutPoint&>(outPoint);
    outpoint.encode(data);
    data.push_back(depType);

    return data;
}


/// Decodes the provided buffer into the transactionInput.
bool TransactionCellDep::decode(const Data& data) {

    size_t index = 0;

    if (!outPoint.decode(data)) {
        return false;
    }
    index += outPoint.size();

    Data temp(std::begin(data)+index, std::end(data));
    if (sizeof(depType)/sizeof(uint8_t) != temp.size()) {
        return false;
    }
    depType = temp[0];
    if (   DepType::CODE      != depType
        && DepType::DEP_GROUP != depType
        ) {
        return false;
    }

    return true;
}


size_t TransactionCellDep::size() {

    return ( sizeof(uint32_t)/sizeof(uint8_t)
            + outPoint.size()
            + sizeof(depType)/sizeof(uint8_t));
}


/// Serializes the input into the provided buffer.
Data TransactionCellDep::serialize() const {

    return serializeFixVector(TransactionCellDep::serialize(*this));
}


std::vector<TW::Data> TransactionCellDep::serialize(const TransactionCellDep& dep) {

    if (!dep.isValid()) {
        return {};
    }

    std::vector<TW::Data> table;
    table.push_back(dep.encode());

    return table;
}
