// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWBase.h>
#include "Hash.h"
#include <TrezorCrypto/hasher.h>

TW_EXTERN_C_BEGIN

/// Elliptic cruves
TW_EXPORT_ENUM()
enum TWCurve {
    TWCurveSECP256k1              /* "secp256k1" */,
    TWCurveED25519                /* "ed25519" */,
    TWCurveSR25519                /* "sr25519" */, ///JuBiter-defined
    TWCurveED25519Blake2bNano     /* "ed25519-blake2b-nano" */,
    TWCurveCurve25519             /* "curve25519" */,
    TWCurveNIST256p1              /* "nist256p1" */,
    TWCurveED25519Extended        /* "ed25519-cardano-seed" */,
    TWCurveNone
};

const char* TWCurve2name(TWCurve curve);

// JuBiter-defined
enum TWCurve curveName2TWCurve(char* name);

// JuBiter-defined
bool hasherType2Hasher(const HasherType hasherType, TW::Hash::Hasher& hasher);

TW_EXTERN_C_END
