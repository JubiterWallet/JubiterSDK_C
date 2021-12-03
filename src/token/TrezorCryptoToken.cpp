//
//  TrezorCryptoToken.cpp
//  JubSDK
//
//  Created by panmin on 2021/4/19.
//  Copyright © 2021 JuBiter. All rights reserved.
//

#include "token/TrezorCrypto/TrezorCryptoToken.h"
#include "PrivateKey.h"
#include "utility/util.h"
#include <TrezorCrypto/bip39.h>
#include <cstring>
#include <stdio.h>
#include <string>

extern "C" {
#include "sr25519.h"
#include <iostream>
#include <vector>
}

JUB_RV _curveToString(JUB_ENUM_CURVES enumCurve, std::string &strCurve) {

    switch (enumCurve) {
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

JUB_RV TrezorCryptoToken::_HdnodeCkd(const std::string &path, HDNode *node, JUB_UINT32 *parentFingerprint,
                                     const TWCoinType &coinNet) {
    TWCoinType _coinNet = coinNet;

    JUB_BIP32_PATH spiltPath = spiltMainPath(path, '/');

    bool witness = false;
    bool nested  = false;
    if (0 == strcmp(spiltPath.purpose, "49'")) {
        witness = true;
        nested  = true;
    }
    if (0 == strcmp(spiltPath.purpose, "84'") || 0 == strcmp(spiltPath.purpose, "86'")) {
        witness = true;
        nested  = false;
    }

    HDNode masterNode;
    JUB_UINT32 hdVersionPrv = TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin);
    JUB_UINT32 hdVersionPub = TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin);
    std::string curve_name  = SECP256K1_NAME;
    if (0 ==
        hdnode_deserialize(_MasterKey_XPRV.data(), hdVersionPub, hdVersionPrv, curve_name.c_str(), &masterNode, 0x00)) {
        hdnode_fill_public_key(&masterNode);

        JUB_CHAR str_pri[200] = {
            0,
        };
        JUB_CHAR str_pub[200] = {
            0,
        };

        hdVersionPrv = TWCoinType2HDVersionPrivate(_coinNet, witness, nested);
        hdVersionPub = TWCoinType2HDVersionPublic(_coinNet, witness, nested);
        if (0 ==
            hdnode_serialize_private(&masterNode, 0x00, hdVersionPrv, str_pri, sizeof(str_pri) / sizeof(JUB_CHAR))) {
            return JUBR_ERROR;
        }
        if (0 ==
            hdnode_serialize_public(&masterNode, 0x00, hdVersionPub, str_pub, sizeof(str_pub) / sizeof(JUB_CHAR))) {
            return JUBR_ERROR;
        }

        _MasterKey_XPRV = str_pri;
        _MasterKey_XPUB = str_pub;
    }

    if (TWCoinType::TWCoinTypeBitcoinTestNet == _coinNet) {
        HDNode masterNode;
        JUB_UINT32 hdVersionPrv = TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin);
        JUB_UINT32 hdVersionPub = TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin);
        std::string curve_name  = SECP256K1_NAME;
        if (0 == hdnode_deserialize(_MasterKey_XPRV.data(), hdVersionPub, hdVersionPrv, curve_name.c_str(), &masterNode,
                                    0x00)) {
            hdnode_fill_public_key(&masterNode);

            JUB_CHAR str_pri[200] = {
                0,
            };
            JUB_CHAR str_pub[200] = {
                0,
            };

            hdVersionPrv = TWCoinType2HDVersionPrivate(_coinNet, witness, nested);
            hdVersionPub = TWCoinType2HDVersionPublic(_coinNet, witness, nested);
            if (0 == hdnode_serialize_private(&masterNode, 0x00, hdVersionPrv, str_pri,
                                              sizeof(str_pri) / sizeof(JUB_CHAR))) {
                return JUBR_ERROR;
            }
            if (0 ==
                hdnode_serialize_public(&masterNode, 0x00, hdVersionPub, str_pub, sizeof(str_pub) / sizeof(JUB_CHAR))) {
                return JUBR_ERROR;
            }

            _MasterKey_XPRV = str_pri;
            _MasterKey_XPUB = str_pub;
        }
    }

    if (JUB_SoftwareTokenType::PRIVATE == _type) {
        return hdnode_priv_ckd(_MasterKey_XPRV, path, _curve_name,
                               TWCoinType2HDVersionPublic((_coinNet ? _coinNet : _coin), witness, nested),
                               TWCoinType2HDVersionPrivate((_coinNet ? _coinNet : _coin), witness, nested), node,
                               parentFingerprint);
    }

    return hdnode_pub_ckd(
        _MasterKey_XPUB, path, _curve_name, TWCoinType2HDVersionPublic((_coinNet ? _coinNet : _coin), witness, nested),
        TWCoinType2HDVersionPrivate((_coinNet ? _coinNet : _coin), witness, nested), node, parentFingerprint);

    return JUBR_ERROR;
}

