// Copyright © 2017-2020 JuBiter.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWBase.h>

TW_EXTERN_C_BEGIN

// JuBiter-defined
enum TWHcashSerializeType {
    TWHcashSerializeFull = 0x00,
    TWHcashSerializeNoWitness = 0x01,
    TWHcashSerializeOnlyWitness = 0x02,
    TWHcashSerializeWitnessSigning = 0x03,
    TWHcashSerializeWitnessValueSigning = 0x04
};

TW_EXTERN_C_END
