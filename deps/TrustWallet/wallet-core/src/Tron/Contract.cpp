// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Contract.h"
#include "balance_contract.pb.h"

namespace TW::Tron {


bool Contract::calculateOffset() {

    pb_length_delimited pbOwnerAddress = getOwnerAddress();
    if (!pbOwnerAddress.isValid()) {
        return false;
    }

    ownerAddrSize  = pbOwnerAddress.sizeValue();
//    ownerAddrIndex = pbOwnerAddress.sizeTag() + pbOwnerAddress.sizeLength();

    return true;
}


pb_length_delimited Contract::getOwnerAddress() const {

    return pb_length_delimited(::protocol::TransferContract::kOwnerAddressFieldNumber,
                               WireFormatLite::FieldType::TYPE_STRING,
                               TW::Tron::Address::toHex(owner_address));
}


} // namespace TW::Tron end
