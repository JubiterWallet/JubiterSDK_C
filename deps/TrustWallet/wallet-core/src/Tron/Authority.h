// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "AccountId.h"

namespace TW::Tron {

/// Authority point for implementation of Tron coin.
class Authority {
public:
    // account.
    AccountId account;
    std::string permission_name;

public:
    Authority() {}
    Authority(const AccountId& account, const std::string& permission_name)
        : account(account), permission_name(permission_name)
    {}
    virtual ~Authority() {}

    void from_internal(const ::protocol::authority& auth);
    ::protocol::authority to_internal() const;

    void deserialize(const Data& o);
    Data serialize();
};  // class Authority end

} // namespace TW::Tron
