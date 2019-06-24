#include <token/JubiterBLDImpl.h>

namespace jub {

stAppInfos JubiterBLDImpl::g_appinfo[] = {
    {{0xD1, 0x56, 0x00, 0x01, 0x32, 0x83, 0x00, 0x42, 0x4C, 0x44, 0x00, 0x00,
      0x42, 0x54, 0x43, 0x01},
     "BTC",
     "0000000"},
    {{0xD1, 0x56, 0x00, 0x01, 0x32, 0x83, 0x00, 0x42, 0x4C, 0x44, 0x00, 0x00,
      0x45, 0x54, 0x48, 0x01},
     "ETH",
     "0000000"},
    // BTC and ETH index position fixed, start adding new apps below:
    {{0xD1, 0x56, 0x00, 0x01, 0x32, 0x83, 0x00, 0x42, 0x4C, 0x44, 0x00, 0x00,
    0x45, 0x54, 0x48, 0x01},
     "ETC",
     "01010000"},
    {{0xD1, 0x56, 0x00, 0x01, 0x32, 0x83, 0x00, 0x42, 0x4C, 0x44, 0x00, 0x00,
    0x42, 0x54, 0x43, 0x01},
     "BCH",
     "01070003",
    },
    {{0xD1, 0x56, 0x00, 0x01, 0x32, 0x83, 0x00, 0x42, 0x4C, 0x44, 0x00, 0x00,
    0x42, 0x54, 0x43, 0x01},
     "LTC",
     "01070003",
    },
    {{0xD1, 0x56, 0x00, 0x01, 0x32, 0x83, 0x00, 0x42, 0x4C, 0x44, 0x00, 0x00,
    0x42, 0x54, 0x43, 0x01},
     "USDT",
     "01080002"
    }
};

JubiterBLDImpl::JubiterBLDImpl(std::string path):
    _apduBuiler(std::make_shared<JubApudBuiler>()),
    _device(std::make_shared<device_type>()),
    _path(path) {

};

JubiterBLDImpl::JubiterBLDImpl(device_type* device):
    _apduBuiler(std::make_shared<JubApudBuiler>()),
    _device(device) {

}

JubiterBLDImpl::~JubiterBLDImpl() {};

JUB_RV JubiterBLDImpl::connectToken() {
    return _device->connect(_path);
}

JUB_RV JubiterBLDImpl::disconnectToken() {
    return _device->disconnect();
}

JUB_RV JubiterBLDImpl::enumApplet(std::string& applet_list) {

    {
        // select main safe scope
        APDU apdu(0x00, 0xa4, 0x04, 0x00, 0x00);
        JUB_UINT16 ret = 0;
        JUB_VERIFY_RV(_sendApdu(&apdu, ret));
        if (0x9000 != ret) {
            return JUBR_TRANSMIT_DEVICE_ERROR;
        }
    }

    // send apdu, then decide which coin types supports.
    APDU apdu(0x80, 0xCB, 0x80, 0x00, 0x05,
        (const JUB_BYTE *)"\xDF\xFF\x02\x81\x06");
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG retLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
    if (0x9000 != ret) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    DataChunk tlvData(retData, retData + retLen);
    auto appList = parseTlv(tlvData);

//        int appindex = 0;
    for (auto appid : appList) {
        uchar_vector id(appid);
        applet_list += id.getHex();
        applet_list += " ";
    }

    return JUBR_OK;
}

JUB_RV JubiterBLDImpl::enumSupportCoins(std::string& coinList) {

    std::string applet_list;
    enumApplet(applet_list);

    auto v_applist = split(applet_list, " ");

    for (auto appid : v_applist) {
        std::string version;
        auto rv = getAppletVersion(appid, version);
        if (JUBR_OK == rv) {
            for (auto appInfo : JubiterBLDImpl::g_appinfo) {
                uchar_vector _appid(appInfo.appId);
                if (_appid .getHex() == appid) {
                    if (appInfo.minimumAppletVersion < version) {
                        coinList += appInfo.coinName;
                        coinList += " ";
                    }
                }
            }
        }
    }

    return JUBR_OK;
}

JUB_RV JubiterBLDImpl::getAppletVersion(std::string appID, std::string& version) {

    uchar_vector id(appID);
    if (0 == appID.length()) {
        return JUBR_ARGUMENTS_BAD;
    }

    JUB_UINT16 ret = 0;
    uchar_vector FidoId(PKIAID_FIDO, 8);
    if (id == FidoId) {
        //select
        APDU apdu(0x00, 0xA4, 0x04, 0x00, id.size(), &id[0]);
        JUB_BYTE retData[1024] = {0,};
        JUB_ULONG retLen = sizeof(retData)/sizeof(JUB_BYTE);
        JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
        if (0x9000 != ret) {
            return JUBR_TRANSMIT_DEVICE_ERROR;
        }

        //get version

        uchar_vector apdu_data("DFFF028001");

        APDU apdu_version(0x80, 0xE2, 0x80, 0x00, apdu_data.size(), &apdu_data[0], 0x00);
        JUB_BYTE retData_version[1024] = {0,};
        JUB_ULONG retLen_version = sizeof(retData_version)/sizeof(JUB_BYTE);
        JUB_VERIFY_RV(_sendApdu(&apdu_version, ret, retData_version, &retLen_version));
        if (0x9000 != ret) {
            return JUBR_TRANSMIT_DEVICE_ERROR;
        }

        uchar_vector v_version(&retData_version[6], 4);
        version = v_version.getHex();
        return JUBR_OK;
    }
    else {
        APDU apdu(0x00, 0xA4, 0x04, 0x00, id.size(), &id[0]);
        JUB_BYTE retData[1024] = {0,};
        JUB_ULONG retLen = sizeof(retData)/sizeof(JUB_BYTE);
        JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
        if (0x9000 != ret) {
            return JUBR_TRANSMIT_DEVICE_ERROR;
        }

        if (   0x84 == retData[2]
            && 0x04 == retData[3]
            ) {
            uchar_vector v_version(&retData[4], 4);
            version = v_version.getHex();
            return JUBR_OK;
        }
    }

    return JUBR_ERROR;
}

JUB_RV JubiterBLDImpl::getDeviceCert(std::string& cert) {

    uchar_vector apdu_data("A60483021518");
    APDU apdu(0x80, 0xca, 0xbf, 0x21, apdu_data.size(), apdu_data.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG retLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
    if (0x9000 == ret) {
        uchar_vector vcert(retData + 4, retData + retLen);
        cert = vcert.getHex();
        return JUBR_OK;
    }

    return JUBR_ERROR;
}

JUB_RV JubiterBLDImpl::sendOneApdu(const std::string& apdu, std::string& response) {

    uchar_vector sendApdu(apdu);
    JUB_BYTE retdata[FT3KHN_READWRITE_SIZE_ONCE_NEW + 6] = {0,};
    JUB_ULONG ulRetLen = FT3KHN_READWRITE_SIZE_ONCE_NEW + 6;
    JUB_RV rv = _device->sendData(sendApdu.data(), sendApdu.size(), retdata, &ulRetLen);
    if(JUBR_OK != rv) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    uchar_vector vResponse(retdata, retdata + ulRetLen);
    response = vResponse.getHex();

    return JUBR_OK;
}

JUB_RV JubiterBLDImpl::queryBattery(JUB_BYTE &percent) {

    JUB_CHECK_NULL(_device);

    APDU apdu(0x00, 0xD6, 0xFE, 0xED, 0x01);
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG retLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
    if (0x9000 != ret) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    percent = retData[0];

    return JUBR_OK;
}

JUB_RV JubiterBLDImpl::showVirtualPwd() {

    JUB_CHECK_NULL(_device);
    //SWITCH_TO_BTC_APP

    APDU apdu(0x00, 0x29, 0x00, 0x00, 0x00);
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG retLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
    if (0x9000 != ret) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    return JUBR_OK;
}

JUB_RV JubiterBLDImpl::cancelVirtualPwd() {

    JUB_CHECK_NULL(_device);
    //SWITCH_TO_BTC_APP

    APDU apdu(0x00, 0x29, 0x80, 0x00, 0x00);
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG retLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
    if (0x9000 != ret) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    return JUBR_OK;
}

JUB_RV JubiterBLDImpl::verifyPIN(const std::string &pinMix, OUT JUB_ULONG &retry) {

    JUB_CHECK_NULL(_device);

    // select app first
    //SWITCH_TO_BTC_APP

    DataChunk pinCoord;
    //auto pinData = buildData({ pin });

    std::transform(pinMix.begin(),
                   pinMix.end(),
                   std::back_inserter(pinCoord),
                   [](const char elem) {
                       return (uint8_t)(elem - 0x30);
                   }
    );

    APDU apdu(0x00, 0x20, 0x02, 0x00, pinCoord.size(), pinCoord.data());
    //APDU apdu(0x00, 0x10, 0x00, 0x00, pinCoord.size(), pinCoord.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG retLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
    if (0x6985 == ret) { //locked
        return JUBR_PIN_LOCKED;
    }
    if (0x63C0 == (ret & 0xfff0)) {
        retry = (ret & 0xf);
        return JUBR_DEVICE_PIN_ERROR;
    }
    else if (0x9000 != ret) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    return JUBR_OK;
}

bool JubiterBLDImpl::isInitialize() {

    uchar_vector apdu_data("DFFF028105");
    APDU apdu(0x80, 0xcb, 0x80, 0x00, apdu_data.size(), apdu_data.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG retLen = sizeof(retData)/sizeof(JUB_BYTE);
    auto rv = _sendApdu(&apdu, ret, retData, &retLen);
    if (JUBR_OK == rv) {
        if (0x5a == retData[0]) {
            return true;
        }
    }

    return false;
}

bool JubiterBLDImpl::isBootLoader() {

    APDU apdu(0x00, 0xa4, 0x04, 0x00, 0x00);
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG retLen = sizeof(retData)/sizeof(JUB_BYTE);
    auto rv = _sendApdu(&apdu, ret, retData, &retLen);
    if (  JUBR_OK == rv
        && 0x6e00 == ret
        ) {
        return true;
    }

    return false;
}

JUB_RV JubiterBLDImpl::getBleVersion(JUB_BYTE ble_version[4]) {

    uchar_vector apdu_data("DFFF028100");
    APDU apdu(0x80, 0xcb, 0x80, 0x00, apdu_data.size(), apdu_data.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG retLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
    if (0x9000 == ret) {
        uchar_vector version(retData, retData + retLen);
        memset(ble_version, 0x00, 4);
        memcpy(ble_version, version.getHex().c_str(), 4);

        return JUBR_OK;
    }

    return JUBR_ERROR;
}

JUB_RV JubiterBLDImpl::getFwVersion(JUB_BYTE fw_version[4]) {

    uchar_vector apdu_data("DFFF028003");
    APDU apdu(0x80, 0xcb, 0x80, 0x00, apdu_data.size(), apdu_data.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG retLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
    if (0x9000 == ret) {
        uchar_vector version(retData, retData + retLen);
        memset(fw_version, 0x00, 4);
        memcpy(fw_version, version.getHex().c_str(), 4);

        return JUBR_OK;
    }

    return JUBR_ERROR;
}

JUB_RV JubiterBLDImpl::getSN(JUB_BYTE sn[24]) {

    uchar_vector apdu_data("DFFF028101");
    APDU apdu(0x80, 0xcb, 0x80, 0x00, apdu_data.size(), apdu_data.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG retLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
    if (0x9000 == ret) {
        memset(sn, 0x00, 24);
        memcpy(sn, retData, 24);

        return JUBR_OK;
    }

    return JUBR_ERROR;
}

JUB_RV JubiterBLDImpl::getLabel(JUB_BYTE label[32]) {

    uchar_vector apdu_data("DFFF028104");
    APDU apdu(0x80, 0xcb, 0x80, 0x00, apdu_data.size(), apdu_data.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG retLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
    if (0x9000 == ret) {
        memset(label, 0x00, 32);
        memcpy(label, retData, 32);

        return JUBR_OK;
    }

    return JUBR_ERROR;
}

JUB_RV JubiterBLDImpl::getPinRetry(JUB_BYTE& retry) {

    uchar_vector apdu_data("DFFF028102");
    APDU apdu(0x80, 0xcb, 0x80, 0x00, apdu_data.size(), apdu_data.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG retLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
    if (0x9000 == ret) {
        retry = retData[0];

        return JUBR_OK;
    }

    return JUBR_ERROR;
}

JUB_RV JubiterBLDImpl::getPinMaxRetry(JUB_BYTE& max_retry) {

    uchar_vector apdu_data("DFFF028103");
    APDU apdu(0x80, 0xcb, 0x80, 0x00, apdu_data.size(), apdu_data.data());
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG retLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
    if (0x9000 == ret) {
        max_retry = retData[0];

        return JUBR_OK;
    }

    return JUBR_ERROR;
}

JUB_RV JubiterBLDImpl::_selectApp(const JUB_BYTE PKIAID[], JUB_BYTE length) {

    APDU apdu(0x00, 0xA4, 0x04, 0x00, length, PKIAID);
    JUB_UINT16 ret = 0;
    JUB_BYTE retData[1024] = {0,};
    JUB_ULONG retLen = sizeof(retData)/sizeof(JUB_BYTE);
    JUB_VERIFY_RV(_sendApdu(&apdu, ret, retData, &retLen));
    if (0x9000 != ret) {
        return JUBR_TRANSMIT_DEVICE_ERROR;
    }

    return JUBR_OK;
}

JUB_RV JubiterBLDImpl::_tranPack(const DataSlice &apduData, JUB_BYTE sigType,JUB_ULONG sendLenOnce, int finalData/* = false*/,int bOnce/* = false*/) {

    if (apduData.empty()) {
        return JUBR_ERROR;
    }

    JUB_UINT16 ret = 0;
    if (bOnce) {
        // one pack enough
        APDU apdu(0x00, 0xF8, 0x00, sigType, apduData.size(), apduData.data());
        JUB_VERIFY_RV(_sendApdu(&apdu, ret));
        if (0x9000 != ret) {
            return JUBR_TRANSMIT_DEVICE_ERROR;
        }

        return JUBR_OK;
    }

    // else send pack by pack
    auto nextTimes = apduData.size() / sendLenOnce;
    auto left = apduData.size() % sendLenOnce;

    // split last pack
    if (   0 == left
        && 0 != nextTimes
        ) {
        nextTimes--;
        left = sendLenOnce;
    }

    // pack by pack
    APDU apdu(0x00, 0xF8, 0x02, sigType, 0x00);
    apdu.lc = sendLenOnce;
    JUB_UINT32 ulTimes = 0;
    for (ulTimes = 0; ulTimes < nextTimes; ulTimes++) {
        apdu.SetData(apduData.data() + ulTimes * sendLenOnce, apdu.lc);
        JUB_VERIFY_RV(_sendApdu(&apdu, ret));
        if (0x9000 != ret) {
            return JUBR_TRANSMIT_DEVICE_ERROR;
        }
    }

    // next pack
    apdu.lc = left;
    if (apdu.lc) {
        if (finalData) {
            apdu.p1 = 0x03;
        }

        apdu.SetData(apduData.data() + ulTimes * sendLenOnce, apdu.lc);
        JUB_VERIFY_RV(_sendApdu(&apdu, ret));
        if (0x9000 != ret) {
            return JUBR_TRANSMIT_DEVICE_ERROR;
        }
    }

    return JUBR_OK;
}

JUB_RV JubiterBLDImpl::_sendApdu(const APDU *apdu, JUB_UINT16 &wRet, JUB_BYTE *pRetData /*= nullptr*/,
    JUB_ULONG *pulRetLen /*= nullptr*/,
    JUB_ULONG ulMiliSecondTimeout /*= 0*/) {

    JUB_CHECK_NULL(_apduBuiler);
    JUB_CHECK_NULL(_device);

    JUB_BYTE retdata[FT3KHN_READWRITE_SIZE_ONCE_NEW + 6] = {0,};
    JUB_ULONG ulRetLen = FT3KHN_READWRITE_SIZE_ONCE_NEW + 6;

    std::vector<JUB_BYTE> sendApdu;
    if (JUBR_OK == _apduBuiler->buildApdu(apdu, sendApdu)) {
        if (JUBR_OK != _device->sendData(sendApdu.data(), sendApdu.size(), retdata, &ulRetLen, ulMiliSecondTimeout)) {
            return JUBR_TRANSMIT_DEVICE_ERROR;
        }

        if (NULL == pulRetLen) {
            wRet = retdata[ulRetLen - 2] * 0x100 + retdata[ulRetLen - 1];
            return JUBR_OK;
        }

        if (NULL == pRetData) {
            *pulRetLen = ulRetLen - 2;
            wRet = (retdata[ulRetLen - 2] * 0x100 + retdata[ulRetLen - 1]);
            return JUBR_OK;
        }

        if (*pulRetLen < (ulRetLen - 2)) {
            *pulRetLen = ulRetLen - 2;
            return JUBR_BUFFER_TOO_SMALL;
        }

        *pulRetLen = ulRetLen - 2;
        memcpy(pRetData, retdata, ulRetLen - 2);

        wRet = retdata[ulRetLen - 2] * 0x100 + retdata[ulRetLen - 1];
        return JUBR_OK;
    }

    return JUBR_TRANSMIT_DEVICE_ERROR;
}

JUB_RV JubiterBLDImpl::setTimeout(JUB_UINT16 timeout) {

    JUB_UINT16 p1 = timeout >> 8;
    JUB_UINT16 p2 = timeout & 0xFF;
    APDU apdu(0x00, 0xfb, p1, p2, 0x00);
    JUB_UINT16 ret = 0;
    JUB_VERIFY_RV(_sendApdu(&apdu, ret));
    if (0x9000 == ret) {
        return JUBR_OK;
    }

    return JUBR_ERROR;
}

}  // namespace jub end
