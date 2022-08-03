#include "token/JubiterBIO/JubiterBIOToken.h"
#include "device/DeviceTypeBase.hpp"
#include "device/JubiterHidDevice.hpp"
#include "utility/util.h"

#include "token/BTC/JubiterBladeBTCImpl.h"
#include "token/ETH/JubiterBladeETHImpl.h"
#include "token/EOS/JubiterBladeEOSImpl.h"
#include "token/ErrorHandler.h"

#include "tlv.hpp"


namespace jub {
namespace token {


stAppInfos JubiterBIOToken::g_appInfo[] = {
    {
        TW::Data(uchar_vector(kPKIAID_BTC, sizeof(kPKIAID_BTC)/sizeof(JUB_BYTE))),
        "BTC",
        "05000000"
    },
    {
        TW::Data(uchar_vector(kPKIAID_ETH, sizeof(kPKIAID_ETH)/sizeof(JUB_BYTE))),
        "ETH",
        "05000000"
    },
    // ETH index position fixed, start adding new apps below:
    {
        TW::Data(uchar_vector(kPKIAID_ETH, sizeof(kPKIAID_ETH)/sizeof(JUB_BYTE))),
        "ETC",
        "05000000"
    },
    {
        TW::Data(uchar_vector(kPKIAID_ETH, sizeof(kPKIAID_ETH)/sizeof(JUB_BYTE))),
        "FIL",
        "05070001"
    },
    // BTC index position fixed, start adding new apps below:
    {
        TW::Data(uchar_vector(kPKIAID_BTC, sizeof(kPKIAID_BTC)/sizeof(JUB_BYTE))),
        "BCH",
        "05000000"
    },
    {
        TW::Data(uchar_vector(kPKIAID_BTC, sizeof(kPKIAID_BTC)/sizeof(JUB_BYTE))),
        "LTC",
        "05000000",
    },
    {
        TW::Data(uchar_vector(kPKIAID_BTC, sizeof(kPKIAID_BTC)/sizeof(JUB_BYTE))),
        "DASH",
        "05000000"
    },
    {
        TW::Data(uchar_vector(kPKIAID_BTC, sizeof(kPKIAID_BTC)/sizeof(JUB_BYTE))),
        "USDT",
        "05000000"
    },
    // MISC applet, start adding new apps below:
    {
        TW::Data(uchar_vector(kPKIAID_MISC, sizeof(kPKIAID_MISC)/sizeof(JUB_BYTE))),
        "EOS",
        "05010000"
    },
    {
        TW::Data(uchar_vector(kPKIAID_MISC, sizeof(kPKIAID_MISC)/sizeof(JUB_BYTE))),
        "XRP",
        "05010000"
    },
    {
        TW::Data(uchar_vector(kPKIAID_MISC, sizeof(kPKIAID_MISC)/sizeof(JUB_BYTE))),
        "TRX",
        "05010000"
    },
};


JubiterBIOToken::JubiterBIOToken(JUB_UINT16 deviceID)
    : JubiterBladeToken(deviceID) {

}


JUB_RV JubiterBIOToken::_SelectApp(const JUB_BYTE PKIAID[], const JUB_BYTE length) {

    uchar_vector vVersion;
    JUB_VERIFY_RV(JubiterBladeToken::_SelectApp(PKIAID, length, vVersion));
    JubiterBIOToken::_appletVersion = stVersionExp::FromHex(vVersion.getHex());

    return JUBR_OK;
}


bool JubiterBIOToken::IsBootLoader() {

    APDU apdu(0x00, 0xa4, 0x04, 0x00, 0x00);
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG ulRetDataLen = sizeof(retData)/sizeof(JUB_BYTE);
    auto rv = _SendApdu(&apdu, ret, retData, &ulRetDataLen);
    if (  JUBR_OK == rv
        && 0x9000 == ret
        ) {
        return true;
    }

    return false;
}


JUB_RV JubiterBIOToken::EnumSupportCoins(std::string& coinList) {

    JUB_RV rv = JUBR_ERROR;

    JUB_BYTE fwVersion[4] = {0x00,};
    JUB_VERIFY_RV(GetFwVersion(fwVersion));
    std::string strFWVersion = convertToString((char*)fwVersion, sizeof(fwVersion)/sizeof(JUB_BYTE));

    std::string appletList;
    JUB_VERIFY_RV(EnumApplet(appletList));

    std::vector<std::string> coinNameList;
    auto vAppList = Split(appletList, " ");
    for (auto appID : vAppList) {
        stVersionExp version;
        rv = GetAppletVersion(appID, version);
        if (JUBR_OK != rv) {
            continue;
        }
        for (auto appInfo : JubiterBIOToken::g_appInfo) {
            uchar_vector _appID(appInfo.appID);
            if (_appID.getHex() != appID) {
                continue;
            }
            if (stVersionExp::FromHex(appInfo.minimumAppletVersion) > version) {
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


JUB_RV JubiterBIOToken::UIShowMain() {

    uchar_vector apduData = tlv_buf(0xDFFF, tlv_buf(0x8144).encodeT()).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x6985 == ret) {
        JUB_VERIFY_RV(JUBR_CONDITIONS_NOT_SATISFIED);
    }
    else {
        JUB_VERIFY_COS_ERROR(ret);
    }

    return JUBR_OK;
}


JUB_RV JubiterBIOToken::IdentityVerify(IN JUB_BYTE mode, OUT JUB_ULONG &retry) {

    if (   JUB_ENUM_IDENTITY_VERIFY_MODE::VIA_DEVICE != mode
        && JUB_ENUM_IDENTITY_VERIFY_MODE::VIA_FPGT   != mode
        ) {
        return JUBR_ARGUMENTS_BAD;
    }

    uchar_vector apduData = tlv_buf(0xDFFF, tlv_buf(0x8220, {mode}).encode()).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x6985 == ret) { //locked
        JUB_VERIFY_RV(JUBR_PIN_LOCKED);
    }
    else if (0x63C0 == (ret & 0xfff0)) {
        retry = (ret & 0xf);
        JUB_VERIFY_RV(JUBR_DEVICE_PIN_ERROR);
    }
    else {
        JUB_VERIFY_COS_ERROR(ret);
    }

    return JUBR_OK;
}


JUB_RV JubiterBIOToken::IdentityVerifyPIN(IN JUB_BYTE mode, IN const std::string &pinMix, OUT JUB_ULONG &retry) {

    // send apdu.
    std::vector<uint8_t> pin;
    switch (mode) {
    case JUB_ENUM_IDENTITY_VERIFY_MODE::VIA_9GRIDS:
    {
        std::transform(pinMix.begin(),
                       pinMix.end(),
                       std::back_inserter(pin),
                       [](const char elem) {
            return (uint8_t)(elem - 0x30);
        });
        break;
    }
    case JUB_ENUM_IDENTITY_VERIFY_MODE::VIA_APDU:
    {
        std::transform(pinMix.begin(),
                       pinMix.end(),
                       std::back_inserter(pin),
                       [](const char elem) {
            return (uint8_t)elem;
        });
        break;
    }
    default:
        return JUBR_ARGUMENTS_BAD;
    }

    uchar_vector apduData = tlv_buf(0xDFFF, tlv_buf(0x8220, tlv_buf(mode, pin).encode()).encode()).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x6985 == ret) { //locked
        JUB_VERIFY_RV(JUBR_PIN_LOCKED);
    }
    else if (0x63C0 == (ret & 0xfff0)) {
        retry = (ret & 0xf);
        JUB_VERIFY_RV(JUBR_DEVICE_PIN_ERROR);
    }
    else {
        JUB_VERIFY_COS_ERROR(ret);
    }

    return JUBR_OK;
}


JUB_RV JubiterBIOToken::IdentityNineGrids(IN bool bShow) {

    JUB_BYTE value = 0x00;
    if (!bShow) {
        value = 0x80;   // clean cache
    }

    uchar_vector apduData = tlv_buf(0xDFFF, tlv_buf(0x8220, tlv_buf(JUB_ENUM_IDENTITY_VERIFY_MODE::VIA_9GRIDS, {value}).encode()).encode()).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x6985 == ret) { //locked
        JUB_VERIFY_RV(JUBR_PIN_LOCKED);
    }

    JUB_VERIFY_COS_ERROR(ret);

    return JUBR_OK;
}


// Test command, not public
JUB_RV JubiterBIOToken::VerifyFgptForIntl(OUT JUB_ULONG &retry) {

    uchar_vector apduData = tlv_buf(0xDFFF, {0x82, 0x11, 0x5A, 0xA5}).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x6985 == ret) { //locked
        JUB_VERIFY_RV(JUBR_PIN_LOCKED);
    }

