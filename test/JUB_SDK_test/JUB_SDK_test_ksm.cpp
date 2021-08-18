//
//  JUB_SDK_test_ksm.cpp
//  JubSDKTest
//
//  Created by Administrator on 2021/3/29.
//  Copyright © 2021 ft. All rights reserved.
//

#include "JUB_SDK_test_ksm.hpp"
#include "JUB_SDK_main.h"
#include "JUB_SDK_test.h"
#include "JUB_SDK_test_dev.hpp"

void KSM_test(JUB_UINT16 deviceID, const char* json_file) {

    JUB_RV rv = JUBR_ERROR;

    Json::Value root = readJSON(json_file);
    if (root.empty()) {
        return ;
    }

    JUB_UINT16 contextID = 0;

    CONTEXT_CONFIG_DOT cfg;
    cfg.mainPath = (char*)root["main_path"].asCString();
    cfg.coinType = JUB_ENUM_COINTYPE_DOT::COINKSM;
    
    JUB_UINT64 curveNum = 0;
    cout << "please input curveNum  (0-ED25519 1-SR25519):" << endl;
    cin >> curveNum;
    cfg.curve = curveNum == 0 ? JUB_ENUM_CURVES::ED25519 : JUB_ENUM_CURVES::SR25519;
    
    rv = JUB_CreateContextKSM(cfg, deviceID, &contextID);
    cout << "[-] JUB_CreateContextKSM() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << endl;

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet KSM  ********|" << endl;
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
            get_address_pubkey_KSM(contextID);
            break;
        case 2:
            transaction_test_KSM(contextID, root);
            break;
        case 9:
            JUB_ClearContext(contextID);
            main_test();
        default:
            continue;
        }   // switch (choice) end
    }   // while (true) end
}
void get_address_pubkey_KSM(JUB_UINT16 contextID) {

    JUB_RV rv = JUBR_ERROR;

    cout << "[----------------------------------- HD Node -----------------------------------]" << endl;
    JUB_CHAR_PTR pubkey = nullptr;
    rv = JUB_GetMainHDNodeKSM(contextID, JUB_ENUM_PUB_FORMAT::HEX, &pubkey);
    cout << "[-] JUB_GetMainHDNodeFIL() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "MainXpub in  HEX format:  " << pubkey << endl;
    JUB_FreeMemory(pubkey);
    cout << endl;

    JUB_CHAR path[] = "//foo//123";

    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressKSM(contextID, path, BOOL_FALSE, &address);
    cout << "[-] JUB_GetAddressKSM() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "         address: " << address << endl;
    JUB_FreeMemory(address);

    address = nullptr;
    rv = JUB_GetAddressKSM(contextID, path, BOOL_TRUE, &address);
    cout << "[-] JUB_GetAddressKSM() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "    show address: " << address << endl;
    JUB_FreeMemory(address);
    cout << "[--------------------------------- Address end ---------------------------------]" << endl;
    cout << endl << endl;
}


void transaction_test_KSM(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    rv = transaction_proc_KSM(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}


JUB_RV transaction_proc_KSM(JUB_UINT16 contextID, Json::Value root)
{
    JUB_RV rv = JUBR_ERROR;
    JUB_TX_DOT tx;
    JUB_CHAR path[] = "//foo//123";

//    JUB_CHAR_PTR genesisHash;
//    JUB_CHAR_PTR blockHash;
//    JUB_CHAR_PTR to;
//    JUB_UINT64 nonce;
//    JUB_UINT32 specVersion;
//    JUB_UINT64 network;
//    JUB_UINT32 transaction_version;
//    JUB_UINT64 blockNumber;
//    JUB_UINT64 value;
//    JUB_UINT64 eraPeriod; // [Optional]
//    JUB_UINT64 tip;       // [Optional]
    
    tx.genesisHash = (char *)root["KSM"]["genesisHash"].asCString();
    tx.blockHash = (char *)root["KSM"]["blockHash"].asCString();
    tx.to = (char *)root["KSM"]["balance_call"]["transfer"]["to"].asCString();
    tx.nonce = root["KSM"]["nonce"].asUInt();
    tx.specVersion = root["KSM"]["specVersion"].asUInt();
    tx.network = root["KSM"]["network"].asUInt();
    tx.transaction_version = root["KSM"]["transaction_version"].asUInt();
    tx.blockNumber = root["KSM"]["era"]["blockNumber"].asUInt();
    tx.value = (char *)root["KSM"]["balance_call"]["transfer"]["value"].asCString();
    tx.eraPeriod = root["KSM"]["era"]["eraPeriod"].asUInt();
    tx.tip = (char *)root["KSM"]["tip"].asCString();;
    char* raw = nullptr;
    rv = JUB_SignTransactionKSM(contextID, path, tx, &raw);
    if (JUBR_OK != rv) {
        cout << "JUB_SignTransactionKSM() return " << GetErrMsg(rv) << endl;
        return rv;
    }
    else {
        cout << "raw[" << strlen(raw) << "]: "  << raw << endl;
        JUB_FreeMemory(raw);
    }

    return rv;
}
