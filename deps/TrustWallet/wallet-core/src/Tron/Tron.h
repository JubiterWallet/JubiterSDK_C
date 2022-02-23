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
#include "PbParser.hpp"

namespace TW::Tron {


// AccountId, (name, address) use name, (null, address) use address, (name, null) use name,
//message AccountId {
//  bytes name = 1;
//  bytes address = 2;
//}
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
    Data serialize() const;
};  // class AccountId end

//::protocol::AccountType to_internal(const int32_t type);

//message Key {
//  bytes address = 1;
//  int64 weight = 2;
//}
class Key : PbParser {
public:
    std::string address;    // Address with this privilege
    int64_t weight;         // This address has weight for this premission

public:
    Key() {}
    Key(const std::string& address, const int64_t weight)
        : address(address)
        , weight(weight)
    {}
    virtual ~Key() {}

    void clear() {
        PbParser::clear();
        address = "";
        weight = 0;
    }

    bool isValid() const {
        return (0 < address.size());
    }

    void from_internal(const ::protocol::Key& key);
    ::protocol::Key to_internal() const;

    virtual bool calculateOffset();

    virtual TW::Data addressSize() const;
    virtual TW::Data addressOffset(const uint64_t offset) const;

    void deserialize(const Data& o);
    Data serialize() const;

protected:
    virtual pb_length_delimited getAddress() const;
    virtual pb_varint getWeight() const;

    virtual uint64_t addressIndex(const uint64_t offset) const;
    virtual uint64_t weightIndex(const uint64_t offset) const;

    size_t _addressSize = 0;
    size_t _addressIndex = 0;

    size_t _weightSize = 0;
    size_t _weightIndex = 0;
};  // class Key end


/// Authority point for implementation of Tron coin.
//message authority {
//  AccountId account = 1;
//  bytes permission_name = 2;
//}
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
    Data serialize() const;
};  // class Authority end


//message Permission {
//  enum PermissionType {
//    Owner = 0;
//    Witness = 1;
//    Active = 2;
//  }
//  PermissionType type = 1;
//  int32 id = 2; //Owner id=0, Witness id=1, Active id start by 2
//  string permission_name = 3;
//  int64 threshold = 4;
//  int32 parent_id = 5;
//  bytes operations = 6; //1 bit 1 contract
//  repeated Key keys = 7;
//}
class Permission : PbParser {
public:
    protocol::Permission_PermissionType type;   // Permission type, currently only supports three permissions.
    int32_t identity;   // The value is automatically set by the system, with Owner id=0 and witness id=1.
                        // Active id is incremented from 2 onwards. When the contract is executed, the id
                        // is used to specify which permission to use. For example, if the owner
                        // permission is used, the id is set to 0.
    std::string permission_name;    // Permission name, set by the user, limited to 32 bytes in length.
    int64_t threshold;  // Threshold, the corresponding operation is allowed only when the sum of the weights of
                        // the participating signatures exceeds the domain value. Requires a maximum value less
                        // than the Long type.
    int32_t parent_id;  // Currently only 0
    //1 bit 1 contract
    std::vector<uint8_t> operations;    // A total of 32 bytes (256 bits), each representing the authority of a contract,
                                        // a 1 means the authority to own the contract. For operations=0x0100...00 (hexadecimal),
                                        // ie 100...0 (binary), look at the Transaction.ContractType definition in proto.
                                        // The id of the contract AccountCreateContract is 0, which means that the permission
                                        // only has the implementation of AccountCreateContract. Permissions can be obtained
                                        // using the "calculation example of operations in active permissions".
    std::vector<Key> keys;  // The address and weight that jointly own the permission can be up to 5 keys.

public:
    Permission() {}
    Permission(const protocol::Permission_PermissionType type,
               const int32_t identity,
               const std::string& permission_name,
               const int64_t threshold,
               const int32_t parent_id,
               const std::vector<uint8_t>& operations,
               const std::vector<Key>& keys)
        : type(type)
        , identity(identity)
        , permission_name(permission_name)
        , threshold(threshold)
        , parent_id(parent_id)
        , operations(operations)
        , keys(keys)
    {}
    Permission(const std::string& permission_name,
               const int64_t threshold,
               const std::vector<uint8_t>& operations,
               const std::vector<Key>& keys)
        : permission_name(permission_name)
        , threshold(threshold)
        , operations(operations)
        , keys(keys)
    {}
    Permission(const std::string& permission_name,
               const int64_t threshold,
               const std::vector<Key>& keys)
        : permission_name(permission_name)
        , threshold(threshold)
        , keys(keys)
    {}

    virtual ~Permission() {}

    void clear() {
        type = protocol::Permission_PermissionType::Permission_PermissionType_Owner;
        identity = 0;
        permission_name = "";
        threshold = 0;
        parent_id = 0;
        operations.clear();
        keys.clear();
    }

    bool isValid() const {
        return (0 < keys.size());
    }

    void from_internal(const ::protocol::Permission& permission);
    ::protocol::Permission to_internal() const;

    virtual bool calculateOffset();

    virtual TW::Data permissionNameSize() const;
    virtual TW::Data permissionNameOffset(const uint64_t offset) const;

//    virtual TW::Data keysOffset(const size_t offset) const;
//    virtual TW::Data keyOffset(const size_t offset, const size_t index) const;
    virtual uint64_t keysOffset(const uint64_t offset) const;
    virtual uint64_t keyOffset(const uint64_t offset, const size_t index) const;

    virtual size_t size() const;

    void deserialize(const Data& o);
    Data serialize() const;

protected:
    virtual pb_length_delimited getPermissionName() const;
    virtual pb_varint getThreshold() const;
    virtual Data getKeys() const;
    virtual pb_length_delimited getKey(const size_t index) const;

    virtual uint64_t permissionNameIndex(const uint64_t offset) const;
    virtual uint64_t keyIndex(const uint64_t offset, const size_t index) const;

    size_t _permissionNameSize = 0;
    size_t _permissionNameIndex = 0;

    size_t _thresholdSize = 0;
    size_t _thresholdIndex = 0;

    size_t _keysSize = 0;
    size_t _keysIndex = 0;
};  // class Permission end


} // namespace TW::Tron
