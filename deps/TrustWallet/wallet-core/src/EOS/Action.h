// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Bravo/Asset.h"
#include "Name.h"

#include <nlohmann/json.hpp>
#include <string>

using Data = TW::Data;

namespace TW::EOS {

class PermissionLevel {
public:
    Name actor, permission;

    PermissionLevel(const Name& actor, const Name& permission) : actor(actor), permission(permission) { }
    virtual ~PermissionLevel() { }

    // JuBiter-defined
    static size_t size();

    void serialize(Data& o) const;
    nlohmann::json serialize() const noexcept;
}; // class PermissionLevel end

class Action {
public:
    Name account, name;
    std::vector<PermissionLevel> authorization;
    Data data;

    virtual ~Action() { }

    // JuBiter-defined
    virtual void deserialize(const Data& o) noexcept;
    // JuBiter-defined
    virtual size_t size() const noexcept;
    // JuBiter-defined
    virtual uint16_t startingPostData() const noexcept;

    virtual void serialize(Data& o) const;
    virtual nlohmann::json serialize() const noexcept;
}; // class Action end

class TransferAction: public Action {
public:
    TransferAction(const std::string& currency, const std::string& from, const std::string& to, const Bravo::Asset& asset, const std::string& memo);
    // JuBiter-defined
    TransferAction() { }

    // JuBiter-defined
    virtual void deserialize(const Data& o) noexcept override;
    // JuBiter-defined
    virtual uint16_t startingPostData() const noexcept override;
    // JuBiter-defined
    virtual uint16_t memoLength() const noexcept;
    // JuBiter-defined
    virtual void serialize(Data& o) const override;
    // JuBiter-defined
    virtual nlohmann::json serialize() const noexcept override;

private:
    void setData(const std::string& from, const std::string& to, const Bravo::Asset& asset, const std::string& memo);

public:
    // JuBiter-defined
    Name from, to;
    // JuBiter-defined
    Bravo::Asset asset;
    // JuBiter-defined
    Data memo;
}; // class TransferAction end
} // namespace TW::EOS end
