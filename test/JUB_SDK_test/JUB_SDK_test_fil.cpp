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


void FIL_test(JUB_UINT16 deviceID, JUB_CHAR_CPTR json_file) {

    JUB_RV rv = JUBR_ERROR;

    Json::Value root = readJSON(json_file);
    if (root.empty()) {
        return ;
    }

    JUB_UINT16 contextID = 0;

    CONTEXT_CONFIG_FIL cfg;
    cfg.mainPath = (JUB_CHAR_PTR)root["main_path"].asCString();
    rv = JUB_CreateContextFIL(cfg, deviceID, &contextID);
    cout << "[-] JUB_CreateContextFIL() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << endl;

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet FIL  ********|" << endl;
        cout << "|  0. show_address_pubkey_test.      |" << endl;
        cout << "|                                    |" << endl;
        cout << "|  2.    transaction_test.           |" << endl;
        cout << "|                                    |" << endl;
        cout << "| 9. return.                         |" << endl;
        cout << "--------------------------------------" << endl;
        cout << "* Please enter your choice:" << endl;

        int choice = 0;
        cin >> choice;

        switch (choice) {
        case 0:
            get_address_pubkey_FIL(contextID);
            break;
        case 2:
            transaction_test_FIL(contextID, root);
            break;
        case 9:
            JUB_ClearContext(contextID);
            main_test();
        default:
            continue;
        }   // switch (choice) end
    }   // while (true) end
}


void get_address_pubkey_FIL(JUB_UINT16 contextID) {

    JUB_RV rv = JUBR_ERROR;

    cout << "[----------------------------------- HD Node -----------------------------------]" << endl;
    JUB_CHAR_PTR pubkey = nullptr;
    rv = JUB_GetMainHDNodeFIL(contextID, JUB_ENUM_PUB_FORMAT::HEX, &pubkey);
    cout << "[-] JUB_GetMainHDNodeFIL() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "MainXpub in  HEX format:  " << pubkey << endl;
    JUB_FreeMemory(pubkey);
    cout << endl;

    pubkey = nullptr;
    rv = JUB_GetMainHDNodeFIL(contextID, JUB_ENUM_PUB_FORMAT::XPUB, &pubkey);
    cout << "[-] JUB_GetMainHDNodeFIL() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "MainXpub in XPUB format:  " << pubkey << endl;
    JUB_FreeMemory(pubkey);
    cout << endl;

    int change = 0;
    JUB_UINT64 index = 0;
    cout << "please input change level (non-zero means 1):" << endl;
    cin >> change;
    cout << "please input index " << endl;
    cin >> index;

    BIP44_Path path;
    path.change = JUB_ENUM_BOOL(change);
    path.addressIndex = index;

    pubkey = nullptr;
    rv = JUB_GetHDNodeFIL(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &pubkey);
    cout << "[-] JUB_GetHDNodeFIL() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "  pubkey in  HEX format :  "<< pubkey << endl;
    JUB_FreeMemory(pubkey);
    cout << endl;

    pubkey = nullptr;
    rv = JUB_GetHDNodeFIL(contextID, JUB_ENUM_PUB_FORMAT::XPUB, path, &pubkey);
    cout << "[-] JUB_GetHDNodeFIL() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "  pubkey in XPUB format :  " << pubkey << endl;
    JUB_FreeMemory(pubkey);
    cout << "[--------------------------------- HD Node end ---------------------------------]" << endl;
    cout << endl << endl;

    cout << "[----------------------------------- Address -----------------------------------]" << endl;
    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressFIL(contextID, path, BOOL_FALSE, &address);
    cout << "[-] JUB_GetAddressFIL() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "         address: " << address << endl;
    JUB_FreeMemory(address);

    address = nullptr;
    rv = JUB_GetAddressFIL(contextID, path, BOOL_TRUE, &address);
    cout << "[-] JUB_GetAddressFIL() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "    show address: " << address << endl;

    rv = JUB_CheckAddressFIL(contextID, address);
    cout << "[-] JUB_CheckAddressFIL() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }

    rv = JUB_IsValidAddressFIL(address);
    cout << "[-] JUB_IsValidAddressFIL() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }

    JUB_FreeMemory(address);
    cout << "[--------------------------------- Address end ---------------------------------]" << endl;
    cout << endl << endl;
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
    JUB_CHAR_PTR gasFeeCapInAtto  = (JUB_CHAR_PTR)root["FIL"]["gasFeeCapInAtto"].asCString();
    JUB_CHAR_PTR gasPremiumInAtto = (JUB_CHAR_PTR)root["FIL"]["gasPremiumInAtto"].asCString();
    JUB_CHAR_PTR valueInAtto = (JUB_CHAR_PTR)root["FIL"]["valueInAtto"].asCString();
    JUB_CHAR_PTR to = (JUB_CHAR_PTR)root["FIL"]["to"].asCString();

    JUB_CHAR_PTR cid = nullptr;
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

    JUB_CHAR_PTR raw = nullptr;
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
