// JUB_SDK_test.cpp : Defines the entry point for the console application.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include "../../include/JUB_core.h"
#include <vector>
#include <iostream>
#include <json/json.h>
#include <iostream>  
#include <fstream>  
#include <string>
#include <sstream>
#include <iostream>
#include <cstring>
#include <thread>
using namespace std;

using std::getline;
using std::istringstream;

std::string GetErrMsg(JUB_RV rv) {

    std::string errMsg;
    switch (rv) {
        case JUBR_OK:                       { errMsg = "JUBR_OK"; break; }
        case JUBR_ERROR:                    { errMsg = "JUBR_ERROR"; break; }
        case JUBR_HOST_MEMORY:              { errMsg = "JUBR_HOST_MEMORY"; break; }
        case JUBR_ARGUMENTS_BAD:            { errMsg = "JUBR_ARGUMENTS_BAD"; break; }
        case JUBR_IMPL_NOT_SUPPORT:         { errMsg = "JUBR_IMPL_NOT_SUPPORT"; break; }
        case JUBR_MEMORY_NULL_PTR:          { errMsg = "JUBR_MEMORY_NULL_PTR"; break; }
        case JUBR_DATABASE_ERROR:           { errMsg = "JUBR_DATABASE_ERROR"; break; }
        case JUBR_DATABASE_EMPTY:           { errMsg = "JUBR_DATABASE_EMPTY"; break; }
        case JUBR_INVALID_MEMORY_PTR:       { errMsg = "JUBR_INVALID_MEMORY_PTR"; break; }
        case JUBR_REPEAT_MEMORY_PTR:        { errMsg = "JUBR_REPEAT_MEMORY_PTR"; break; }
        case JUBR_BUFFER_TOO_SMALL:         { errMsg = "JUBR_BUFFER_TOO_SMALL"; break; }

        case JUBR_COIN_TYPE_ALREADY_EXIST:  { errMsg = "JUBR_COIN_TYPE_ALREADY_EXIST"; break; }
        case JUBR_COIN_TYPE_NOT_EXIST:      { errMsg = "JUBR_COIN_TYPE_NOT_EXIST"; break; }
        case JUBR_ADD_COIN_TYPE_ERROR:      { errMsg = "JUBR_ADD_COIN_TYPE_ERROR"; break; }
        case JUBR_UNKNOWN_COIN_TYPE:        { errMsg = "JUBR_UNKNOWN_COIN_TYPE"; break; }

        case JUBR_WALLET_ALREADY_EXIST:     { errMsg = "JUBR_WALLET_ALREADY_EXIST"; break; }
        case JUBR_CREATE_WALLET_ERROR:      { errMsg = "JUBR_CREATE_WALLET_ERROR"; break; }
        case JUBR_WALLET_NOT_EXIST:         { errMsg = "JUBR_WALLET_NOT_EXIST"; break; }
        case JUBR_WALLET_LOADED:            { errMsg = "JUBR_WALLET_LOADED"; break; }
        case JUBR_WALLET_NOT_LOAD:          { errMsg = "JUBR_WALLET_NOT_LOAD"; break; }
        case JUBR_WALLETS_EMPTY:            { errMsg = "JUBR_WALLETS_EMPTY"; break; }

        case JUBR_INVALID_BIP_PATH:         { errMsg = "JUBR_INVALID_BIP_PATH"; break; }
        case JUBR_INVALID_MNEMONIC:         { errMsg = "JUBR_INVALID_MNEMONIC"; break; }
        case JUBR_INVALID_ADDRESS:          { errMsg = "JUBR_INVALID_ADDRESS"; break; }
        case JUBR_INSUFFICIENT_COINS:       { errMsg = "JUBR_INSUFFICIENT_COINS"; break; }
        case JUBR_TOO_MANY_INPUTS:          { errMsg = "JUBR_TOO_MANY_INPUTS"; break; }
        case JUBR_NOT_SUPPORT_INPUTS:       { errMsg = "JUBR_NOT_SUPPORT_INPUTS"; break; }
        case JUBR_INVAIDE_TRANSACTION:      { errMsg = "JUBR_INVAIDE_TRANSACTION"; break; }
        case JUBR_NO_REDEEMSCRIPT:          { errMsg = "JUBR_NO_REDEEMSCRIPT"; break; }

        case JUBR_VERIFY_PIN_ERROR:         { errMsg = "JUBR_VERIFY_PIN_ERROR"; break; }
        case JUBR_CHANGE_PIN_ERROR:         { errMsg = "JUBR_CHANGE_PIN_ERROR"; break; }
        case JUBR_PIN_NOT_SET:              { errMsg = "JUBR_PIN_NOT_SET"; break; }

        case JUBR_INIT_DEVICE_LIB_ERROR:    { errMsg = "JUBR_INIT_DEVICE_LIB_ERROR"; break; }
        case JUBR_CONNECT_DEVICE_ERROR:     { errMsg = "JUBR_CONNECT_DEVICE_ERROR"; break; }
        case JUBR_TRANSMIT_DEVICE_ERROR:    { errMsg = "JUBR_TRANSMIT_DEVICE_ERROR"; break; }
        case JUBR_NOT_CONNECT_DEVICE:       { errMsg = "JUBR_NOT_CONNECT_DEVICE"; break; }
        case JUBR_DEVICE_PIN_ERROR:         { errMsg = "JUBR_DEVICE_PIN_ERROR"; break; }
        case JUBR_USER_CANCEL:              { errMsg = "JUBR_USER_CANCEL"; break; }
        case JUBR_ERROR_ARGS:               { errMsg = "JUBR_ERROR_ARGS"; break; }
        case JUBR_PIN_LOCKED:               { errMsg = "JUBR_PIN_LOCKED"; break; }

        case JUBR_ACCT_SYNC_DATA_FINISH:    { errMsg = "JUBR_ACCT_SYNC_DATA_FINISH"; break; }

        case JUBR_DEVICE_BUSY:              { errMsg = "JUBR_DEVICE_BUSY"; break; }
        case JUBR_TRANSACT_TIMEOUT:         { errMsg = "JUBR_TRANSACT_TIMEOUT"; break; }
        case JUBR_OTHER_ERROR:              { errMsg = "JUBR_OTHER_ERROR"; break; }
        case JUBR_CMD_ERROR:                { errMsg = "JUBR_CMD_ERROR"; break; }
        case JUBR_BT_BOND_FAILED:           { errMsg = "JUBR_BT_BOND_FAILED"; break; }
        case JUBR_CUSTOM_DEFINED:           { errMsg = "JUBR_CUSTOM_DEFINED"; break; }
        default:                            { errMsg = "UNKNOWN ERROR."; break; }
    }

    return errMsg;
}

