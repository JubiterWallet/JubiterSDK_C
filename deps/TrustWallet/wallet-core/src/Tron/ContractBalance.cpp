// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ContractBalance.h"

#include "../HexCoding.h"


namespace TW::Tron {


void TransferContract::from_internal(const ::protocol::TransferContract& contract) {

    owner_address = TW::Tron::Address::fromHex(contract.owner_address());
       to_address = TW::Tron::Address::fromHex(contract.to_address());
    amount = contract.amount();

    // Calculate the offset of each item
    if (!calculateOffset()) {
        clear();
    }
}


/// Converts an external TransferContract to an internal one used for signing.
::protocol::TransferContract TransferContract::to_internal() const {

    ::protocol::TransferContract encode;

    encode.clear_owner_address();
    if (0 < owner_address.size()) {
        Data address = TW::Tron::Address::toHex(owner_address);
        if (0 < address.size()) {
            *encode.mutable_owner_address() = {address.begin(), address.end()};
        }
        else {
            *encode.mutable_owner_address() = owner_address;
        }
    }

    encode.clear_to_address();
    if (0 < to_address.size()) {
        Data address = TW::Tron::Address::toHex(to_address);
        if (0 < address.size()) {
            *encode.mutable_to_address() = {address.begin(), address.end()};
        }
        else {
            *encode.mutable_to_address() = to_address;
        }
    }

    encode.clear_amount();
    if (0 < amount) {
        encode.set_amount(amount);
    }

    return encode;
}


void TransferContract::deserialize(const Data& o) {

    ::protocol::TransferContract decode;
    bool status = decode.ParseFromArray(&o[0], (int)o.size());
    if (status) {
        from_internal(decode);
    }
}


Data TransferContract::serialize() {

    auto data = Data();

    ::protocol::TransferContract encode = to_internal();
    size_t szSize = encode.ByteSizeLong();
    auto ou = Data(szSize);

    bool status = encode.SerializeToArray(&ou[0], (int)szSize);
    if (status) {
        data.resize(szSize);
        std::copy(std::begin(ou), std::end(ou), std::begin(data));
    }

    // Calculate the offset of each item
    if (!calculateOffset()) {
        data.clear();
        return {};
    }

    return data;
}


TW::Data TransferContract::toAddressSize() const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)toAddrSize, o)) {
        o.clear();
    }

    return o;
}


TW::Data TransferContract::toAddressOffset(const size_t offset) const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)toAddressIndex(offset), o)) {
        o.clear();
    }

    return o;
}


size_t TransferContract::toAddressIndex(const size_t offset) const {

    return (toAddrIndex + offset);
}


TW::Data TransferContract::amountSize() const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)amtSize, o)) {
        o.clear();
    }

    return o;
}


TW::Data TransferContract::amountOffset(const size_t offset) const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)amountIndex(offset), o)) {
        o.clear();
    }

    return o;
}


size_t TransferContract::amountIndex(const size_t offset) const {

    return (amtIndex + offset);
}


bool TransferContract::calculateOffset() {

    pb_length_delimited pbOwnerAddress = getOwnerAddress();
    pb_length_delimited    pbToAddress = getToAddress();
    pb_varint pbAmount = getAmount();

    if (!pbOwnerAddress.isValid()
        || !pbToAddress.isValid()
        ||    !pbAmount.isValid()
        ) {
        return false;
    }
    size_t szOwnerAddress = pbOwnerAddress.size();
    size_t    szToAddress = pbToAddress.size();

    ownerAddrSize  = pbOwnerAddress.sizeValue();
//    ownerAddrIndex = pbOwnerAddress.sizeTag() + pbOwnerAddress.sizeLength();

    toAddrSize  = pbToAddress.sizeValue();
    toAddrIndex = szOwnerAddress
                + pbToAddress.sizeTag() + pbToAddress.sizeLength();

    amtSize  = pbAmount.sizeValue();
    amtIndex = szOwnerAddress
             + szToAddress
             + pbAmount.sizeTag();

    return true;
}


pb_length_delimited TransferContract::getToAddress() const {

    return pb_length_delimited(::protocol::TransferContract::kToAddressFieldNumber,
                               WireFormatLite::FieldType::TYPE_STRING,
                               TW::Tron::Address::toHex(to_address));
}


pb_varint TransferContract::getAmount() const {

    return pb_varint(::protocol::TransferContract::kAmountFieldNumber,
                     WireFormatLite::FieldType::TYPE_INT64,
                     amount);
}


void FreezeBalanceContract::from_internal(const ::protocol::FreezeBalanceContract& contract) {

       owner_address  = TW::Tron::Address::fromHex(contract.owner_address());
    receiver_address  = TW::Tron::Address::fromHex(contract.receiver_address());
      frozen_balance  = contract.frozen_balance();
      frozen_duration = contract.frozen_duration();
    resource = contract.resource();

    // Calculate the offset of each item
    if (!calculateOffset()) {
        clear();
    }
}


::protocol::FreezeBalanceContract FreezeBalanceContract::to_internal() const {

    ::protocol::FreezeBalanceContract encode;

    encode.clear_owner_address();
    if (0 < owner_address.size()) {
        Data address = TW::Tron::Address::toHex(owner_address);
        if (0 < address.size()) {
            *encode.mutable_owner_address() = {address.begin(), address.end()};
        }
        else {
            *encode.mutable_owner_address() = owner_address;
        }
    }

    encode.clear_receiver_address();
    if (0 < receiver_address.size()) {
        Data address = TW::Tron::Address::toHex(receiver_address);
        if (0 < address.size()) {
            *encode.mutable_receiver_address() = {address.begin(), address.end()};
        }
        else {
            *encode.mutable_receiver_address() = receiver_address;
        }
    }

    encode.clear_frozen_balance();
    if (0 < frozen_balance) {
        encode.set_frozen_balance(frozen_balance);
    }

    encode.clear_frozen_duration();
    if (0 < frozen_duration) {
        encode.set_frozen_duration(frozen_duration);
    }

    encode.clear_resource();
    if (0 < resource) {
        encode.set_resource(resource);
    }

    return encode;
}


