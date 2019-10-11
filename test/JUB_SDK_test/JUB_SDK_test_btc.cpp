//
//  JUB_SDK_test_btc.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/9/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
#include "JUB_SDK_test_dev.hpp"
#include "JUB_SDK_test_btc.hpp"

#include "JUB_SDK_main.h"

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

            BIP44_Path path;
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

void show_address_test(JUB_UINT16 contextID) {

    int change = 0;
    JUB_UINT64 index = 0;
    cout << "please input change level (non-zero means 1):" << endl;
    cin >> change;
    cout << "please input index " << endl;
    cin >> index;

    BIP44_Path path;
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

    BIP44_Path path;
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

void transaction_test(JUB_UINT16 contextID, Json::Value root) {

    JUB_ENUM_BTC_UNIT_TYPE unit = mBTC;

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
            output.type = OUTPUT_ENUM_BTC_TYPE::STANDARD;
            output.outputStandard.address = (char*)root["outputs"][i]["address"].asCString();
            output.outputStandard.amount = root["outputs"][i]["amount"].asUInt64();
            output.outputStandard.changeAddress = (JUB_ENUM_BOOL)root["outputs"][i]["change_address"].asBool();
            if (output.outputStandard.changeAddress) {
                output.outputStandard.path.change = (JUB_ENUM_BOOL)root["outputs"][i]["bip32_path"]["change"].asBool();
                output.outputStandard.path.addressIndex = root["outputs"][i]["bip32_path"]["addressIndex"].asInt();
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
            output.type = OUTPUT_ENUM_BTC_TYPE::STANDARD;
            output.outputStandard.address = (char*)root["outputs"][i]["address"].asCString();
            output.outputStandard.amount = root["outputs"][i]["amount"].asUInt64();
            output.outputStandard.changeAddress = (JUB_ENUM_BOOL)root["outputs"][i]["change_address"].asBool();
            if (output.outputStandard.changeAddress) {
                output.outputStandard.path.change = (JUB_ENUM_BOOL)root["outputs"][i]["bip32_path"]["change"].asBool();
                output.outputStandard.path.addressIndex = root["outputs"][i]["bip32_path"]["addressIndex"].asInt();
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
