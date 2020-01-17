// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCoinType.h>

#include <cstring>

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

/// P2PKH prefix for this coin type
uint8_t TWCoinTypeP2pkhPrefix(enum TWCoinType coin) {

    uint8_t prefix = 0x00;

    switch (coin) {
    case TWCoinTypeAeternity:
    case TWCoinTypeAion:
    case TWCoinTypeBinance:
        break;
    case TWCoinTypeBitcoin:
    case TWCoinTypeBitcoinCash:
        // https://en.bitcoin.it/wiki/List_of_address_prefixes
        prefix = 0x00;
        break;
    case TWCoinTypeBravoCoin:
    case TWCoinTypeCallisto:
    case TWCoinTypeCardano:
    case TWCoinTypeCosmos:
    case TWCoinTypeDash:
    case TWCoinTypeDecred:
    case TWCoinTypeDigiByte:
    case TWCoinTypeDogecoin:
    case TWCoinTypeEOS:
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeFIO:
    case TWCoinTypeGoChain:
    case TWCoinTypeGroestlcoin:
    case TWCoinTypeICON:
    case TWCoinTypeIoTeX:
    case TWCoinTypeKava:
    case TWCoinTypeKin:
    case TWCoinTypeLitecoin:
        prefix = 0x30;
        break;
    case TWCoinTypeMonacoin:
    case TWCoinTypeNebulas:
    case TWCoinTypeNULS:
    case TWCoinTypeNano:
    case TWCoinTypeNEAR:
    case TWCoinTypeNimiq:
    case TWCoinTypeOntology:
    case TWCoinTypePOANetwork:
    case TWCoinTypeQtum:
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
    case TWCoinTypeViacoin:
    case TWCoinTypeWanchain:
    case TWCoinTypeZcash:
    case TWCoinTypeZcoin:
    case TWCoinTypeZilliqa:
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

    return prefix;
}

/// P2SH prefix for this coin type
uint8_t TWCoinTypeP2shPrefix(enum TWCoinType coin) {

    uint8_t prefix = 0x00;

    switch (coin) {
    case TWCoinTypeAeternity:
    case TWCoinTypeAion:
    case TWCoinTypeBinance:
        break;
    case TWCoinTypeBitcoin:
    case TWCoinTypeBitcoinCash:
    case TWCoinTypeLitecoin:
        // https://en.bitcoin.it/wiki/List_of_address_prefixes
        prefix = 0x05;
        break;
    case TWCoinTypeBravoCoin:
    case TWCoinTypeCallisto:
    case TWCoinTypeCardano:
    case TWCoinTypeCosmos:
    case TWCoinTypeDash:
    case TWCoinTypeDecred:
    case TWCoinTypeDigiByte:
    case TWCoinTypeDogecoin:
    case TWCoinTypeEOS:
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeFIO:
    case TWCoinTypeGoChain:
    case TWCoinTypeGroestlcoin:
    case TWCoinTypeICON:
    case TWCoinTypeIoTeX:
    case TWCoinTypeKava:
    case TWCoinTypeKin:
    case TWCoinTypeMonacoin:
    case TWCoinTypeNebulas:
    case TWCoinTypeNULS:
    case TWCoinTypeNano:
    case TWCoinTypeNEAR:
    case TWCoinTypeNimiq:
    case TWCoinTypeOntology:
    case TWCoinTypePOANetwork:
    case TWCoinTypeQtum:
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
    case TWCoinTypeViacoin:
    case TWCoinTypeWanchain:
    case TWCoinTypeZcash:
    case TWCoinTypeZcoin:
    case TWCoinTypeZilliqa:
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

    return prefix;
}

/// Static prefix for this coin type
uint8_t TWCoinTypeStaticPrefix(enum TWCoinType coin) {
    return 0;
}
