// Copyright © 2017-2021 JuBiter Wallet.
//
// This file is part of JuBiter. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Bitcoin/OutPoint.h"
#include "../NervosCKB/Script.h"
#include "../Data.h"

#include <vector>

namespace TW::NervosCKB {

enum DepType {
    CODE,
    DEP_GROUP
};

/// CKB transaction celldep.
class TransactionCellDep {
  public:
    /// Reference to the previous transaction's output.
    TW::Bitcoin::OutPoint outPoint;

    /// Transaction version as defined by the sender.
    ///
    /// 0 - Code
    /// 1 - DepGroup
    uint8_t depType;

    TransactionCellDep() {}
    /// Initializes a transaction input with a previous output, a script and a
    /// sequence number.
    TransactionCellDep(TW::Bitcoin::OutPoint outPoint, uint8_t depType)
        : outPoint(std::move(outPoint)), depType(depType) {}
   ~TransactionCellDep() {
       depType = 0;
   }

    bool isValid() const {
        return outPoint.isValid();
    }

    virtual size_t size();

    /// Encodes the transaction into the provided buffer.
    virtual void encode(Data& data) const;
    virtual Data encode() const;

    /// Decodes the provided buffer into the transactionInput.
    virtual bool decode(const Data& data);

    /// Serializes the input into the provided buffer.
    virtual Data serialize() const;

    static std::vector<TW::Data> serialize(const TransactionCellDep& dep);
};  // class TransactionCellDep end

} // namespace TW::NervosCKB
