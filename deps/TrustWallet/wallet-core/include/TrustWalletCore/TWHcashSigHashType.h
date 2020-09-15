// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWBase.h>

TW_EXTERN_C_BEGIN

// JuBiter-defined
enum TWHcashSigHashType {
    TWHcashSigHashOld = 0x00,
    TWHcashSigHashAll = 0x01,
    TWHcashSigHashNone =  0x02,
    TWHcashSigHashAllValue = 0x04,
    TWHcashSigHashAnyOneCanPay = 0x80
};

// JuBiter-defined
TW_EXPORT_METHOD
bool TWHcashSigHashTypeIsAll(enum TWHcashSigHashType type);

TW_EXTERN_C_END
