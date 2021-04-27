//
//  JUB_SDK_test_hcash.cpp
//  JubSDKTest
//
//  Created by panmin on 2020/1/19.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
#include "JUB_SDK_test_dev.hpp"
#include "JUB_SDK_test_hcash.hpp"

#include "JUB_SDK_main.h"


void get_address_test_HC(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

    cout << "[----------------------------------- HD Node -----------------------------------]" << endl;
    JUB_CHAR_PTR pubkey = nullptr;
    rv = JUB_GetMainHDNodeHC(contextID, &pubkey);
    cout << "[-] JUB_GetMainHDNodeHC() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "MainXpub in  HEX format:  " << pubkey << endl;
    JUB_FreeMemory(pubkey);
    cout << endl;

    int inputNumber = root["inputs"].size();
    for (int i = 0; i < inputNumber; i++) {
        JUB_CHAR_PTR xpub;

        BIP44_Path path;
        path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
        path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();

        JUB_RV rv = JUB_GetHDNodeHC(contextID, path, &xpub);
        cout << "[-] JUB_GetHDNodeHC() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            break;
        }
        cout << "    input " << i << " xpub : " << xpub << endl;
        JUB_FreeMemory(xpub);
        cout << endl;

        cout << "[----------------------------------- Address -----------------------------------]" << endl;
        JUB_CHAR_PTR address = nullptr;
        rv = JUB_GetAddressHC(contextID, path, BOOL_FALSE, &address);
        cout << "[-] JUB_GetAddressHC() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            break;
        }
        cout << "    input " << i << " address : " << address << endl;
        JUB_FreeMemory(address);
        cout << "[--------------------------------- Address end ---------------------------------]" << endl;
        cout << endl << endl;
    }   // for (int i = 0; i < inputNumber; i++) end
    cout << "[--------------------------------- HD Node end ---------------------------------]" << endl;
    cout << endl << endl;

    if (JUBR_OK != rv) {
        return;
    }
}


void show_address_test_HC(JUB_UINT16 contextID) {

    JUB_RV rv = JUBR_ERROR;

    int change = 0;
    JUB_UINT64 index = 0;
    cout << "please input change level (non-zero means 1):" << endl;
    cin >> change;
    cout << "please input index " << endl;
    cin >> index;

    BIP44_Path path;
    path.change = JUB_ENUM_BOOL(change);
    path.addressIndex = index;

    cout << "[----------------------------------- Address -----------------------------------]" << endl;
    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressHC(contextID, path, BOOL_TRUE, &address);
    cout << "[-] JUB_GetAddressHC() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "    show address: " << address << endl;

    JUB_FreeMemory(address);
    cout << "[--------------------------------- Address end ---------------------------------]" << endl;
    cout << endl << endl;
}


void transactionHC_test(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    rv = transactionHC_proc(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}


JUB_RV transactionHC_proc(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

    JUB_UINT32 version = root["ver"].asInt();

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

    char* unsignedRaw = (char*)root["unsigned_tx"].asCString();
    //NSString* unsignedTx = [NSString stringWithUTF8String:(char*)root["unsigned_tx"].asCString()];

    char* raw = nullptr;
    rv = JUB_SignTransactionHC(contextID, version, &inputs[0], (JUB_UINT16)inputs.size(), &outputs[0], (JUB_UINT16)outputs.size(), unsignedRaw, &raw);
    cout << "[-] JUB_SignTransactionHC() return " << GetErrMsg(rv) << endl;

    if (JUBR_USER_CANCEL == rv) {
        cout << "User cancel the transaction !" << endl;
        return rv;
    }
    if (   JUBR_OK != rv
        || nullptr == raw
        ) {
        cout << "error sign tx" << endl;
        return rv;
    }
    if (raw) {
        cout << "    Hcash raw[" << strlen(raw) << "]: " << raw << endl;
        JUB_FreeMemory(raw);
    }

    return rv;
}


void HC_test(JUB_UINT16 deviceID, const char* json_file) {

    JUB_RV rv = JUBR_ERROR;

    Json::Value root = readJSON(json_file);
    if (root.empty()) {
        return ;
    }

    JUB_UINT16 contextID = 0;

    CONTEXT_CONFIG_HC cfg;
    cfg.mainPath = (char*)root["main_path"].asCString();

    rv = JUB_CreateContextHC(cfg, deviceID, &contextID);
    cout << "[-] JUB_CreateContextHC() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << endl;

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet Hcash *******|" << endl;
        cout << "| 1. get_address_test.               |" << endl;
        cout << "| 2. show_address_test.              |" << endl;
        cout << "|                                    |" << endl;
        cout << "| 3. transaction_test.               |" << endl;
        cout << "|                                    |" << endl;
        cout << "| 9. return.                         |" << endl;
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
        case 9:
            main_test();
        default:
            continue;
        }   // switch (choice) end
    }   // while (true) end
}