    if (2 == ulRetDataLen) {
        JUB_UINT16 uRetry = retData[0] * 0x100 + retData[1];
        if (0x8000 == (uRetry & 0xfff0)) {
            retry = (uRetry & 0xf);
            JUB_VERIFY_RV(JUBR_DEVICE_PIN_ERROR);
        }
    }
    JUB_VERIFY_COS_ERROR(ret);

    return JUBR_OK;
}


JUB_RV JubiterBIOToken::VerifyFingerprint(OUT JUB_ULONG &retry) {

    APDU apdu(0x00, 0x20, 0x04, 0x00, 0x00);
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


JUB_RV JubiterBIOToken::EnrollFingerprint(IN JUB_UINT16 fpTimeout,
                                          INOUT JUB_BYTE_PTR fgptIndex, OUT JUB_ULONG_PTR ptimes,
                                          OUT JUB_BYTE_PTR fgptID) {

    uchar_vector tagData;
    tagData.push_back(*fgptIndex);
    if (DEFAULT_FP_TIMEOUT != fpTimeout) {
        tagData.push_back(fpTimeout);
    }

    uchar_vector apduData = tlv_buf(0xDFFE, tlv_buf(0x8210, tagData).encode()).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG ulRetDataLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 != ret) {
        JUB_RV rv = JUBR_ERROR;
        switch (ret | 0x40000000) {
        case JUBR_BIO_FINGERPRINT_MODALITY_ERROR:   // The ID of fingerprint modality error
            rv = JUBR_BIO_FINGERPRINT_MODALITY_ERROR;
            break;
        case JUBR_BIO_SPACE_LIMITATION:             // Space limitation
            rv = JUBR_BIO_SPACE_LIMITATION;
            break;
        case 0x9BE1:            // Function error.
            rv = JUBR_OTHER_ERROR;
            break;
        case JUBR_BIO_TIMEOUT:  // Time out.
            rv = JUBR_BIO_TIMEOUT;
            break;
        case 0x9BEF:            // PIN verified error
            rv = JUBR_DEVICE_PIN_ERROR;
            break;
        default:
            break;
        }
        return rv;
    }

    TW::Data v;
    size_t vSize = tlv_buf::decodeTV(0x8210, TW::Data(retData, retData + ulRetDataLen), v);
    if (!(v.size() == vSize
          &&     3 == vSize)
        ) {
        return JUBR_ERROR;
    }

    *fgptID    = v[0];  // Modality ID.
    *fgptIndex = v[1];  // The number of times of current fingerprint modality, start from 1.
    *ptimes    = v[2];  // The total times that current fingerprint needs.

    return JUBR_OK;
}


JUB_RV JubiterBIOToken::EnumFingerprint(std::string& fgptList) {

    uchar_vector apduData = tlv_buf(0xDFFF, tlv_buf(0x8211).encodeT()).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG ulRetDataLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 != ret) {
        JUB_RV rv = JUBR_ERROR;
        switch (ret | 0x40000000) {
        case JUBR_BIO_FINGERPRINT_MODALITY_ERROR:   // The ID of fingerprint modality error
            rv = JUBR_BIO_FINGERPRINT_MODALITY_ERROR;
            break;
        case JUBR_BIO_SPACE_LIMITATION:             // Space limitation
            rv = JUBR_BIO_SPACE_LIMITATION;
            break;
        case 0x9BE1:            // Function error.
            rv = JUBR_OTHER_ERROR;
            break;
        case JUBR_BIO_TIMEOUT:  // Time out.
            rv = JUBR_BIO_TIMEOUT;
            break;
        case 0x9BEF:            // PIN verified error
            rv = JUBR_DEVICE_PIN_ERROR;
            break;
        default:
            break;
        }
        return rv;
    }

