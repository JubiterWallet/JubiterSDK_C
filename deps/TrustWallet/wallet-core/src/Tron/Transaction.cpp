// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Vote.h"

#include "Serialization.h"
#include "BinaryCoding.h"


namespace TW::Tron {


nlohmann::json Transaction::serialize() const noexcept {

    return transactionJSON(raw_data.to_internal(), txID, signature);
}


} // namespace TW::Tron end
