// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransactionRaw.h"

#include "../HexCoding.h"

#include <google/protobuf/port_def.inc>
#include <google/protobuf/io/coded_stream.h>

#include "pbparse/pb_varint.hpp"
#include "pbparse/pb_length_delimited.hpp"


namespace TW::Tron {


// JuBiter-defined
void TransactionRaw::from_internal(const ::protocol::Transaction_raw& raw) {

    if (0 < raw.ref_block_bytes().size()) {
        ref_block_bytes.resize(raw.ref_block_bytes().size());
        std::copy(std::begin(raw.ref_block_bytes()), std::end(raw.ref_block_bytes()), ref_block_bytes.begin());
    }
    ref_block_num = raw.ref_block_num();
    if (0 < raw.ref_block_hash().size()) {
        ref_block_hash.resize(raw.ref_block_hash().size());
        std::copy(std::begin(raw.ref_block_hash()), std::end(raw.ref_block_hash()), ref_block_hash.begin());
    }
    expiration = raw.expiration();
    if (0 < raw.auths_size()) {
        for (int i=0; i<raw.auths_size(); ++i) {
            Authority auth;
            auth.from_internal(raw.auths(i));
            authority.push_back(auth);
        }
    }
    if (0 < raw.data().size()) {
        data.resize(raw.data().size());
        std::copy(std::begin(raw.data()), std::end(raw.data()), data.begin());
    }
    if (0 < raw.contract_size()) {
        for (int i=0; i<raw.contract_size(); ++i) {
            TransactionContract contract;
            contract.from_internal(raw.contract(i));
            contracts.push_back(contract);
        }
    }
    if (0 < raw.scripts().size()) {
        scripts.resize(raw.scripts().size());
        std::copy(std::begin(raw.scripts()), std::end(raw.scripts()), scripts.begin());
    }
    timestamp = raw.timestamp();
    fee_limit = raw.fee_limit();

    // Calculate the offset of each item
    if (!calculateOffset()) {
        clear();
    }
}


// JuBiter-defined
::protocol::Transaction_raw TransactionRaw::to_internal() const {

    ::protocol::Transaction_raw encode;

    encode.clear_ref_block_bytes();
    if (0 < ref_block_bytes.size()) {
        *encode.mutable_ref_block_bytes() = {ref_block_bytes.begin(), ref_block_bytes.end()};
    }

    encode.clear_ref_block_num();
    if (0 < ref_block_num) {
        encode.set_ref_block_num(ref_block_num);
    }

    encode.clear_ref_block_hash();
    if (0 < ref_block_hash.size()) {
        *encode.mutable_ref_block_hash() = {ref_block_hash.begin(), ref_block_hash.end()};
    }

    encode.clear_expiration();
    if (0 < expiration) {
        encode.set_expiration(expiration);
    }

    encode.clear_auths();
    if (0 < authority.size()) {
        for (const auto& auth : authority) {
            ::protocol::authority *en = encode.add_auths();
            *en = auth.to_internal();
        }
    }

    encode.clear_data();
    if (0 < data.size()) {
        *encode.mutable_data() = {data.begin(), data.end()};
    }

    encode.clear_contract();
    if (0 < contracts.size()) {
        for (const auto& contract : contracts) {
            ::protocol::Transaction_Contract *en = encode.add_contract();
            *en = contract.to_internal();
        }
    }

    encode.clear_scripts();
    if (0 < scripts.size()) {
        *encode.mutable_scripts() = {scripts.begin(), scripts.end()};
    }

    encode.clear_timestamp();
    if (0 < timestamp) {
        encode.set_timestamp(timestamp);
    }

    encode.clear_fee_limit();
    if (0 < fee_limit) {
         encode.set_fee_limit(fee_limit);
    }

    return encode;
}


Data TransactionRaw::serialize() {

    auto o = Data();

    ::protocol::Transaction_raw encode = to_internal();

    size_t szSize = encode.ByteSizeLong();
    auto ou = Data(szSize);
    bool status = encode.SerializeToArray(&ou[0], (int)szSize);
    if (status) {
        o.resize(szSize);
        std::copy(std::begin(ou), std::end(ou), std::begin(o));
    }

    // Calculate the offset of each item
    if (!calculateOffset(o)) {
        clear();
    }

    return o;
}


// JuBiter-defined
void TransactionRaw::deserialize(const Data& o) {

    ::protocol::Transaction_raw decode;
    bool status = decode.ParseFromArray(&o[0], (int)o.size());
    if (status) {
        from_internal(decode);
    }
}


size_t TransactionRaw::contractOffset(const size_t offset) const {

    return contractValueIndex(offset, contrIndex);
}


TW::Data TransactionRaw::feeLimitSize() const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)feeLimSize, o)) {
        o.clear();
    }

    return o;
}


