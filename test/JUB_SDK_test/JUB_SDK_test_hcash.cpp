//
//  JUB_SDK_test_hcash.cpp
//  JubSDKTest
//
//  Created by panmin on 2020/1/19.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
#include "JUB_SDK_test_dev.hpp"
//#include "JUB_SDK_test_btc.hpp"

#include "JUB_SDK_main.h"


void get_address_test_HC(JUB_UINT16 contextID, Json::Value root) {

    try {
        JUB_CHAR_PTR mainXpub;
        JUB_RV rv = JUB_GetMainHDNodeHC(contextID, &mainXpub);
        if (JUBR_OK != rv) {
            cout << "JUB_GetMainHDNodeHC() return " << GetErrMsg(rv) << endl;
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

            JUB_RV rv = JUB_GetHDNodeHC(contextID, path, &xpub);
            if (JUBR_OK != rv) {
                cout << "JUB_GetHDNodeHC() return " << GetErrMsg(rv) << endl;
                break;
            }

            cout << "input " << i << " xpub : " << xpub << endl;
            JUB_FreeMemory(xpub);

            JUB_CHAR_PTR address;
            rv = JUB_GetAddressHC(contextID, path, BOOL_FALSE, &address);
            if (JUBR_OK != rv) {
                cout << "JUB_GetAddressHC() return " << GetErrMsg(rv) << endl;
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


void show_address_test_HC(JUB_UINT16 contextID) {

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
    JUB_RV rv = JUB_GetAddressHC(contextID, path, BOOL_TRUE, &address);
    if (JUBR_OK != rv) {
        cout << "JUB_GetAddressHC() return " << GetErrMsg(rv) << endl;
        return;
    }
    cout << "show address is : " << address << endl;

    JUB_FreeMemory(address);
}


void transactionHC_test(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    try {
        std::vector<INPUT_HC> inputs;
        std::vector<OUTPUT_HC> outputs;
        int inputNumber = root["inputs"].size();

        for (int i = 0; i < inputNumber; i++) {
            INPUT_HC input;
            input.path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
            input.path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();
            input.amount = root["inputs"][i]["amount"].asUInt64();
            inputs.push_back(input);
        }

        int outputNumber = root["outputs"].size();

        for (int i = 0; i < outputNumber; i++) {
            OUTPUT_HC output;
            output.changeAddress = (JUB_ENUM_BOOL)root["outputs"][i]["change_address"].asBool();
            if (output.changeAddress) {
                output.path.change = (JUB_ENUM_BOOL)root["outputs"][i]["bip32_path"]["change"].asBool();
                output.path.addressIndex = root["outputs"][i]["bip32_path"]["addressIndex"].asInt();
            }
            outputs.push_back(output);
        }

        char* raw = nullptr;
        rv = JUB_SignTransactionHC(contextID, &inputs[0], (JUB_UINT16)inputs.size(), &outputs[0], (JUB_UINT16)outputs.size(), 0, &raw);
        cout << "JUB_SignTransactionHC() return " << GetErrMsg(rv) << endl;

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

void HC_test(JUB_UINT16 deviceID, const char* json_file) {

    JUB_RV rv = JUBR_ERROR;

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
        CONTEXT_CONFIG_HC cfg;
        cfg.mainPath = (char*)root["main_path"].asCString();

        rv = JUB_CreateContextHC(cfg, deviceID, &contextID);
        if (JUBR_OK != rv) {
            cout << "JUB_CreateContextHC() return " << GetErrMsg(rv) << endl;
            return;
        }
    }
    catch (...) {
        error_exit("Error format json file\n");
    }

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet Hcash *******|" << endl;
        cout << "| 1. get_address_test.               |" << endl;
        cout << "| 2. show_address_test.              |" << endl;
        cout << "| 3. transaction_test.               |" << endl;
        cout << "| 5. set_timeout_test.               |" << endl;
        cout << "| 0. return.                         |" << endl;
        cout << "--------------------------------------" << endl;
        cout << "* Please enter your choice:" << endl;

        int choice = 0;
        cin >> choice;

        switch (choice) {
            case 1:
                get_address_test_HC(contextID, root);
                break;
            case 2:
                show_address_test_HC(contextID);
                break;
            case 3:
                transactionHC_test(contextID, root);
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