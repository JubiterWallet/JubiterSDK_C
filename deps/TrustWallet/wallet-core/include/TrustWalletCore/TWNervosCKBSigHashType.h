// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWBase.h>

TW_EXTERN_C_BEGIN

TW_EXPORT_ENUM(uint8_t)
enum TWNervosCKBCodeHashIndex {
    TWNervosCKBSingleSig    = 0x00,
    TWNervosCKBMultiSig     = 0x01,
    TWNervosCKBAnyOneCanPay = 0x02
};

//TW_EXPORT_METHOD
//bool TWBitcoinSigHashTypeIsSingle(enum TWBitcoinSigHashType type);
//
//TW_EXPORT_METHOD
//bool TWBitcoinSigHashTypeIsNone(enum TWBitcoinSigHashType type);
//
TW_EXTERN_C_END
