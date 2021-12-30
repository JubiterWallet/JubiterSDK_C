#include "token/JubiterBlade/JubiterBladeToken.h"

#include "token/BTC/JubiterBladeBTCImpl.h"
#include "token/ETH/JubiterBladeETHImpl.h"
#include "token/EOS/JubiterBladeEOSImpl.h"
#include "token/ErrorHandler.h"

#include "device/DeviceTypeBase.hpp"
#include "device/JubiterHidDevice.hpp"

#include "utility/Debug.hpp"
#include "utility/util.h"
#include "tlv.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cstring>

//clang-format off
namespace jub {
namespace token {

stAppInfos JubiterBladeToken::g_appInfo[] = {
    {
        TW::Data(uchar_vector(kPKIAID_BTC, sizeof(kPKIAID_BTC)/sizeof(JUB_BYTE))),
        "BTC",
        "00000000"
    },
    {
        TW::Data(uchar_vector(kPKIAID_ETH, sizeof(kPKIAID_ETH)/sizeof(JUB_BYTE))),
        "ETH",
        "00000000"
    },
    // ETH index position fixed, start adding new apps below:
    {
        TW::Data(uchar_vector(kPKIAID_ETH, sizeof(kPKIAID_ETH)/sizeof(JUB_BYTE))),
        "ETC",
        "01010000"
    },
    {
        TW::Data(uchar_vector(kPKIAID_ETH, sizeof(kPKIAID_ETH)/sizeof(JUB_BYTE))),
        "FIL",
        "01010000"
    },
    // BTC index position fixed, start adding new apps below:
    {
        TW::Data(uchar_vector(kPKIAID_BTC, sizeof(kPKIAID_BTC)/sizeof(JUB_BYTE))),
        "BCH",
        "01070003"
    },
    {
        TW::Data(uchar_vector(kPKIAID_BTC, sizeof(kPKIAID_BTC)/sizeof(JUB_BYTE))),
        "LTC",
        "01070003",
    },
    {
        TW::Data(uchar_vector(kPKIAID_BTC, sizeof(kPKIAID_BTC)/sizeof(JUB_BYTE))),
        "DASH",
        "01080002"
    },
    {
        TW::Data(uchar_vector(kPKIAID_BTC, sizeof(kPKIAID_BTC)/sizeof(JUB_BYTE))),
        "USDT",
        "01080002"
    },
    {
        TW::Data(uchar_vector(kPKIAID_BTC, sizeof(kPKIAID_BTC)/sizeof(JUB_BYTE))),
        "CKB",
        "01090300"
    },
    // EOS-independent applet JUBR_PKIAID_INVALID
    {
        TW::Data(uchar_vector(kPKIAID_EOS, sizeof(kPKIAID_EOS)/sizeof(JUB_BYTE))),
        "EOS",
        "01000009"
    },
    // MISC applet, start adding new apps below:
    {
        TW::Data(uchar_vector(kPKIAID_MISC, sizeof(kPKIAID_MISC)/sizeof(JUB_BYTE))),
        "EOS",
        "01000001"
    },
    {
        TW::Data(uchar_vector(kPKIAID_MISC, sizeof(kPKIAID_MISC)/sizeof(JUB_BYTE))),
        "XRP",
        "01000001"
    },
    {
        TW::Data(uchar_vector(kPKIAID_MISC, sizeof(kPKIAID_MISC)/sizeof(JUB_BYTE))),
        "TRX",
        "01000002"
    },
};


JubiterBladeToken::JubiterBladeToken(JUB_UINT16 deviceID)
    :_apduBuilder(std::make_shared<JubApduBuiler>()),
    _deviceID(deviceID) {

}


JUB_RV JubiterBladeToken::GetHDNode(const JUB_ULONG p1, const JUB_ULONG p2, const std::string& path, TW::Data& pubkey) {

    uchar_vector vPath;
    vPath << path;
    uchar_vector apduData = ToTlv(JUB_ENUM_APDU_DATA::TAG_PATH_08, vPath);

    APDU apdu(0x00, 0xE6, p1, p2, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[2048] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);

    pubkey.resize(ulRetDataLen);
    std::copy(retData, retData+ulRetDataLen, std::begin(pubkey));

    return JUBR_OK;
}


JUB_RV JubiterBladeToken::GetAddress(const JUB_ULONG p1, const JUB_ULONG p2, const TW::Data& apduData, TW::Data& address) {

    uchar_vector data(apduData.begin(), apduData.end());

    APDU apdu(0x00, 0xF6, p1, p2, (JUB_ULONG)data.size(), data.data(), data.size());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[2048] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);

