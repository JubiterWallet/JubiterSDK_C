#pragma once
#include <string>
#include <token/interface/SoftwareTokenInterface.h>
#include <TrezorCrypto/bip32.h>
#include <string.h>
#include <HDKey/HDKey.hpp>


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
        if(JUB_SoftwareTokenType::PRIVATE == _type) {
            return hdnode_priv_ckd(_MasterKey_XPRV, path, _curve_name, TWCoinType2HDVersionPublic(_coin),  TWCoinType2HDVersionPrivate(_coin), node, parentFingerprint);
        }

        if(JUB_SoftwareTokenType::PUBLIC == _type) {
            return hdnode_pub_ckd(_MasterKey_XPUB, path, _curve_name, TWCoinType2HDVersionPublic(_coin),  TWCoinType2HDVersionPrivate(_coin), node, parentFingerprint);
        }

        return JUBR_ERROR;
    }


protected:
    std::string _MasterKey_XPRV{""};
    std::string _MasterKey_XPUB{""};
}; // class TrezorCryptoToken end


} // namespace token end
} // namespace jub end
