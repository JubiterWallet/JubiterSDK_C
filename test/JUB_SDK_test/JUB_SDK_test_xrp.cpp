//
//  JUB_SDK_test_xrp.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/12/03.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
#include "JUB_SDK_test_dev.hpp"
#include "JUB_SDK_test_xrp.hpp"

#include "JUB_SDK_main.h"
#include <time.h>

void XRP_test(const char* json_file) {

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

    CONTEXT_CONFIG_XRP cfg;
    cfg.mainPath = (char*)root["main_path"].asCString();
    JUB_UINT16 contextID = 0;
    rv = JUB_CreateContextXRP(cfg, deviceIDs[0], &contextID);
    if (JUBR_OK != rv) {
        cout << "JUB_CreateContextXRP() return " << GetErrMsg(rv) << endl;
        return;
    }

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet XRP  ********|" << endl;
        cout << "| 1. show_address_pubkey_test.       |" << endl;
        cout << "| 2. transaction_test.               |" << endl;
        cout << "| 3. set_my_address_test.            |" << endl;
        cout << "| 4. set_timeout_test.               |" << endl;
        cout << "| 0. return.                         |" << endl;
        cout << "--------------------------------------" << endl;
        cout << "* Please enter your choice:" << endl;

        int choice = 0;
        cin >> choice;

        switch (choice) {
            case 1:
                get_address_pubkey_XRP(contextID);
                break;
            case 2:
                transaction_test_XRP(contextID, root);
                break;
            case 3:
                set_my_address_test_XRP(contextID);
                break;
            case 4:
                set_timeout_test(contextID);
                break;
            case 0:
                main_test();
            default:
                continue;
        }
    }
}

void set_my_address_test_XRP(JUB_UINT16 contextID) {

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
    rv = JUB_SetMyAddressXRP(contextID, path, &address);
    if (JUBR_OK != rv) {
        cout << "JUB_SetMyAddressXRP() return " << GetErrMsg(rv) << endl;
        return;
    }
    else {
        cout << "set my address is : " << address << endl;
        JUB_FreeMemory(address);
    }
}

void get_address_pubkey_XRP(JUB_UINT16 contextID) {
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

//    char* pubkey = nullptr;
//    rv = JUB_GetMainHDNodeXRP(contextID, JUB_ENUM_XRP_PUB_FORMAT::HEX, &pubkey);
//    if (JUBR_OK != rv) {
//        cout << "JUB_GetMainHDNodeXRP() return " << GetErrMsg(rv) << endl;
//        return;
//    }
//
//    cout << "MainXpub in  hex format :  " << pubkey << endl;
//    JUB_FreeMemory(pubkey);
//
//    pubkey = nullptr;
//    rv = JUB_GetMainHDNodeXRP(contextID, JUB_ENUM_XRP_PUB_FORMAT::XPUB, &pubkey);
//    if (JUBR_OK != rv) {
//        cout << "JUB_GetMainHDNodeXRP() return " << GetErrMsg(rv) << endl;
//        return;
//    }
//
//    cout << "MainXpub in  xpub format :  " << pubkey << endl;
//    JUB_FreeMemory(pubkey);
//
//    pubkey = nullptr;
//    rv = JUB_GetHDNodeXRP(contextID, JUB_ENUM_XRP_PUB_FORMAT::HEX, path, &pubkey);
//    if (JUBR_OK != rv) {
//        cout << "JUB_GetHDNodeXRP() return " << GetErrMsg(rv) << endl;
//        return;
//    }
//
//    cout << "pubkey in  hex format :  "<< pubkey << endl;
//    JUB_FreeMemory(pubkey);
//
//    pubkey = nullptr;
//    rv = JUB_GetHDNodeXRP(contextID, JUB_ENUM_XRP_PUB_FORMAT::XPUB, path, &pubkey);
//    if (JUBR_OK != rv) {
//        cout << "JUB_GetHDNodeXRP() return " << GetErrMsg(rv) << endl;
//        return;
//    }
//
//    cout << "pubkey in xpub format :  " << pubkey << endl;
//    JUB_FreeMemory(pubkey);

    char* address = nullptr;
    rv = JUB_GetAddressXRP(contextID, path, BOOL_TRUE, &address);
    if (JUBR_OK != rv) {
        cout << "JUB_GetAddressXRP() return " << GetErrMsg(rv) << endl;
        return;
    }
    cout << "address: " << address << endl;
    JUB_FreeMemory(address);
}