JUB_RV TrezorCryptoToken::ToMasterKey(const JUB_ENUM_CURVES &curve, const std::string &privOrPub,
                                      const JUB_UINT32 xpubPrefix, const JUB_UINT32 xprvPrefix) {

    JUB_RV rv = JUBR_ARGUMENTS_BAD;

    switch (curve) {
    case JUB_ENUM_CURVES::SECP256K1: {
        HDNode hdkey;
        uint32_t parentFingerprint;
        // bad data
        if (0 !=
            hdnode_deserialize(privOrPub.c_str(), xpubPrefix, xprvPrefix, SECP256K1_NAME, &hdkey, &parentFingerprint)) {
            _MasterKey_XPRV = "";
            _MasterKey_XPUB = "";
            _type           = JUB_SoftwareTokenType::NONE;
            return rv;
        }

        // xprv
        JUB_BYTE zero[33] = {
            0,
        };
        if (0 != memcmp(hdkey.private_key, zero, sizeof(hdkey.private_key) / sizeof(uint8_t))) {
            hdnode_fill_public_key(&hdkey);
            JUB_CHAR _pk[200] = {
                0,
            };
            if (0 ==
                hdnode_serialize_public(&hdkey, parentFingerprint, xpubPrefix, _pk, sizeof(_pk) / sizeof(JUB_CHAR))) {
                _type = JUB_SoftwareTokenType::NONE;
                return JUBR_ARGUMENTS_BAD;
            }

            _MasterKey_XPRV = privOrPub;
            _MasterKey_XPUB = _pk;
            _type           = JUB_SoftwareTokenType::PRIVATE;
            return JUBR_OK;
        }

        if (0 != memcmp(hdkey.public_key, zero, sizeof(hdkey.public_key) / sizeof(uint8_t))) {
            _MasterKey_XPRV = "";
            _MasterKey_XPUB = std::string(privOrPub);
            _type           = JUB_SoftwareTokenType::PUBLIC;
            return JUBR_OK;
        }

        _type = JUB_SoftwareTokenType::NONE;
        break;
    }
    case JUB_ENUM_CURVES::NIST256P1: {
        rv = JUBR_IMPL_NOT_SUPPORT;
        break;
    }
    case JUB_ENUM_CURVES::ED25519: {
        if (!TW::PrivateKey::isValid(uchar_vector(privOrPub))) {
            return JUBR_ARGUMENTS_BAD;
        }
        uchar_vector prvData(privOrPub);
        auto privateKey         = TW::PrivateKey(prvData);
        TW::PublicKey publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);

        std::string xpub = uchar_vector(publicKey.bytes).getHex();
        std::string xprv = uchar_vector(privateKey.bytes).getHex();
        _MasterKey_XPUB  = xpub;
        _MasterKey_XPRV  = privOrPub;

        if ("" != _MasterKey_XPUB) {
            _type = JUB_SoftwareTokenType::PUBLIC;
        }
        if ("" != _MasterKey_XPRV) {
            _type = JUB_SoftwareTokenType::PRIVATE;
        }
        rv = JUBR_OK;
        break;
    }
    case JUB_ENUM_CURVES::SR25519: {
        if (privOrPub.length() != SR25519_SEED_SIZE * 2) {
            return JUBR_ARGUMENTS_BAD;
        }
        uchar_vector vSeed(privOrPub);
        // Key derive requires a key of SR25519_KEYPAIR_SIZE length，When using a private key, use 64
        // bytes（SR25519_SECRET_SIZE）
        uint8_t privateKey[SR25519_KEYPAIR_SIZE];
        uint8_t publiKey[SR25519_PUBLIC_SIZE];

        std::vector<uint8_t> kp(SR25519_KEYPAIR_SIZE, 0);
        ///* Size of SR25519 PRIVATE (SECRET) KEY, which consists of [32 bytes key | 32 bytes nonce]
        sr25519_keypair_from_seed(kp.data(), vSeed.data());
        memcpy(privateKey, &kp[0], SR25519_KEYPAIR_SIZE);

        // memcpy(node.nonce,  &kp[0] + 32, 32);
        memcpy(publiKey, &kp[0] + 32 + 32, SR25519_PUBLIC_SIZE);
        uchar_vector vPublicKey(publiKey, sizeof(publiKey) / sizeof(uint8_t));
        uchar_vector vprivateKey(privateKey, sizeof(privateKey) / sizeof(uint8_t));

        std::string xpub = vPublicKey.getHex();
        std::string xprv = vprivateKey.getHex();

        _MasterKey_XPUB = xpub;
        _MasterKey_XPRV = xprv;

        if ("" != _MasterKey_XPUB) {
            _type = JUB_SoftwareTokenType::PUBLIC;
        }
        if ("" != _MasterKey_XPRV) {
            _type = JUB_SoftwareTokenType::PRIVATE;
        }
        break;
    }
    default:
        break;
    }

    return rv;
}

