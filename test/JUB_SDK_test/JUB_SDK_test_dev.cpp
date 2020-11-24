//
//  JUB_SDK_test_dev.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/9/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
#include "JUB_SDK_test_dev.hpp"
#include "../../include/JUB_SDK_DEV_HID.h"
#include "../../include/JUB_SDK_DEV_BIO.h"

#include "JUB_SDK_main.h"


vector<string> split(const string &str, char delim, bool bSkipEmpty/* = true*/) {

    istringstream iss(str);
    vector<string> elems;
    for (string item; getline(iss, item, delim); ) {
        if (bSkipEmpty && item.empty()) {
            continue;
        }
        else {
            elems.push_back(item);
        }
    }

    return elems;
}


void get_device_info_test() {

    JUB_UINT16 deviceIDs[MAX_DEVICE] = { 0xffff, };
    JUB_ListDeviceHid(deviceIDs);

    JUB_RV rv = JUB_ConnetDeviceHid(deviceIDs[0]);
    if (JUBR_OK != rv) {
        cout << "JUB_ConnetDeviceHid() return " << GetErrMsg(rv) << endl;
        return;
    }

    JUB_UINT16 deviceID = deviceIDs[0];

    char* appList;
    rv = JUB_EnumApplets(deviceID, &appList);
    if (JUBR_OK != rv) {
        cout << "JUB_EnumApplets() return " << GetErrMsg(rv) << endl;
        return;
    }
    std::string appletList = appList;
    JUB_FreeMemory(appList);

    auto vAppList = split(appletList, ' ');

    for (auto appID : vAppList) {
        char* version;
        auto rv = JUB_GetAppletVersion(deviceID, (char*)appID.c_str(), &version);
        if (JUBR_OK != rv) {
            cout << "JUB_GetAppletVersion() return " << GetErrMsg(rv) << endl;
            return;
        }

        cout << appID << "Applet Version : " << version << endl;
    }

    JUB_DEVICE_INFO info;
    rv = JUB_GetDeviceInfo(deviceID, &info);
    if (JUBR_OK != rv) {
        cout << "JUB_GetDeviceInfo() return " << GetErrMsg(rv) << endl;
        return;
    }

    cout << "device Label :" << info.label << endl;
    cout << "device sn :" << info.sn << endl;
    cout << "device pinRetry :" << info.pinRetry << endl;
    cout << "device pinMaxRetry :" << info.pinMaxRetry << endl;
    JUB_BYTE bleVersion[5] = {0,};
    JUB_BYTE fwVersion[5] = {0,};
    memcpy(bleVersion, info.bleVersion, 4);
    memcpy(fwVersion, info.firmwareVersion, 4);
    cout << "device bleVersion :" << bleVersion << endl;
    cout << "device fwVersion :" << fwVersion << endl;

    char* cert;
    rv = JUB_GetDeviceCert(deviceID, &cert);
    if (JUBR_OK != rv) {
        cout << "JUB_GetDeviceCert() return " << GetErrMsg(rv) << endl;
        return;
    }

    cout << "device cert is :" << cert << endl;
    JUB_FreeMemory(cert);

    char* coinList;
    rv = JUB_EnumSupportCoins(deviceID, &coinList);
    if (JUBR_OK != rv) {
        cout << "JUB_EnumSupportCoins() return " << GetErrMsg(rv) << endl;
        return;
    }

    cout << "support coin list is :" << coinList << endl;
    JUB_FreeMemory(coinList);
}


JUB_RV verify_pin(JUB_UINT16 contextID) {

    JUB_RV rv = JUBR_ERROR;
    while (rv) {
        //����pin��λ�ã�������123456789
        cout << "1 2 3" << endl;
        cout << "4 5 6" << endl;
        cout << "7 8 9" << endl;

        cout << "to cancel the virtualpwd iput 'c'" << endl;
        rv = JUB_ShowVirtualPwd(contextID);
        if (   JUBR_OK               != rv
            && JUBR_IMPL_NOT_SUPPORT != rv
            ) {
            cout << "JUB_ShowVirtualPwd() return " << GetErrMsg(rv) << endl;
            break;
        }

        char str[9] = {0,};

        cin >> str;
        cout << str << endl;

        if (   'c' == str[0]
            || 'C' == str[0]
            ) {
            cout << "cancel the VirtualPwd "<< endl;
            rv = JUB_CancelVirtualPwd(contextID);
            if (JUBR_OK != rv) {
                cout << "JUB_CancelVirtualPwd() return " << GetErrMsg(rv) << endl;
                break;
            }
            return rv;
        }

        JUB_ULONG retry;
        rv = JUB_VerifyPIN(contextID, str, &retry);
        if (JUBR_OK != rv) {
            cout << "JUB_VerifyPIN() return " << GetErrMsg(rv) << endl;
            break;
        }
    }

    return rv;
}


JUB_RV verify_fgpt(JUB_UINT16 contextID) {

    JUB_RV rv = JUBR_ERROR;
    cout << endl;
    cout << "  -  " << endl;
    cout << "- - -" << endl;
    cout << "- - -" << endl;
    cout << "  -  " << endl;

    cout << "to cancel the fgpt iput 'c'" << endl;
    JUB_ULONG retry;
    rv = JUB_VerifyFingerprint(contextID, &retry);
    if (JUBR_OK != rv) {
        cout << "JUB_VerifyFingerprint() return " << GetErrMsg(rv) << endl;
    }

    return rv;
}

void set_timeout_test(JUB_UINT16 contextID) {

    cout << "* Please enter timeout in second ( < 600 ):" << endl;

    int timeout = 0;
    cin >> timeout;
    JUB_SetTimeOut(contextID, timeout);
}

void send_apdu_test() {

    JUB_UINT16 deviceIDs[MAX_DEVICE] = { 0xffff, };
    JUB_RV rv = JUB_ListDeviceHid(deviceIDs);
    if (JUBR_OK != rv) {
        cout << "JUB_ListDeviceHid() return " << GetErrMsg(rv) << endl;
    }

    rv = JUB_ConnetDeviceHid(deviceIDs[0]);
    if (JUBR_OK != rv) {
        cout << "JUB_ConnetDeviceHid() return " << GetErrMsg(rv) << endl;
    }

    JUB_UINT16 deviceID = deviceIDs[0];

    cout << "please input apdu in hex:" << endl;
    cout << "|------ example: select main safe scope ------|" << endl;
    cout << "|-- APDU apdu(0x00, 0xa4, 0x04, 0x00, 0x00) --|" << endl;
    char apdu[4096+6] = {0,};
    cin >> apdu;

    char* response = nullptr;
    rv = JUB_SendOneApdu(deviceID, apdu, &response);
    if (JUBR_OK != rv) {
        cout << "JUB_SendOneApdu() return " << GetErrMsg(rv) << endl;
        return;
    }

    cout << response << endl;
    JUB_FreeMemory(response);

    return;
}
