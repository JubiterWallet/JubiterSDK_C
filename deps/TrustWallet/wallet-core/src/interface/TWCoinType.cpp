// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWHDVersion.h>

//#include "../Coin.h"

//enum TWBlockchain TWCoinTypeBlockchain(enum TWCoinType coin) {
//    return TW::blockchain(coin);
//}
//
//enum TWPurpose TWCoinTypePurpose(enum TWCoinType coin) {
//    return TW::purpose(coin);
//}
//
//enum TWCurve TWCoinTypeCurve(enum TWCoinType coin) {
//    return TW::curve(coin);
//}
//
//enum TWHDVersion TWCoinTypeXpubVersion(enum TWCoinType coin) {
//    return TW::xpubVersion(coin);
//}
//
//enum TWHDVersion TWCoinTypeXprvVersion(enum TWCoinType coin) {
//    return TW::xprvVersion(coin);
//}
//
//bool TWCoinTypeValidate(enum TWCoinType coin, TWString *_Nonnull address) {
//    return TW::validateAddress(coin, *reinterpret_cast<const std::string*>(address));
//}
//
//TWString *_Nonnull TWCoinTypeDerivationPath(enum TWCoinType coin) {
//    const auto path = TW::derivationPath(coin);
//    const auto string = path.string();
//    return TWStringCreateWithUTF8Bytes(string.c_str());
//}
//
//TWString *_Nonnull TWCoinTypeDeriveAddress(enum TWCoinType coin, struct TWPrivateKey *_Nonnull privateKey) {
//    const auto string = TW::deriveAddress(coin, privateKey->impl);
//    return TWStringCreateWithUTF8Bytes(string.c_str());
//}
//
//TWString *_Nonnull TWCoinTypeDeriveAddressFromPublicKey(enum TWCoinType coin, struct TWPublicKey *_Nonnull publicKey) {
//    const auto string = TW::deriveAddress(coin, publicKey->impl);
//    return TWStringCreateWithUTF8Bytes(string.c_str());
//}

/// P2PKH prefix for this coin type
uint8_t TWCoinTypeP2pkhPrefix(enum TWCoinType coin) {

    uint8_t prefix = 0x00;

    switch (coin) {
    case TWCoinTypeBitcoin:
    case TWCoinTypeBitcoinCash:
        // https://en.bitcoin.it/wiki/List_of_address_prefixes
        prefix = 0x00;
        break;
    case TWCoinTypeLitecoin:
        prefix = 0x30;
        break;
    case TWCoinTypeQtum:
        prefix = 0x3a;
        break;
    case TWCoinTypeDash:
        prefix = 0x4c;
        break;
    case TWCoinTypeAeternity:
    case TWCoinTypeAion:
    case TWCoinTypeBinance:
    case TWCoinTypeBravoCoin:
    case TWCoinTypeCallisto:
    case TWCoinTypeCardano:
    case TWCoinTypeCosmos:
    case TWCoinTypeDecred:
    case TWCoinTypeDigiByte:
    case TWCoinTypeDogecoin:
    case TWCoinTypeEOS:
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeFIO:
    case TWCoinTypeGoChain:
    case TWCoinTypeGroestlcoin:
    case TWCoinTypeHcash:
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
    case TWCoinTypeBitcoin:
    case TWCoinTypeBitcoinCash:
        // https://en.bitcoin.it/wiki/List_of_address_prefixes
        prefix = 0x05;
        break;
    case TWCoinTypeLitecoin:
    case TWCoinTypeQtum:
        prefix = 0x32;
        break;
    case TWCoinTypeDash:
        prefix = 0x10;
        break;
    case TWCoinTypeAeternity:
    case TWCoinTypeAion:
    case TWCoinTypeBinance:
    case TWCoinTypeBravoCoin:
    case TWCoinTypeCallisto:
    case TWCoinTypeCardano:
    case TWCoinTypeCosmos:
    case TWCoinTypeDecred:
    case TWCoinTypeDigiByte:
    case TWCoinTypeDogecoin:
    case TWCoinTypeEOS:
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeFIO:
    case TWCoinTypeGoChain:
    case TWCoinTypeGroestlcoin:
    case TWCoinTypeHcash:
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


// JuBiter-defined
std::vector<uint8_t> TWCoinTypeP2pkhPrefixData(enum TWCoinType coin) {
    std::vector<uint8_t> prefix;

    switch (coin) {
    case TWCoinTypeHcash:
        prefix = {0x09, 0x7f};  //MainNetID
//        prefix = {0x0f, 0x21};  //TestNetID
        break;
    case TWCoinTypeBitcoin:
    case TWCoinTypeBitcoinCash:
    case TWCoinTypeLitecoin:
    case TWCoinTypeQtum:
    case TWCoinTypeAeternity:
    case TWCoinTypeAion:
    case TWCoinTypeBinance:
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


// JuBiter-defined
uint32_t TWCoinType2HDVersionPublic(enum TWCoinType coin, bool witness) {
    uint32_t hdVersionPublic = TWHDVersionNone;

    switch (coin) {
    case TWCoinTypeBitcoin:
    case TWCoinTypeBitcoinCash:
    case TWCoinTypeQtum:
    case TWCoinTypeDash:
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeEOS:
    case TWCoinTypeXRP:
    case TWCoinTypeLitecoin:
        hdVersionPublic = TWHDVersionXPUB;
        if (witness) {
            hdVersionPublic = TWHDVersionYPUB;
        }
        break;
    case TWCoinTypeHcash:
        hdVersionPublic = TWHDVersionDPUB;
        break;
    case TWCoinTypeAeternity:
    case TWCoinTypeAion:
    case TWCoinTypeBinance:
    case TWCoinTypeBravoCoin:
    case TWCoinTypeCallisto:
    case TWCoinTypeCardano:
    case TWCoinTypeCosmos:
    case TWCoinTypeDecred:
    case TWCoinTypeDigiByte:
    case TWCoinTypeDogecoin:
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

    return hdVersionPublic;
}


// JuBiter-defined
uint32_t TWCoinType2HDVersionPrivate(enum TWCoinType coin, bool witness) {
    uint32_t hdVersionPrivate = TWHDVersionNone;

    switch (coin) {
    case TWCoinTypeBitcoin:
    case TWCoinTypeBitcoinCash:
    case TWCoinTypeQtum:
    case TWCoinTypeDash:
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeEOS:
    case TWCoinTypeXRP:
    case TWCoinTypeLitecoin:
        hdVersionPrivate = TWHDVersionXPRV;
        if (witness) {
            hdVersionPrivate = TWHDVersionYPRV;
        }
        break;
    case TWCoinTypeHcash:
        hdVersionPrivate = TWHDVersionDPRV;
        break;
    case TWCoinTypeAeternity:
    case TWCoinTypeAion:
    case TWCoinTypeBinance:
    case TWCoinTypeBravoCoin:
    case TWCoinTypeCallisto:
    case TWCoinTypeCardano:
    case TWCoinTypeCosmos:
    case TWCoinTypeDecred:
    case TWCoinTypeDigiByte:
    case TWCoinTypeDogecoin:
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

    return hdVersionPrivate;
}