pb_length_delimited FreezeBalanceContract::getReceiverAddress() const {

    return pb_length_delimited(::protocol::FreezeBalanceContract::kReceiverAddressFieldNumber,
                               WireFormatLite::FieldType::TYPE_STRING,
                               TW::Tron::Address::toHex(receiver_address));
}


void FreezeBalanceContract::deserialize(const Data& o) {

    ::protocol::FreezeBalanceContract decode;
    bool status = decode.ParseFromArray(&o[0], (int)o.size());
    if (status) {
        from_internal(decode);
    }
}


Data FreezeBalanceContract::serialize() {

    auto data = Data();

    ::protocol::FreezeBalanceContract encode = to_internal();
    size_t szSize = encode.ByteSizeLong();
    auto ou = Data(szSize);

    bool status = encode.SerializeToArray(&ou[0], (int)szSize);
    if (status) {
        data.resize(szSize);
        std::copy(std::begin(ou), std::end(ou), std::begin(data));
    }

    // Calculate the offset of each item
    if (!calculateOffset()) {
        clear();
    }

    return data;
}


void UnfreezeBalanceContract::from_internal(const ::protocol::UnfreezeBalanceContract& contract) {

       owner_address = TW::Tron::Address::fromHex(contract.owner_address());
    receiver_address = TW::Tron::Address::fromHex(contract.receiver_address());

    resource = contract.resource();

    // Calculate the offset of each item
    if (!calculateOffset()) {
        clear();
    }
}


::protocol::UnfreezeBalanceContract UnfreezeBalanceContract::to_internal() const {

    ::protocol::UnfreezeBalanceContract encode;

    encode.clear_owner_address();
    if (0 < owner_address.size()) {
        Data address = TW::Tron::Address::toHex(owner_address);
        if (0 < address.size()) {
            *encode.mutable_owner_address() = {address.begin(), address.end()};
        }
        else {
            *encode.mutable_owner_address() = owner_address;
        }
    }

    encode.clear_receiver_address();
    if (0 < receiver_address.size()) {
        Data address = TW::Tron::Address::toHex(receiver_address);
        if (0 < address.size()) {
            *encode.mutable_receiver_address() = {address.begin(), address.end()};
        }
        else {
            *encode.mutable_receiver_address() = receiver_address;
        }
    }

    encode.clear_resource();
    if (0 < resource) {
        encode.set_resource(resource);
    }

    return encode;
}


void UnfreezeBalanceContract::deserialize(const Data& o) {

    ::protocol::UnfreezeBalanceContract decode;
    bool status = decode.ParseFromArray(&o[0], (int)o.size());
    if (status) {
        from_internal(decode);
    }
}


Data UnfreezeBalanceContract::serialize() {

    auto data = Data();

    ::protocol::UnfreezeBalanceContract encode = to_internal();
    size_t szSize = encode.ByteSizeLong();
    auto ou = Data(szSize);

    bool status = encode.SerializeToArray(&ou[0], (int)szSize);
    if (status) {
        data.resize(szSize);
        std::copy(std::begin(ou), std::end(ou), std::begin(data));
    }

    // Calculate the offset of each item
    if (!calculateOffset()) {
        data.clear();
        return {};
    }

    return data;
}


pb_length_delimited UnfreezeBalanceContract::getReceiverAddress() const {

    return pb_length_delimited(::protocol::UnfreezeBalanceContract::kReceiverAddressFieldNumber,
                               WireFormatLite::FieldType::TYPE_STRING,
                               TW::Tron::Address::toHex(receiver_address));
}


void WithdrawBalanceContract::from_internal(const ::protocol::WithdrawBalanceContract& contract) {

    owner_address = TW::Tron::Address::fromHex(contract.owner_address());

    // Calculate the offset of each item
    if (!calculateOffset()) {
        clear();
    }
}


::protocol::WithdrawBalanceContract WithdrawBalanceContract::to_internal() const {

    ::protocol::WithdrawBalanceContract encode;

    encode.clear_owner_address();
    if (0 < owner_address.size()) {
        Data addr = TW::Tron::Address::toHex(owner_address);
        if (0 < addr.size()) {
            *encode.mutable_owner_address() = {addr.begin(), addr.end()};
        }
        else {
            *encode.mutable_owner_address() = owner_address;
        }
    }

    return encode;
}


void WithdrawBalanceContract::deserialize(const Data& o) {

    ::protocol::WithdrawBalanceContract decode;
    bool status = decode.ParseFromArray(&o[0], (int)o.size());
    if (status) {
        from_internal(decode);
    }
}


Data WithdrawBalanceContract::serialize() {

    auto data = Data();

    ::protocol::WithdrawBalanceContract encode = to_internal();
    size_t szSize = encode.ByteSizeLong();
    auto ou = Data(szSize);

    bool status = encode.SerializeToArray(&ou[0], (int)szSize);
    if (status) {
        data.resize(szSize);
        std::copy(std::begin(ou), std::end(ou), std::begin(data));
    }

    // Calculate the offset of each item
    if (!calculateOffset()) {
        data.clear();
        return {};
    }

    return data;
}


} // namespace TW::Tron end
