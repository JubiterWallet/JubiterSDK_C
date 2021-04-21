//
//  TrezorCryptoToken.cpp
//  JubSDK
//
//  Created by panmin on 2021/4/19.
//  Copyright © 2021 JuBiter. All rights reserved.
//

#include <stdio.h>
#include <TrezorCrypto/bip39.h>
#include "token/TrezorCrypto/TrezorCryptoToken.h"
#include "utility/util.h"


JUB_RV _curveToString(JUB_ENUM_CURVES enumCurve, std::string& strCurve) {

    switch(enumCurve) {
    case JUB_ENUM_CURVES::SECP256K1:
        strCurve = SECP256K1_NAME;
        return JUBR_OK;
    case JUB_ENUM_CURVES::ED25519:
        strCurve = ED25519_NAME;
        return JUBR_OK;
    case JUB_ENUM_CURVES::NIST256P1:
        strCurve = NIST256P1_NAME;
        return JUBR_OK;
    default:
        return JUBR_ARGUMENTS_BAD;
    }
}


namespace jub {
namespace token {


JUB_RV TrezorCryptoToken::ToMasterKey(const JUB_ENUM_CURVES& curve, const std::string& privOrPub,
                                      const JUB_UINT32 xpubPrefix,
                                      const JUB_UINT32 xprvPrefix) {

    JUB_RV rv = JUBR_ARGUMENTS_BAD;

    switch (curve) {
    case JUB_ENUM_CURVES::SECP256K1:
    {
        HDNode hdkey;
        uint32_t parentFingerprint;
        // bad data
        if(0 != hdnode_deserialize(privOrPub.c_str(),
                                   xpubPrefix, xprvPrefix,
                                   SECP256K1_NAME, &hdkey, &parentFingerprint)) {
            _MasterKey_XPRV = "";
            _MasterKey_XPUB = "";
            _type = JUB_SoftwareTokenType::NONE;
            return rv;
        }

        //xprv
        JUB_BYTE zero[33] = {0,};
        if(0 != memcmp(hdkey.private_key, zero, sizeof(hdkey.private_key)/sizeof(uint8_t))) {
            hdnode_fill_public_key(&hdkey);
            JUB_CHAR _pk[200] = {0,};
            if (0 == hdnode_serialize_public(&hdkey, parentFingerprint, xpubPrefix, _pk, sizeof(_pk) / sizeof(JUB_CHAR))) {
                _type = JUB_SoftwareTokenType::NONE;
                return JUBR_ARGUMENTS_BAD;
            }

            _MasterKey_XPRV = privOrPub;
            _MasterKey_XPUB = _pk;
            _type = JUB_SoftwareTokenType::PRIVATE;
            return JUBR_OK;
        }

        if(0 != memcmp(hdkey.public_key, zero, sizeof(hdkey.public_key)/sizeof(uint8_t))) {
            _MasterKey_XPRV = "";
            _MasterKey_XPUB = std::string(privOrPub);
            _type = JUB_SoftwareTokenType::PUBLIC;
            return JUBR_OK;
        }

        _type = JUB_SoftwareTokenType::NONE;
        break;
    }
    case JUB_ENUM_CURVES::NIST256P1:
    {
        rv = JUBR_IMPL_NOT_SUPPORT;
        break;
    }
    case JUB_ENUM_CURVES::ED25519:
    {
        break;
    }
    default:
        break;
    }

    return rv;
}


JUB_RV TrezorCryptoToken::MnemonicToMasterPrivateKey(const JUB_ENUM_CURVES& curve,
                                                     const std::string& passphrase, const std::string& mnemonic) {
    JUB_RV rv = JUBR_ARGUMENTS_BAD;

    switch (curve) {
    case JUB_ENUM_CURVES::SECP256K1:
    {
        uchar_vector vSeed;
        JUB_VERIFY_RV(MnemonicToSeed(passphrase, mnemonic, vSeed, nullptr));

        std::string xpub;
        std::string xprv;
        JUB_VERIFY_RV(SeedToMasterPrivateKey(vSeed, curve, xpub, xprv));

        _MasterKey_XPUB = xpub;
        _MasterKey_XPRV = xprv;

        if ("" != _MasterKey_XPUB) {
            _type = JUB_SoftwareTokenType::PUBLIC;
        }
        if ("" != _MasterKey_XPRV) {
            _type = JUB_SoftwareTokenType::PRIVATE;
        }
        rv = JUBR_OK;
        break;
    }
    case JUB_ENUM_CURVES::ED25519:
    default:
        break;
    }

    return rv;
}


JUB_RV TrezorCryptoToken::MnemonicToSeed(const std::string& passphrase, const std::string& mnemonic,
                                         uchar_vector& vSeed,
                                         void (*progress_callback)(JUB_UINT32 current, JUB_UINT32 total)) {

    JUB_BYTE seed[64] = {0x00,};
    mnemonic_to_seed(mnemonic.c_str(), passphrase.c_str(), seed, progress_callback);
    if (0 == vSeed.size()) {
        vSeed.resize(64);
    }
    std::copy(std::begin(seed), std::end(seed), std::begin(vSeed));

    return JUBR_OK;
}


JUB_RV TrezorCryptoToken::SeedToMasterPrivateKey(const uchar_vector& seed,
                                                 const JUB_ENUM_CURVES& curve,
                                                 std::string& xpub, std::string& xprv) {

    if (0 == seed.size()) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::string curve_name;
    JUB_VERIFY_RV(_curveToString(curve, curve_name));

    JUB_UINT32 hdVersionPrv = TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin);
    JUB_UINT32 hdVersionPub = TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin);

    HDNode node;
    if (0 == hdnode_from_seed(&seed[0], (int)seed.size(),
                              curve_name.c_str(),
                              &node)) {
        return JUBR_ERROR;
    }

    JUB_CHAR str_pri[200] = {0,};
    if (0 == hdnode_serialize_private(&node, 0x00, hdVersionPrv, str_pri, sizeof(str_pri) / sizeof(JUB_CHAR))) {
        return JUBR_ERROR;
    }

    JUB_CHAR str_pub[200] = {0,};
    if (0 == hdnode_serialize_public(&node, 0x00, hdVersionPub, str_pub, sizeof(str_pub) / sizeof(JUB_CHAR))) {
        return JUBR_ERROR;
    }

    xpub = std::string(str_pub);
    xprv = std::string(str_pri);

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
