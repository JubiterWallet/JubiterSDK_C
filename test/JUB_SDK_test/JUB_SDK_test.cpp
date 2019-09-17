// JUB_SDK_test.cpp : Defines the entry point for the console application.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include "JUB_SDK_test.h"
#include "JUB_SDK_test_dev.hpp"
#include "JUB_SDK_test_btc.hpp"
#include "JUB_SDK_test_eth.hpp"
#include "../../include/JUB_core.h"

#include "JUB_SDK_main.h"

using std::getline;
using std::istringstream;

void getVersion() {

    cout << "~~~~~~~~~~~Device Version ~~~~~~~~~~~~~~" << endl;

    JUB_UINT16 deviceIDs[MAX_DEVICE] = { 0xffff, };
    JUB_ListDeviceHid(deviceIDs);

    JUB_RV rv = JUB_ConnetDeviceHid(deviceIDs[0]);
    if (JUBR_OK != rv) {
        cout << "JUB_ConnetDeviceHid() return " << GetErrMsg(rv) << endl;
        return;
    }

    JUB_UINT16 deviceID = deviceIDs[0];

    JUB_DEVICE_INFO info;
    rv = JUB_GetDeviceInfo(deviceID, &info);
    if (JUBR_OK != rv) {
        cout << "JUB_GetDeviceInfo() return " << GetErrMsg(rv) << endl;
        return;
    }

    JUB_BYTE bleVersion[5] = {0,};
    JUB_BYTE fwVersion[5] = {0,};
    memcpy(bleVersion, info.bleVersion, 4);
    memcpy(fwVersion, info.firmwareVersion, 4);
    cout << "device bleVersion :" << bleVersion << endl;
    cout << "device fwVersion :" << fwVersion << endl;

    cout << "~~~~~~~~~~~Applet Version ~~~~~~~~~~~~~~" << endl;

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
            break;
        }
        else {
            cout << appID << " version : " << version << endl;
        }
    }

    cout << "~~~~~~~~~~~SDK    Version ~~~~~~~~~~~~~~" << endl;

    cout <<"SDK Version:"<< JUB_GetVersion() << endl;
}

void main_test() {

    JUB_UINT16 deviceIDs[MAX_DEVICE] = { 0xffff, };
    JUB_RV rv = JUB_ListDeviceHid(deviceIDs);
    if (JUBR_OK != rv) {
        cout << "JUB_ListDeviceHid() return " << GetErrMsg(rv) << endl;
    }
    auto deviceID = deviceIDs[0];

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet Test ********|" << endl;
        cout << "|  1. get_device_info_test           |" << endl;
        cout << "|  2. send_one_apdu_test.            |" << endl;
        cout << "|  3. BTC_test.                      |" << endl;
        cout << "|  4. BTC_segwit_test.               |" << endl;
        cout << "|  5. BCH_test.                      |" << endl;
        cout << "|  6. LTC_test.                      |" << endl;
        cout << "|  7. USDT_test.                     |" << endl;
        cout << "|  8. ETH_test & ETC_test.           |" << endl;
        cout << "| 99. get_version.                   |" << endl;
        cout << "| 0. exit.                           |" << endl;
        cout << "--------------------------------------" << endl;
        cout << "* Please enter your choice:" << endl;

        rv = JUB_DisconnetDeviceHid(deviceID);
        if (JUBR_OK != rv) {
            cout << "JUB_DisconnetDeviceHid() return " << GetErrMsg(rv) << endl;
        }

        int choice = 0;
        cin >> choice;

        switch (choice) {
        case 1:
            get_device_info_test();
            break;
        case 2:
            send_apdu_test();
            break;
        case 3:
            BTC_test("json/testBTC44.json", COINBTC);
            break;
        case 4:
            BTC_test("json/testBTC49.json", COINBTC);
            break;
        case 5:
            BTC_test("json/testBCH.json", COINBCH);
            break;
        case 6:
            BTC_test("json/testLTC.json", COINLTC);
            break;
        case 7:
            USDT_test("json/testUSDT.json");
            break;
        case 8:
            ETH_test("json/testETH.json");
            break;
        case 99:
            getVersion();
            break;
        case 0:
            exit(0);
        default:
            continue;
        }
    }
}

void monitor_test() {

    while (true) {
        JUB_UINT16 deviceIDs[MAX_DEVICE] = {0,};
        fill_n(deviceIDs, MAX_DEVICE, 0xffff);
        auto count = 0;
        JUB_RV rv = JUB_ListDeviceHid(deviceIDs);
        cout << "JUB_ListDeviceHid() return " << std::hex << rv << endl;
        for (auto id : deviceIDs) {
            if (id != 0xffff) {
                count++;
            }
        }
        cout << count << endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
