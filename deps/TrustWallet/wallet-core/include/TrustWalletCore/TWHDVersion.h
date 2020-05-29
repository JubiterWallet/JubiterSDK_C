// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "TWBase.h"

TW_EXTERN_C_BEGIN

///  Registered HD version bytes
///
/// - SeeAlso: https://github.com/satoshilabs/slips/blob/master/slip-0132.md
TW_EXPORT_ENUM(uint32_t)
enum TWHDVersion {
    TWHDVersionNone = 0,

    // Bitcoin
    // Bitcoin - mainnet
    // m/44'/0', P2PKH or P2SH
    TWHDVersionXPUB = 0x0488b21e,
    TWHDVersionXPRV = 0x0488ade4,
    // m/49'/0', P2WPKH in P2SH
    TWHDVersionYPUB = 0x049d7cb2,
    TWHDVersionYPRV = 0x049d7878,
    // m/84'/0', P2WPKH
    TWHDVersionZPUB = 0x04b24746,
    TWHDVersionZPRV = 0x04b2430c,

    // Litecoin
    // Litecoin - mainnet
    // m/44'/2', P2PKH or P2SH
    TWHDVersionLTUB = 0x019da462,
    TWHDVersionLTPV = 0x019d9cfe,
    // m/49'/1', P2WPKH in P2SH
    TWHDVersionMTUB = 0x01b26ef6,
    TWHDVersionMTPV = 0x01b26792,

    // Decred, Hcash
    // Decred, Hcash - mainnet
    TWHDVersionDPUB = 0x02fda926,
    TWHDVersionDPRV = 0x02fda4e8,

//    // Dogecoin
//    TWHDVersionDGUB = 0x02facafd,
//    TWHDVersionDGPV = 0x02fac398,
};

TW_EXPORT_PROPERTY
bool TWHDVersionIsPublic(enum TWHDVersion version);

TW_EXPORT_PROPERTY
bool TWHDVersionIsPrivate(enum TWHDVersion version);

TW_EXTERN_C_END
