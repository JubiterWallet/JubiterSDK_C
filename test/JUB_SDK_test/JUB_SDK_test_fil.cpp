//
//  JUB_SDK_test_fil.cpp
//  JubSDKTest
//
//  Created by panmin on 2020/11/13.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
#include "JUB_SDK_test_dev.hpp"
#include "JUB_SDK_test_fil.hpp"

#include "JUB_SDK_main.h"
#include <time.h>

void FIL_test(const char* json_file) {

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

    Json::Value root = readJSON(json_file);
    JUB_UINT16 contextID = 0;

    CONTEXT_CONFIG_XRP cfg;
    cfg.mainPath = (char*)root["main_path"].asCString();
    rv = JUB_CreateContextFIL(cfg, deviceIDs[0], &contextID);
    if (JUBR_OK != rv) {
        cout << "JUB_CreateContextFIL() return " << GetErrMsg(rv) << endl;
        return;
    }

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet FIL *********|" << endl;
        cout << "| 1. show_address_pubkey_test.       |" << endl;
        cout << "|                                    |" << endl;
        cout << "| 2. transaction_test.               |" << endl;
        cout << "|                                    |" << endl;
        cout << "| 3. set_my_address_test.            |" << endl;
        cout << "| 4. set_timeout_test.               |" << endl;
        cout << "| 9. return.                         |" << endl;
        cout << "--------------------------------------" << endl;
        cout << "* Please enter your choice:" << endl;

        int choice = 0;
        cin >> choice;

        switch (choice) {
        case 1:
            get_address_pubkey_FIL(contextID);
            break;
        case 2:
            transaction_test_FIL(contextID, root);
            break;
        case 3:
            set_my_address_test_FIL(contextID);
            break;
        case 4:
            set_timeout_test(contextID);
            break;
        case 9:
            main_test();
        default:
            continue;
        }   // switch (choice) end
    }   // while (true) end
}

void set_my_address_test_FIL(JUB_UINT16 contextID) {

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
    rv = JUB_SetMyAddressFIL(contextID, path, &address);
    if (JUBR_OK != rv) {
        cout << "JUB_SetMyAddressFIL() return " << GetErrMsg(rv) << endl;
        return;
    }
    else {
        cout << "set my address is : " << address << endl;
        JUB_FreeMemory(address);
    }
}

void get_address_pubkey_FIL(JUB_UINT16 contextID) {
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

    char* pubkey = nullptr;
    rv = JUB_GetMainHDNodeFIL(contextID, JUB_ENUM_PUB_FORMAT::HEX, &pubkey);
    if (JUBR_OK != rv) {
        cout << "JUB_GetMainHDNodeFIL() return " << GetErrMsg(rv) << endl;
        return;
    }

    cout << "MainXpub in hex format :  " << pubkey << endl;
    JUB_FreeMemory(pubkey);

    pubkey = nullptr;
    rv = JUB_GetHDNodeFIL(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &pubkey);
    if (JUBR_OK != rv) {
        cout << "JUB_GetHDNodeFIL() return " << GetErrMsg(rv) << endl;
        return;
    }

    cout << "pubkey in hex format :  " << pubkey << endl;
    JUB_FreeMemory(pubkey);

    char* address = nullptr;
    rv = JUB_GetAddressFIL(contextID, path, BOOL_TRUE, &address);
    if (JUBR_OK != rv) {
        cout << "JUB_GetAddressFIL() return " << GetErrMsg(rv) << endl;
        return;
    }
    cout << "address: " << address << endl;
    JUB_FreeMemory(address);
}

void transaction_test_FIL(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    rv = transaction_proc_FIL(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}

JUB_RV transaction_proc_FIL(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["FIL"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["FIL"]["bip32_path"]["addressIndex"].asUInt();

    //FIL Test
    JUB_UINT64 nonce = root["FIL"]["nonce"].asUInt();//.asDouble();
    JUB_UINT64 gasLimit = root["FIL"]["gasLimit"].asUInt();//.asDouble();
    char* gasFeeCapInAtto  = (char*)root["FIL"]["gasFeeCapInAtto"].asCString();
    char* gasPremiumInAtto = (char*)root["FIL"]["gasPremiumInAtto"].asCString();
    char* valueInAtto = (char*)root["FIL"]["valueInAtto"].asCString();
    char* to = (char*)root["FIL"]["to"].asCString();

    char* cid = nullptr;
    rv = JUB_CalculateTransactionCIDFIL(contextID,
                                        path,
                                        nonce,
                                        gasLimit,
                                        gasFeeCapInAtto,
                                        gasPremiumInAtto,
                                        to,
                                        valueInAtto,
                                        "",
                                        &cid);
    if (JUBR_OK != rv) {
        cout << "JUB_CalculateTransactionCIDFIL() return " << GetErrMsg(rv) << endl;
        return rv;
    }
    else {
        cout << "cid[" << strlen(cid) << "]: "  << cid << endl;
        JUB_FreeMemory(cid);
    }

    char* raw = nullptr;
    rv = JUB_SignTransactionFIL(contextID,
                                path,
                                nonce,
                                gasLimit,
                                gasFeeCapInAtto,
                                gasPremiumInAtto,
                                to,
                                valueInAtto,
                                "",
                                &raw);
    if (JUBR_OK != rv) {
        cout << "JUB_SignTransactionFIL() return " << GetErrMsg(rv) << endl;
        return rv;
    }
    else {
        cout << "raw[" << strlen(raw) << "]: "  << raw << endl;
        JUB_FreeMemory(raw);
    }

    return rv;
}
