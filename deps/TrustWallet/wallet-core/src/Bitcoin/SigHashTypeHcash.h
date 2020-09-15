// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWHcashSigHashType.h>
#include "SigHashType.h"

namespace TW::Bitcoin {

// JuBiter-defined
inline bool hashTypeIsAll(enum TWHcashSigHashType type) { return (type & SigHashMask) == TWHcashSigHashAll; }

} // namespace TW::Bitcoin
