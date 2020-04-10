#include <token/JubiterNFC/JubiterNFCToken.h>
#include <device/DeviceTypeBase.hpp>
#include <device/JubiterHidDevice.hpp>
#include <utility/util.h>
#include <token/ErrorHandler.h>

namespace jub {
namespace token {


stAppInfos JubiterNFCToken::g_appInfo[] = {
//    {
//        TW::Data(uchar_vector(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE))),
//        "NFC",
//        "0000000"
//    },
    {
        TW::Data(uchar_vector(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE))),
        "BTC",
        "0000000"
    },
    {
        TW::Data(uchar_vector(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE))),
        "ETH",
        "0000000"
    },
    // BTC and ETH index position fixed, start adding new apps below:
    {
        TW::Data(uchar_vector(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE))),
        "ETC",
        "0000000"
    },
    {
        TW::Data(uchar_vector(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE))),
        "BCH",
        "0000000"
    },
    {
        TW::Data(uchar_vector(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE))),
        "LTC",
        "0000000",
    },
    {
        TW::Data(uchar_vector(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE))),
        "USDT",
        "0000000"
    },
    // MISC applet, start adding new apps below:
    {
        TW::Data(uchar_vector(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE))),
        "EOS",
        "0000000"
    },
    {
        TW::Data(uchar_vector(kPKIAID_NFC, sizeof(kPKIAID_NFC)/sizeof(JUB_BYTE))),
        "XRP",
        "0000000"
    },
};

JubiterNFCToken::JubiterNFCToken(JUB_UINT16 deviceID)
    : JubiterBladeToken(deviceID) {

}