    // (  Enroll FP  )     (   Enum FP     )           (binary)    (ID list)
    //     fgptID
    //      |  |           00000000 00000000     0..56 00000000     null
    //-------------------------------------------------------------------------------------
    // 8210 |00| xx 04     00000000 00000001     0..56 00000001     0
    // 8210 |01| xx 04     00000000 00000003     0..56 00000011     0, 1
    // 8210 |02| xx 04     00000000 00000007     0..56 00000111     0, 1, 2
    // 8210 |03| xx 04     00000000 0000000f     0..56 00001111     0, 1, 2, 3
    //-------------------------------------------------------------------------------------
    // 8210 |04| xx 04     00000000 0000001f     0..56 00011111     0, 1, 2, 3, 4
    // 8210 |05| xx 04     00000000 0000003f     0..56 00111111     0, 1, 2, 3, 4, 5
    // 8210 |06| xx 04     00000000 0000007f     0..56 01111111     0, 1, 2, 3, 4, 5, 6
    // 8210 |07| xx 04     00000000 000000ff     0..56 11111111     0, 1, 2, 3, 4, 5, 6, 7
    //-------------------------------------------------------------------------------------
    // delete   02
    //  07 dffe 04 8213 01 |02|
    //          02         00000000 000000fd     0..56 11111011     0, 1, _, 3, 4, 5, 6, 7
    uchar_vector vBitmap(retData, retData + ulRetDataLen);
    vBitmap.reverse();

