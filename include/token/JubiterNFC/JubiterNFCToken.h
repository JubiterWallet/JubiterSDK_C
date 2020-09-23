#pragma once
#include <memory>

#include <token/JubiterBlade/JubiterBladeToken.h>
#include <device/ApduBuilder.hpp>
#include "utility/util.h"


namespace jub {
namespace token {


//00A40400|08|A000000151000000
constexpr JUB_BYTE kPKIAID_NFC_DOMAIN[8] = {
    0xA0, 0x00, 0x00, 0x01, 0x51, 0x00, 0x00, 0x00
};

//D1560001328300424C4400006E666301
constexpr JUB_BYTE kPKIAID_NFC[16] = {
    0xD1, 0x56, 0x00, 0x01, 0x32, 0x83, 0x00, 0x42, 0x4C, 0x44, 0x00, 0x00, 0x6E, 0x66, 0x63, 0x01
};
//bixin: D156000132834001
//constexpr JUB_BYTE kPKIAID_NFC[8] = {
//    0xD1, 0x56, 0x00, 0x01, 0x32, 0x83, 0x40, 0x01
//};


class JubiterNFCToken :
    public JubiterBladeToken {
public:
    JubiterNFCToken(JUB_UINT16 deviceID);
    virtual ~JubiterNFCToken() = default;
    //common token functions
    virtual JUB_RV QueryBattery(JUB_BYTE &percent) override;
    virtual JUB_RV ShowVirtualPwd() override;
    virtual JUB_RV CancelVirtualPwd() override;
//    virtual bool   IsInitialize();
    virtual bool   IsBootLoader() override;
    virtual JUB_RV SelectMainSecurityDomain() override;
    virtual JUB_RV GetPinRetry(JUB_BYTE& retry) override;
    virtual JUB_RV GetBleVersion(JUB_BYTE bleVersion[4]) override;

    virtual JUB_RV EnumApplet(std::string& appletList) override;
    virtual JUB_RV GetAppletVersionBlade(const std::string& appID, std::string& version) override;
    virtual JUB_RV EnumSupportCoins(std::string& coinList) override;
//    virtual JUB_RV GetDeviceCert(std::string& cert) override;
//    virtual JUB_RV SendOneApdu(const std::string& apdu, std::string& response);

    virtual JUB_RV VerifyPIN(const std::string &pinMix, OUT JUB_ULONG &retry) override;
    virtual JUB_RV ChangePIN(const std::string &pinMix, const std::string &pinNew) override;

    virtual JUB_RV SetTimeout(const JUB_UINT16 timeout) override;

    virtual JUB_RV Reset() override;
    virtual JUB_RV GenerateSeed(const std::string& pinMix,
                                const JUB_ENUM_CURVES& curve) override;

    virtual JUB_RV SetMnemonic(const std::string& pinMix,
                               const JUB_ENUM_MNEMONIC_STRENGTH& strength,
                               const std::string& entropy,
                               const std::string& seed) override;
    virtual JUB_RV GetMnemonic(const std::string& pinMix,
                               OUT std::string& mnemonic) override;
    virtual bool   HasRootKey() override;

    static stAppInfos g_appInfo[];

protected:
    virtual JUB_RV _SendApdu(const APDU *apdu, JUB_UINT16 &wRet,
                             JUB_BYTE *retData = nullptr, JUB_ULONG *pulRetDataLen = nullptr,
                             JUB_ULONG ulMiliSecondTimeout = 1200000) override;
    JUB_RV _SendSafeApdu(const APDU *apdu, JUB_UINT16 &wRet,
                         JUB_BYTE *retData = nullptr, JUB_ULONG *pulRetDataLen = nullptr,
                         JUB_ULONG ulMiliSecondTimeout = 1200000);

    virtual JUB_RV PerformSecurityOperation(const uchar_vector& oceCert);
    virtual JUB_RV MutualAuthenticate(const uchar_vector& apduData,
                                      uchar_vector& receipt);
}; // class JubiterNFCToken end


} // namespace token end
} // namespace jub end
