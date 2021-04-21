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

void XRP_test(JUB_UINT16 deviceID, const char* json_file) {

    JUB_RV rv = JUBR_ERROR;

    JUB_ENUM_COMMODE commode;
    JUB_ENUM_DEVICE deviceClass;
    rv = JUB_GetDeviceType(deviceID,
                           &commode, &deviceClass);
    cout << "[-] JUB_GetDeviceType() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }

    switch (commode) {
    case JUB_ENUM_COMMODE::HID:
    case JUB_ENUM_COMMODE::BLE:
    case JUB_ENUM_COMMODE::NFC:
    case JUB_ENUM_COMMODE::SIM:
    {
        char* appList;
        rv = JUB_EnumApplets(deviceID, &appList);
        cout << "[-] JUB_EnumApplets() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            return;
        }

        break;
    }
    case JUB_ENUM_COMMODE::SWI:
    default:
        break;
    }

    Json::Value root = readJSON(json_file);
    JUB_UINT16 contextID = 0;

    CONTEXT_CONFIG_XRP cfg;
    cfg.mainPath = (char*)root["main_path"].asCString();
    rv = JUB_CreateContextXRP(cfg, deviceID, &contextID);
    cout << "[-] JUB_CreateContextXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << endl;

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet XRP  ********|" << endl;
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
        case 9:
            main_test();
        default:
            continue;
        }   // switch (choice) end
    }   // while (true) end
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
    cout << "[-] JUB_SetMyAddressXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    else {
        cout << "    set my address is : " << address << endl;
        JUB_FreeMemory(address);
    }
}

void get_address_pubkey_XRP(JUB_UINT16 contextID) {

    cout << "[----------------------------------- HD Node -----------------------------------]" << endl;

    int change = 0;
    JUB_UINT64 index = 0;
    cout << "please input change level (non-zero means 1):" << endl;
    cin >> change;
    cout << "please input index " << endl;
    cin >> index;

    BIP44_Path path;
    path.change = JUB_ENUM_BOOL(change);
    path.addressIndex = index;

    JUB_CHAR_PTR xpub = nullptr;
    JUB_RV rv = JUB_GetMainHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::HEX, &xpub);
    cout << "[-] JUB_GetMainHDNodeXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }

    cout << "    Main xpub in HEX: " << xpub << endl;
    JUB_FreeMemory(xpub);
    cout << endl;

    xpub = nullptr;
    rv = JUB_GetHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &xpub);
    cout << "[-] JUB_GetHDNodeXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }

    cout << "    xpub in HEX: " << xpub << endl;
    JUB_FreeMemory(xpub);
    cout << "[--------------------------------- HD Node end ---------------------------------]" << endl;
    cout << endl << endl;

    cout << "[----------------------------------- Address -----------------------------------]" << endl;
    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressXRP(contextID, path, BOOL_TRUE, &address);
    cout << "[-] JUB_GetAddressXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "    show address: " << address << endl;

    JUB_FreeMemory(address);
    cout << "[--------------------------------- Address end ---------------------------------]" << endl;
    cout << endl << endl;
}

void transaction_test_XRP(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    rv = transaction_proc_XRP(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}

JUB_RV transaction_proc_XRP(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

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
    //    JUB_CHAR_PTR accountTxnID;// [Optional]
    //    JUB_CHAR_PTR flags;
    //    JUB_CHAR_PTR lastLedgerSequence;
    //    JUB_XRP_MEMO memo;        // [Optional]
    //    JUB_CHAR_PTR sourceTag;   // [Optional]
    //    union {
    //        JUB_PYMT_XRP pymt;
    //    };
    //} JUB_TX_XRP;
    JUB_TX_XRP xrp;
    xrp.type = (JUB_ENUM_XRP_TX_TYPE)root["XRP"]["type"].asUInt();
    xrp.memo.type   = (char*)root["XRP"]["memo"]["type"].asCString();
    xrp.memo.data   = (char*)root["XRP"]["memo"]["data"].asCString();
    xrp.memo.format = (char*)root["XRP"]["memo"]["format"].asCString();
    switch (xrp.type) {
    case JUB_ENUM_XRP_TX_TYPE::PYMT:
    {
        xrp.account  = (char*)root["XRP"]["account"].asCString();
        xrp.fee      = (char*)root["XRP"]["fee"].asCString();
        xrp.flags    = (char*)root["XRP"]["flags"].asCString();
        xrp.sequence = (char*)root["XRP"]["sequence"].asCString();
        xrp.lastLedgerSequence = (char*)root["XRP"]["lastLedgerSequence"].asCString();
        break;
    }
    default:
        return JUBR_ARGUMENTS_BAD;
    }   // switch (xrp.type) end

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
        return JUBR_ARGUMENTS_BAD;
    }   // switch (xrp.pymt.type) end
    char* raw = nullptr;
    rv = JUB_SignTransactionXRP(contextID,
                                path,
                                xrp,
                                &raw);
    cout << "[-] JUB_SignTransactionXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return rv;
    }
    cout << "    XRP raw[" << strlen(raw) << "]: " << raw << endl;
    JUB_FreeMemory(raw);

    return JUBR_OK;
}
