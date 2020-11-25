// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>
#include <cstdint>
#include "../Data.h"

#include "Tron.pb.h"

namespace TW::Tron {

/// Authority point for implementation of Tron coin.
class AccountId {
public:
    // Account name.
    std::string name;
    // Account address
    std::string address;

public:
    AccountId() {}
    AccountId(std::string name, std::string address)
        : name(name)
        , address(address)
    {}
    virtual ~AccountId() {}

    void from_internal(const ::protocol::AccountId& acct);
    ::protocol::AccountId to_internal() const;

    void deserialize(const Data& o);
    Data serialize();
};  // class AccountId end

//::protocol::AccountType to_internal(const int32_t type);

} // namespace TW::Tron
