// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCurve.h>
#include <TrezorCrypto/curves.h>
#include "../Data.h"

const char* TWCurve2name(TWCurve curve) {
    switch (curve) {
    case TWCurveSECP256k1:
        return SECP256K1_NAME;
    case TWCurveED25519:
        return ED25519_NAME;
    case TWCurveED25519Blake2bNano:
        return ED25519_BLAKE2B_NANO_NAME;
    case TWCurveED25519Extended:
        return ED25519_CARDANO_NAME;
    case TWCurveNIST256p1:
        return NIST256P1_NAME;
    case TWCurveCurve25519:
        return CURVE25519_NAME;
    case TWCurveNone:
    default:
        return "";
    }
}

// JuBiter-defined
enum TWCurve curveName2TWCurve(char* name) {

    enum TWCurve curve = TWCurveNone;
    if (SECP256K1_NAME == name) {
        curve = TWCurveSECP256k1;
    }
    else if (      ED25519_NAME == name
        || ED25519_CARDANO_NAME == name
        || ED25519_SHA3_NAME    == name
        || ED25519_KECCAK_NAME  == name
        ) {
        curve = TWCurveED25519;
    }
    else if (ED25519_BLAKE2B_NANO_NAME == name) {
        curve = TWCurveED25519Blake2bNano;
    }
    else if (ED25519_CARDANO_NAME == name) {
        curve = TWCurveED25519Extended;
    }
    else if (NIST256P1_NAME == name) {
        curve = TWCurveNIST256p1;
    }
    else if (CURVE25519_NAME == name) {
        curve = TWCurveCurve25519;
    }

    return curve;
}

// JuBiter-defined
bool hasherType2Hasher(const HasherType hasherType, TW::Hash::Hasher& hasher) {
    bool b = true;

    switch (hasherType) {
    case HASHER_SHA2:
//        hasher = [](const TW::byte* begin, const TW::byte* end) mutable -> TW::Data {
//           return TW::Hash::sha256(begin, end);
//        };
        hasher = static_cast<TW::Hash::HasherSimpleType>(TW::Hash::sha256);
        break;
    case HASHER_SHA2D:
        hasher = TW::Hash::sha256d;
        break;
    case HASHER_SHA2_RIPEMD:
        hasher = TW::Hash::sha256ripemd;
        break;
    case HASHER_BLAKE:
//        hasher = [](const TW::byte* begin, const TW::byte* end) mutable -> TW::Data {
//           return TW::Hash::blake256(begin, end);
//        };
        hasher = static_cast<TW::Hash::HasherSimpleType>(TW::Hash::blake256);
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
    case HASHER_SHA2_KECCAK:
//        hasher = [](const TW::byte* begin, const TW::byte* end) mutable -> TW::Data {
//           return TW::Hash::keccak256(begin, end);
//        };
        hasher = static_cast<TW::Hash::HasherSimpleType>(TW::Hash::keccak256);
        break;
    case HASHER_SHA3_KECCAK:
//        hasher = [](const TW::byte* begin, const TW::byte* end) mutable -> TW::Data {
//           return TW::Hash::keccak512(begin, end);
//        };
        hasher = static_cast<TW::Hash::HasherSimpleType>(TW::Hash::keccak512);
        break;
    case HASHER_BLAKE2B:
    case HASHER_BLAKE2B_PERSONAL:
    default:
        b = false;
        break;
    }

    return b;
}
