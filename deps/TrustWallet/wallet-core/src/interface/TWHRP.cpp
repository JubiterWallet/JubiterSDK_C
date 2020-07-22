// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWCoinType.h>

#include <cstring>

const char* stringForHRP(enum TWHRP hrp) {
    switch (hrp) {
    case TWHRPBitcoin: return HRP_BITCOIN;
    case TWHRPLitecoin: return HRP_LITECOIN;
    case TWHRPViacoin: return HRP_VIACOIN;
    case TWHRPBitcoinCash: return HRP_BITCOINCASH;
    case TWHRPBinance: return HRP_BINANCE;
    case TWHRPCosmos: return HRP_COSMOS;
    case TWHRPDigiByte: return HRP_DIGIBYTE;
    case TWHRPGroestlcoin: return HRP_GROESTLCOIN;
    case TWHRPQtum: return HRP_QTUM;
    case TWHRPZilliqa: return HRP_ZILLIQA;
    default: return "";
    }
}

enum TWHRP hrpForString(const char *_Nullable string) {
    if (std::strcmp(string, HRP_BITCOIN) == 0) {
        return TWHRPBitcoin;
    } else if (std::strcmp(string, HRP_LITECOIN) == 0) {
        return TWHRPLitecoin;
    } else if (std::strcmp(string, HRP_VIACOIN) == 0) {
        return TWHRPViacoin;
    } else if (std::strcmp(string, HRP_BITCOINCASH) == 0) {
        return TWHRPBitcoinCash;
    } else if (std::strcmp(string, HRP_BINANCE) == 0) {
        return TWHRPBinance;
    } else if (std::strcmp(string, HRP_COSMOS) == 0) {
        return TWHRPCosmos;
    } else if (std::strcmp(string, HRP_DIGIBYTE) == 0) {
        return TWHRPDigiByte;
    } else if (std::strcmp(string, HRP_GROESTLCOIN) == 0) {
        return TWHRPGroestlcoin;
    } else if (std::strcmp(string, HRP_QTUM) == 0) {
        return TWHRPQtum;
    } else if (std::strcmp(string, HRP_ZILLIQA) == 0) {
        return TWHRPZilliqa;
    } else {
        return TWHRPUnknown;
    }
}

/// HRP for this coin type
//enum TWHRP TWCoinTypeHRP(enum TWCoinType coin);
TWHRP TWCoinTypeHRP(enum TWCoinType coin) {

    TWHRP hrp = TWHRP::TWHRPUnknown;

    switch (coin) {
    case TWCoinTypeAeternity:
    case TWCoinTypeAion:
        break;
    case TWCoinTypeBinance:
        hrp = TWHRP::TWHRPBinance;
        break;
    case TWCoinTypeBitcoin:
        hrp = TWHRP::TWHRPBitcoin;
        break;
    case TWCoinTypeBitcoinCash:
        hrp = TWHRP::TWHRPBitcoinCash;
        break;
    case TWCoinTypeBravoCoin:
    case TWCoinTypeCallisto:
    case TWCoinTypeCardano:
        break;
    case TWCoinTypeCosmos:
        hrp = TWHRP::TWHRPCosmos;
        break;
    case TWCoinTypeDash:
    case TWCoinTypeDecred:
        break;
    case TWCoinTypeDigiByte:
        hrp = TWHRP::TWHRPDigiByte;
        break;
    case TWCoinTypeDogecoin:
    case TWCoinTypeEOS:
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeFIO:
    case TWCoinTypeGoChain:
        break;
    case TWCoinTypeGroestlcoin:
        hrp = TWHRP::TWHRPGroestlcoin;
        break;
    case TWCoinTypeICON:
    case TWCoinTypeIoTeX:
    case TWCoinTypeKava:
    case TWCoinTypeKin:
        break;
    case TWCoinTypeLitecoin:
        hrp = TWHRP::TWHRPLitecoin;
        break;
    case TWCoinTypeMonacoin:
    case TWCoinTypeNebulas:
    case TWCoinTypeNULS:
    case TWCoinTypeNano:
    case TWCoinTypeNEAR:
    case TWCoinTypeNimiq:
    case TWCoinTypeOntology:
    case TWCoinTypePOANetwork:
        break;
    case TWCoinTypeQtum:
        hrp = TWHRP::TWHRPQtum;
        break;
    case TWCoinTypeXRP:
    case TWCoinTypeSolana:
    case TWCoinTypeStellar:
    case TWCoinTypeTON:
    case TWCoinTypeTezos:
    case TWCoinTypeTheta:
    case TWCoinTypeThunderToken:
    case TWCoinTypeTomoChain:
    case TWCoinTypeTron:
    case TWCoinTypeVeChain:
        break;
    case TWCoinTypeViacoin:
        hrp = TWHRP::TWHRPViacoin;
        break;
    case TWCoinTypeWanchain:
    case TWCoinTypeZcash:
    case TWCoinTypeZcoin:
        break;
    case TWCoinTypeZilliqa:
        hrp = TWHRP::TWHRPZilliqa;
        break;
    case TWCoinTypeZelcash:
    case TWCoinTypeRavencoin:
    case TWCoinTypeWaves:
    case TWCoinTypeTerra:
    case TWCoinTypeHarmony:
    case TWCoinTypeAlgorand:
    case TWCoinTypeKusama:
    case TWCoinTypePolkadot:
    default:
        break;
    }

    return hrp;
}
