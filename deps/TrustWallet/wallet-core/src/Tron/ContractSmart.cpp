// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ContractSmart.h"


namespace TW::Tron {


void SmartContract::deserialize(const Data& o) {

}


void CreateSmartContract::from_internal(const ::protocol::CreateSmartContract& contract) {

}


::protocol::CreateSmartContract CreateSmartContract::to_internal() const {

    ::protocol::CreateSmartContract pb;
    return pb;
}


void CreateSmartContract::deserialize(const Data& o) {

}


Data CreateSmartContract::serialize() {
    return {};
}


void TriggerSmartContract::from_internal(const ::protocol::TriggerSmartContract& contract) {

       owner_address = TW::Tron::Address::fromHex(contract.owner_address());
    contract_address = TW::Tron::Address::fromHex(contract.contract_address());

    call_value = contract.call_value();

    data.resize(contract.data().size());
    std::copy(std::begin(contract.data()), std::end(contract.data()), std::begin(data));

    call_token_value = contract.call_token_value();
    token_id = contract.token_id();

    // Calculate the offset of each item
    if (!calculateOffset()) {
        clear();
    }
}


::protocol::TriggerSmartContract TriggerSmartContract::to_internal() const {

    ::protocol::TriggerSmartContract encode;

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

    encode.clear_contract_address();
    if (0 < contract_address.size()) {
        Data address = TW::Tron::Address::toHex(contract_address);
        if (0 < address.size()) {
            *encode.mutable_contract_address() = {address.begin(), address.end()};
        }
        else {
            *encode.mutable_contract_address() = contract_address;
        }
    }

    encode.clear_call_value();
    if (0 < call_value) {
        encode.set_call_value(call_value);
    }

    encode.clear_data();
    if (0 < data.size()) {
        *encode.mutable_data() = {data.begin(), data.end()};
    }

    encode.clear_call_token_value();
    if (0 < call_token_value) {
        encode.set_call_token_value(call_token_value);
    }

    encode.clear_token_id();
    if (0 < token_id) {
        encode.set_token_id(token_id);
    }

    return encode;
}


void TriggerSmartContract::deserialize(const Data& o) {

    ::protocol::TriggerSmartContract decode;
    bool status = decode.ParseFromArray(&o[0], (int)o.size());
    if (status) {
        from_internal(decode);
    }
}


Data TriggerSmartContract::serialize() {

    auto data = Data();

    ::protocol::TriggerSmartContract encode = to_internal();
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


TW::Data TriggerSmartContract::contractAddressSize() const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)contractAddrSize, o)) {
        o.clear();
    }

    return o;
}


TW::Data TriggerSmartContract::contractAddressOffset(const size_t offset) const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)contractAddressIndex(offset), o)) {
        o.clear();
    }

    return o;
}


size_t TriggerSmartContract::contractAddressIndex(const size_t offset) const {

    return (contractAddrIndex + offset);
}


TW::Data TriggerSmartContract::callValueSize() const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)callVSize, o)) {
        o.clear();
    }

    return o;
}


TW::Data TriggerSmartContract::callValueOffset(const size_t offset) const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)callValueIndex(offset), o)) {
        o.clear();
    }

    return o;
}


size_t TriggerSmartContract::callValueIndex(const size_t offset) const {

    return (callVIndex + offset);
}


TW::Data TriggerSmartContract::dataSize() const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)dSize, o)) {
        o.clear();
    }

    return o;
}


TW::Data TriggerSmartContract::dataOffset(const size_t offset) const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)dataIndex(offset), o)) {
        o.clear();
    }

    return o;
}


size_t TriggerSmartContract::dataIndex(const size_t offset) const {

    return (dIndex + offset);
}


bool TriggerSmartContract::calculateOffset() {

    pb_length_delimited pbOwnerAddress = getOwnerAddress();
    pb_length_delimited pbContractAddress = getContractAddress();
    pb_varint pbCallValue = getCallValue();
    pb_length_delimited pbData = getData();
    pb_varint pbCallTokenValue = getCallTokenValue();
    if (      !pbOwnerAddress.isValid()
        || !pbContractAddress.isValid()
        ||       !pbCallValue.isValid()
        ||            !pbData.isValid()
        ||  !pbCallTokenValue.isValid()
        ) {
        return false;
    }
    size_t szOwnerAddress = pbOwnerAddress.size();
    size_t szContractAddress = pbContractAddress.size();
    size_t szCallValue = pbCallValue.size();
//    size_t szData = pbData.size();

//         ownerAddrIndex = pbOwnerAddress.sizeTag() + pbOwnerAddress.sizeLength();

    contractAddrSize  = pbContractAddress.sizeValue();
    contractAddrIndex = szOwnerAddress
                      + pbContractAddress.sizeTag() + pbContractAddress.sizeLength();

    callVSize  = pbCallValue.sizeValue();
    callVIndex = szOwnerAddress
               + szContractAddress
               + pbCallValue.sizeTag();

    dSize  = pbData.sizeValue();
    dIndex = szOwnerAddress
           + szContractAddress
           + szCallValue
           + pbData.sizeTag() + pbData.sizeLength();

//    callTokenVSize  = pbCallTokenValue.sizeValue();
//    callTokenVIndex = szOwnerAddress
//                    + szContractAddress
//                    + szCallValue
//                    + szData
//                    + pbCallTokenValue.sizeTag();
//
    return true;
}


pb_length_delimited TriggerSmartContract::getContractAddress() const {

    return pb_length_delimited(::protocol::TriggerSmartContract::kContractAddressFieldNumber,
                               WireFormatLite::FieldType::TYPE_STRING,
                               TW::Tron::Address::toHex(contract_address));
}


pb_varint TriggerSmartContract::getCallValue() const {

    return pb_varint(::protocol::TriggerSmartContract::kCallValueFieldNumber,
                     WireFormatLite::FieldType::TYPE_INT64,
                     call_value);
}


pb_length_delimited TriggerSmartContract::getData() const {

    return pb_length_delimited(::protocol::TriggerSmartContract::kDataFieldNumber,
                               WireFormatLite::FieldType::TYPE_BYTES,
                               data);
}


pb_varint TriggerSmartContract::getCallTokenValue() const {

    return pb_varint(::protocol::TriggerSmartContract::kCallTokenValueFieldNumber,
                     WireFormatLite::FieldType::TYPE_INT64,
                     call_token_value);
}


} // namespace TW::Tron end
