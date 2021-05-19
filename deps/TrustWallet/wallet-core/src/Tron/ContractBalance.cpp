// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ContractBalance.h"

#include "../HexCoding.h"


namespace TW::Tron {


::protocol::ResourceCode toInternal(const int source) {

   switch (source) {
   case 1:
       return ::protocol::ResourceCode::ENERGY;
   default:
       return ::protocol::ResourceCode::BANDWIDTH;
   }
}


void TransferContract::from_internal(const ::protocol::TransferContract& contract) {

    Contract::from_internal(contract);

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


bool TransferContract::calculateOffset() {

    if (!Contract::calculateOffset()) {
        return false;
    }

    pb_length_delimited    pbToAddress = getToAddress();
    pb_varint pbAmount = getAmount();
    if (!pbToAddress.isValid()
        || !pbAmount.isValid()
        ) {
        return false;
    }

    size_t    szToAddress = 0;
    if (search(szOwnerAddress,
               pbToAddress.serialize(),
               toAddrIndex)
        ) {
        szToAddress = pbToAddress.size();

        toAddrSize  = pbToAddress.sizeValue();
        toAddrIndex += pbToAddress.sizeTag() + pbToAddress.sizeLength();
    }

    if (search(szOwnerAddress+szToAddress,
               pbAmount.serialize(),
               amtIndex)
        ) {
        amtSize  = pbAmount.sizeValue();
        amtIndex += pbAmount.sizeTag();
    }

    return true;
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

    if (0 == toAddrIndex) {
        return toAddrIndex;
    }

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

    if (0 == amtIndex) {
        return amtIndex;
    }

    return (amtIndex + offset);
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

    // When resource == bandwidth, pb will not encode this item,
    // so we need to override it's serialize()
//    Contract::from_internal(contract);

       owner_address  = TW::Tron::Address::fromHex(contract.owner_address());
    receiver_address  = TW::Tron::Address::fromHex(contract.receiver_address());
      frozen_balance  = contract.frozen_balance();
      frozen_duration = contract.frozen_duration();
    resource = contract.resource();

    // When resource == bandwidth, pb will not encode this item,
    // so we need to override it's serialize()
    save(serialize());

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


// When resource == bandwidth, pb will not encode this item,
// so we need to override it's serialize()
TW::Data FreezeBalanceContract::serialize() {

    pb_length_delimited pbOwnerAddress = getOwnerAddress();
    pb_varint pbFrozenBalance = getFrozenBalance();
    pb_varint pbFrozenDuration = getFrozenDuration();
    pb_varint pbResource = getResource();
    pb_length_delimited pbReceiverAddress = getReceiverAddress();

    if (      !pbOwnerAddress.isValid()
        ||  !pbFrozenDuration.isValid()
        ||  !pbFrozenDuration.isValid()
        ||        !pbResource.isValid()
        || !pbReceiverAddress.isValid()
        ) {
        {};
    }

    TW::Data vOwnerAddress = pbOwnerAddress.serialize();
    TW::Data vFrozenBalance  = pbFrozenBalance.serialize();
    TW::Data vFrozenDuration = pbFrozenDuration.serialize();
    TW::Data vResource = pbResource.serialize();
    TW::Data vReceiverAddress = pbReceiverAddress.serialize();

    size_t sz = vOwnerAddress.size()
           +   vFrozenBalance.size()
           +  vFrozenDuration.size()
           +        vResource.size()
           + vReceiverAddress.size();
    TW::Data pb(sz);
    memcpy(&pb[0],                  &vOwnerAddress[0],    vOwnerAddress.size());
    memcpy(&pb[0]
           +  vOwnerAddress.size(), &vFrozenBalance[0],   vFrozenBalance.size());
    memcpy(&pb[0]
           +  vOwnerAddress.size()
           + vFrozenBalance.size(), &vFrozenDuration[0],  vFrozenDuration.size());
    memcpy(&pb[0]
           +  vOwnerAddress.size()
           + vFrozenBalance.size()
           +vFrozenDuration.size(), &vResource[0],        vResource.size());
    memcpy(&pb[0]
           +  vOwnerAddress.size()
           + vFrozenBalance.size()
           +vFrozenDuration.size()
           +      vResource.size(), &vReceiverAddress[0], vReceiverAddress.size());

    return pb;
}


bool FreezeBalanceContract::calculateOffset() {

    if (!Contract::calculateOffset()) {
        return false;
    }

    pb_varint pbFrozenBalance  = getFrozenBalance();
    pb_varint pbFrozenDuration = getFrozenDuration();
    pb_varint pbResource = getResource();
    pb_length_delimited pbReceiverAddress = getReceiverAddress();
    if (     !pbFrozenBalance.isValid()
        ||  !pbFrozenDuration.isValid()
        ||        !pbResource.isValid()
        || !pbReceiverAddress.isValid()
        ) {
        return false;
    }

    size_t szFrozenBalance = 0;
    if (search(szOwnerAddress,
               pbFrozenBalance.serialize(),
               frozenBalIndex)
        ) {
        szFrozenBalance = pbFrozenBalance.size();

        frozenBalSize   = pbFrozenBalance.sizeValue();
        frozenBalIndex += pbFrozenBalance.sizeTag();
    }

    size_t szFrozenDuration = 0;
    if (search(szOwnerAddress+szFrozenBalance,
               pbFrozenDuration.serialize(),
               frozenDIndex)
        ) {
        szFrozenDuration = pbFrozenDuration.size();

        frozenDSize   = pbFrozenDuration.sizeValue();
        frozenDIndex += pbFrozenDuration.sizeTag();
    }

    size_t szResource = 0;
    if (search(szOwnerAddress+szFrozenBalance+szFrozenDuration,
               pbResource.serialize(),
               resIndex)
        ) {
        szResource = pbResource.size();

        resSize = pbResource.sizeValue();
        resIndex += pbResource.sizeTag();
    }

    if (search(szOwnerAddress+szFrozenBalance+szFrozenDuration+szResource,
               pbReceiverAddress.serialize(),
               rxAddrIndex)
        ) {
        rxAddrSize   = pbReceiverAddress.sizeValue();
        rxAddrIndex += pbReceiverAddress.sizeTag() + pbReceiverAddress.sizeLength();
    }

    return true;
}


pb_varint FreezeBalanceContract::getFrozenBalance() const {

    return pb_varint(::protocol::FreezeBalanceContract::kFrozenBalanceFieldNumber,
                     WireFormatLite::FieldType::TYPE_UINT64,
                     frozen_balance);
}


pb_varint FreezeBalanceContract::getFrozenDuration() const {

    return pb_varint(::protocol::FreezeBalanceContract::kFrozenDurationFieldNumber,
                     WireFormatLite::FieldType::TYPE_UINT64,
                     frozen_duration);
}


pb_varint UnfreezeBalanceContract::getResource() const {

    return pb_varint(::protocol::FreezeBalanceContract::kResourceFieldNumber,
                     WireFormatLite::FieldType::TYPE_UINT64,
                     resource);
}


pb_length_delimited UnfreezeBalanceContract::getReceiverAddress() const {

    return pb_length_delimited(::protocol::FreezeBalanceContract::kReceiverAddressFieldNumber,
                               WireFormatLite::FieldType::TYPE_STRING,
                               TW::Tron::Address::toHex(receiver_address));
}


Data FreezeBalanceContract::frozenBalanceSize() const {

    TW::Data o;
    if (!pb_basevarint::encode((int64_t)frozenBalSize, o)) {
        o.clear();
    }

    return o;
}


Data FreezeBalanceContract::frozenBalanceOffset(const size_t offset) const {

    TW::Data o;
    if (!pb_basevarint::encode((int64_t)frozenBalanceIndex(offset), o)) {
        o.clear();
    }

    return o;
}


size_t FreezeBalanceContract::frozenBalanceIndex(const size_t offset) const {

    if (0 == frozenBalIndex) {
        return frozenBalIndex;
    }

    return (frozenBalIndex + offset);
}


Data FreezeBalanceContract::frozenDurationSize() const {

    TW::Data o;
    if (!pb_basevarint::encode((int64_t)frozenDSize, o)) {
        o.clear();
    }

    return o;
}


Data FreezeBalanceContract::frozenDurationOffset(const size_t offset) const {

    TW::Data o;
    if (!pb_basevarint::encode((int64_t)frozenDurationIndex(offset), o)) {
        o.clear();
    }

    return o;
}


size_t FreezeBalanceContract::frozenDurationIndex(const size_t offset) const {

    if (0 == frozenDIndex) {
        return frozenDIndex;
    }

    return (frozenDIndex + offset);
}


Data UnfreezeBalanceContract::resourceSize() const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)resSize, o)) {
        o.clear();
    }

    return o;
}


