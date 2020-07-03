// JUB_SDK_test.cpp : Defines the entry point for the console application.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include "JUB_SDK_test.h"
#include "JUB_SDK_test_dev.hpp"
#include "JUB_SDK_test_btc.hpp"
#include "JUB_SDK_test_hcash.hpp"
#include "JUB_SDK_test_qtum.hpp"
#include "JUB_SDK_test_eth.hpp"
#include "JUB_SDK_test_eos.hpp"
#include "JUB_SDK_test_xrp.hpp"
#include "../../include/JUB_CORE.h"

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
    JUB_BYTE  fwVersion[5] = {0,};
    memcpy(bleVersion, info.bleVersion, 4);
    memcpy( fwVersion, info.firmwareVersion, 4);
    cout << "device bleVersion :" << bleVersion << endl;
    cout << "device  fwVersion :" <<  fwVersion << endl;

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
        cout << "-------------------------------------------" << endl;
        cout << "|********** Jubiter Wallet Test **********|" << endl;
        cout << "|   1. get_device_info_test               |" << endl;
        cout << "|   2. LTC_test.                          |" << endl;
        cout << "|   5.DASH_test.                          |" << endl;
        cout << "|  31. BTC_test.                          |" << endl;
        cout << "|  32. BTC_segwit_test.                   |" << endl;
        cout << "|  39. BTC_USDT_test.                     |" << endl;
        cout << "|  60. ETH_test & ETC_test.               |" << endl;
        cout << "| 144. XRP_test.                          |" << endl;
        cout << "| 145. BCH_test.                          |" << endl;
        cout << "| 171. Hcash_test.                        |" << endl;
        cout << "| 194. EOS_test.                          |" << endl;
        cout << "|  88.QTUM_QRC20_test.                    |" << endl;
        cout << "|2301.QTUM_test.                          |" << endl;
        cout << "|  98. send_apdu_test.                    |" << endl;
        cout << "|  99. get_version.                       |" << endl;
        cout << "|   0. exit.                              |" << endl;
        cout << "-------------------------------------------" << endl;
        cout << "* Please enter your choice:" << endl;

        rv = JUB_DisconnetDeviceHid(deviceID);
        if (JUBR_OK != rv) {
            cout << "JUB_DisconnetDeviceHid() return " << GetErrMsg(rv) << endl;
        }

#ifdef TESTNET
        std::string json_file = "json_test/";
#else
        std::string json_file = "json/";
#endif

        int choice = 0;
        cin >> choice;

        switch (choice) {
        case 1:
            get_device_info_test();
            break;
        case 2:
            json_file += "testLTC.json";
            BTC_test(json_file.c_str(), COINLTC);
            break;
        case 5:
            json_file += "testDASH.json";
            BTC_test(json_file.c_str(), COINDASH);
            break;
        case 31:
            json_file += "testBTC44.json";
            BTC_test(json_file.c_str(), COINBTC);
            break;
        case 32:
            json_file += "testBTC49.json";
            BTC_test(json_file.c_str(), COINBTC);
            break;
        case 39:
            json_file += "testUSDT.json";
            USDT_test(json_file.c_str());
            break;
        case 60:
            json_file += "testETH.json";
            ETH_test(json_file.c_str());
            break;
        case 144:
            json_file += "testXRP.json";
            XRP_test(json_file.c_str());
            break;
        case 145:
            json_file += "testBCH.json";
            BTC_test(json_file.c_str(), COINBCH);
            break;
        case 171:
            json_file += "testHCash.json";
            HC_test(deviceID, json_file.c_str());
            break;
        case 194:
            json_file += "testEOS.json";
            EOS_test(json_file.c_str());
            break;
        case 88:
            json_file += "testQTUM_qrc20.json";
            QTUM_test(deviceID, json_file.c_str());
            break;
        case 2301:
            json_file += "testQTUM.json";
            BTC_test(json_file.c_str(), COINQTUM);
            break;
        case 98:
            send_apdu_test();
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
