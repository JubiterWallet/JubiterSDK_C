// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCurve.h>
#include <TrezorCrypto/curves.h>

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
