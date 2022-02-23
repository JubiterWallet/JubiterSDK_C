// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Tron.h"
#include "Address.h"

using namespace std;


namespace TW::Tron {


void AccountId::from_internal(const ::protocol::AccountId& acct) {

    name    = acct.name();
    address = TW::Tron::Address::fromHex(acct.address());
}


::protocol::AccountId AccountId::to_internal() const {

    ::protocol::AccountId encode;

    encode.clear_name();
    if (0 < name.size()) {
        *encode.mutable_name() = name;
    }

    encode.clear_address();
    if (0 < address.size()) {
        Data addr = TW::Tron::Address::toHex(address);
        if (0 < addr.size()) {
            *encode.mutable_address() = {addr.begin(), addr.end()};
        }
        else {
            *encode.mutable_address() = address;
        }
    }

    return encode;
}


void AccountId::deserialize(const Data& o) {

    ::protocol::AccountId decode;
    bool status = decode.ParseFromArray(&o[0], (int)o.size());
    if (status) {
        from_internal(decode);
    }
}


Data AccountId::serialize() const {

    auto data = Data();

    ::protocol::AccountId encode = to_internal();
    size_t szSize = encode.ByteSizeLong();
    auto o = Data(szSize);

    bool status = encode.SerializeToArray(&o[0], (int)szSize);
    if (status) {
        data.resize(szSize);
        std::copy(std::begin(o), std::end(o), std::begin(data));
    }

    return data;
}


void Key::from_internal(const ::protocol::Key& key) {

    PbParser::from_internal(key);

    address = TW::Tron::Address::fromHex(key.address());
    weight = key.weight();

    // Calculate the offset of each item
    if (!calculateOffset()) {
        clear();
    }
}


::protocol::Key Key::to_internal() const {

    ::protocol::Key encode;

    encode.clear_address();
    if (0 < address.size()) {
        Data addr = TW::Tron::Address::toHex(address);
        if (0 < addr.size()) {
            *encode.mutable_address() = {addr.begin(), addr.end()};
        }
        else {
            *encode.mutable_address() = address;
        }
    }

    encode.set_weight(weight);

    return encode;
}


bool Key::calculateOffset() {

    pb_length_delimited pbAddress = getAddress();
    pb_varint pbWeight = getWeight();
    if (   !pbAddress.isValid()
        || !pbWeight.isValid()
        ) {
        return false;
    }

    uint64_t    szAddress = 0;
    if (search(szAddress,
               pbAddress.serialize(),
               _addressIndex)
        ) {
        szAddress = pbAddress.size();

        _addressSize  = pbAddress.sizeValue();
        _addressIndex += pbAddress.sizeTag() + pbAddress.sizeLength();
    }

    uint64_t    szWeight = 0;
    if (search(szAddress,
               pbWeight.serialize(),
               _weightIndex)
        ) {
        szWeight = pbWeight.size();

        _weightSize  = pbWeight.sizeValue();
        _weightIndex += pbWeight.sizeTag();
    }

    return true;
}


TW::Data Key::addressSize() const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)_addressSize, o)) {
        o.clear();
    }

    return o;
}


TW::Data Key::addressOffset(const uint64_t offset) const {

    TW::Data o;
    if (!pb_basevarint::encode(addressIndex(offset), o)) {
        o.clear();
    }

    return o;
}


pb_length_delimited Key::getAddress() const {

    if (0 >= address.size()) {
        return pb_length_delimited();
    }

    return pb_length_delimited(::protocol::Key::kAddressFieldNumber,
                               WireFormatLite::FieldType::TYPE_BYTES,
                               TW::Tron::Address::toHex(address));
}


pb_varint Key::getWeight() const {

    return pb_varint(::protocol::Key::kWeightFieldNumber,
                     WireFormatLite::FieldType::TYPE_INT64,
                     weight);
}


uint64_t Key::addressIndex(const uint64_t offset) const {

    return (0 == _addressIndex) ? _addressIndex : (_addressIndex + offset);
}


uint64_t Key::weightIndex(const uint64_t offset) const {

    return (0 == _weightIndex) ? _weightIndex : (_weightIndex + offset);
}


