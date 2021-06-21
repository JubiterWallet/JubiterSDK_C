// JUB_SDK_test.cpp : Defines the entry point for the console application.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include "JUB_SDK_test.h"
#include "JUB_SDK_test_dev.hpp"
#include "JUB_SDK_test_dev_bio.hpp"
#include "JUB_SDK_test_btc.hpp"
#include "JUB_SDK_test_hcash.hpp"
#include "JUB_SDK_test_qtum.hpp"
#include "JUB_SDK_test_eth.hpp"
#include "JUB_SDK_test_eos.hpp"
#include "JUB_SDK_test_xrp.hpp"
#include "JUB_SDK_test_trx.hpp"
#include "JUB_SDK_test_fil.hpp"
#include "../../include/JUB_CORE.h"

#include "JUB_SDK_main.h"

using std::getline;
using std::istringstream;

void getVersion(JUB_UINT16 deviceID) {

    JUB_RV rv = JUBR_ERROR;

    cout << "[--------------------------------- Device Info ---------------------------------]" << endl;
    JUB_DEVICE_INFO info;
    rv = JUB_GetDeviceInfo(deviceID, &info);
    cout << "[-] JUB_GetDeviceInfo() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }

    JUB_BYTE bleVersion[5] = {0,};
    JUB_BYTE  fwVersion[5] = {0,};
    memcpy(bleVersion, info.bleVersion, 4);
    memcpy( fwVersion, info.firmwareVersion, 4);
    cout << "    device bleVersion :" << bleVersion << endl;
    cout << "    device  fwVersion :" <<  fwVersion << endl;
    cout << "[------------------------------- Device Info end -------------------------------]" << endl;
    cout << endl << endl;

    cout << "[-------------------------------- Applet Version -------------------------------]" << endl;
    char* appList;
    rv = JUB_EnumApplets(deviceID, &appList);
    cout << "[-] JUB_EnumApplets() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    std::string appletList = appList;
    JUB_FreeMemory(appList);

    auto vAppList = split(appletList, ' ');
    for (auto appID : vAppList) {
        JUB_VERSION version;
        auto rv = JUB_GetAppletVersion(deviceID, (JUB_CHAR_PTR)appID.c_str(), &version);
        cout << "[-] JUB_GetAppletVersion() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            break;
        }
        else {
            cout << appID << "    Applet Version : " << version.major << "." << version.minor << "." << version.patch << endl;
        }
    }
    cout << "[------------------------------ Applet Version end -----------------------------]" << endl;
    cout << endl << endl;

    cout << "[--------------------------------- SDK Version ---------------------------------]" << endl;
    cout <<"    SDK Version:"<< JUB_GetVersion() << endl;
    cout << "[------------------------------- SDK Version end -------------------------------]" << endl;
    cout << endl << endl;
}