Data UnfreezeBalanceContract::resourceOffset(const size_t offset) const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)resourceIndex(offset), o)) {
        o.clear();
    }

    return o;
}


size_t UnfreezeBalanceContract::resourceIndex(const size_t offset) const {

    if (0 == resIndex) {
        return resIndex;
    }

    return (resIndex + offset);
}


Data UnfreezeBalanceContract::receiverAddressSize() const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)rxAddrSize, o)) {
        o.clear();
    }

    return o;
}


Data UnfreezeBalanceContract::receiverAddressOffset(const size_t offset) const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)receiverAddressIndex(offset), o)) {
        o.clear();
    }

    return o;
}


size_t UnfreezeBalanceContract::receiverAddressIndex(const size_t offset) const {

    if (0 == rxAddrIndex) {
        return rxAddrIndex;
    }

    return (rxAddrIndex + offset);
}


void UnfreezeBalanceContract::from_internal(const ::protocol::UnfreezeBalanceContract& contract) {

    // When resource == bandwidth, pb will not encode this item,
    // so we need to override it's serialize()
//    Contract::from_internal(contract);

       owner_address = TW::Tron::Address::fromHex(contract.owner_address());
    receiver_address = TW::Tron::Address::fromHex(contract.receiver_address());

    resource = contract.resource();

    // When resource == bandwidth, pb will not encode this item,
    // so we need to override it's serialize()
    save(serialize());

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
    if (0 <= resource) {
        encode.set_resource(resource);
    }

    return encode;
}


