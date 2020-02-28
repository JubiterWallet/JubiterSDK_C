// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCurve.h>
#include <TrezorCrypto/curves.h>
#include "../Data.h"

char* TWCurve2name(enum TWCurve curve) {

    char *name = (char*)SECP256K1_NAME;

    switch (curve) {
    case TWCurveED25519:
        name = (char*)ED25519_NAME;
        break;
    case TWCurveED25519Blake2bNano:
        name = (char*)ED25519_BLAKE2B_NANO_NAME;
        break;
    case TWCurveNIST256p1:
        name = (char*)NIST256P1_NAME;
        break;
    case TWCurveSECP256k1:
    default:
        break;
    }

    return name;
}

// JuBiter-defined
enum TWCurve curveName2TWCurve(char* name) {

    enum TWCurve curve = TWCurveSECP256k1;
    if (   ED25519_NAME         == name
        || ED25519_CARDANO_NAME == name
        || ED25519_SHA3_NAME    == name
        || ED25519_KECCAK_NAME  == name
        ) {
        curve = TWCurveED25519;
    }
    else if (ED25519_BLAKE2B_NANO_NAME == name) {
        curve = TWCurveED25519Blake2bNano;
    }
    else if (NIST256P1_NAME == name) {
        curve = TWCurveNIST256p1;
    }

    return curve;
}

// JuBiter-defined
bool hasherType2Hasher(const HasherType hasherType, TW::Hash::Hasher& hasher) {
    bool b = true;

    switch (hasherType) {
    case HASHER_SHA2:
        hasher = [](const TW::byte* begin, const TW::byte* end) mutable -> TW::Data {
           return TW::Hash::sha256(begin, end);
        };
        break;
    case HASHER_SHA2D:
        hasher = TW::Hash::sha256d;
        break;
    case HASHER_SHA2_RIPEMD:
        hasher = TW::Hash::sha256ripemd;
        break;
    case HASHER_BLAKE:
        hasher = [](const TW::byte* begin, const TW::byte* end) mutable -> TW::Data {
           return TW::Hash::blake256(begin, end);
        };
        break;
    case HASHER_BLAKED:
        hasher = TW::Hash::blake256d;
        break;
    case HASHER_BLAKE_RIPEMD:
        hasher = TW::Hash::blake256ripemd;
        break;
    case HASHER_GROESTLD_TRUNC:
        hasher = TW::Hash::groestl512d;
        break;
    case HASHER_BLAKE2B:
    case HASHER_BLAKE2B_PERSONAL:
    default:
        b = false;
        break;
    }

    return b;
}