TW::Data TransactionRaw::feeLimitOffset(const size_t offset) const {

    TW::Data o;
    if (!pb_basevarint::encode((uint32_t)feeLimitIndex(offset), o)) {
        o.clear();
    }

    return o;
}


size_t TransactionRaw::feeLimitIndex(const size_t offset) const {

    return (feeLimIndex + offset);
}


size_t TransactionRaw::contractValueIndex(const size_t index, const size_t offset) const {

    size_t contrValueOffset = 0;
    pb_length_delimited pbContract = getContract();
    if (!pbContract.isValid()) {
        return contrValueOffset;
    }
    contrValueOffset += pbContract.sizeTag();
    contrValueOffset += pbContract.sizeLength();

    size_t contrValueIndex = contrValueOffset;
    for (size_t i=0; i<contracts.size(); ++i) {
        TransactionContract txContract = getContract(i);
        if (i != index) {
            contrValueIndex += txContract.serialize().size();
        }
        else {
            contrValueIndex += txContract.parameterValueIndex(0);
        }
    }

    return (contrValueIndex + offset);
}


bool TransactionRaw::calculateOffset() {

    Data o;

    pb_length_delimited pbRefBlockBytes = getRefBlockBytes();
    pb_varint pbRefBlockNum = getRefBlockNum();
    pb_length_delimited pbRefBlockHash = getRefBlockHash();
    pb_varint pbExpiration = getExpiration();
    pb_length_delimited pbAuths = getAuths();
    pb_length_delimited pbData = getData();
    pb_length_delimited pbContract = getContract();
    pb_length_delimited pbScripts = getScripts();
    pb_varint pbTimestamp = getTimestamp();

    if (  !pbRefBlockBytes.isValid()
        || !pbRefBlockHash.isValid()
        ||   !pbExpiration.isValid()
        ||     !pbContract.isValid()
        ||    !pbTimestamp.isValid()
        ) {
        return false;
    }
    size_t szRefBlockBytes = pbRefBlockBytes.size();
    size_t szRefBlockNum   = pbRefBlockNum.size();
    size_t szRefBlockHash  = pbRefBlockHash.size();
    size_t szExpiration    = pbExpiration.size();
    size_t szAuths = pbAuths.size();
    size_t szData  = pbData.size();
    size_t szContract = pbContract.size();
    size_t szScripts  = pbScripts.size();
    size_t szTimestamp= pbTimestamp.size();

    contrIndex = szRefBlockBytes
               + szRefBlockNum
               + szRefBlockHash
               + szExpiration
               + szAuths
               + szData;

    feeLimSize = 0;
    feeLimIndex = 0;
    if (0 < fee_limit) {
        pb_varint pbFeeLimit = getFeeLimit();
        feeLimSize = pbFeeLimit.sizeValue();
        feeLimIndex = szRefBlockBytes
                    + szRefBlockNum
                    + szRefBlockHash
                    + szExpiration
                    + szAuths
                    + szData
                    + szContract
                    + szScripts
                    + szTimestamp;
        feeLimIndex += pbFeeLimit.sizeTag();
    }

    return true;
}


bool TransactionRaw::calculateOffset(const TW::Data raw) {

    pb_length_delimited pbContract = getContract();
    if (!pbContract.isValid()) {
        return false;
    }

    TW::Data rawSearch(raw);

    contrIndex = 0;
    TW::Data contract = pbContract.serialize();
    Data::iterator it = std::search(rawSearch.begin(), rawSearch.end(), contract.begin(), contract.end());
    contrIndex = it - raw.begin();

    feeLimSize = 0;
    feeLimIndex = 0;
    if (0 < fee_limit) {
        pb_varint pbFeeLimit = getFeeLimit();
        feeLimSize = pbFeeLimit.sizeValue();
        TW::Data feeLimit = pbFeeLimit.serialize();
        it = std::search(rawSearch.begin(), rawSearch.end(), feeLimit.begin(), feeLimit.end());
        feeLimIndex = it - raw.begin();
    }

    return true;
}


