#ifndef __TokenInterface__
#define __TokenInterface__
#include "JUB_SDK.h"

#include <string>
#include <vector>

#include "mSIGNA/stdutils/uchar_vector.h"
#include "utility/Singleton.h"
#include "utility/xManager.hpp"

#include <token/interface/BaseToken.h>

namespace jub {
namespace token {


class HardwareTokenInterface :
virtual public BaseToken {
public:
    /* functions */
    virtual JUB_RV ShowVirtualPwd() = 0;
    virtual JUB_RV CancelVirtualPwd() = 0;
    virtual bool   IsInitialize() = 0;
    virtual bool   IsBootLoader() = 0;
    virtual JUB_RV GetSN(JUB_BYTE sn[24]) = 0;
    virtual JUB_RV GetLabel(JUB_BYTE label[32]) = 0;
    virtual JUB_RV GetPinRetry(JUB_BYTE& retry) = 0;
    virtual JUB_RV GetPinMaxRetry(JUB_BYTE& maxRetry) = 0;
    virtual JUB_RV GetBleVersion(JUB_BYTE bleVersion[4]) = 0;
    virtual JUB_RV GetFwVersion(JUB_BYTE fwVersion[4]) = 0;
    virtual JUB_RV EnumApplet(std::string& appletList) = 0;
    virtual JUB_RV GetAppletVersionBlade(const std::string& appID, std::string& version) = 0;
    virtual JUB_RV EnumSupportCoins(std::string& coinList) = 0;
    virtual JUB_RV GetDeviceCert(std::string& cert) = 0;
    virtual JUB_RV SendOneApdu(const std::string& apdu, std::string& response) = 0;

    virtual JUB_RV QueryBattery(JUB_BYTE &percent) = 0;

    virtual JUB_RV VerifyPIN(const std::string &pinMix, OUT JUB_ULONG &retry) = 0;
    virtual JUB_RV ChangePIN(const std::string &pinMix, const std::string &pinNew) = 0;
    virtual JUB_RV SetTimeout(const JUB_UINT16 timeout) = 0;

    virtual JUB_RV GenerateSeed(const JUB_ENUM_CURVES& curve) = 0;
    virtual JUB_RV SetSeed(const std::string& pinMix,
                           const JUB_ENUM_MNEMONIC_STRENGTH& strength,
                           const std::string& entropy,
                           const std::string& seed) = 0;
    virtual JUB_RV GetMnemonic(const std::string& pinMix,
                               OUT std::string& mnemonic) = 0;

}; // class HardwareTokenInterface end


} // namespace token end
} // namespace jub end
#endif  // __TokenInterface__