    std::vector<uint16_t> vFgpt;
    for (uint16_t i=0; i<vBitmap.size(); ++i) {
        for (uint16_t shift=0; shift<2; ++shift) {
            uint8_t t = vBitmap[i];
            t >>= (shift*4);
            t  &= 0x0F;
            std::vector<uint8_t> v;
            decToBinary(t, v);
            for (uint16_t bit=0; bit<v.size(); ++bit) {
                if (v[bit]) {
                    vFgpt.push_back((shift*4 + bit)+(i*8));
                }
            }
        }
    }

    for (auto fgpt : vFgpt) {
        std::ostringstream o;
        o << fgpt;
        fgptList += o.str();
        fgptList += " ";
    }

    return JUBR_OK;
}


JUB_RV JubiterBIOToken::EraseFingerprint(IN JUB_UINT16 fpTimeout) {

    uchar_vector apduData = tlv_buf(0xDFFE, tlv_buf(0x8212).encodeT()).encode();
    if (DEFAULT_FP_TIMEOUT != fpTimeout) {
        apduData.clear();
        apduData = tlv_buf(0xDFFE, tlv_buf(0x8212, fpTimeout).encode()).encode();
    }
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG ulRetDataLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 == ret) {
        return JUBR_OK;
    }
    else if (   0x6982 == ret
             || 0x6985 == ret
             ) {
        return JUBR_CONDITIONS_NOT_SATISFIED;
    }
    else if (0x9bef == ret) {
        return JUBR_DEVICE_PIN_ERROR;
    }

    JUB_VERIFY_RV(JUBR_ERROR);

    return JUBR_OK;
}


JUB_RV JubiterBIOToken::DeleteFingerprint(IN JUB_UINT16 fpTimeout,
                                          JUB_BYTE fgptID) {

    uchar_vector tagData;
    tagData.push_back(fgptID);
    if (DEFAULT_FP_TIMEOUT != fpTimeout) {
        tagData.push_back(fpTimeout);
    }

    uchar_vector apduData = tlv_buf(0xDFFE, tlv_buf(0x8213, tagData).encode()).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG ulRetDataLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 == ret) {
        return JUBR_OK;
    }
    else if (   0x6982 == ret
             || 0x6985 == ret
             ) {
        return JUBR_CONDITIONS_NOT_SATISFIED;
    }
    else if (0x9bef == ret) {
        return JUBR_DEVICE_PIN_ERROR;
    }

    JUB_VERIFY_RV(JUBR_ERROR);

    return JUBR_OK;
}


} // namespace token end
} // namespace jub end