    address.resize(ulRetDataLen);
    std::copy(retData, retData+ulRetDataLen, std::begin(address));

    return JUBR_OK;
}


JUB_RV JubiterBladeToken::_SendApdu(const APDU *apdu, JUB_UINT16 &wRet, JUB_BYTE *retData /*= nullptr*/,
    JUB_ULONG *pulRetDataLen /*= nullptr*/,
    JUB_ULONG ulMiliSecondTimeout /*= 0*/) {

    auto device = jub::device::DeviceManager::GetInstance()->GetOne(_deviceID);
    JUB_CHECK_NULL(device);

    JUB_BYTE _retData[FT3KHN_READWRITE_SIZE_ONCE_NEW + 6] = { 0, };
    JUB_ULONG ulRetDataLen = FT3KHN_READWRITE_SIZE_ONCE_NEW + 6;

    std::vector<JUB_BYTE> vSendApdu;
    if (JUBR_OK == _apduBuilder->BuildApdu(apdu, vSendApdu)) {
        DEBUG_LOG("Send APDU[%d]: %s\n", vSendApdu.size(), uchar_vector(vSendApdu).getHex().c_str());

        if (JUBR_OK != device->SendData(vSendApdu.data(), (JUB_ULONG)vSendApdu.size(), _retData, &ulRetDataLen, ulMiliSecondTimeout)) {
            JUB_VERIFY_RV(JUBR_TRANSMIT_DEVICE_ERROR);
        }

        if (NULL == pulRetDataLen) {
            wRet = _retData[ulRetDataLen - 2] * 0x100 + _retData[ulRetDataLen - 1];
            return JUBR_OK;
        }

        if (NULL == retData) {
            *pulRetDataLen = ulRetDataLen - 2;
            wRet = (_retData[ulRetDataLen - 2] * 0x100 + _retData[ulRetDataLen - 1]);
            return JUBR_OK;
        }

        if (*pulRetDataLen < (ulRetDataLen - 2)) {
            *pulRetDataLen = ulRetDataLen - 2;
            JUB_VERIFY_RV(JUBR_BUFFER_TOO_SMALL);
        }

        *pulRetDataLen = ulRetDataLen - 2;
        memcpy(retData, _retData, ulRetDataLen - 2);

        wRet = _retData[ulRetDataLen - 2] * 0x100 + _retData[ulRetDataLen - 1];

        DEBUG_LOG("APDU Resp[%d]: %s\n", ulRetDataLen, uchar_vector(_retData, (unsigned int)ulRetDataLen).getHex().c_str());

        if (0x6aa4 == wRet) {
            return JUBR_TRANSACT_TIMEOUT;
        }

        return JUBR_OK;
    }

    return JUBR_TRANSMIT_DEVICE_ERROR;
}


JUB_RV JubiterBladeToken::_TranPack(const TW::Data &apduData, const JUB_BYTE highMark, const JUB_BYTE sigType, const JUB_ULONG ulSendOnceLen, int finalData/* = false*/, int bOnce/* = false*/) {

    if (apduData.empty()) {
        JUB_VERIFY_RV(JUBR_ERROR);
    }

    JUB_UINT16 ret = 0;
    if (bOnce) {
        // one pack enough
        JUB_BYTE p1 = 0x00;
        p1 |= highMark;
        APDU apdu(0x00, 0xF8, p1, sigType, (JUB_ULONG)apduData.size(), apduData.data());
        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
        JUB_VERIFY_COS_ERROR(ret);

        return JUBR_OK;
    }

    // else send pack by pack
    auto nextTimes = apduData.size() / ulSendOnceLen;
    auto left = apduData.size() % ulSendOnceLen;

    // split last pack
    if (   0 == left
        && 0 != nextTimes
        ) {
        nextTimes--;
        left = ulSendOnceLen;
    }

    // pack by pack
    JUB_BYTE p1 = 0x02;
    p1 |= highMark;
    APDU apdu(0x00, 0xF8, p1, sigType, 0x00);
    apdu.lc = ulSendOnceLen;
    JUB_UINT32 times = 0;
    for (times = 0; times < nextTimes; times++) {
        apdu.SetData(apduData.data() + times * ulSendOnceLen, apdu.lc);
        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
        JUB_VERIFY_COS_ERROR(ret);
    }

    // next pack
    apdu.lc = (JUB_ULONG)left;
    if (apdu.lc) {
        if (finalData) {
            apdu.p1 = 0x03;
            apdu.p1 |= highMark;
        }

        apdu.SetData(apduData.data() + times * ulSendOnceLen, apdu.lc);
        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
        JUB_VERIFY_COS_ERROR(ret);
    }

    return JUBR_OK;
}


JUB_RV JubiterBladeToken::_TranPackApdu(const JUB_ULONG ncla, const JUB_ULONG nins,
                                        const TW::Data &apduData,
                                        const JUB_BYTE highMark,
                                        const JUB_BYTE sigType,
                                        const JUB_ULONG ulSendOnceLen,
                                        JUB_BYTE *retData/* = nullptr*/, JUB_ULONG *pulRetDataLen/* = nullptr*/,
                                        int finalData/* = false*/, int bOnce/* = false*/) {

    if (apduData.empty()) {
        JUB_VERIFY_RV(JUBR_ERROR);
    }

    JUB_UINT16 ret = 0;
    if (bOnce) {
        // one pack enough
        JUB_BYTE p1 = 0x00;
        p1 |= highMark;
        APDU apdu(ncla, nins, p1, sigType, (JUB_ULONG)apduData.size(), apduData.data());
        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
        JUB_VERIFY_COS_ERROR(ret);

        return JUBR_OK;
    }

    // else send pack by pack
    auto nextTimes = apduData.size() / ulSendOnceLen;
    auto left = apduData.size() % ulSendOnceLen;

    // split last pack
    if (   0 == left
        && 0 != nextTimes
        ) {
        nextTimes--;
        left = ulSendOnceLen;
    }

    // pack by pack
    JUB_BYTE p1 = 0x02;
    p1 |= highMark;
    APDU apdu(ncla, nins, p1, sigType, 0x00);
    apdu.lc = ulSendOnceLen;
    JUB_UINT32 times = 0;
    for (times = 0; times < nextTimes; times++) {
        apdu.SetData(apduData.data() + times * ulSendOnceLen, apdu.lc);
        JUB_VERIFY_RV(_SendApdu(&apdu, ret));
        JUB_VERIFY_COS_ERROR(ret);
    }

    // next pack
    apdu.lc = (JUB_ULONG)left;
    if (apdu.lc) {
        if (finalData) {
            apdu.p1 = 0x03;
            apdu.p1 |= highMark;
        }

        apdu.SetData(apduData.data() + times * ulSendOnceLen, apdu.lc);
        JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, pulRetDataLen));
        JUB_VERIFY_COS_ERROR(ret);
    }

    return JUBR_OK;
}


