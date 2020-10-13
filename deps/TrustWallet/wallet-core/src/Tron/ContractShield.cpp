// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ContractShield.h"

#include "../HexCoding.h"


namespace TW::Tron {


void ShieldedTransferContract::from_internal(const ::protocol::ShieldedTransferContract& contract) {
//
//    contract_address = TW::Tron::Address::fromHex(contract.contract_address());
//       owner_address = TW::Tron::Address::fromHex(contract.owner_address());
//          to_address = TW::Tron::Address::fromHex(contract.to_address());
//
//    amount.resize(contract.amount().size());
//    std::copy(std::begin(contract.amount()), std::end(contract.amount()), std::begin(amount));

    // Calculate the offset of each item
    if (!calculateOffset()) {
        clear();
    }
}


::protocol::ShieldedTransferContract ShieldedTransferContract::to_internal() const {

    ::protocol::ShieldedTransferContract encode;

    encode.clear_spend_description();
//    if (0 < contract_address.size()) {
//        Data address = TW::Tron::Address::toHex(contract_address);
//        if (0 < address.size()) {
//            *encode.mutable_contract_address() = {address.begin(), address.end()};
//        }
//        else {
//            *encode.mutable_contract_address() = contract_address;
//        }
//    }
//
//    encode.clear_owner_address();
//    if (0 < owner_address.size()) {
//        Data address = TW::Tron::Address::toHex(owner_address);
//        if (0 < address.size()) {
//            *encode.mutable_owner_address() = {address.begin(), address.end()};
//        }
//        else {
//            *encode.mutable_owner_address() = owner_address;
//        }
//    }
//
//    encode.clear_to_address();
//    if (0 < to_address.size()) {
//        Data address = TW::Tron::Address::toHex(to_address);
//        if (0 < address.size()) {
//            *encode.mutable_to_address() = {address.begin(), address.end()};
//        }
//        else {
//            *encode.mutable_to_address() = to_address;
//        }
//    }
//
//    encode.clear_amount();
//    if (0 < amount.size()) {
//        *encode.mutable_amount() = {amount.begin(), amount.end()};
//    }

    return encode;
}


void ShieldedTransferContract::deserialize(const Data& o) {

    ::protocol::ShieldedTransferContract decode;
    bool status = decode.ParseFromArray(&o[0], (int)o.size());
    if (status) {
        from_internal(decode);
    }
}


Data ShieldedTransferContract::serialize() {

    auto data = Data();

    ::protocol::ShieldedTransferContract encode = to_internal();
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


bool ShieldedTransferContract::calculateOffset() {

//    pb_length_delimited pbOwnerAddress = getOwnerAddress();
//    if (!pbOwnerAddress.isValid()) {
//        return false;
//    }
//
//    ownerAddrIndex = pbOwnerAddress.sizeTag() + pbOwnerAddress.sizeLength();

    return true;
}


pb_length_delimited ShieldedTransferContract::getTransparentFromAddress() const {

    return pb_length_delimited(::protocol::ShieldedTransferContract::kTransparentFromAddressFieldNumber,
                               WireFormatLite::FieldType::TYPE_BYTES,
                               transparent_from_address);
}


pb_varint ShieldedTransferContract::getFromAmount() const {

    return pb_varint(::protocol::ShieldedTransferContract::kFromAmountFieldNumber,
                     WireFormatLite::FieldType::TYPE_INT64,
                     from_amount);
}


pb_length_delimited ShieldedTransferContract::getTransparentToAddress() const {

    return pb_length_delimited(::protocol::ShieldedTransferContract::kTransparentToAddressFieldNumber,
                               WireFormatLite::FieldType::TYPE_BYTES,
                               transparent_to_address);
}


pb_varint ShieldedTransferContract::getToAmount() const {

    return pb_varint(::protocol::ShieldedTransferContract::kToAmountFieldNumber,
                     WireFormatLite::FieldType::TYPE_INT64,
                     to_amount);
}


} // namespace TW::Tron end
