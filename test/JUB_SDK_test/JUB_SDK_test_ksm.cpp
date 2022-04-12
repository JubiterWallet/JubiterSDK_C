//
//  JUB_SDK_test_ksm.cpp
//  JubSDKTest
//
//  Created by Administrator on 2021/3/29.
//  Copyright © 2022 ft. All rights reserved.
//

#include "JUB_SDK_test_dot.hpp"
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
    cfg.mainNet = ((true == root["main_net"].asBool()) ? JUB_ENUM_BOOL::BOOL_TRUE : JUB_ENUM_BOOL::BOOL_FALSE);

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
        cout << "----------------------------------------" << endl;
        cout << "|********* Jubiter Wallet KSM *********|" << endl;
        cout << "|  0. show_address_pubkey_test.        |" << endl;
        cout << "|                                      |" << endl;
        cout << "| 51. balances_transfer_test.          |" << endl;
        cout << "| 52. balances_transferAll_test.       |" << endl;
        cout << "| 53. balances_transferKeepAlive_test. |" << endl;
        cout << "|                                      |" << endl;
        cout << "| 71. staking_bondExtra_test.          |" << endl;
        cout << "| 72. staking_unbond_test.             |" << endl;
        cout << "| 73. staking_withdrawUnbonded_test.   |" << endl;
        cout << "| 74. staking_payoutStakers_test.      |" << endl;
        cout << "| 75. staking_nominate_test.           |" << endl;
        cout << "|                                      |" << endl;
        cout << "| 9. return.                           |" << endl;
        cout << "----------------------------------------" << endl;
        cout << "* Please enter your choice:" << endl;

        int choice = 0;
        cin >> choice;

        switch (choice) {
            case 0: {
                get_address_pubkey_KSM(contextID);
            } break;
            case 51:
            case 52:
            case 53:
            case 71:
            case 72:
            case 73:
            case 74:
            case 75: {
                transaction_test_DOT(contextID, root, "KSM", choice);
            } break;
            case 9: {
                JUB_ClearContext(contextID);
                main_test();
            default:
                continue;
            }
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

    rv = JUB_CheckAddressKSM(contextID, address);
    cout << "[-] JUB_CheckAddressKSM() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }

    rv = JUB_IsValidAddressKSM(address);
    cout << "[-] JUB_IsValidAddressKSM() return " << GetErrMsg(rv) << endl;

    JUB_FreeMemory(address);
    cout << "[--------------------------------- Address end ---------------------------------]" << endl;
    cout << endl << endl;
}
