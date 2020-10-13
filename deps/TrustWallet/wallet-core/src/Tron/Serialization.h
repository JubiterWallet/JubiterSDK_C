// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once


#include "Transaction.h"


namespace TW::Tron {


nlohmann::json transactionJSON(const ::protocol::Transaction_raw& raw, const TW::Data& txID, const TW::Data& signature);


} // namespace TW::Tron end
