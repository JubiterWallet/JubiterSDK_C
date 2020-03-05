#pragma once
#include <token/interface/HardwareTokenInterface.hpp>
#include <device/ApduBuilder.hpp>
#include "utility/util.h"

namespace jub {
namespace token {


constexpr JUB_BYTE kPKIAID_MISC[16] = {
    0xD1, 0x56, 0x00, 0x01, 0x32, 0x03, 0x00, 0x42, 0x4C, 0x44, 0x00, 0x6D, 0x69, 0x73, 0x63, 0x01
};


constexpr JUB_BYTE kPKIAID_FIDO[8] = {
    0xa0, 0x00, 0x00, 0x06, 0x47, 0x2f, 0x00, 0x01
};


typedef struct _stAppInfos_ {
    std::vector<uint8_t> appID;
    std::string coinName;
    std::string minimumAppletVersion;
} stAppInfos;


class JubiterBladeToken :
    public HardwareTokenInterface {
public:
    JubiterBladeToken(JUB_UINT16 deviceID);
    virtual ~JubiterBladeToken() = default;
    //common token functions
    virtual JUB_RV QueryBattery(JUB_BYTE &percent) override;
    virtual JUB_RV ShowVirtualPwd() override;
    virtual JUB_RV CancelVirtualPwd() override;
    virtual bool   IsInitialize() override;
    virtual bool   IsBootLoader() override;
    virtual JUB_RV GetSN(JUB_BYTE sn[24]) override;
    virtual JUB_RV GetLabel(JUB_BYTE label[32]) override;
    virtual JUB_RV GetPinRetry(JUB_BYTE& retry) override;
    virtual JUB_RV GetPinMaxRetry(JUB_BYTE& maxRetry) override;
    virtual JUB_RV GetBleVersion(JUB_BYTE bleVersion[4]) override;
    virtual JUB_RV GetFwVersion(JUB_BYTE fwVersion[4]) override;

    virtual JUB_RV EnumApplet(std::string& appletList) override ;
    virtual JUB_RV GetAppletVersionBlade(const std::string& appID, std::string& version) override;
    virtual JUB_RV EnumSupportCoins(std::string& coinList) override;
    virtual JUB_RV GetDeviceCert(std::string& cert) override;
    virtual JUB_RV SendOneApdu(const std::string& apdu, std::string& response) override;

    virtual JUB_RV VerifyPIN(const std::string &pinMix, OUT JUB_ULONG &retry) override;

    virtual JUB_RV SetTimeout(const JUB_UINT16 timeout) override;

    static stAppInfos g_appInfo[];

protected:
    JUB_BYTE _HighMark(const JUB_ULONG& highMark) {
        return ((highMark & 0x0F) << 4);
    }

    JUB_RV _SendApdu(const APDU *apdu, JUB_UINT16 &wRet,
                     JUB_BYTE *retData = nullptr, JUB_ULONG *pulRetDataLen = nullptr,
                     JUB_ULONG ulMiliSecondTimeout = 1200000);

    JUB_RV _TranPack(const TW::Data &apduData,
                     const JUB_BYTE highMark,
                     const JUB_BYTE sigType,
                     const JUB_ULONG ulSendOnceLen,
                     int finalData = false,
                     int bOnce = false);

    JUB_RV _TranPackApdu(const JUB_ULONG ncla,
                         const JUB_ULONG nins,
                         const TW::Data &apduData,
                         const JUB_BYTE highMark,
                         const JUB_BYTE sigType,
                         const JUB_ULONG ulSendOnceLen,
                         JUB_BYTE *retData = nullptr, JUB_ULONG *pulRetDataLen = nullptr,
                         int finalData = false,
                         int bOnce = false);

    JUB_RV _SelectApp(const JUB_BYTE PKIAID[],
                      JUB_BYTE length);

    std::shared_ptr<ApduBuilder> _apduBuilder;
    JUB_UINT16 _deviceID;

    std::string _appletVersion;
}; // class JubiterBladeToken end


} // namespace token end
} // namespace jub end