JUB_RV JubiterBladeToken::_SelectApp(const JUB_BYTE PKIAID[], JUB_BYTE length, uchar_vector& version) {

    APDU apdu(0x00, 0xA4, 0x04, 0x00, length, PKIAID);
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(JubiterBladeToken::_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);

    version = uchar_vector(&retData[4], retData[3]);

    return JUBR_OK;
}


JUB_RV JubiterBladeToken::_SelectApp(const JUB_BYTE PKIAID[], const JUB_BYTE length) {

    uchar_vector vVersion;
    JUB_VERIFY_RV(JubiterBladeToken::_SelectApp(PKIAID, length, vVersion));
    JubiterBladeToken::_appletVersion = stVersionExp::FromString(vVersion.getHex());

    return JUBR_OK;
}


JUB_RV JubiterBladeToken::QueryBattery(JUB_BYTE &percent) {

    APDU apdu(0x00, 0xD6, 0xFE, 0xED, 0x00, NULL, 0x01);
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);

    percent = retData[0];

    return JUBR_OK;
}


JUB_RV JubiterBladeToken::ShowVirtualPwd() {

    APDU apdu(0x00, 0x29, 0x00, 0x00, 0x00);
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


JUB_RV JubiterBladeToken::CancelVirtualPwd() {

    APDU apdu(0x00, 0x29, 0x80, 0x00, 0x00);
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);

    return JUBR_OK;
}


