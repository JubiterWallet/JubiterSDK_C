// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Contract.hpp"
#include "balance_contract.pb.h"

namespace TW::Tron {


void Contract::from_internal(const google::protobuf::Message& msg) {

    size_t sz = msg.ByteSizeLong();
    TW::Data o(sz);
    bool b = msg.SerializeToArray(&o[0], (int)sz);
    if (b) {
        // Stores the PB serialization encoding data so that you can check the existence of each item when calculating the it's INDEX
        save(o);
    }
}


void Contract::deserialize(const Data& o, google::protobuf::Message& decode) {

    bool status = decode.ParseFromArray(&o[0], (int)o.size());
    if (status) {
        // Stores the PB serialization encoding data so that you can check the existence of each item when calculating the it's INDEX
        save(o);
        from_internal(decode);
    }
}


bool Contract::calculateOffset() {

    return calculateOffset(0);
}


bool Contract::calculateOffset(const size_t offset) {

    pb_length_delimited pbOwnerAddress = getOwnerAddress();
    if (!pbOwnerAddress.isValid()) {
        return false;
    }

    size_t ownerAddrIndex = 0;
    if (search(offset,
               pbOwnerAddress.serialize(),
               ownerAddrIndex)
        ) {
        szOwnerAddress = pbOwnerAddress.size();

        ownerAddrSize  = pbOwnerAddress.sizeValue();
    }

    return true;
}


bool Contract::search(const size_t offset, const TW::Data &itemInpb, size_t& index) const {

    auto it = std::search(std::begin(pb)+offset, std::end(pb),
                          itemInpb.begin(), itemInpb.end());
    if (pb.end() == it) {
        index = it - pb.end();
        return false;
    }
    else {
        index = (it - pb.begin());
        return true;
    }
}


pb_length_delimited Contract::getOwnerAddress() const {

    return pb_length_delimited(::protocol::TransferContract::kOwnerAddressFieldNumber,
                               WireFormatLite::FieldType::TYPE_STRING,
                               TW::Tron::Address::toHex(owner_address));
}


} // namespace TW::Tron end
