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
   virtual JUB_RV _HdnodeCkd(const std::string& path, HDNode* node, JUB_UINT32* parentFingerprint);

protected:
    JUB_UINT16 _deviceID;
    JUB_ENUM_CURVES _curves;

    std::string _MasterKey_XPRV{""};
    std::string _MasterKey_XPUB{""};
}; // class TrezorCryptoToken end


} // namespace token end
} // namespace jub end