JUB_RV JubiterBladeToken::GetRootKeyStatus(JUB_ENUM_DEVICE_ROOT_KEY_STATUS_PTR status) {

    // send apdu
    uchar_vector apduData = tlv_buf(0xDFFF, uchar_vector("8105")).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(JubiterBladeToken::_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 != ret
        ||   1 != ulRetDataLen
        ) {
        return JUBR_ERROR;
    }

    if (   JUB_ENUM_DEVICE_ROOT_KEY_STATUS::HAS_PIN      != retData[0]
        && JUB_ENUM_DEVICE_ROOT_KEY_STATUS::RESETTED     != retData[0]
        && JUB_ENUM_DEVICE_ROOT_KEY_STATUS::HAS_ROOT_KEY != retData[0]
        ) {
        return JUBR_ERROR;
    }

    *status = (JUB_ENUM_DEVICE_ROOT_KEY_STATUS)retData[0];

    return JUBR_OK;
}


bool JubiterBladeToken::IsBootLoader() {

    APDU apdu(0x00, 0xA4, 0x04, 0x00, 0x00);
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    auto rv = _SendApdu(&apdu, ret, retData, &ulRetDataLen);
    if (  JUBR_OK == rv
        && 0x6e00 == ret
        ) {
        return true;
    }
    else if (0x9000 == ret) {
        return true;
    }

    return false;
}


JUB_RV JubiterBladeToken::SelectMainSecurityDomain() {

    // select main safe scope
    APDU apdu(0x00, 0xA4, 0x04, 0x00, 0x00);
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(JubiterBladeToken::_SendApdu(&apdu, ret));
    JUB_VERIFY_COS_ERROR(ret);

    return JUBR_OK;
}


JUB_RV JubiterBladeToken::GetSN(JUB_BYTE sn[24]) {

    uchar_vector apduData = tlv_buf(0xDFFF, uchar_vector("8101")).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(JubiterBladeToken::_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 == ret) {
        memset(sn, 0x00, 24);
        memcpy(sn, retData, 24);

        return JUBR_OK;
    }

    return JUBR_ERROR;
}


JUB_RV JubiterBladeToken::GetLabel(JUB_BYTE label[32]) {

    uchar_vector apduData = tlv_buf(0xDFFF, uchar_vector("8104")).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(JubiterBladeToken::_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 == ret) {
        memset(label, 0x00, 32);
        memcpy(label, retData, 32);

        return JUBR_OK;
    }

    return JUBR_ERROR;
}


JUB_RV JubiterBladeToken::GetPinRetry(JUB_BYTE& retry) {

    uchar_vector apduData = tlv_buf(0xDFFF, uchar_vector("8102")).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(JubiterBladeToken::_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 == ret) {
        retry = retData[0];

        return JUBR_OK;
    }

    return JUBR_ERROR;
}


