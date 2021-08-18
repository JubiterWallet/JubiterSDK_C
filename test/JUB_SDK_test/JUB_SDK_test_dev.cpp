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
#include "../../include/JUB_SDK_DEV_SWI.h"

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


void get_device_info_test(JUB_UINT16 deviceID) {

    JUB_RV rv = JUBR_ERROR;

    JUB_ENUM_COMMODE commode;
    JUB_ENUM_DEVICE deviceClass;
    rv = JUB_GetDeviceType(deviceID,
                           &commode, &deviceClass);
    cout << "[-] JUB_GetDeviceType() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }

    cout << "[--------------------------------- Device Info ---------------------------------]" << endl;
    JUB_DEVICE_INFO info;
    rv = JUB_GetDeviceInfo(deviceID, &info);
    cout << "[-] JUB_GetDeviceInfo() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }

    cout << "    device Label :" << info.label << endl;
    cout << "    device sn :" << info.sn << endl;
    cout << "    device pinRetry :" << info.pinRetry << endl;
    cout << "    device pinMaxRetry :" << info.pinMaxRetry << endl;
    JUB_BYTE bleVersion[5] = {0,};
    JUB_BYTE fwVersion[5] = {0,};
    memcpy(bleVersion, info.bleVersion, 4);
    memcpy(fwVersion, info.firmwareVersion, 4);
    cout << "    device bleVersion :" << bleVersion << endl;
    cout << "    device fwVersion :" << fwVersion << endl;

    JUB_CHAR_PTR coinList;
    rv = JUB_EnumSupportCoins(deviceID, &coinList);
    cout << "[-] JUB_EnumSupportCoins() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }

    cout << "    support coin list is:" << coinList << endl;
    JUB_FreeMemory(coinList);
    cout << endl;

    JUB_CHAR_PTR cert;
    rv = JUB_GetDeviceCert(deviceID, &cert);
    cout << "[-] JUB_GetDeviceCert() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "    device cert is :" << cert << endl << endl;;

    JUB_CHAR_PTR sn;
    JUB_CHAR_PTR subjectID;
    rv = JUB_ParseDeviceCert(cert, &sn, &subjectID);
    cout << "[-] JUB_ParseDeviceCert() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "    device cert sn is :" << sn << endl;
    cout << "    device cert subject ID is :" << subjectID << endl;

    JUB_FreeMemory(sn);
    JUB_FreeMemory(subjectID);

    JUB_FreeMemory(cert);
    cout << "[------------------------------- Device Info end -------------------------------]" << endl;
    cout << endl << endl;
}


JUB_RV verify_pin(JUB_UINT16 contextID) {

    JUB_RV rv = JUBR_ERROR;

    JUB_ENUM_DEVICE deviceClass;
    rv = JUB_GetContextType(contextID, &deviceClass);
    if (JUBR_OK != rv) {
        return rv;
    }

    switch (deviceClass) {
    case JUB_ENUM_DEVICE::BLADE:
    case JUB_ENUM_DEVICE::BIO:
    case JUB_ENUM_DEVICE::LITE:
        break;
    case JUB_ENUM_DEVICE::VD:
        return rv;
    default:
        return JUBR_ARGUMENTS_BAD;
    }

    while (1) {
        //����pin��λ�ã�������123456789
        cout << "1 2 3" << endl;
        cout << "4 5 6" << endl;
        cout << "7 8 9" << endl;

        cout << "to cancel the virtualpwd iput 'c'" << endl;
        rv = JUB_ShowVirtualPwd(contextID);
        cout << "[-] JUB_ShowVirtualPwd() return " << GetErrMsg(rv) << endl;
        if (   JUBR_OK               != rv
            && JUBR_IMPL_NOT_SUPPORT != rv
            ) {
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
            cout << "[-] JUB_CancelVirtualPwd() return " << GetErrMsg(rv) << endl;
            if (JUBR_OK != rv) {
                break;
            }
            return rv;
        }

        JUB_ULONG retry;
        rv = JUB_VerifyPIN(contextID, str, &retry);
        cout << "[-] JUB_VerifyPIN(retry=" << retry << ") return " << GetErrMsg(rv) << endl;
        if (JUBR_OK == rv
            ||   0 == retry
            ) {
            break;
        }
        cout << endl;
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
    cout << "[-] JUB_VerifyFingerprint() return " << GetErrMsg(rv) << endl;

    return rv;
}

void set_timeout_test(JUB_UINT16 contextID) {

    cout << "* Please enter timeout in second ( < 600 ):" << endl;

    int timeout = 0;
    cin >> timeout;
    JUB_RV rv = JUB_SetTimeOut(contextID, timeout);
    cout << "[-] JUB_SetTimeOut() return " << GetErrMsg(rv) << endl;
}

void send_apdu_test(JUB_UINT16 deviceID) {

    JUB_RV rv = JUBR_ERROR;

    cout << "please input apdu in hex:" << endl;
    cout << "|------ example: select main safe scope ------|" << endl;
    cout << "|-- APDU apdu(0x00, 0xa4, 0x04, 0x00, 0x00) --|" << endl;
    char apdu[4096+6] = {0,};
    cin >> apdu;

    JUB_CHAR_PTR response = nullptr;
    rv = JUB_SendOneApdu(deviceID, apdu, &response);
    cout << "[-] JUB_SendOneApdu() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }

    cout << response << endl;
    JUB_FreeMemory(response);

    return;
}