vector<string> split(const string &str, char delim, bool bSkipEmpty = true) {

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

void error_exit(const char* message) {

    cout << message << endl;
    cout << "press any key to exit" << endl;
    char str[9] = { 0, };
    cin >> str;
    exit(0);
}

void main_test();

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
    rv = Jub_EnumSupportCoins(deviceID, &coinList);
    if (JUBR_OK != rv) {
        cout << "Jub_EnumSupportCoins() return " << GetErrMsg(rv) << endl;
        return;
    }

    cout << "support coin list is :" << coinList << endl;
    JUB_FreeMemory(coinList);
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

JUB_RV verify_pin(JUB_UINT16 contextID) {

    JUB_RV rv = JUBR_ERROR;
    while (rv) {
        //����pin��λ�ã�������123456789
        cout << "1 2 3" << endl;
        cout << "4 5 6" << endl;
        cout << "7 8 9" << endl;

        cout << "to cancel the virtualpwd iput 'c'" << endl;
        rv = JUB_ShowVirtualPwd(contextID);
        if (JUBR_OK != rv) {
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

void show_address_test(JUB_UINT16 contextID) {

    int change = 0;
    JUB_UINT64 index = 0;
    cout << "please input change level (non-zero means 1):" << endl;
    cin >> change;
    cout << "please input index " << endl;
    cin >> index;

    BIP32_Path path;
    path.change = JUB_ENUM_BOOL(change);
    path.addressIndex = index;

    JUB_CHAR_PTR address;
    JUB_RV rv = JUB_GetAddressBTC(contextID, path, BOOL_TRUE, &address);
    if (JUBR_OK != rv) {
        cout << "JUB_GetAddressBTC() return " << GetErrMsg(rv) << endl;
        return;
    }
    cout << "show address is : " << address << endl;

    JUB_FreeMemory(address);
}

void set_my_address_test_BTC(JUB_UINT16 contextID) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    int change = 0;
    JUB_UINT64 index = 0;
    cout << "please input change level (non-zero means 1):" << endl;
    cin >> change;
    cout << "please input index " << endl;
    cin >> index;

    BIP32_Path path;
    path.change = JUB_ENUM_BOOL(change);
    path.addressIndex = index;

    JUB_CHAR_PTR address = nullptr;
    rv = JUB_SetMyAddressBTC(contextID, path, &address);
    if (JUBR_OK != rv) {
        cout << "JUB_SetMyAddressBTC() return " << GetErrMsg(rv) << endl;
        return;
    }
    else {
        cout << "set my address is : " << address << endl;
        JUB_FreeMemory(address);
    }
}

void set_my_address_test_ETH(JUB_UINT16 contextID) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    int change = 0;
    JUB_UINT64 index = 0;
    cout << "please input change level (non-zero means 1):" << endl;
    cin >> change;
    cout << "please input index " << endl;
    cin >> index;

    BIP32_Path path;
    path.change = JUB_ENUM_BOOL(change);
    path.addressIndex = index;

    JUB_CHAR_PTR address = nullptr;
    rv = JUB_SetMyAddressETH(contextID, path, &address);
    if (JUBR_OK != rv) {
        cout << "JUB_SetMyAddressETH() return " << GetErrMsg(rv) << endl;
        return;
    }
    else {
        cout << "set my address is : " << address << endl;
        JUB_FreeMemory(address);
    }
}

void get_address_test(JUB_UINT16 contextID, Json::Value root) {

    try {
        JUB_CHAR_PTR mainXpub;
        JUB_RV rv = JUB_GetMainHDNodeBTC(contextID, &mainXpub);
        if (JUBR_OK != rv) {
            cout << "JUB_GetMainHDNodeBTC() return " << GetErrMsg(rv) << endl;
            return;
        }

        cout << "Main xpub : " << mainXpub << endl;
        JUB_FreeMemory(mainXpub);

        int inputNumber = root["inputs"].size();
        for (int i = 0; i < inputNumber; i++) {
            JUB_CHAR_PTR xpub;

            BIP32_Path path;
            path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
            path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();

            JUB_RV rv = JUB_GetHDNodeBTC(contextID, path, &xpub);
            if (JUBR_OK != rv) {
                cout << "JUB_GetHDNodeBTC() return " << GetErrMsg(rv) << endl;
                break;
            }

            cout << "input " << i << " xpub : " << xpub << endl;
            JUB_FreeMemory(xpub);

            JUB_CHAR_PTR address;
            rv = JUB_GetAddressBTC(contextID, path, BOOL_FALSE, &address);
            if (JUBR_OK != rv) {
                cout << "JUB_GetAddressBTC() return " << GetErrMsg(rv) << endl;
                break;
            }
            cout << "input " << i << " address : " << address << endl;
            JUB_FreeMemory(address);
        }
        if (JUBR_OK != rv) {
            return;
        }
    }
    catch (...) {
        error_exit("Error format json file\n");
    }
}

void transaction_test(JUB_UINT16 contextID, Json::Value root) {

    JUB_BTC_UNIT_TYPE unit = mBTC;

    cout << "Please input BTCunit on JubiterBLD" << endl;
    cout << "1: BTC" << endl;
    cout << "2: cBTC" << endl;
    cout << "3: mBTC" << endl;
    cout << "4: uBTC" << endl;
    cout << "5: Satoshi" << endl;

    int choice = 0;
    cin >> choice;

    switch (choice) {
        case 1:
        unit = BTC;
        break;
        case 2:
        unit = cBTC;
        break;
        case 3:
        unit = mBTC;
        break;
        case 4:
        unit = uBTC;
        break;
        case 5:
        unit = Satoshi;
        break;
    }

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    try {
        std::vector<INPUT_BTC> inputs;
        std::vector<OUTPUT_BTC> outputs;
        int inputNumber = root["inputs"].size();

        for (int i = 0; i < inputNumber; i++) {
            INPUT_BTC input;
            input.preHash = (char*)root["inputs"][i]["preHash"].asCString();
            input.preIndex = root["inputs"][i]["preIndex"].asInt();
            input.path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
            input.path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();
            input.amount = root["inputs"][i]["amount"].asUInt64();
            inputs.push_back(input);
        }

        int outputNumber = root["outputs"].size();

        for (int i = 0; i < outputNumber; i++) {
            OUTPUT_BTC output;
            output.type = OUTPUT_BTC_TYPE::P2PKH;
            output.outputP2pkh.address = (char*)root["outputs"][i]["address"].asCString();
            output.outputP2pkh.amount = root["outputs"][i]["amount"].asUInt64();
            output.outputP2pkh.changeAddress = (JUB_ENUM_BOOL)root["outputs"][i]["change_address"].asBool();
            if (output.outputP2pkh.changeAddress) {
                output.outputP2pkh.path.change = (JUB_ENUM_BOOL)root["outputs"][i]["bip32_path"]["change"].asBool();
                output.outputP2pkh.path.addressIndex = root["outputs"][i]["bip32_path"]["addressIndex"].asInt();
            }
            outputs.push_back(output);
        }

        rv = JUB_SetUnitBTC(contextID, unit);
        if (JUBR_OK != rv) {
            cout << "JUB_SetUnitBTC() return " << GetErrMsg(rv) << endl;
            return;
        }

        char* raw = nullptr;
        rv = JUB_SignTransactionBTC(contextID, &inputs[0], (JUB_UINT16)inputs.size(), &outputs[0], (JUB_UINT16)outputs.size(), 0, &raw);
        cout << "JUB_SignTransactionBTC() return " << GetErrMsg(rv) << endl;

        if (JUBR_USER_CANCEL == rv) {
            cout << "User cancel the transaction !" << endl;
            return;
        }
        if (   JUBR_OK != rv
            || nullptr == raw
        ) {
            cout << "error sign tx" << endl;
            return;
        }
        if (raw) {
            cout << raw;
            JUB_FreeMemory(raw);
        }
    }
    catch (...) {
        error_exit("Error format json file\n");
    }
}

void transactionUSDT_test(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    try {
        std::vector<INPUT_BTC> inputs;
        std::vector<OUTPUT_BTC> outputs;
        int inputNumber = root["inputs"].size();

        for (int i = 0; i < inputNumber; i++) {
            INPUT_BTC input;
            input.preHash = (char*)root["inputs"][i]["preHash"].asCString();
            input.preIndex = root["inputs"][i]["preIndex"].asInt();
            input.path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
            input.path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();
            input.amount = root["inputs"][i]["amount"].asUInt64();
            inputs.push_back(input);
        }

        int outputNumber = root["outputs"].size();

        for (int i = 0; i < outputNumber; i++) {
            OUTPUT_BTC output;
            output.type = OUTPUT_BTC_TYPE::P2PKH;
            output.outputP2pkh.address = (char*)root["outputs"][i]["address"].asCString();
            output.outputP2pkh.amount = root["outputs"][i]["amount"].asUInt64();
            output.outputP2pkh.changeAddress = (JUB_ENUM_BOOL)root["outputs"][i]["change_address"].asBool();
            if (output.outputP2pkh.changeAddress) {
                output.outputP2pkh.path.change = (JUB_ENUM_BOOL)root["outputs"][i]["bip32_path"]["change"].asBool();
                output.outputP2pkh.path.addressIndex = root["outputs"][i]["bip32_path"]["addressIndex"].asInt();
            }
            outputs.push_back(output);
        }

        OUTPUT_BTC USDT_outputs[2] = {};
        rv = JUB_BuildUSDTOutputs(contextID, (char*)root["USDT_to"].asCString(), root["USDT_amount"].asUInt64(), USDT_outputs);
        if (JUBR_OK != rv) {
            cout << "JUB_BuildUSDTOutputs() return " << GetErrMsg(rv) << endl;
            return;
        }
        outputs.emplace_back(USDT_outputs[0]);
        outputs.emplace_back(USDT_outputs[1]);

        char* raw = nullptr;
        rv = JUB_SignTransactionBTC(contextID, &inputs[0], (JUB_UINT16)inputs.size(), &outputs[0], (JUB_UINT16)outputs.size(), 0, &raw);
        cout << "JUB_SignTransactionBTC() return " << GetErrMsg(rv) << endl;

        if (JUBR_USER_CANCEL == rv) {
            cout << "User cancel the transaction !" << endl;
            return;
        }
        if (   JUBR_OK != rv
            || nullptr == raw
        ) {
            cout << "error sign tx" << endl;
            return;
        }
        if (raw) {
            cout << raw;
            JUB_FreeMemory(raw);
        }
    }
    catch (...) {
        error_exit("Error format json file\n");
    }
}

void USDT_test(const char* json_file) {

    JUB_UINT16 deviceIDs[MAX_DEVICE] = { 0xffff, };
    JUB_ListDeviceHid(deviceIDs);

    JUB_RV rv = JUB_ConnetDeviceHid(deviceIDs[0]);
    if (JUBR_OK != rv) {
        cout << "JUB_ConnetDeviceHid() return " << GetErrMsg(rv) << endl;
        return;
    }

    char* appList;
    rv = JUB_EnumApplets(deviceIDs[0], &appList);
    if (JUBR_OK != rv) {
        cout << "JUB_EnumApplets() return " << GetErrMsg(rv) << endl;
        return;
    }

    Json::CharReaderBuilder builder;
    Json::Value root;
    ifstream in(json_file, ios::binary);
    if (!in.is_open()) {
        error_exit("Error opening json file\n");
    }
    JSONCPP_STRING errs;
    if (!parseFromStream(builder, in, &root, &errs)) {
        error_exit("Error parse json file\n");
    }
    JUB_UINT16 contextID = 0;

    try {
        CONTEXT_CONFIG_BTC cfg;
        cfg.mainPath = (char*)root["main_path"].asCString();
        cfg.coinType = COINUSDT;
        cfg.transType = p2pkh;

        rv = JUB_CreateContextBTC(cfg, deviceIDs[0], &contextID);
        if (JUBR_OK != rv) {
            cout << "JUB_CreateContextBTC() return " << GetErrMsg(rv) << endl;
            return;
        }
    }
    catch (...) {
        error_exit("Error format json file\n");
    }

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet USDT ********|" << endl;
        cout << "| 1. get_address_test.               |" << endl;
        cout << "| 2. show_address_test.              |" << endl;
        cout << "| 3. transaction_test.               |" << endl;
        cout << "| 4. set_my_address_test.            |" << endl;
        cout << "| 5. set_timeout_test.               |" << endl;
        cout << "| 0. return.                         |" << endl;
        cout << "--------------------------------------" << endl;
        cout << "* Please enter your choice:" << endl;

        int choice = 0;
        cin >> choice;

        switch (choice) {
        case 1:
            get_address_test(contextID, root);
            break;
        case 2:
            show_address_test(contextID);
            break;
        case 3:
            transactionUSDT_test(contextID, root);
            break;
        case 4:
            set_my_address_test_BTC(contextID);
            break;
        case 5:
            set_timeout_test(contextID);
            break;
        case 0:
            main_test();
        default:
            continue;
        }
    }
}

void BTC_test(const char* json_file, JUB_ENUM_COINTYPE_BTC coinType) {

    JUB_UINT16 deviceIDs[MAX_DEVICE] = { 0xffff, };
    JUB_ListDeviceHid(deviceIDs);

    JUB_RV rv = JUB_ConnetDeviceHid(deviceIDs[0]);
    if (JUBR_OK != rv) {
        cout << "JUB_ConnetDeviceHid() return " << GetErrMsg(rv) << endl;
        return;
    }

    char* appList;
    rv = JUB_EnumApplets(deviceIDs[0], &appList);
    if (JUBR_OK != rv) {
        cout << "JUB_EnumApplets() return " << GetErrMsg(rv) << endl;
        return;
    }

    Json::CharReaderBuilder builder;
    Json::Value root;
    ifstream in(json_file, ios::binary);
    if (!in.is_open()) {
        error_exit("Error opening json file\n");
    }
    JSONCPP_STRING errs;
    if (!parseFromStream(builder, in, &root, &errs)) {
        error_exit("Error parse json file\n");
    }
    JUB_UINT16 contextID = 0;

    try {
        CONTEXT_CONFIG_BTC cfg;
        cfg.mainPath = (char*)root["main_path"].asCString();
        cfg.coinType = coinType;

        if (COINBCH == coinType) {
            cfg.transType = p2pkh;
        }
        else {
            if (root["p2sh-segwit"].asBool()) {
                cfg.transType = p2sh_p2wpkh;
            }
            else {
                cfg.transType = p2pkh;
            }
        }

        rv = JUB_CreateContextBTC(cfg, deviceIDs[0], &contextID);
        if (JUBR_OK != rv) {
            cout << "JUB_CreateContextBTC() return " << GetErrMsg(rv) << endl;
            return;
        }
    }
    catch (...) {
        error_exit("Error format json file\n");
    }

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet BTC  ********|" << endl;
        cout << "| 1. get_address_test.               |" << endl;
        cout << "| 2. show_address_test.              |" << endl;
        cout << "| 3. transaction_test.               |" << endl;
        cout << "| 4. set_my_address_test.            |" << endl;
        cout << "| 5. set_timeout_test.               |" << endl;
        cout << "| 0. return.                         |" << endl;
        cout << "--------------------------------------" << endl;
        cout << "* Please enter your choice:" << endl;

        int choice = 0;
        cin >> choice;

        switch (choice) {
        case 1:
            get_address_test(contextID, root);
            break;
        case 2:
            show_address_test(contextID);
            break;
        case 3:
            transaction_test(contextID, root);
            break;
        case 4:
            set_my_address_test_BTC(contextID);
            break;
        case 5:
            set_timeout_test(contextID);
            break;
        case 0:
            main_test();
        default:
            continue;
        }
    }
}