JUB_RV TrezorCryptoToken::MnemonicToMasterPrivateKey(const JUB_ENUM_CURVES &curve, const std::string &passphrase,
                                                     const std::string &mnemonic) {
    JUB_RV rv = JUBR_ARGUMENTS_BAD;

    switch (curve) {
    case JUB_ENUM_CURVES::SECP256K1: {
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
    case JUB_ENUM_CURVES::ED25519: {
        uchar_vector vSeed;
        JUB_VERIFY_RV(MnemonicToMiniSecret(passphrase, mnemonic, vSeed, nullptr));

        uchar_vector prvData(vSeed.begin(), vSeed.begin() + 32);
        std::cout << prvData.getHex() << std::endl;
        auto privateKey         = TW::PrivateKey(prvData);
        TW::PublicKey publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);

        std::string xpub = uchar_vector(publicKey.bytes).getHex();
        std::string xprv = uchar_vector(privateKey.bytes).getHex();

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
    case JUB_ENUM_CURVES::SR25519: {
        /// Size of SR25519 KEYPAIR. [32 bytes key | 32 bytes nonce | 32 bytes public]
        uchar_vector vSeed;
        // Key derive requires a key of SR25519_KEYPAIR_SIZE length，When using a private key, use 64
        // bytes（SR25519_SECRET_SIZE）
        uint8_t privateKey[SR25519_KEYPAIR_SIZE];
        uint8_t publiKey[SR25519_PUBLIC_SIZE];

        JUB_VERIFY_RV(MnemonicToMiniSecret(passphrase, mnemonic, vSeed, nullptr));
        std::vector<uint8_t> kp(SR25519_KEYPAIR_SIZE, 0);
        ///* Size of SR25519 PRIVATE (SECRET) KEY, which consists of [32 bytes key | 32 bytes nonce]
        sr25519_keypair_from_seed(kp.data(), vSeed.data());
        memcpy(privateKey, &kp[0], SR25519_KEYPAIR_SIZE);

        // memcpy(node.nonce,  &kp[0] + 32, 32);
        memcpy(publiKey, &kp[0] + 32 + 32, SR25519_PUBLIC_SIZE);
        uchar_vector vPublicKey(publiKey, sizeof(publiKey) / sizeof(uint8_t));
        uchar_vector vprivateKey(privateKey, sizeof(privateKey) / sizeof(uint8_t));

        std::string xpub = vPublicKey.getHex();
        std::string xprv = vprivateKey.getHex();

        _MasterKey_XPUB = xpub;
        _MasterKey_XPRV = xprv;

        if ("" != _MasterKey_XPUB) {
            _type = JUB_SoftwareTokenType::PUBLIC;
        }
        if ("" != _MasterKey_XPRV) {
            _type = JUB_SoftwareTokenType::PRIVATE;
        }
        break;
    }
    default:
        break;
    }

    return rv;
}

JUB_RV TrezorCryptoToken::MnemonicToMiniSecret(const std::string &passphrase, const std::string &mnemonic,
                                               uchar_vector &vSeed,

                                               void (*progress_callback)(JUB_UINT32 current, JUB_UINT32 total)) {
    JUB_BYTE seed[32] = {
        0x00,
    };

    mnemonic_to_mini_secret(mnemonic.c_str(), passphrase.c_str(), seed, progress_callback);
    if (0 == vSeed.size()) {
        vSeed.resize(32);
    }

    std::copy(std::begin(seed), std::end(seed), std::begin(vSeed));

    return JUBR_OK;
}

JUB_RV TrezorCryptoToken::MnemonicToSeed(const std::string &passphrase, const std::string &mnemonic,
                                         uchar_vector &vSeed,
                                         void (*progress_callback)(JUB_UINT32 current, JUB_UINT32 total)) {

    JUB_BYTE seed[64] = {
        0x00,
    };
    mnemonic_to_seed(mnemonic.c_str(), passphrase.c_str(), seed, progress_callback);
    if (0 == vSeed.size()) {
        vSeed.resize(64);
    }
    std::copy(std::begin(seed), std::end(seed), std::begin(vSeed));

    return JUBR_OK;
}

JUB_RV TrezorCryptoToken::SeedToMasterPrivateKey(const uchar_vector &seed, const JUB_ENUM_CURVES &curve,
                                                 std::string &xpub, std::string &xprv) {

    if (0 == seed.size()) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::string curve_name;
    JUB_VERIFY_RV(_curveToString(curve, curve_name));

    JUB_UINT32 hdVersionPrv = TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin);
    JUB_UINT32 hdVersionPub = TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin);

    HDNode node;
    if (0 == hdnode_from_seed(&seed[0], (int)seed.size(), curve_name.c_str(), &node)) {
        return JUBR_ERROR;
    }

    hdnode_fill_public_key(&node);

    JUB_CHAR str_pri[200] = {
        0,
    };
    if (0 == hdnode_serialize_private(&node, 0x00, hdVersionPrv, str_pri, sizeof(str_pri) / sizeof(JUB_CHAR))) {
        return JUBR_ERROR;
    }

    JUB_CHAR str_pub[200] = {
        0,
    };
    if (0 == hdnode_serialize_public(&node, 0x00, hdVersionPub, str_pub, sizeof(str_pub) / sizeof(JUB_CHAR))) {
        return JUBR_ERROR;
    }

    xpub = std::string(str_pub);
    xprv = std::string(str_pri);

    return JUBR_OK;
}

} // namespace token
} // namespace jub
