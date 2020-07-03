// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWHDVersion.h>

#pragma clang diagnostic push
#pragma clang diagnostic fatal "-Wswitch"

bool TWHDVersionIsPublic(enum TWHDVersion version) {
    switch (version) {
    case TWHDVersionXPUB:
    case TWHDVersionYPUB:
    case TWHDVersionZPUB:
    case TWHDVersionTPUB:
    case TWHDVersionUPUB:
    case TWHDVersionVPUB:
    case TWHDVersionLTUB:
    case TWHDVersionMTUB:
    case TWHDVersionTTUB:
    case TWHDVersionDPUB:
//    case TWHDVersionDGUB:
        return true;

    case TWHDVersionXPRV:
    case TWHDVersionYPRV:
    case TWHDVersionZPRV:
    case TWHDVersionTPRV:
    case TWHDVersionUPRV:
    case TWHDVersionVPRV:
    case TWHDVersionLTPV:
    case TWHDVersionMTPV:
    case TWHDVersionTTPV:
    case TWHDVersionDPRV:
//    case TWHDVersionDGPV:
        return false;

    case TWHDVersionNone:
    default:
        return false;
    }
}

bool TWHDVersionIsPrivate(enum TWHDVersion version) {
    if (version == TWHDVersionNone) return false;
    return !TWHDVersionIsPublic(version);
}

#pragma clang diagnostic pop
