#pragma once
#include <memory>
#include <token/interface/HardwareTokenInterface.hpp>
#include <token/JubiterBlade/JubiterBladeToken.h>
#include <device/ApduBuilder.hpp>

namespace jub {
namespace token {


//00A40400|08|A000000003000000
constexpr JUB_BYTE kPKIAID_DOMAIN[8] = {
    0xA0, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
};


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
    virtual JUB_RV SelectMainSecurityDomain() override;
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
    virtual JUB_RV ChangePIN(const std::string &pinMix, const std::string &pinNew) override;

    virtual JUB_RV SetTimeout(const JUB_UINT16 timeout) override;

    // NFC
    virtual JUB_RV Reset() override;
    virtual JUB_RV GenerateSeed(const std::string& pinMix,
                                const JUB_ENUM_CURVES& curve) override;

    virtual JUB_RV ImportMnemonic(const std::string& pinMix,
                                  const std::string& mnemonic) override;
    virtual JUB_RV SetMnemonic(const std::string& pinMix,
                               const JUB_ENUM_MNEMONIC_STRENGTH& strength,
                               const std::string& entropy,
                               const std::string& seed) override;
    virtual JUB_RV GetMnemonic(const std::string& pinMix,
                               OUT std::string& mnemonic) override;

    // BIO
    virtual JUB_RV VerifyFingerprint(OUT JUB_ULONG &retry) override;
    virtual JUB_RV EnrollFingerprint(INOUT JUB_BYTE_PTR fgptIndex, OUT JUB_ULONG_PTR ptimes,
                                     OUT JUB_BYTE_PTR fgptID) override;
    virtual JUB_RV EnumFingerprint(std::string& fgptList) override;
    virtual JUB_RV EraseFingerprint() override;
    virtual JUB_RV DeleteFingerprint(JUB_BYTE fgptID) override;

    static stAppInfos g_appInfo[];

protected:
    JUB_BYTE _HighMark(const JUB_ULONG& highMark) {
        return ((highMark & 0x0F) << 4);
    }

    virtual JUB_RV _SendApdu(const APDU *apdu, JUB_UINT16 &wRet,
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
