#pragma once
#include <string>
#include "token/interface/SoftwareTokenInterface.hpp"
#include <TrezorCrypto/bip32.h>
#include <string.h>
#include "utility/util.h"

namespace jub {
namespace token {


class TrezorCryptoToken :
   public SoftwareTokenInterface {
public:
   //for Factory
   template<typename T>
   static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID, const JUB_ENUM_CURVES curve=JUB_ENUM_CURVES::SECP256K1) {
       auto token = std::make_shared<T>(deviceID, curve);
       if (nullptr == token) {
           return nullptr;
       }

       return SoftwareTokenInterface::Create<T>(deviceID, curve);
   }


public:
    TrezorCryptoToken(JUB_UINT16 deviceID, JUB_ENUM_CURVES curve)
        : _deviceID(deviceID),
          _curve(curve) {
    }
    ~TrezorCryptoToken() {}

    virtual JUB_RV ToMasterKey(const JUB_ENUM_CURVES& curve, const std::string& privOrPub,
                               const JUB_UINT32 xpubPrefix = TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin),
                               const JUB_UINT32 xprvPrefix = TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin)) override;

    virtual JUB_RV MnemonicToMasterPrivateKey(const JUB_ENUM_CURVES& curve,
                                              const std::string& passphrase, const std::string& mnemonic) override;

    virtual JUB_RV MnemonicToSeed(const std::string& passphrase, const std::string& mnemonic,
                                  uchar_vector& vSeed,
                                  void (*progress_callback)(JUB_UINT32 current, JUB_UINT32 total)) override;
    virtual JUB_RV MnemonicToMiniSecret(const std::string& passphrase, const std::string& mnemonic,
                                        uchar_vector& vSeed,

                                        void (*progress_callback)(JUB_UINT32 current, JUB_UINT32 total));
    virtual JUB_RV SeedToMasterPrivateKey(const uchar_vector& seed,
                                          const JUB_ENUM_CURVES& curve,
                                          std::string& xpub, std::string& xprv) override;

protected:
   virtual JUB_RV _HdnodeCkd(const std::string& path, HDNode* node, JUB_UINT32* parentFingerprint, const TWCoinType& coinNet=TWCoinType::TWCoinTypeBitcoin);

protected:
    JUB_UINT16 _deviceID;
    JUB_ENUM_CURVES _curve;

    std::string _MasterKey_XPRV{""};
    std::string _MasterKey_XPUB{""};
}; // class TrezorCryptoToken end


} // namespace token end
} // namespace jub end
