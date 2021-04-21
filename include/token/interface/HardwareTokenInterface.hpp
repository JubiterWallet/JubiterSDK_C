#ifndef __TokenInterface__
#define __TokenInterface__

#include <string>
#include <vector>

#include "mSIGNA/stdutils/uchar_vector.h"
#include "utility/Singleton.h"
#include "utility/xManager.hpp"

#include "token/interface/BaseToken.h"


namespace jub {
namespace token {


class HardwareTokenInterface :
virtual public BaseToken {
public:
    //for Factory
    template<typename T>
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<T>(deviceID);
    }

    /* functions */
    virtual JUB_RV ShowVirtualPwd() = 0;
    virtual JUB_RV CancelVirtualPwd() = 0;
    virtual JUB_RV GetRootKeyStatus(JUB_ENUM_DEVICE_ROOT_KEY_STATUS_PTR status) = 0;
    virtual bool   IsBootLoader() = 0;
    virtual JUB_RV SelectMainSecurityDomain() = 0;
    virtual JUB_RV GetSN(JUB_BYTE sn[24]) = 0;
    virtual JUB_RV GetLabel(JUB_BYTE label[32]) = 0;
    virtual JUB_RV GetPinRetry(JUB_BYTE& retry) = 0;
    virtual JUB_RV GetPinMaxRetry(JUB_BYTE& maxRetry) = 0;
    virtual JUB_RV GetBleVersion(JUB_BYTE bleVersion[4]) = 0;
    virtual JUB_RV GetFwVersion(JUB_BYTE fwVersion[4]) = 0;
    virtual JUB_RV EnumApplet(std::string& appletList) = 0;
    virtual JUB_RV GetAppletVersion(const std::string& appID, stVersion& version) = 0;
    virtual JUB_RV EnumSupportCoins(std::string& coinList) = 0;
    virtual JUB_RV GetDeviceCert(std::string& cert) = 0;
    virtual JUB_RV SendOneApdu(const std::string& apdu, std::string& response) = 0;

    virtual JUB_RV QueryBattery(JUB_BYTE &percent) = 0;

    virtual JUB_RV VerifyPIN(const std::string &pinMix, OUT JUB_ULONG &retry) = 0;
    virtual JUB_RV ChangePIN(const std::string &pinMix, const std::string &pinNew) = 0;
    virtual JUB_RV SetTimeout(const JUB_UINT16 timeout) = 0;

    virtual JUB_RV SetERC20Token(JUB_CHAR_CPTR tokenName,
                                 JUB_UINT16 unitDP,
                                 JUB_CHAR_CPTR contractAddress) = 0;

    // NFC
    virtual JUB_RV SetLabel(const std::string& label) = 0;
    virtual JUB_RV Reset() = 0;
    virtual JUB_RV GenerateSeed(const std::string& pinMix,
                                const JUB_ENUM_CURVES& curve) = 0;

    virtual JUB_RV ImportMnemonic(const std::string& pinMix,
                                  const std::string& mnemonic) = 0;
    virtual JUB_RV ExportMnemonic(const std::string& pinMix,
                                  OUT std::string& mnemonic) = 0;

    // BIO
    virtual JUB_RV UIShowMain() = 0;

    virtual JUB_RV IdentityVerify(IN JUB_BYTE mode, OUT JUB_ULONG &retry) = 0;
    virtual JUB_RV IdentityVerifyPIN(IN JUB_BYTE mode, IN const std::string &pinMix, OUT JUB_ULONG &retry) = 0;
    virtual JUB_RV IdentityNineGrids(IN bool bShow) = 0;

    virtual JUB_RV VerifyFgptForIntl(OUT JUB_ULONG &retry) = 0;  // Test command, not public
    virtual JUB_RV VerifyFingerprint(OUT JUB_ULONG &retry) = 0;
    virtual JUB_RV EnrollFingerprint(IN JUB_UINT16 fpTimeout,
                                     INOUT JUB_BYTE_PTR fgptIndex, OUT JUB_ULONG_PTR ptimes,
                                     OUT JUB_BYTE_PTR fgptID) = 0;
    virtual JUB_RV EnumFingerprint(std::string& fgptList) = 0;
    virtual JUB_RV EraseFingerprint(IN JUB_UINT16 fpTimeout) = 0;
    virtual JUB_RV DeleteFingerprint(IN JUB_UINT16 fpTimeout,
                                     JUB_BYTE fgptID) = 0;
}; // class HardwareTokenInterface end


} // namespace token end
} // namespace jub end
#endif  // __TokenInterface__
