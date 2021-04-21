#pragma once
#include <string>
#include "token/interface/SoftwareTokenInterface.h"
#include <TrezorCrypto/bip32.h>
#include <string.h>
#include "HDKey/HDKey.hpp"
#include "utility/util.h"

namespace jub {
namespace token {


class TrezorCryptoToken :
   public SoftwareToken {

public:
    TrezorCryptoToken(const std::string& XPRVorXPUB) {

        //master key only support xpub and xprv encoding
        //if some coin didn't match this code , it may "override" it's constructrue.
        JUB_UINT32 xpubPrefix = TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin);
        JUB_UINT32 xprvPrefix = TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin);

        HDNode hdkey;
        uint32_t parentFingerprint;
        int rv = hdnode_deserialize(XPRVorXPUB.c_str(),
                                    xpubPrefix, xprvPrefix,
                                    SECP256K1_NAME, &hdkey, &parentFingerprint);
        // bad data
        if(0 != rv) {
            _type = JUB_SoftwareTokenType::NONE;
            return;
        }

        //xprv
        JUB_BYTE zero[33] = {0,};
        if(0 != memcmp(hdkey.private_key, zero, sizeof(hdkey.private_key)/sizeof(uint8_t))) {
            hdnode_fill_public_key(&hdkey);
            JUB_CHAR _pk[200] = {0,};
            if (0 == hdnode_serialize_public(&hdkey, parentFingerprint, xpubPrefix, _pk, sizeof(_pk) / sizeof(JUB_CHAR))) {
                _type = JUB_SoftwareTokenType::NONE;
                return;
            }

            _MasterKey_XPRV = XPRVorXPUB;
            _MasterKey_XPUB = _pk;
            _type = JUB_SoftwareTokenType::PRIVATE;
            return;
        }

        if(0 != memcmp(hdkey.public_key, zero, sizeof(hdkey.public_key)/sizeof(uint8_t))) {
            _MasterKey_XPUB = XPRVorXPUB;
            _type = JUB_SoftwareTokenType::PUBLIC;
            return;
        }

        _type = JUB_SoftwareTokenType::NONE;

        return;
    }
    ~TrezorCryptoToken() {}


    virtual JUB_RV _HdnodeCkd(std::string path, HDNode* node, JUB_UINT32* parentFingerprint) {

        JUB_BIP32_PATH spiltPath = spiltMainPath(path, '/');
        if (0 == strcmp(spiltPath.coin_type, "1'")
            && (   ('t' != _MasterKey_XPRV[0] && 0 == strcmp(spiltPath.purpose, "44'"))
                || ('u' != _MasterKey_XPRV[0] && 0 == strcmp(spiltPath.purpose, "49'")))
            ) {
            HDNode node;
            JUB_UINT32 hdVersionPrv = TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin);
            JUB_UINT32 hdVersionPub = TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin);
            std::string curve_name = SECP256K1_NAME;
            hdnode_deserialize(_MasterKey_XPRV.data(), hdVersionPub, hdVersionPrv, curve_name.c_str(), &node, 0x00);

            JUB_CHAR str_pri[200] = {0,};
            JUB_CHAR str_pub[200] = {0,};

            hdVersionPrv = TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoinTestNet);
            hdVersionPub = TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoinTestNet);
            if (0 == strcmp(spiltPath.purpose, "49'")) {
                hdVersionPrv = TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoinTestNet,true);
                hdVersionPub = TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoinTestNet, true);
            }

            if (0 == hdnode_serialize_private(&node, 0x00, hdVersionPrv, str_pri, sizeof(str_pri) / sizeof(JUB_CHAR))) {
                return JUBR_ERROR;
            }
            if (0 == hdnode_serialize_public(&node, 0x00, hdVersionPub, str_pub, sizeof(str_pub) / sizeof(JUB_CHAR))) {
                return JUBR_ERROR;
            }

            _MasterKey_XPRV = str_pri;
            _MasterKey_XPUB = str_pub;
            _coin = TWCoinType::TWCoinTypeBitcoinTestNet;
        }

        if(JUB_SoftwareTokenType::PRIVATE == _type) {
            if (   0 == strcmp(spiltPath.purpose, "49'")
                && 0 == strcmp(spiltPath.coin_type, "1'")
                ) {
                return hdnode_priv_ckd(_MasterKey_XPRV, path, _curve_name,
                                       TWCoinType2HDVersionPublic(_coin,true),
                                       TWCoinType2HDVersionPrivate(_coin,true),
                                       node, parentFingerprint);
            }
            return hdnode_priv_ckd(_MasterKey_XPRV, path, _curve_name,
                                   TWCoinType2HDVersionPublic(_coin),
                                   TWCoinType2HDVersionPrivate(_coin),
                                   node, parentFingerprint);
        }

        if(JUB_SoftwareTokenType::PUBLIC == _type) {
            if (   0 == strcmp(spiltPath.purpose, "49'")
                && 0 == strcmp(spiltPath.coin_type, "1'")
                ) {
                return hdnode_priv_ckd(_MasterKey_XPUB, path, _curve_name,
                                       TWCoinType2HDVersionPublic(_coin, true),
                                       TWCoinType2HDVersionPrivate(_coin,true),
                                       node, parentFingerprint);
            }
            return hdnode_pub_ckd(_MasterKey_XPUB, path, _curve_name,
                                  TWCoinType2HDVersionPublic(_coin),
                                  TWCoinType2HDVersionPrivate(_coin),
                                  node, parentFingerprint);
        }

        return JUBR_ERROR;
    }


protected:
    std::string _MasterKey_XPRV{""};
    std::string _MasterKey_XPUB{""};
}; // class TrezorCryptoToken end


} // namespace token end
} // namespace jub end
