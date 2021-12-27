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


void XRP_test(JUB_UINT16 deviceID, JUB_CHAR_CPTR json_file) {

    JUB_RV rv = JUBR_ERROR;

    Json::Value root = readJSON(json_file);
    if (root.empty()) {
        return ;
    }

    JUB_UINT16 contextID = 0;

    CONTEXT_CONFIG_XRP cfg;
    cfg.mainPath = (JUB_CHAR_PTR)root["main_path"].asCString();
    rv = JUB_CreateContextXRP(cfg, deviceID, &contextID);
    cout << "[-] JUB_CreateContextXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << endl;

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet XRP  ********|" << endl;
        cout << "|  0. show_address_pubkey_test.      |" << endl;
        cout << "|                                    |" << endl;
        cout << "|  2. transaction_test.              |" << endl;
        cout << "|                                    |" << endl;
        cout << "| 9. return.                         |" << endl;
        cout << "--------------------------------------" << endl;
        cout << "* Please enter your choice:" << endl;

        int choice = 0;
        cin >> choice;

        switch (choice) {
        case 0:
            get_address_pubkey_XRP(contextID);
            break;
        case 2:
            transaction_test_XRP(contextID, root);
            break;
        case 9:
            JUB_ClearContext(contextID);
            main_test();
        default:
            continue;
        }   // switch (choice) end
    }   // while (true) end
}


void get_address_pubkey_XRP(JUB_UINT16 contextID) {

    JUB_RV rv = JUBR_ERROR;

    cout << "[----------------------------------- HD Node -----------------------------------]" << endl;
    JUB_CHAR_PTR pubkey = nullptr;
    rv = JUB_GetMainHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::HEX, &pubkey);
    cout << "[-] JUB_GetMainHDNodeXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "MainXpub in  HEX format:  " << pubkey << endl;
    JUB_FreeMemory(pubkey);
    cout << endl;

    pubkey = nullptr;
    rv = JUB_GetMainHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::XPUB, &pubkey);
    cout << "[-] JUB_GetMainHDNodeXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "MainXpub in XPUB format:  " << pubkey << endl;
        JUB_FreeMemory(pubkey);
        cout << endl;
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

    pubkey = nullptr;
    rv = JUB_GetHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &pubkey);
    cout << "[-] JUB_GetHDNodeXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "  pubkey in  HEX format :  "<< pubkey << endl;
    JUB_FreeMemory(pubkey);
    cout << endl;

    pubkey = nullptr;
    rv = JUB_GetHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::XPUB, path, &pubkey);
    cout << "[-] JUB_GetHDNodeXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "  pubkey in XPUB format :  " << pubkey << endl;
        JUB_FreeMemory(pubkey);
    }
    cout << "[--------------------------------- HD Node end ---------------------------------]" << endl;
    cout << endl << endl;

    cout << "[----------------------------------- Address -----------------------------------]" << endl;
    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressXRP(contextID, path, BOOL_FALSE, &address);
    cout << "[-] JUB_GetAddressXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "         address: " << address << endl;
    JUB_FreeMemory(address);

    address = nullptr;
    rv = JUB_GetAddressXRP(contextID, path, BOOL_TRUE, &address);
    cout << "[-] JUB_GetAddressXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "    show address: " << address << endl;

    JUB_CHAR_PTR addr = nullptr, tag = nullptr;
    rv = JUB_CheckAddressXRP(contextID, address,
                             &addr, &tag);
    cout << "[-] JUB_CheckAddressXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "    show    addr: " << addr << endl;
    cout << "    show     tag: " << tag << endl;
    JUB_FreeMemory(addr);
    JUB_FreeMemory(tag);

    rv = JUB_IsValidAddressXRP(address,
                               &addr, &tag);
    cout << "[-] JUB_IsValidAddressXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "    show    addr: " << addr << endl;
    cout << "    show     tag: " << tag << endl;
    JUB_FreeMemory(addr);
    JUB_FreeMemory(tag);

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
    xrp.memo.type   = (JUB_CHAR_PTR)root["XRP"]["memo"]["type"].asCString();
    xrp.memo.data   = (JUB_CHAR_PTR)root["XRP"]["memo"]["data"].asCString();
    xrp.memo.format = (JUB_CHAR_PTR)root["XRP"]["memo"]["format"].asCString();
    switch (xrp.type) {
    case JUB_ENUM_XRP_TX_TYPE::PYMT:
    {
        xrp.account  = (JUB_CHAR_PTR)root["XRP"]["account"].asCString();
        xrp.fee      = (JUB_CHAR_PTR)root["XRP"]["fee"].asCString();
        xrp.flags    = (JUB_CHAR_PTR)root["XRP"]["flags"].asCString();
        xrp.sequence = (JUB_CHAR_PTR)root["XRP"]["sequence"].asCString();
        xrp.lastLedgerSequence = (JUB_CHAR_PTR)root["XRP"]["lastLedgerSequence"].asCString();
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
    JUB_CHAR_CPTR sType = std::to_string((unsigned int)xrp.type).c_str();
    xrp.pymt.type = (JUB_ENUM_XRP_PYMT_TYPE)root["XRP"][sType]["type"].asUInt();
    switch (xrp.pymt.type) {
    case JUB_ENUM_XRP_PYMT_TYPE::DXRP:
    {
        xrp.pymt.destination    = (JUB_CHAR_PTR)root["XRP"][sType]["destination"].asCString();
        xrp.pymt.amount.value   = (JUB_CHAR_PTR)root["XRP"][sType]["amount"]["value"].asCString();
        xrp.pymt.destinationTag = (JUB_CHAR_PTR)root["XRP"][sType]["destinationTag"].asCString();
        break;
    }
    default:
        return JUBR_ARGUMENTS_BAD;
    }   // switch (xrp.pymt.type) end
    JUB_CHAR_PTR raw = nullptr;
    rv = JUB_SignTransactionXRP(contextID,
                                path,
                                xrp,
                                &raw);
    cout << "[-] JUB_SignTransactionXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return rv;
    }
    cout << "    XRP raw[" << strlen(raw)/2 << "]: " << raw << endl;
    JUB_FreeMemory(raw);

    return JUBR_OK;
}
