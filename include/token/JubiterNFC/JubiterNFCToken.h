#pragma once
#include <token/JubiterBlade/JubiterBladeToken.h>
#include <memory>
#include <device/ApduBuilder.hpp>
#include "utility/util.h"

namespace jub {
namespace token {


constexpr JUB_BYTE kPKIAID_NFC[16] = {
    0xD1, 0x56, 0x00, 0x01, 0x32, 0x83, 0x00, 0x42, 0x4C, 0x44, 0x00, 0x00, 0x6E, 0x66, 0x63, 0x01
};


class JubiterNFCToken :
    public JubiterBladeToken {
public:
    JubiterNFCToken(JUB_UINT16 deviceID);
    virtual ~JubiterNFCToken() = default;
    //common token functions
//    virtual JUB_RV QueryBattery(JUB_BYTE &percent);
    virtual JUB_RV ShowVirtualPwd() override;
    virtual JUB_RV CancelVirtualPwd() override;
//    virtual bool   IsInitialize();
//    virtual bool   IsBootLoader();
//    virtual JUB_RV GetSN(JUB_BYTE sn[24]);
//    virtual JUB_RV GetLabel(JUB_BYTE label[32]);
//    virtual JUB_RV GetPinRetry(JUB_BYTE& retry);
//    virtual JUB_RV GetPinMaxRetry(JUB_BYTE& maxRetry);
//    virtual JUB_RV GetBleVersion(JUB_BYTE bleVersion[4]);
//    virtual JUB_RV GetFwVersion(JUB_BYTE fwVersion[4]);
//
//    virtual JUB_RV EnumApplet(std::string& appletList) ;
//    virtual JUB_RV GetAppletVersionBlade(const std::string& appID, std::string& version);
//    virtual JUB_RV EnumSupportCoins(std::string& coinList);
//    virtual JUB_RV GetDeviceCert(std::string& cert);
//    virtual JUB_RV SendOneApdu(const std::string& apdu, std::string& response);

    virtual JUB_RV VerifyPIN(const std::string &pinMix, OUT JUB_ULONG &retry) override;

    virtual JUB_RV SetTimeout(const JUB_UINT16 timeout) override;
}; // class JubiterNFCToken end


} // namespace token end
} // namespace jub end
