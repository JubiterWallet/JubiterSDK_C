// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Vote.h"

#include "Serialization.h"
#include "BinaryCoding.h"
#include "../HexCoding.h"


namespace TW::Tron {


// JuBiter-defined
::protocol::Transaction Transaction::to_internal() {

    ::protocol::Transaction encode;

    encode.clear_raw_data();
    if (0 < raw_data.serialize().size()) {
        *encode.mutable_raw_data() = raw_data.to_internal();
    }

    encode.clear_signature();
    if (0 < signature.size()) {
        encode.add_signature(&signature[0], signature.size());
    }

    return encode;
}


nlohmann::json Transaction::serialize() noexcept {

    if (txID.empty()) {
        auto serialized = raw_data.serialize();
        txID = Hash::sha256(Data(serialized.begin(), serialized.end()));
    }
    return transactionJSON(raw_data.to_internal(), txID, signature);
}


} // namespace TW::Tron end