void Key::deserialize(const Data& o) {

    ::protocol::Key decode;
    bool status = decode.ParseFromArray(&o[0], (int)o.size());
    if (status) {
        from_internal(decode);
    }
}


Data Key::serialize() const {

    auto data = Data();

    ::protocol::Key encode = to_internal();
    size_t szSize = encode.ByteSizeLong();
    auto o = Data(szSize);

    bool status = encode.SerializeToArray(&o[0], (int)szSize);
    if (status) {
        data.resize(szSize);
        std::copy(std::begin(o), std::end(o), std::begin(data));
    }

    return data;
}


void Authority::from_internal(const ::protocol::authority& auth) {

    account.from_internal(auth.account());
    permission_name = auth.permission_name();
}


::protocol::authority Authority::to_internal() const {

    ::protocol::authority encode;

    encode.clear_account();
    *encode.mutable_account() = account.to_internal();

    encode.clear_permission_name();
    if (0 < permission_name.size()) {
        *encode.mutable_permission_name() = permission_name;
    }

    return encode;
}


void Authority::deserialize(const Data& o) {

    ::protocol::authority decode;
    bool status = decode.ParseFromArray(&o[0], (int)o.size());
    if (status) {
        from_internal(decode);
    }
}


Data Authority::serialize() const {

    auto data = Data();

    ::protocol::authority encode = to_internal();
    size_t szSize = encode.ByteSizeLong();
    auto o = Data(szSize);

    bool status = encode.SerializeToArray(&o[0], (int)szSize);
    if (status) {
        data.resize(szSize);
        std::copy(std::begin(o), std::end(o), std::begin(data));
    }

    return data;
}


void Permission::from_internal(const ::protocol::Permission& permission) {

    PbParser::from_internal(permission);

    type = permission.type();
    identity = permission.id();
    permission_name = permission.permission_name();
    threshold = permission.threshold();
    parent_id = permission.parent_id();
    if (0 < permission.operations().size()) {
        operations.resize(permission.operations().size());
        std::copy(permission.operations().begin(), permission.operations().end(), operations.begin());
    }

    for (int i=0; i<permission.keys_size(); ++i) {
        Key key;
        key.from_internal(permission.keys(i));
        keys.push_back(key);
    }

    // Calculate the offset of each item
    if (!calculateOffset()) {
        clear();
    }
}


::protocol::Permission Permission::to_internal() const {

    ::protocol::Permission encode;

    // Permission.type
//    encode.set_type(type);
    // Permission.id
//    encode.set_id(identity);
    // Permission.permission_name
    encode.clear_permission_name();
    if (0 < permission_name.size()) {
        *encode.mutable_permission_name() = permission_name;
    }
    // Permission.threshold
    encode.set_threshold(threshold);
    // Permission.parent_id
//    encode.set_parent_id(parent_id);
    // Permission.operations
    if (0 < operations.size()) {
        encode.clear_operations();
        *encode.mutable_operations() = {operations.begin(), operations.end()};
    }
    // Permission.keys
    for (const auto& key : keys) {
        ::protocol::Key *en = encode.add_keys();
        *en = key.to_internal();
    }

    return encode;
}


size_t Permission::size() const {

    return to_internal().ByteSizeLong();
}


bool Permission::calculateOffset() {

    pb_length_delimited pbPermissionName = getPermissionName();
    pb_varint pbThreshold = getThreshold();
    Data pbKeys = getKeys();
    if (   !pbPermissionName.isValid()
        || !pbThreshold.isValid()
        || pbKeys.empty()
        ) {
        return false;
    }

    size_t    szPermissionName = 0;
    if (search(szPermissionName,
               pbPermissionName.serialize(),
               _permissionNameIndex)
        ) {
        szPermissionName = pbPermissionName.size();

        _permissionNameSize  = pbPermissionName.sizeValue();
        _permissionNameIndex += pbPermissionName.sizeTag() + pbPermissionName.sizeLength();
    }

    size_t    szThreshold = 0;
    if (search(szPermissionName,
               pbThreshold.serialize(),
               _thresholdIndex)
        ) {
        szThreshold = pbThreshold.size();

        _thresholdSize  = pbThreshold.sizeValue();
        _thresholdIndex += pbThreshold.sizeTag();
    }

    if (search(szPermissionName+szThreshold,
               pbKeys,
               _keysIndex)
        ) {
        _keysSize  = pbKeys.size();
    }

    return true;
}


