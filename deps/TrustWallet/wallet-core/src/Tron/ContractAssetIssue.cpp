// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ContractAssetIssue.h"
#include "asset_issue_contract.pb.h"


namespace TW::Tron {


void TransferAssetContract::from_internal(const ::protocol::TransferAssetContract& contract) {

    Contract::from_internal(contract);

    asset_name    = contract.asset_name();
    owner_address = TW::Tron::Address::fromHex(contract.owner_address());
       to_address = TW::Tron::Address::fromHex(contract.to_address());
    amount = contract.amount();

    // Calculate the offset of each item
    if (!calculateOffset()) {
        clear();
    }
}


/// Converts an external TransferAssetContract to an internal one used for
/// signing.
::protocol::TransferAssetContract TransferAssetContract::to_internal() const {

    ::protocol::TransferAssetContract encode;

    encode.clear_asset_name();
    if (0 < asset_name.size()) {
        *encode.mutable_asset_name() = asset_name;
    }

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


bool TransferAssetContract::calculateOffset() {

    pb_length_delimited pbAssetName = getAssetName();
    pb_length_delimited pbToAddress = getToAddress();
    pb_varint pbAmount = getAmount();
    if (   !pbAssetName.isValid()
        || !pbToAddress.isValid()
        ||    !pbAmount.isValid()
        ) {
        return false;
    }

    size_t szAssetName = 0;
    if (search(0,
               pbAssetName.serialize(),
               asNameIndex)
        ) {
        szAssetName = pbAssetName.size();

        asNameSize  = pbAssetName.sizeValue();
        asNameIndex += pbAssetName.sizeTag() + pbAssetName.sizeLength();
    }

    if (!Contract::calculateOffset(szAssetName)) {
        return false;
    }

    size_t szToAddress = 0;
    if (search(szAssetName+szOwnerAddress,
               pbToAddress.serialize(),
               toAddrIndex)
        ) {
        szToAddress = pbToAddress.size();

        toAddrSize  = pbToAddress.sizeValue();
        toAddrIndex += pbAssetName.sizeTag() + pbAssetName.sizeLength();
    }

    if (search(szAssetName+szOwnerAddress+szToAddress,
               pbAmount.serialize(),
               amtIndex)
        ) {
        amtSize  = pbAmount.sizeValue();
        amtIndex += pbAmount.sizeTag();
    }

    return true;
}


TW::Data TransferAssetContract::assetNameSize() const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)asNameSize, o)) {
        o.clear();
    }

    return o;
}


TW::Data TransferAssetContract::assetNameOffset(const size_t offset) const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)assetNameIndex(offset), o)) {
        o.clear();
    }

    return o;
}


size_t TransferAssetContract::assetNameIndex(const size_t offset) const {

    if (0 == asNameIndex) {
        return asNameIndex;
    }

    return (asNameIndex + offset);
}


TW::Data TransferAssetContract::toAddressSize() const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)toAddrSize, o)) {
        o.clear();
    }

    return o;
}


TW::Data TransferAssetContract::toAddressOffset(const size_t offset) const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)toAddressIndex(offset), o)) {
        o.clear();
    }

    return o;
}


size_t TransferAssetContract::toAddressIndex(const size_t offset) const {

    if (0 == toAddrIndex) {
        return toAddrIndex;
    }

    return (toAddrIndex + offset);
}


TW::Data TransferAssetContract::amountSize() const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)amtSize, o)) {
        o.clear();
    }

    return o;
}


TW::Data TransferAssetContract::amountOffset(const size_t offset) const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)amountIndex(offset), o)) {
        o.clear();
    }

    return o;
}


size_t TransferAssetContract::amountIndex(const size_t offset) const {

    if (0 == amtIndex) {
        return amtIndex;
    }

    return (amtIndex + offset);
}


pb_length_delimited TransferAssetContract::getAssetName() const {

    return pb_length_delimited(::protocol::TransferAssetContract::kAssetNameFieldNumber,
                               WireFormatLite::FieldType::TYPE_STRING,
                               asset_name);
}


pb_length_delimited TransferAssetContract::getOwnerAddress() const {

    return pb_length_delimited(::protocol::TransferAssetContract::kOwnerAddressFieldNumber,
                               WireFormatLite::FieldType::TYPE_STRING,
                               TW::Tron::Address::toHex(owner_address));
}


pb_length_delimited TransferAssetContract::getToAddress() const {

    return pb_length_delimited(::protocol::TransferAssetContract::kToAddressFieldNumber,
                               WireFormatLite::FieldType::TYPE_STRING,
                               TW::Tron::Address::toHex(to_address));
}


pb_varint TransferAssetContract::getAmount() const {

    return pb_varint(::protocol::TransferAssetContract::kAmountFieldNumber,
                     WireFormatLite::FieldType::TYPE_INT64,
                     amount);
}


void UnfreezeAssetContract::from_internal(const ::protocol::UnfreezeAssetContract& contract) {

    owner_address = TW::Tron::Address::fromHex(contract.owner_address());

    // Calculate the offset of each item
    if (!calculateOffset()) {
        clear();
    }
}


::protocol::UnfreezeAssetContract UnfreezeAssetContract::to_internal() const {

    ::protocol::UnfreezeAssetContract encode;

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

    return encode;
}


} // namespace TW::Tron end