void get_address_pubkey_ETH(JUB_UINT16 contextID) {

    int change = 0;
    JUB_UINT64 index = 0;
    cout << "please input change level (non-zero means 1):" << endl;
    cin >> change;
    cout << "please input index " << endl;
    cin >> index;

    BIP32_Path path;
    path.change = JUB_ENUM_BOOL(change);
    path.addressIndex = index;

    char* pubkey = nullptr;
    JUB_RV rv = JUB_GetMainHDNodeETH(contextID, HEX, &pubkey);
    if (JUBR_OK != rv) {
        cout << "JUB_GetMainHDNodeETH() return " << GetErrMsg(rv) << endl;
        return;
    }

    cout << "MainXpub in  hex format :  " << pubkey << endl;
    JUB_FreeMemory(pubkey);

    pubkey = nullptr;
    rv = JUB_GetMainHDNodeETH(contextID, XPUB, &pubkey);
    if (JUBR_OK != rv) {
        cout << "JUB_GetMainHDNodeETH() return " << GetErrMsg(rv) << endl;
        return;
    }

    cout << "MainXpub in  xpub format :  " << pubkey << endl;
    JUB_FreeMemory(pubkey);

    pubkey = nullptr;
    rv = JUB_GetHDNodeETH(contextID, HEX, path, &pubkey);
    if (JUBR_OK != rv) {
        cout << "JUB_GetHDNodeETH() return " << GetErrMsg(rv) << endl;
        return;
    }

    cout << "pubkey in  hex format :  "<< pubkey << endl;
    JUB_FreeMemory(pubkey);

    pubkey = nullptr;
    rv = JUB_GetHDNodeETH(contextID, XPUB, path, &pubkey);
    if (JUBR_OK != rv) {
        cout << "JUB_GetHDNodeETH() return " << GetErrMsg(rv) << endl;
        return;
    }

    cout << "pubkey in xpub format :  " << pubkey << endl;
    JUB_FreeMemory(pubkey);

    char* address = nullptr;
    rv = JUB_GetAddressETH(contextID, path, BOOL_TRUE, &address);
    if (JUBR_OK != rv) {
        cout << "JUB_GetAddressETH() return " << GetErrMsg(rv) << endl;
        return;
    }
    cout << "address: " << address << endl;
    JUB_FreeMemory(address);
}