TW::Data Permission::permissionNameSize() const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)permission_name.size(), o)) {
        o.clear();
    }

    return o;
}


TW::Data Permission::permissionNameOffset(const uint64_t offset) const {

    TW::Data o;
    if (!pb_basevarint::encode(permissionNameIndex(offset), o)) {
        o.clear();
    }

    return o;
}


//TW::Data Permission::keysOffset(const size_t offset) const {
//
//    return keyOffset(offset, 0);
//}
//
//
//TW::Data Permission::keyOffset(const size_t offset, const size_t index) const {
//
//    TW::Data o;
//    if (!pb_basevarint::encode((uint64_t)keyIndex(offset, index), o)) {
//        o.clear();
//    }
//
//    return o;
//}


uint64_t Permission::keysOffset(const uint64_t offset) const {

    return keyOffset(offset, 0);
}


uint64_t Permission::keyOffset(const uint64_t offset, const size_t index) const {

    return keyIndex(offset, index);
}


pb_length_delimited Permission::getPermissionName() const {

    if (0 >= permission_name.size()) {
        return pb_length_delimited();
    }

    return pb_length_delimited(::protocol::Permission::kPermissionNameFieldNumber,
                               WireFormatLite::FieldType::TYPE_STRING,
                               permission_name);
}


pb_varint Permission::getThreshold() const {

    return pb_varint(::protocol::Permission::kThresholdFieldNumber,
                     WireFormatLite::FieldType::TYPE_INT64,
                     threshold);
}


Data Permission::getKeys() const {

    std::vector<pb_length_delimited> vKeys;
    for (const auto& key : keys) {
        vKeys.push_back(
            pb_length_delimited(::protocol::Permission::kKeysFieldNumber,
                                WireFormatLite::FieldType::TYPE_MESSAGE,
                                key.serialize())
        );
    }

    size_t keys_len = 0;
    for (const auto& key : vKeys) {
        keys_len += key.size();
    }
    if (0 >= keys_len) {
        return {};
    }

    Data data(keys_len);
    size_t index = 0;
    for (const auto& key : vKeys) {
        auto encode = key.serialize();
        std::copy(encode.begin(), encode.end(), data.begin()+index);
        index += key.size();
    }

    return data;
}


pb_length_delimited Permission::getKey(const size_t index) const {

    Key key;
    for (int i=0; i<keys.size(); ++i) {
        if (i == index) {
            key = keys[i];
            break;
        }
    }

    if (!key.isValid()) {
        return pb_length_delimited();
    }

    return pb_length_delimited(::protocol::Permission::kKeysFieldNumber,
                               WireFormatLite::FieldType::TYPE_MESSAGE,
                               key.serialize());
}


uint64_t Permission::permissionNameIndex(const uint64_t offset) const {

    return (0 == _permissionNameIndex) ? _permissionNameIndex : (_permissionNameIndex + offset);
}


uint64_t Permission::keyIndex(const uint64_t offset, const size_t index) const {

    pb_length_delimited pbKey = getKey(index);
    if (!pbKey.isValid()) {
        return offset;
    }

    size_t keyIndex = 0;
    if (search(_keysIndex,
               pbKey.serialize(),
               keyIndex)
        ) {
        keyIndex += pbKey.sizeTag() + pbKey.sizeLength();
    }

    return (0 == keyIndex) ? keyIndex : (keyIndex + offset);
}


void Permission::deserialize(const Data& o) {

    ::protocol::Permission decode;
    bool status = decode.ParseFromArray(&o[0], (int)o.size());
    if (status) {
        from_internal(decode);
    }
}


Data Permission::serialize() const {

    auto data = Data();

    ::protocol::Permission encode = to_internal();
    size_t szSize = encode.ByteSizeLong();
    auto o = Data(szSize);

    bool status = encode.SerializeToArray(&o[0], (int)szSize);
    if (status) {
        data.resize(szSize);
        std::copy(std::begin(o), std::end(o), std::begin(data));
    }

    return data;
}


} // namespace TW::Tron end