//JUB_RV JubiterNFCToken::_SendApdu(const APDU *apdu, JUB_UINT16 &wRet, JUB_BYTE *retData /*= nullptr*/,
//    JUB_ULONG *pulRetDataLen /*= nullptr*/,
//    JUB_ULONG ulMiliSecondTimeout /*= 0*/) {
//
//    auto device = jub::device::DeviceManager::GetInstance()->GetOne(_deviceID);
//    JUB_CHECK_NULL(device);
//
//    JUB_BYTE _retData[FT3KHN_READWRITE_SIZE_ONCE_NEW + 6] = { 0, };
//    JUB_ULONG ulRetDataLen = FT3KHN_READWRITE_SIZE_ONCE_NEW + 6;
//
//    std::vector<JUB_BYTE> vSendApdu;
//    if (JUBR_OK == _apduBuilder->BuildApdu(apdu, vSendApdu)) {
//        if (JUBR_OK != device->SendData(vSendApdu.data(), (JUB_ULONG)vSendApdu.size(), _retData, &ulRetDataLen, ulMiliSecondTimeout)) {
//            JUB_VERIFY_RV(JUBR_TRANSMIT_DEVICE_ERROR);
//        }
//
//        if (NULL == pulRetDataLen) {
//            wRet = _retData[ulRetDataLen - 2] * 0x100 + _retData[ulRetDataLen - 1];
//            return JUBR_OK;
//        }
//
//        if (NULL == retData) {
//            *pulRetDataLen = ulRetDataLen - 2;
//            wRet = (_retData[ulRetDataLen - 2] * 0x100 + _retData[ulRetDataLen - 1]);
//            return JUBR_OK;
//        }
//
//        if (*pulRetDataLen < (ulRetDataLen - 2)) {
//            *pulRetDataLen = ulRetDataLen - 2;
//            JUB_VERIFY_RV(JUBR_BUFFER_TOO_SMALL);
//        }
//
//        *pulRetDataLen = ulRetDataLen - 2;
//        memcpy(retData, _retData, ulRetDataLen - 2);
//
//        wRet = _retData[ulRetDataLen - 2] * 0x100 + _retData[ulRetDataLen - 1];
//        return JUBR_OK;
//    }
//
//    return JUBR_TRANSMIT_DEVICE_ERROR;
//}
//
//
//JUB_RV JubiterNFCToken::_TranPack(const abcd::DataSlice &apduData, const JUB_BYTE highMark, const JUB_BYTE sigType, const JUB_ULONG ulSendOnceLen, int finalData/* = false*/, int bOnce/* = false*/) {
//
//    if (apduData.empty()) {
//        JUB_VERIFY_RV(JUBR_ERROR);
//    }
//
//    JUB_UINT16 ret = 0;
//    if (bOnce) {
//        // one pack enough
//        JUB_BYTE p1 = 0x00;
//        p1 |= highMark;
//        APDU apdu(0x00, 0xF8, p1, sigType, (JUB_ULONG)apduData.size(), apduData.data());
//        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
//        JUB_VERIFY_COS_ERROR(ret);
//
//        return JUBR_OK;
//    }
//
//    // else send pack by pack
//    auto nextTimes = apduData.size() / ulSendOnceLen;
//    auto left = apduData.size() % ulSendOnceLen;
//
//    // split last pack
//    if (   0 == left
//        && 0 != nextTimes
//        ) {
//        nextTimes--;
//        left = ulSendOnceLen;
//    }
//
//    // pack by pack
//    JUB_BYTE p1 = 0x02;
//    p1 |= highMark;
//    APDU apdu(0x00, 0xF8, p1, sigType, 0x00);
//    apdu.lc = ulSendOnceLen;
//    JUB_UINT32 times = 0;
//    for (times = 0; times < nextTimes; times++) {
//        apdu.SetData(apduData.data() + times * ulSendOnceLen, apdu.lc);
//        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
//        JUB_VERIFY_COS_ERROR(ret);
//    }
//
//    // next pack
//    apdu.lc = (JUB_ULONG)left;
//    if (apdu.lc) {
//        if (finalData) {
//            apdu.p1 = 0x03;
//            apdu.p1 |= highMark;
//        }
//
//        apdu.SetData(apduData.data() + times * ulSendOnceLen, apdu.lc);
//        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
//        JUB_VERIFY_COS_ERROR(ret);
//    }
//
//    return JUBR_OK;
//}
//
//
//JUB_RV JubiterNFCToken::_TranPackApdu(const JUB_ULONG ncla, const JUB_ULONG nins,
//                                      const abcd::DataSlice &apduData,
//                                      const JUB_BYTE highMark,
//                                      const JUB_BYTE sigType,
//                                      const JUB_ULONG ulSendOnceLen,
//                                      JUB_BYTE *retData/* = nullptr*/, JUB_ULONG *pulRetDataLen/* = nullptr*/,
//                                      int finalData/* = false*/, int bOnce/* = false*/) {
//
//    if (apduData.empty()) {
//        JUB_VERIFY_RV(JUBR_ERROR);
//    }
//
//    JUB_UINT16 ret = 0;
//    if (bOnce) {
//        // one pack enough
//        JUB_BYTE p1 = 0x00;
//        p1 |= highMark;
//        APDU apdu(ncla, nins, p1, sigType, (JUB_ULONG)apduData.size(), apduData.data());
//        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
//        JUB_VERIFY_COS_ERROR(ret);
//
//        return JUBR_OK;
//    }
//
//    // else send pack by pack
//    auto nextTimes = apduData.size() / ulSendOnceLen;
//    auto left = apduData.size() % ulSendOnceLen;
//
//    // split last pack
//    if (   0 == left
//        && 0 != nextTimes
//        ) {
//        nextTimes--;
//        left = ulSendOnceLen;
//    }
//
//    // pack by pack
//    JUB_BYTE p1 = 0x02;
//    p1 |= highMark;
//    APDU apdu(ncla, nins, p1, sigType, 0x00);
//    apdu.lc = ulSendOnceLen;
//    JUB_UINT32 times = 0;
//    for (times = 0; times < nextTimes; times++) {
//        apdu.SetData(apduData.data() + times * ulSendOnceLen, apdu.lc);
//        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
//        JUB_VERIFY_COS_ERROR(ret);
//    }
//
//    // next pack
//    apdu.lc = (JUB_ULONG)left;
//    if (apdu.lc) {
//        if (finalData) {
//            apdu.p1 = 0x03;
//            apdu.p1 |= highMark;
//        }
//
//        apdu.SetData(apduData.data() + times * ulSendOnceLen, apdu.lc);
//        JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, pulRetDataLen));
//        JUB_VERIFY_COS_ERROR(ret);
//    }
//
//    return JUBR_OK;
//}
//
//
//JUB_RV JubiterNFCToken::_SelectApp(const JUB_BYTE PKIAID[], JUB_BYTE length) {
//
//    APDU apdu(0x00, 0xA4, 0x04, 0x00, length, PKIAID);
//    JUB_UINT16 ret = 0;
//    JUB_BYTE retData[1024] = { 0, };
//    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
//    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
//    JUB_VERIFY_COS_ERROR(ret);
//
//    uchar_vector vVersion(&retData[4], retData[3]);
//    _appletVersion = vVersion.getHex();
//
//    return JUBR_OK;
//}
//
//
JUB_RV JubiterNFCToken::QueryBattery(JUB_BYTE &percent) {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterNFCToken::ShowVirtualPwd() {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterNFCToken::CancelVirtualPwd() {

    return JUBR_IMPL_NOT_SUPPORT;
}


bool JubiterNFCToken::IsBootLoader() {

    APDU apdu(0x00, 0xa4, 0x04, 0x00, sizeof(kPKIAID_NFC_DOMAIN)/sizeof(JUB_BYTE), kPKIAID_NFC_DOMAIN);
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    auto rv = _SendApdu(&apdu, ret, retData, &ulRetDataLen);
    if (  JUBR_OK == rv
        && 0x9000 == ret
        ) {
        return true;
    }

    return false;
}


JUB_RV JubiterNFCToken::GetBleVersion(JUB_BYTE bleVersion[4]) {

    return JUBR_OK;
}


JUB_RV JubiterNFCToken::EnumApplet(std::string& appletList) {

    {
        // select main safe scope
        APDU apdu(0x00, 0xa4, 0x04, 0x00, sizeof(kPKIAID_NFC_DOMAIN)/sizeof(JUB_BYTE), kPKIAID_NFC_DOMAIN);
        JUB_UINT16 ret = 0;
        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
        JUB_VERIFY_COS_ERROR(ret);
    }
    // send apdu, then decide which coin types supports.
    APDU apdu(0x80, 0xCB, 0x80, 0x00, 0x05,
        (const JUB_BYTE *)"\xDF\xFF\x02\x81\x06");
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);

    std::vector<uint8_t> tlvData(retData, retData + ulRetDataLen);
    auto appList = ParseTlv(tlvData);

    for (auto appID : appList) {
        uchar_vector id(appID);
        appletList += id.getHex();
        appletList += " ";
    }

    return JUBR_OK;
}


JUB_RV JubiterNFCToken::GetAppletVersionBlade(const std::string& appID, std::string& version) {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterNFCToken::EnumSupportCoins(std::string& coinList) {

    std::string appletList;
    JUB_VERIFY_RV(EnumApplet(appletList));

    std::vector<std::string> coinNameList;
    auto vAppList = Split(appletList, " ");
    for (auto appID : vAppList) {
        for (auto appInfo : JubiterNFCToken::g_appInfo) {
            uchar_vector _appID(appInfo.appID);
            if (_appID.getHex() != appID) {
                continue;
            }
            if (coinNameList.end() == std::find(coinNameList.begin(), coinNameList.end(), appInfo.coinName)) {
                coinList += appInfo.coinName;
                coinList += " ";
                coinNameList.insert(coinNameList.end(), appInfo.coinName);
            }
        }
    }

    return JUBR_OK;
}


JUB_RV JubiterNFCToken::VerifyPIN(const std::string &pinMix, OUT JUB_ULONG &retry) {

    std::vector<uint8_t> pin;
    std::transform(pinMix.begin(),
                   pinMix.end(),
                   std::back_inserter(pin),
                   [](const char elem) {
        return (uint8_t)elem;
    });

    APDU apdu(0x00, 0x20, 0x03, 0x00, (JUB_ULONG)pin.size(), pin.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x6985 == ret) { //locked
        JUB_VERIFY_RV(JUBR_PIN_LOCKED);
    }
    if (0x63C0 == (ret & 0xfff0)) {
        retry = (ret & 0xf);
        JUB_VERIFY_RV(JUBR_DEVICE_PIN_ERROR);
    }
    JUB_VERIFY_COS_ERROR(ret);

    return JUBR_OK;
}


JUB_RV JubiterNFCToken::SetTimeout(const JUB_UINT16 timeout) {

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
