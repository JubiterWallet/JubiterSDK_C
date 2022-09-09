#pragma once
#include "token/interface/BaseToken.h"
#include "utility/xManager.hpp"
#include "device/DeviceFactory.h"
#include "device/DeviceTypeBase.hpp"


namespace jub {
namespace token {


typedef enum {
    NONE    = 0x00,
    PUBLIC  = 0x01,
    PRIVATE = 0x02,
} JUB_SoftwareTokenType;


class SoftwareTokenInterface :
virtual public BaseToken {
public:
    template<typename T>
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID, const JUB_ENUM_CURVES& curve) {

        auto device = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
        if (nullptr == device
            || !dynamic_cast<jub::device::JubiterSWIDevice*>(device)
            ) {
            return nullptr;
        }

        auto ptr = std::make_shared<T>(deviceID);
        if (nullptr == ptr) {
            return ptr;
        }

        if ("" != dynamic_cast<jub::device::JubiterSWIDevice*>(device)->getMnemonic()) {
            ptr->MnemonicToMasterPrivateKey(curve,
                                            dynamic_cast<jub::device::JubiterSWIDevice*>(device)->getPassphrase(),
                                            dynamic_cast<jub::device::JubiterSWIDevice*>(device)->getMnemonic());
        }
        else if (0 < dynamic_cast<jub::device::JubiterSWIDevice*>(device)->privKeySize()) {
            ptr->ToMasterKey(curve, dynamic_cast<jub::device::JubiterSWIDevice*>(device)->getPrivKey(curve));
        }

        return ptr;
    }


public:
    virtual ~SoftwareTokenInterface() {}
    virtual JUB_SoftwareTokenType Type() { return _type; }

    virtual JUB_RV ToMasterKey(const JUB_ENUM_CURVES& curve, const std::string& privOrPub,
                               const JUB_UINT32 xpubPrefix = TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin),
                               const JUB_UINT32 xprvPrefix = TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin)) = 0;

    virtual JUB_RV MnemonicToMasterPrivateKey(const JUB_ENUM_CURVES& curve, const std::string& passphrase, const std::string& mnemonic) = 0;
    virtual JUB_RV MnemonicToSeed(const std::string& passphrase, const std::string& mnemonic,
                                  uchar_vector& vSeed,
                                  void (*progress_callback)(JUB_UINT32 current, JUB_UINT32 total)) = 0;
    virtual JUB_RV SeedToMasterPrivateKey(const uchar_vector& seed,
                                          const JUB_ENUM_CURVES& curve,
                                          std::string& xpub, std::string& xprv) = 0;

protected:
    JUB_SoftwareTokenType _type { NONE };
}; // class SoftwareTokenInterface end


} // namespace token end
} // namespace jub end
