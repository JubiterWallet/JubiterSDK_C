#pragma once
#include <string>
#include "token/interface/SoftwareTokenInterface.hpp"
#include <TrezorCrypto/bip32.h>
#include <string.h>
#include "HDKey/HDKey.hpp"
#include "utility/util.h"

namespace jub {
namespace token {


class TrezorCryptoToken :
   public SoftwareTokenInterface {
public:
   //for Factory
   template<typename T>
   static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
       auto token = std::make_shared<T>(deviceID);
       if (nullptr == token) {
           return nullptr;
       }

       return SoftwareTokenInterface::Create<T>(deviceID, token->getCurves());
   }


public:
    TrezorCryptoToken(JUB_UINT16 deviceID, JUB_ENUM_CURVES curves)
        : _deviceID(deviceID),
          _curves(curves) {
    }
    ~TrezorCryptoToken() {}

    virtual JUB_ENUM_CURVES getCurves() {
        return _curves;
    }

    virtual JUB_RV ToMasterKey(const JUB_ENUM_CURVES& curve, const std::string& privOrPub,
                               const JUB_UINT32 xpubPrefix = TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin),
                               const JUB_UINT32 xprvPrefix = TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin)) override;

    virtual JUB_RV MnemonicToMasterPrivateKey(const JUB_ENUM_CURVES& curve,
                                              const std::string& passphrase, const std::string& mnemonic) override;

    virtual JUB_RV MnemonicToSeed(const std::string& passphrase, const std::string& mnemonic,
                                  uchar_vector& vSeed,
                                  void (*progress_callback)(JUB_UINT32 current, JUB_UINT32 total)) override;
    virtual JUB_RV SeedToMasterPrivateKey(const uchar_vector& seed,
                                          const JUB_ENUM_CURVES& curve,
                                          std::string& xpub, std::string& xprv) override;

protected:
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
    JUB_UINT16 _deviceID;
    JUB_ENUM_CURVES _curves;

    std::string _MasterKey_XPRV{""};
    std::string _MasterKey_XPUB{""};
}; // class TrezorCryptoToken end


} // namespace token end
} // namespace jub end