void transaction_test_XRP(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["XRP"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["XRP"]["bip32_path"]["addressIndex"].asUInt();

    //typedef struct stPaymentXRP {
    //    JUB_ENUM_XRP_PYMT_TYPE type;
    //    JUB_CHAR_PTR invoiceID; // [Optional]
    //    JUB_CHAR_PTR sendMax;   // [Optional]
    //    JUB_CHAR_PTR deliverMin;// [Optional]
    //    union {
    //        JUB_PYMT_DXRP dxrp;
    //        JUB_PYMT_DXRP fx;
    //    };
    //} JUB_PYMT_XRP;
    //typedef struct stTxXRP {
    //    JUB_CHAR_PTR account;
    //    JUB_ENUM_XRP_TX_TYPE type;
    //    JUB_CHAR_PTR fee;
    //    JUB_CHAR_PTR sequence;
    //    JUB_CHAR_PTR accountTxnID;
    //    JUB_CHAR_PTR flags;
    //    JUB_CHAR_PTR lastLedgerSequence;
    //    JUB_CHAR_PTR memos;
    //    JUB_CHAR_PTR sourceTag;
    //    union {
    //        JUB_PYMT_XRP pymt;
    //    };
    //} JUB_TX_XRP;
    JUB_TX_XRP xrp;
    xrp.type = (JUB_ENUM_XRP_TX_TYPE)root["XRP"]["type"].asUInt();
    switch (xrp.type) {
        case JUB_ENUM_XRP_TX_TYPE::PYMT:
        {
            xrp.account = (char*)root["XRP"]["account"].asCString();
            xrp.fee   = (char*)root["XRP"]["fee"].asCString();
            xrp.flags = (char*)root["XRP"]["flags"].asCString();
            xrp.sequence = (char*)root["XRP"]["sequence"].asCString();
            xrp.lastLedgerSequence = (char*)root["XRP"]["lastLedgerSequence"].asCString();
            break;
        }
        default:
            return;
    }

    //typedef struct stDxrpPymt {
    //    JUB_CHAR_PTR destination;
    //    JUB_CHAR_PTR amount;
    //    JUB_CHAR_PTR destinationTag;
    //} JUB_PYMT_DXRP;
    const char* sType = std::to_string((unsigned int)xrp.type).c_str();
    xrp.pymt.type = (JUB_ENUM_XRP_PYMT_TYPE)root["XRP"][sType]["type"].asUInt();
    switch (xrp.pymt.type) {
        case JUB_ENUM_XRP_PYMT_TYPE::DXRP:
        {
            xrp.pymt.destination    = (char*)root["XRP"][sType]["destination"].asCString();
            xrp.pymt.amount.value   = (char*)root["XRP"][sType]["amount"]["value"].asCString();
            xrp.pymt.destinationTag = (char*)root["XRP"][sType]["destinationTag"].asCString();
            break;
        }
        default:
            return;
    }
    char* raw = nullptr;
    rv = JUB_SignTransactionXRP(contextID,
                                path,
                                xrp,
                                &raw);
    if (JUBR_OK != rv) {
        cout << "JUB_SignTransactionXRP return " << rv << endl;
        return ;
    }
    cout << "JUB_SignTransactionXRP return " << raw << endl;
    JUB_FreeMemory(raw);
}