void main_test() {

    JUB_RV rv = JUBR_ERROR;

    int choice = 0;
    JUB_ENUM_COMMODE commode = JUB_ENUM_COMMODE::COMMODE_NS_ITEM;
    cout << "-------------------------------------------" << endl;
    cout << "|********** Jubiter Wallet Test **********|" << endl;
    cout << "| *. device test via HID                  |" << endl;
    cout << "|                                         |" << endl;
    cout << "|  ------------------------------------   |" << endl;
    cout << "| | gRPC config file (ip_bridge.info): |  |" << endl;
    cout << "| | ip_bridge.info example:            |  |" << endl;
    cout << "| | 192.168.17.60:5001                 |  |" << endl;
    cout << "|   -----------------------------------   |" << endl;
    cout << "| 2. virtual JuBiter Blade test           |" << endl;
    cout << "| 3. virtual JuBiter Bio   test           |" << endl;
    cout << "| 4. virtual JuBiter Lite  test           |" << endl;
    cout << "|                                         |" << endl;
    cout << "| 0.  exit.                               |" << endl;
    cout << "-------------------------------------------" << endl;
    cout << "* Please enter your choice:" << endl;

    cin >> choice;

    JUB_UINT16 deviceIDs[MAX_DEVICE] = { 0xffff, };
    JUB_UINT16 deviceID = 0;
    switch (choice) {
    case 0:
        exit(0);
        break;
    case 2:
    case 3:
    {
        commode = JUB_ENUM_COMMODE::SIM;
        rv = JUB_ListDeviceSIM("ip_bridge.info", (JUB_ENUM_DEVICE)choice, deviceIDs);
        cout << "JUB_ListDeviceSIM() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            return;
        }
        deviceID = deviceIDs[0];
        rv = JUB_ConnetDeviceSIM(deviceID);
        cout << "JUB_ConnetDeviceSIM() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            return;
        }
        break;
    }
    case 4:
    {
        commode = JUB_ENUM_COMMODE::SIM;
        LITE_DEVICE_INIT_PARAM param;
        Json::Value root = readJSON("settings/42584E46433230303532353030303031_apk.settings");
        param.crt = (JUB_CHAR_PTR)root["SCP11c"]["OCE"][1][0].asCString();
        param.sk  = (JUB_CHAR_PTR)root["SCP11c"]["OCE"][1][2].asCString();
        param.hostID = (JUB_CHAR_PTR)root["SCP11c"]["HostID"].asCString();
        param.keyLength = root["SCP11c"]["KeyLength"].asUInt();

        rv = JUB_ListLiteSIM("ip_bridge.info", (JUB_ENUM_DEVICE)choice, param, deviceIDs);
        cout << "JUB_ListDeviceSIM() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            return;
        }
        deviceID = deviceIDs[0];
        rv = JUB_ConnetDeviceSIM(deviceID);
        cout << "JUB_ConnetDeviceSIM() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            return;
        }
        break;
    }
    default:
    {
        cout << "[----------------------------- HID device connection ---------------------------]" << endl;
        commode = JUB_ENUM_COMMODE::HID;
        rv = JUB_ListDeviceHid(deviceIDs);
        cout << "[-] JUB_ListDeviceHid() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            return;
        }
        cout << endl;

        deviceID = deviceIDs[0];
        rv = JUB_ConnetDeviceHid(deviceID);
        cout << "[-] JUB_ConnetDeviceHid() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            return;
        }
        cout << "[--------------------------- HID device connection end -------------------------]" << endl;
        cout << endl << endl;
        break;
    }
    }

    while (true) {
        cout << "-------------------------------------------" << endl;
        cout << "|********** Jubiter Wallet Test **********|" << endl;
        cout << "|   1. get_device_info_test               |" << endl;
        cout << "|  11.     device_fgpt_test               |" << endl;
        cout << "|                                         |" << endl;
        cout << "|   2.   LTC_test.                        |" << endl;
        cout << "|   5.  DASH_test.                        |" << endl;
        cout << "|  30.   BTCTN_test.                      |" << endl;
        cout << "|  31.   BTC_test.                        |" << endl;
        cout << "|  32.   BTC_segwit_test.                 |" << endl;
        cout << "|  33.   BTC_segwitTN_test.               |" << endl;
        cout << "|  39.   BTC_USDT_test.                   |" << endl;
        cout << "|  88.  QTUM_QRC20_test.                  |" << endl;
        cout << "|2301.  QTUM_test.                        |" << endl;
        cout << "| 145.   BCH_test.                        |" << endl;
        cout << "| 171. Hcash_test.                        |" << endl;
        cout << "|                                         |" << endl;
        cout << "|  60.   ETH_test & ETC_test.             |" << endl;
        cout << "| 461.   FIL_test.                        |" << endl;
        cout << "|                                         |" << endl;
        cout << "| 144.   XRP_test.                        |" << endl;
        cout << "| 194.   EOS_test.                        |" << endl;
        cout << "| 195.   TRX_test.                        |" << endl;
        cout << "|                                         |" << endl;
        cout << "|  98.  send_apdu_test.                   |" << endl;
        cout << "|  99.  get_version.                      |" << endl;
        cout << "|                                         |" << endl;
        cout << "|   0.  exit.                             |" << endl;
        cout << "-------------------------------------------" << endl;
        cout << "* Please enter your choice:" << endl;

//        rv = JUB_DisconnetDeviceHid(deviceID);
//        if (JUBR_OK != rv) {
//            cout << "JUB_DisconnetDeviceHid() return " << GetErrMsg(rv) << endl;
//        }

        std::string json_file = "json/";

        cin >> choice;

        switch (choice) {
        case 1:
            get_device_info_test(deviceID);
            break;
        case 11:
            device_fgpt_test(deviceID);
            break;

        case 2:
            json_file += "testLTC.json";
            BTC_test(deviceID, json_file.c_str(), COINLTC);
            break;
        case 5:
            json_file += "testDASH.json";
            BTC_test(deviceID, json_file.c_str(), COINDASH);
            break;
        case 30:
            json_file += "testBTCTN44.json";
            BTC_test(deviceID, json_file.c_str(), COINBTC);
            break;
        case 31:
            json_file += "testBTC44.json";
            BTC_test(deviceID, json_file.c_str(), COINBTC);
            break;
        case 32:
            json_file += "testBTC49.json";
            BTC_test(deviceID, json_file.c_str(), COINBTC);
            break;
        case 33:
            json_file += "testBTCTN49.json";
            BTC_test(deviceID, json_file.c_str(), COINBTC);
            break;
        case 39:
            json_file += "testUSDT.json";
            USDT_test(deviceID, json_file.c_str());
            break;
        case 88:
            json_file += "testQTUM_qrc20.json";
            QTUM_test(deviceID, json_file.c_str());
            break;
        case 2301:
            json_file += "testQTUM.json";
            BTC_test(deviceID, json_file.c_str(), COINQTUM);
            break;
        case 145:
            json_file += "testBCH.json";
            BTC_test(deviceID, json_file.c_str(), COINBCH);
            break;
        case 171:
            json_file += "testHCash.json";
            HC_test(deviceID, json_file.c_str());
            break;

        case 60:
            json_file += "testETH.json";
            ETH_test(deviceID, json_file.c_str());
            break;
        case 461:
            json_file += "testFIL.json";
            FIL_test(json_file.c_str());
            break;

        case 144:
            json_file += "testXRP.json";
            XRP_test(deviceID, json_file.c_str());
            break;
        case 194:
            json_file += "testEOS.json";
            EOS_test(deviceID, json_file.c_str());
            break;
        case 195:
            json_file += "testTRX.json";
            TRX_test(deviceID, json_file.c_str());
            break;

        case 98:
            send_apdu_test(deviceID);
            break;
        case 99:
            getVersion(deviceID);
            break;
        case 0:
            exit(0);
        default:
            continue;
        }   // switch (choice) end

        switch (commode) {
        case JUB_ENUM_COMMODE::HID:
        {
            cout << "[---------------------------- HID device disconnection -------------------------]" << endl;
            rv = JUB_DisconnetDeviceHid(deviceID);
            cout << "[-] JUB_DisconnetDeviceHid() return " << GetErrMsg(rv) << endl;
            cout << "[------------------------- HID device disconnection end ------------------------]" << endl;
            cout << endl << endl;
            break;
        }
        case JUB_ENUM_COMMODE::SIM:
        {
            rv = JUB_DisconnetDeviceSIM(deviceID);
            cout << "JUB_DisconnetDeviceSIM() return " << GetErrMsg(rv) << endl;
            break;
        }
        default:
            return;
        }
    }   // while (true) end
}

void monitor_test() {

    while (true) {
        JUB_UINT16 deviceIDs[MAX_DEVICE] = {0,};
        fill_n(deviceIDs, MAX_DEVICE, 0xffff);
        auto count = 0;
        JUB_RV rv = JUB_ListDeviceHid(deviceIDs);
        cout << "[-] JUB_ListDeviceHid() return " << std::hex << rv << endl;
        for (auto id : deviceIDs) {
            if (id != 0xffff) {
                count++;
            }
        }
        cout << count << endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