// When resource == bandwidth, pb will not encode this item,
// so we need to override it's serialize()
TW::Data UnfreezeBalanceContract::serialize() {

    pb_length_delimited pbOwnerAddress = getOwnerAddress();
    pb_varint pbResource = getResource();
    pb_length_delimited pbReceiverAddress = getReceiverAddress();

    if (      !pbOwnerAddress.isValid()
        ||        !pbResource.isValid()
        || !pbReceiverAddress.isValid()
        ) {
        {};
    }

    TW::Data vOwnerAddress = pbOwnerAddress.serialize();
    TW::Data vResource = pbResource.serialize();
    TW::Data vReceiverAddress = pbReceiverAddress.serialize();

    size_t sz = vOwnerAddress.size()
           +        vResource.size()
           + vReceiverAddress.size();
    TW::Data pb(sz);
    memcpy(&pb[0],                &vOwnerAddress[0],    vOwnerAddress.size());
    memcpy(&pb[0]
           +vOwnerAddress.size(), &vResource[0],        vResource.size());
    memcpy(&pb[0]
           +vOwnerAddress.size()
           +    vResource.size(), &vReceiverAddress[0], vReceiverAddress.size());

    return pb;
}


bool UnfreezeBalanceContract::calculateOffset() {

    if (!Contract::calculateOffset()) {
        return false;
    }

    pb_varint pbResource = getResource();
    pb_length_delimited pbReceiverAddress = getReceiverAddress();
    if (          !pbResource.isValid()
        || !pbReceiverAddress.isValid()
        ) {
        return false;
    }

    size_t szResource = 0;
    if (search(szOwnerAddress,
               pbResource.serialize(),
               resIndex)
        ) {
        szResource = pbResource.size();

        resSize = pbResource.sizeValue();
        resIndex += pbResource.sizeTag();
    }

    if (search(szOwnerAddress+szResource,
               pbReceiverAddress.serialize(),
               rxAddrIndex)
        ) {
        rxAddrSize  = pbReceiverAddress.sizeValue();
        rxAddrIndex += pbReceiverAddress.sizeTag() + pbReceiverAddress.sizeLength();
    }

    return true;
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


} // namespace TW::Tron end