JUB_RV JubiterBladeToken::GetPinMaxRetry(JUB_BYTE& maxRetry) {

    uchar_vector apduData = tlv_buf(0xDFFF, uchar_vector("8103")).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(JubiterBladeToken::_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 == ret) {
        maxRetry = retData[0];

        return JUBR_OK;
    }

    return JUBR_ERROR;
}


JUB_RV JubiterBladeToken::GetBleVersion(JUB_BYTE bleVersion[4]) {

    uchar_vector apduData = tlv_buf(0xDFFF, uchar_vector("8100")).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(JubiterBladeToken::_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 == ret) {
        uchar_vector version(retData, retData + ulRetDataLen);
        memset(bleVersion, 0x00, 4);
        memcpy(bleVersion, version.getHex().c_str(), 4);

        return JUBR_OK;
    }

    return JUBR_ERROR;
}


JUB_RV JubiterBladeToken::GetFwVersion(JUB_BYTE fwVersion[4]) {

    uchar_vector apduData = tlv_buf(0xDFFF, uchar_vector("8003")).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(JubiterBladeToken::_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 == ret) {
        uchar_vector version(retData, retData + ulRetDataLen);
        memset(fwVersion, 0x00, 4);
        memcpy(fwVersion, version.getHex().c_str(), 4);

        return JUBR_OK;
    }

    return JUBR_ERROR;
}