pb_length_delimited TransactionRaw::getRefBlockBytes() const {

    pb_length_delimited pb;
    if (0 < ref_block_bytes.size()) {
        return pb_length_delimited(::protocol::Transaction_raw::kRefBlockBytesFieldNumber,
                                   WireFormatLite::FieldType::TYPE_BYTES,
                                   ref_block_bytes);
    }

    return pb;
}


pb_varint TransactionRaw::getRefBlockNum() const {

    pb_varint pb;
    if (0 < ref_block_num) {
        return pb_varint(::protocol::Transaction_raw::kRefBlockNumFieldNumber,
                         WireFormatLite::FieldType::TYPE_INT64,
                         ref_block_num);
    }

    return pb;
}


pb_length_delimited TransactionRaw::getRefBlockHash() const {

    pb_length_delimited pb;
    if (0 < ref_block_hash.size()) {
        return pb_length_delimited(::protocol::Transaction_raw::kRefBlockHashFieldNumber,
                                   WireFormatLite::FieldType::TYPE_BYTES,
                                   ref_block_hash);
    }

    return pb;
}


pb_varint TransactionRaw::getExpiration() const {

    pb_varint pb;
    if (0 < expiration) {
        return pb_varint(::protocol::Transaction_raw::kExpirationFieldNumber,
                         WireFormatLite::FieldType::TYPE_INT64,
                         expiration);
    }

    return pb;
}


pb_length_delimited TransactionRaw::getAuths() const {

    pb_length_delimited pb;
    return pb;
}


pb_length_delimited TransactionRaw::getData() const {

    pb_length_delimited pb;
    if (0 < data.size()) {
        return pb_length_delimited(::protocol::Transaction_raw::kDataFieldNumber,
                                   WireFormatLite::FieldType::TYPE_BYTES,
                                   data);
    }

    return pb;
}


TransactionContract TransactionRaw::getContract(const size_t index) const {

    TransactionContract decode;
    decode.deserialize(serializeContract(index));

    return decode;
}


pb_length_delimited TransactionRaw::getContract() const {

    pb_length_delimited pb;
    if (0 < contracts.size()) {
        return pb_length_delimited(::protocol::Transaction_raw::kContractFieldNumber,
                                   WireFormatLite::FieldType::TYPE_MESSAGE,
                                   serializeContract());
    }

    return pb;
}


TW::Data TransactionRaw::serializeContract(const size_t index) const {

    Data o;

    ::protocol::Transaction_Contract encode = contracts[index].to_internal();
    size_t szSize = encode.ByteSizeLong();
    auto ou = Data(szSize);
    bool status = encode.SerializeToArray(&ou[0], (int)szSize);
    if (status) {
        o.resize(szSize);
        std::copy(std::begin(ou), std::end(ou), std::begin(o));
    }

    return o;
}


TW::Data TransactionRaw::serializeContract() const {

    Data o;

    size_t total = 0;
    size_t offset = 0;
    for (size_t i=0; i<contracts.size(); ++i) {
        Data oItem = serializeContract(i);
        if (0 >= oItem.size()) {
            continue;
        }

        total += oItem.size();
        o.resize(total);
        std::copy(std::begin(oItem), std::end(oItem), std::begin(o)+offset);
        offset += oItem.size();
    }

    return o;
}


pb_length_delimited TransactionRaw::getScripts() const {

    pb_length_delimited pb;
    return pb;
}


pb_varint TransactionRaw::getTimestamp() const {

    pb_varint pb;
    if (0 < timestamp) {
        return pb_varint(::protocol::Transaction_raw::kTimestampFieldNumber,
                         WireFormatLite::FieldType::TYPE_INT64,
                         timestamp);
    }

    return pb;
}


pb_varint TransactionRaw::getFeeLimit() const {

    pb_varint pb;
    if (0 < fee_limit) {
        return pb_varint(::protocol::Transaction_raw::kFeeLimitFieldNumber,
                         WireFormatLite::FieldType::TYPE_INT64,
                         fee_limit);
    }

    return pb;
}


} // namespace TW::Tron end