void transaction_test_ETH(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    BIP32_Path path;
    path.change = (JUB_ENUM_BOOL)root["ETH"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["ETH"]["bip32_path"]["addressIndex"].asUInt();

    //ETH Test

	uint32_t nonce = root["ETH"]["nonce"].asUInt();//.asDouble();
	uint32_t gasLimit = root["ETH"]["gasLimit"].asUInt();//.asDouble();
    char* gasPriceInWei = (char*)root["ETH"]["gasPriceInWei"].asCString();
    char* valueInWei = (char*)root["ETH"]["valueInWei"].asCString();
    char* to = (char*)root["ETH"]["to"].asCString();
    char* data = (char*)root["ETH"]["data"].asCString();

    char* raw = nullptr;
    rv = JUB_SignTransactionETH(contextID, path, nonce, gasLimit, gasPriceInWei, to, valueInWei, data, &raw);
    if (JUBR_OK != rv) {
        cout << "JUB_SignTransactionETH() return " << GetErrMsg(rv) << endl;
        return;
    }
    else {
        cout << "raw : " << raw << endl;
        JUB_FreeMemory(raw);
    }
}

//ERC-20 Test
void transaction_ERC20_ETH(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

	uint32_t nonce = root["ERC20"]["nonce"].asUInt();//.asDouble();
	uint32_t gasLimit = root["ERC20"]["gasLimit"].asUInt();//.asDouble();
    char* gasPriceInWei = (char*)root["ERC20"]["gasPriceInWei"].asCString();
    char* to = (char*)root["ERC20"]["contract_address"].asCString();
    char* token_to = (char*)root["ERC20"]["token_to"].asCString();
    char* token_value = (char*)root["ERC20"]["token_value"].asCString();

    BIP32_Path path;
    path.change = (JUB_ENUM_BOOL)root["ERC20"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["ERC20"]["bip32_path"]["addressIndex"].asUInt();

    char* abi = nullptr;
    rv = JUB_BuildERC20AbiETH(contextID, token_to, token_value, &abi);
    if (JUBR_OK != rv) {
        cout << "JUB_BuildERC20AbiETH() return " << GetErrMsg(rv) << endl;
        return;
    }

    char* raw = nullptr;
    rv = JUB_SignTransactionETH(contextID, path, nonce, gasLimit, gasPriceInWei, to, nullptr, abi, &raw);

    JUB_FreeMemory(abi);
    if (JUBR_OK != rv) {
        cout << "JUB_SignTransactionETH() return " << GetErrMsg(rv) << endl;
        return;
    }
    else {
        cout << raw << endl;
        JUB_FreeMemory(raw);
    }
}

void ETH_test(const char* json_file) {

    JUB_UINT16 deviceIDs[MAX_DEVICE] = { 0xffff, };
    JUB_ListDeviceHid(deviceIDs);

    JUB_RV rv = JUB_ConnetDeviceHid(deviceIDs[0]);
    if (JUBR_OK != rv) {
        cout << "JUB_ConnetDeviceHid() return " << GetErrMsg(rv) << endl;
        return;
    }

    char* appList;
    rv = JUB_EnumApplets(deviceIDs[0], &appList);
    if (JUBR_OK != rv) {
        cout << "JUB_EnumApplets() return " << GetErrMsg(rv) << endl;
        return;
    }

    Json::CharReaderBuilder builder;
    Json::Value root;
    ifstream in(json_file, ios::binary);
    if (!in.is_open()) {
        error_exit("Error opening json file\n");
    }
    JSONCPP_STRING errs;
    if (!parseFromStream(builder, in, &root , &errs)) {
        error_exit("Error parse json file\n");
    }

    CONTEXT_CONFIG_ETH cfg;
    cfg.mainPath = (char*)root["main_path"].asCString();
    cfg.chainID = root["chainID"].asInt();
    JUB_UINT16 contextID = 0;
    rv = JUB_CreateContextETH(cfg, deviceIDs[0], &contextID);
    if (JUBR_OK != rv) {
        cout << "JUB_CreateContextETH() return " << GetErrMsg(rv) << endl;
        return;
    }

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet ETH  ********|" << endl;
        cout << "| 1. show_address_pubkey_test.       |" << endl;
        cout << "| 2. transaction_test.               |" << endl;
        cout << "| 3. transaction_ERC20_test.         |" << endl;
        cout << "| 4. set_my_address_test.            |" << endl;
        cout << "| 5. set_timeout_test.               |" << endl;
        cout << "| 0. return.                         |" << endl;
        cout << "--------------------------------------" << endl;
        cout << "* Please enter your choice:" << endl;

        int choice = 0;
        cin >> choice;

        switch (choice) {
        case 1:
            get_address_pubkey_ETH(contextID);
            break;
        case 2:
            transaction_test_ETH(contextID, root);
            break;
        case 3:
            transaction_ERC20_ETH(contextID, root);
            break;
        case 4:
            set_my_address_test_ETH(contextID);
            break;
        case 5:
            set_timeout_test(contextID);
            break;
        case 0:
            main_test();
        default:
            continue;
        }
    }
}

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

void software_test(){
    JUB_CHAR_PTR mnemonic = nullptr;
    
    JUB_RV rv = JUB_GenerateMnemonic(STRENGTH128,&mnemonic);
    if(rv == JUBR_OK){
        
        cout << mnemonic << endl;
    }
}

int main() {
    
    while(true){
            cout << "--------------------------------------" << endl;
            cout << "|******* Jubiter Wallet Test ********|" << endl;
            cout << "|  1. hardware_test.                 |" << endl;
            cout << "|  2. software_test.                 |" << endl;
            cout << "|  0. exit.                          |" << endl;
            cout << "--------------------------------------" << endl;
            cout << "* Please enter your choice:" << endl;


        int choice = 0;
        cin >> choice;
        switch (choice) {
            case 1:
                main_test();
                break;
            case 2:
                software_test();
                break;
            case 0:
                exit(0);
            default:
                continue;
        }
    }
    
    

    return 0;
}