JUB_RV JubiterBladeToken::EnumApplet(std::string& appletList) {

//    {
//        // select main safe scope
//        APDU apdu(0x00, 0xA4, 0x04, 0x00, 0x00);
//        JUB_UINT16 ret = 0;
//        JUB_VERIFY_RV(JubiterBladeToken::_SendApdu(&apdu, ret));
//        JUB_VERIFY_COS_ERROR(ret);
//    }

    // send apdu, then decide which coin types supports.
    uchar_vector apduData = tlv_buf(0xDFFF, uchar_vector("8106")).encode();
    APDU apdu(0x80, 0xCB, 0x80, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
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


JUB_RV JubiterBladeToken::GetAppletVersion(const std::string& appID, stVersion& version) {

    uchar_vector id(appID);
    if (0 == appID.length()) {
        JUB_VERIFY_RV(JUBR_ARGUMENTS_BAD);
    }

    JUB_UINT16 ret = 0;
    //select
    APDU apdu(0x00, 0xA4, 0x04, 0x00, (JUB_ULONG)id.size(), &id[0]);
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(JubiterBladeToken::_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    JUB_VERIFY_COS_ERROR(ret);

    uchar_vector vVersion(4);
    uchar_vector FidoID(kPKIAID_FIDO, sizeof(kPKIAID_FIDO)/sizeof(JUB_BYTE));
    if (id == FidoID) {
        //get version
        uchar_vector apduData = tlv_buf(0xDFFF, uchar_vector("8001")).encode();
        APDU apduVersion(0x80, 0xE2, 0x80, 0x00, (JUB_ULONG)apduData.size(), &apduData[0], 0x00);
        ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
        memset(retData, 0x00, ulRetDataLen);
        JUB_VERIFY_RV(JubiterBladeToken::_SendApdu(&apduVersion, ret, retData, &ulRetDataLen));
        JUB_VERIFY_COS_ERROR(ret);

        vVersion = uchar_vector(&retData[6], 4);
    }
    else {
        if (!(0x84 == retData[2]
           && 0x04 == retData[3])
            ) {
            return JUBR_ERROR;
        }
        vVersion = uchar_vector(&retData[4], 4);
    }

    version = stVersionExp::FromString(vVersion.getHex());

    return JUBR_OK;
}


JUB_RV JubiterBladeToken::EnumSupportCoins(std::string& coinList) {

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
        for (auto appInfo : JubiterBladeToken::g_appInfo) {
            uchar_vector _appID(appInfo.appID);
            if (_appID.getHex() != appID) {
                continue;
            }
            if (stVersionExp::FromString(appInfo.minimumAppletVersion) > version) {
                continue;
            }
            if (coinNameList.end() == std::find(coinNameList.begin(), coinNameList.end(), appInfo.coinName)) {
                // v2.0.04 of COS couldn't support EOS.
                if (  "2004" == strFWVersion
                    && (   "EOS" == appInfo.coinName
                        || "FIL" == appInfo.coinName
                        || "DOT" == appInfo.coinName
                        || "KSM" == appInfo.coinName
                        )
                    ) {
                    continue;
                }
                coinList += appInfo.coinName;
                coinList += " ";
                coinNameList.insert(coinNameList.end(), appInfo.coinName);
            }
        }
    }

    return JUBR_OK;
}


JUB_RV JubiterBladeToken::GetDeviceCert(std::string& cert) {

    uchar_vector apduData = tlv_buf(0xA6, tlv_buf(0x83, uchar_vector("1518")).encode()).encode();
    APDU apdu(0x80, 0xCA, 0xBF, 0x21, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(JubiterBladeToken::_SendApdu(&apdu, ret, retData, &ulRetDataLen));
    if (0x9000 == ret) {
        uchar_vector vcert(retData + 4, retData + ulRetDataLen);
        cert = vcert.getHex();
        return JUBR_OK;
    }

    return JUBR_ERROR;
}


JUB_RV JubiterBladeToken::SendOneApdu(const std::string& apdu, std::string& response) {

    auto device = jub::device::DeviceManager::GetInstance()->GetOne(_deviceID);
    JUB_CHECK_NULL(device);

    uchar_vector sendApdu(apdu);
    JUB_BYTE retData[FT3KHN_READWRITE_SIZE_ONCE_NEW + 6] = { 0, };
    JUB_ULONG ulRetDataLen = FT3KHN_READWRITE_SIZE_ONCE_NEW + 6;
    JUB_RV rv = device->SendData(sendApdu.data(), (JUB_ULONG)sendApdu.size(), retData, &ulRetDataLen);
    if (JUBR_OK != rv) {
        JUB_VERIFY_RV(JUBR_TRANSMIT_DEVICE_ERROR);
    }

    uchar_vector vResponse(retData, retData + ulRetDataLen);
    response = vResponse.getHex();

    return JUBR_OK;
}


JUB_RV JubiterBladeToken::VerifyPIN(const std::string &pinMix, OUT JUB_ULONG &retry) {

    std::vector<uint8_t> pinCoord;
    std::transform(pinMix.begin(),
                   pinMix.end(),
                   std::back_inserter(pinCoord),
                   [](const char elem) {
        return (uint8_t)(elem - 0x30);
    });

    APDU apdu(0x00, 0x20, 0x02, 0x00, (JUB_ULONG)pinCoord.size(), pinCoord.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = { 0, };
    JUB_ULONG ulRetDataLen = sizeof(retData) / sizeof(JUB_BYTE);
    JUB_VERIFY_RV(JubiterBladeToken::_SendApdu(&apdu, ret, retData, &ulRetDataLen));
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


JUB_RV JubiterBladeToken::ChangePIN(const std::string &pinMix, const std::string &pinNew) {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterBladeToken::SetTimeout(const JUB_UINT16 timeout) {

    JUB_UINT16 p1 = timeout >> 8;
    JUB_UINT16 p2 = timeout & 0xFF;
    APDU apdu(0x00, 0xFB, p1, p2, 0x00);
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(JubiterBladeToken::_SendApdu(&apdu, ret));
    if (0x9000 == ret) {
        return JUBR_OK;
    }

    return JUBR_ERROR;
}


JUB_RV JubiterBladeToken::SetERC20Tokens(const ERC20_TOKEN_INFO tokens[],
                                         const JUB_UINT16 iCount) {

    if (2 < iCount) {
        return JUBR_ARGUMENTS_BAD;
    }

    uchar_vector apduData;
    apduData << (uint8_t)iCount;
    for (JUB_UINT16 i=0; i<iCount; ++i) {
        uchar_vector lvName = Tollv(tokens[i].tokenName);
        uchar_vector address;
        address << ETHHexStr2CharPtr(tokens[i].contractAddress);

        apduData << (uint8_t)tokens[i].unitDP;
        apduData << (uint8_t)lvName.size();
        apduData << lvName;
        apduData << (uint8_t)address.size();
        apduData << address;
    }

    APDU apdu(0x00, 0xc7, JUB_ENUM_APDU_ERC_P1::TOKENS_INFO, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    if (0x9000 != ret) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    return JUBR_OK;
}


JUB_RV JubiterBladeToken::SetERC20Token(JUB_CHAR_CPTR tokenName,
                                        JUB_UINT16 unitDP,
                                        JUB_CHAR_CPTR contractAddress) {

    uchar_vector lvName = Tollv(tokenName);
    uchar_vector address = HexStr2CharPtr(contractAddress);

    uchar_vector apduData;
    apduData << (uint8_t)unitDP;
    apduData << (uint8_t)lvName.size();
    apduData << lvName;
    apduData << (uint8_t)address.size();
    apduData << address;

    APDU apdu(0x00, 0xC7, JUB_ENUM_APDU_ERC_P1::ERC20, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    JUB_VERIFY_COS_ERROR(ret);

    return JUBR_OK;
}


JUB_RV JubiterBladeToken::SetERC721Token(JUB_CHAR_CPTR tokenName,
                                         JUB_CHAR_CPTR contractAddress) {

    uchar_vector lvName = Tollv(tokenName);
    uchar_vector address = HexStr2CharPtr(contractAddress);

    uchar_vector apduData;
    apduData << (uint8_t)lvName.size();
    apduData << lvName;
    apduData << (uint8_t)address.size();
    apduData << address;

    APDU apdu(0x00, 0xC7, JUB_ENUM_APDU_ERC_P1::ERC721, 0x00, (JUB_ULONG)apduData.size(), apduData.data());
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_SendApdu(&apdu, ret));
    JUB_VERIFY_COS_ERROR(ret);

    return JUBR_OK;
}


/// NFC
JUB_RV JubiterBladeToken::SetLabel(const std::string& label) {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterBladeToken::Reset() {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterBladeToken::GenerateSeed(const std::string& pinMix,
                                       const JUB_ENUM_CURVES& curve) {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterBladeToken::ImportMnemonic(const std::string& pinMix,
                                         const std::string& mnemonic) {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterBladeToken::ExportMnemonic(const std::string& pinMix,
                                         OUT std::string& mnemonic) {

    return JUBR_IMPL_NOT_SUPPORT;
}


/// BIO
JUB_RV JubiterBladeToken::UIShowMain() {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterBladeToken::IdentityVerify(IN JUB_BYTE mode, OUT JUB_ULONG &retry) {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterBladeToken::IdentityVerifyPIN(IN JUB_BYTE mode, IN const std::string &pinMix, OUT JUB_ULONG &retry) {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterBladeToken::IdentityNineGrids(IN bool bShow) {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterBladeToken::VerifyFgptForIntl(OUT JUB_ULONG &retry) {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterBladeToken::VerifyFingerprint(OUT JUB_ULONG &retry) {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterBladeToken::EnrollFingerprint(IN JUB_UINT16 fpTimeout,
                                            INOUT JUB_BYTE_PTR fgptIndex, OUT JUB_ULONG_PTR ptimes,
                                            OUT JUB_BYTE_PTR fgptID) {
    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterBladeToken::EnumFingerprint(std::string& fgptList) {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterBladeToken::EraseFingerprint(IN JUB_UINT16 fpTimeout) {

    return JUBR_IMPL_NOT_SUPPORT;
}


JUB_RV JubiterBladeToken::DeleteFingerprint(IN JUB_UINT16 fpTimeout,
                                            JUB_BYTE fgptID)  {

    return JUBR_IMPL_NOT_SUPPORT;
}


} // namespace token end
} // namespace jub end
