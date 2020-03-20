// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

namespace TW::Ripple {

// https://xrpl.org/serialization.html -> Type List
// https://github.com/ripple/rippled/blob/master/src/ripple/protocol/SField.h#L57-L74
enum class FieldType: int {
    // types (common)
    int16   = 1,
    int32   = 2,
    int64   = 3,
    hash128 = 4,
    hash256 = 5,
    amount  = 6,
    vl      = 7,
    account = 8,

    // 9-13 are reserved
    stobject = 14,
    starray = 15,

    // types (uncommon)
    int8    = 16,
    hash160 = 17,
    pathset = 18,
    vector256 = 19,

    // high level types
    transaction = 10001,
    ledgerentry = 10002,
    validation  = 10003,
    metadata    = 10004,
};

// JuBiter-defined
// https://xrpl.org/serialization.html#accountid-fields
enum class FieldAmountKey: int {
    negative = 0,
    positive = 1
};

// JuBiter-defined
enum class FieldObjectKey: int {
    memo = 0xa,
    end  = 0x1
};

// JuBiter-defined
enum class FieldMemoKey: int {
    type   = 0xc,
    data   = 0xd,
    format = 0xe
};

} // namespace TW::Ripple
