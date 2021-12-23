//
//  JUB_SDK_test_dot.cpp
//  JubSDKTest
//
//  Created by Administrator on 2021/3/29.
//  Copyright © 2021 ft. All rights reserved.
//

#include "JUB_SDK_test_dot.hpp"
#include "JUB_SDK_main.h"
#include "JUB_SDK_test.h"
#include "JUB_SDK_test_dev.hpp"

void DOT_test(JUB_UINT16 deviceID, const char* json_file) {

    JUB_RV rv = JUBR_ERROR;

    Json::Value root = readJSON(json_file);
    if (root.empty()) {
        return ;
    }

    JUB_UINT16 contextID = 0;

    CONTEXT_CONFIG_DOT cfg;
    cfg.mainPath = (char*)root["main_path"].asCString();
    cfg.coinType = JUB_ENUM_COINTYPE_DOT::COINDOT;
//    cfg.curve = JUB_ENUM_CURVES::SR25519;
    cfg.mainNet = ((true == root["main_net"].asBool()) ? JUB_ENUM_BOOL::BOOL_TRUE : JUB_ENUM_BOOL::BOOL_FALSE);

    JUB_UINT64 curveNum = 0;
    cout << "please input curveNum  (0-ED25519 1-SR25519):" << endl;
    cin >> curveNum;
    cfg.curve = ((0 == curveNum) ? JUB_ENUM_CURVES::ED25519 : JUB_ENUM_CURVES::SR25519);

    rv = JUB_CreateContextDOT(cfg, deviceID, &contextID);
    cout << "[-] JUB_CreateContextDOT() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << endl;

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet DOT  ********|" << endl;
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
            get_address_pubkey_DOT(contextID, root);
            break;
        case 2:
            transaction_test_DOT(contextID, root);
            break;
        case 9:
            JUB_ClearContext(contextID);
            main_test();
        default:
            continue;
        }   // switch (choice) end
    }   // while (true) end
}


void get_address_pubkey_DOT(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

    cout << "[----------------------------------- Main HD Node -----------------------------------]" << endl;
    JUB_CHAR_PTR pubkey = nullptr;
    rv = JUB_GetMainHDNodeDOT(contextID, JUB_ENUM_PUB_FORMAT::HEX, &pubkey);
    cout << "[-] JUB_GetMainHDNodeDOT() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "MainXpub in  HEX format:  " << pubkey << endl;
    JUB_FreeMemory(pubkey);
    cout << "[----------------------------------- Main HD Node -----------------------------------]" << endl;
    cout << endl;

    JUB_CHAR_PTR path = (JUB_CHAR_PTR)root["main_path"].asCString();
    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressDOT(contextID, path, BOOL_FALSE, &address);
    cout << "[-] JUB_GetAddressDOT() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "         address: " << address << endl;
    JUB_FreeMemory(address);

    address = nullptr;
    rv = JUB_GetAddressDOT(contextID, path, BOOL_TRUE, &address);
    cout << "[-] JUB_GetAddressDOT() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "    show address: " << address << endl;

    rv = JUB_CheckAddressDOT(contextID, address);
    cout << "[-] JUB_CheckAddressDOT() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }

    rv = JUB_IsValidAddressDOT(address);
    cout << "[-] JUB_IsValidAddressDOT() return " << GetErrMsg(rv) << endl;
    JUB_FreeMemory(address);
    cout << "[--------------------------------- Address end ---------------------------------]" << endl;
    cout << endl << endl;
}


void transaction_test_DOT(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    rv = transaction_proc_DOT(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}


JUB_RV transaction_proc_DOT(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;
    JUB_CHAR_PTR path = (JUB_CHAR_PTR)root["main_path"].asCString();

    JUB_TX_DOT tx;
    tx.genesisHash = (char *)root["DOT"]["genesisHash"].asCString();
    tx.blockHash = (char *)root["DOT"]["blockHash"].asCString();
    tx.to = (char *)root["DOT"]["balance_call"]["transfer"]["to"].asCString();
    tx.nonce = root["DOT"]["nonce"].asUInt();
    tx.specVersion = root["DOT"]["specVersion"].asUInt();
    tx.transaction_version = root["DOT"]["transaction_version"].asUInt();
    tx.blockNumber = root["DOT"]["era"]["blockNumber"].asUInt();
    tx.value = (char *)root["DOT"]["balance_call"]["transfer"]["value"].asCString();
    tx.eraPeriod = root["DOT"]["era"]["eraPeriod"].asUInt();
    tx.tip = (char *)root["DOT"]["tip"].asCString();;
    char* raw = nullptr;
    rv = JUB_SignTransactionDOT(contextID, path, tx, &raw);
    if (JUBR_OK != rv) {
        cout << "JUB_SignTransactionDOT() return " << GetErrMsg(rv) << endl;
        return rv;
    }
    else {
        cout << "raw[" << strlen(raw) << "]: "  << raw << endl;
        JUB_FreeMemory(raw);
    }

    return rv;
}
