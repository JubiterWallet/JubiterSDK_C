//
//  JUB_SDK_test_dot.cpp
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
        cout << "----------------------------------------" << endl;
        cout << "|********* Jubiter Wallet DOT *********|" << endl;
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
                get_address_pubkey_DOT(contextID, root);
            } break;
            case 51:
            case 52:
            case 53:
            case 71:
            case 72:
            case 73:
            case 74:
            case 75: {
                transaction_test_DOT(contextID, root, "DOT", choice);
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


void transaction_test_DOT(JUB_UINT16 contextID, Json::Value root, const std::string& coin, int choice) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    rv = transaction_proc_DOT(contextID, root, coin, choice);
    if (JUBR_OK != rv) {
        return;
    }
}


JUB_RV transaction_proc_DOT(JUB_UINT16 contextID, Json::Value root, const std::string& coin, int choice) {

    JUB_RV rv = JUBR_ERROR;
    JUB_CHAR_PTR path = (JUB_CHAR_PTR)root["main_path"].asCString();

    JUB_TX_DOT tx;
    switch (choice) {
        case 51: {   // balances.transfer
            tx.type = BALANCE_XFER;
            tx.nonce = root[coin]["balance_call"]["transfer"]["nonce"].asUInt();
            tx.balances.to = (char *)root[coin]["balance_call"]["transfer"]["to"].asCString();
            tx.balances.value = (char *)root[coin]["balance_call"]["transfer"]["value"].asCString();
            tx.balances.keep_alive = false;
        } break;
        case 52: {  // balances.transferAll
            tx.type = BALANCE_XFER;
            tx.nonce = root[coin]["balance_call"]["transferAll"]["nonce"].asUInt();
            tx.balances.to = (char *)root[coin]["balance_call"]["transferAll"]["to"].asCString();
            tx.balances.value = "";
            tx.balances.keep_alive = (JUB_BBOOL)root["DOT"]["balance_call"]["transferAll"]["keep_alive"].asBool();
        } break;
        case 53: {  // balances.transferKeepAlive
            tx.type = BALANCE_XFER;
            tx.nonce = root[coin]["balance_call"]["transferKeepAlive"]["nonce"].asUInt();
            tx.balances.to = (char *)root[coin]["balance_call"]["transferKeepAlive"]["to"].asCString();
            tx.balances.value = (char *)root[coin]["balance_call"]["transferKeepAlive"]["value"].asCString();
            tx.balances.keep_alive = true;
        } break;
        case 71: {  // staking.bondExtra
            tx.type = STAKING_BOND_EXTRA;
            tx.nonce = root[coin]["staking_call"]["bondExtra"]["nonce"].asUInt();
            tx.staking.type = (JUB_ENUM_DOT_STAKING_TYPE)((STAKING_BOND_EXTRA & 0xFF00) >> 8);
            tx.staking.extra.value = (char *)root[coin]["staking_call"]["bondExtra"]["value"].asCString();
        } break;
        case 72: {  // staking.unbond
            tx.type = STAKING_UNBOND;
            tx.nonce = root[coin]["staking_call"]["unbond"]["nonce"].asUInt();
            tx.staking.type = (JUB_ENUM_DOT_STAKING_TYPE)((STAKING_UNBOND & 0xFF00) >> 8);
            tx.staking.unbond.value = (char *)root[coin]["staking_call"]["unbond"]["value"].asCString();
        } break;
        case 73: {  // staking.withdrawUnbonded
            tx.type = STAKING_WITHDRAW_UNBONDED;
            tx.nonce = root[coin]["staking_call"]["withdrawUnbonded"]["nonce"].asUInt();
            tx.staking.type = (JUB_ENUM_DOT_STAKING_TYPE)((STAKING_WITHDRAW_UNBONDED & 0xFF00) >> 8);
            tx.staking.withdrawUnbonded.slashing_spans = root[coin]["staking_call"]["withdrawUnbonded"]["slashing_spans"].asUInt64();
        } break;
        case 74: {  // staking.payoutStakers
            tx.type = STAKING_PAYOUT_STAKERS;
            tx.nonce = root[coin]["staking_call"]["payoutStakers"]["nonce"].asUInt();
            tx.staking.type = (JUB_ENUM_DOT_STAKING_TYPE)((STAKING_PAYOUT_STAKERS & 0xFF00) >> 8);
            tx.staking.payoutStakers.validator_stash = (char *)root["DOT"]["staking_call"]["payoutStakers"]["validator_stash"].asCString();
            tx.staking.payoutStakers.era = root[coin]["staking_call"]["payoutStakers"]["era"].asUInt64();
        } break;
        case 75: {  // staking.nominate
            tx.type = STAKING_NOMINATE;
            tx.nonce = root[coin]["staking_call"]["nominate"]["nonce"].asUInt();
            tx.staking.type = (JUB_ENUM_DOT_STAKING_TYPE)((STAKING_NOMINATE & 0xFF00) >> 8);

            int nominators_n = root[coin]["staking_call"]["nominate"]["id"].size();
//            std::vector<JUB_CHAR_PTR> nominators;
            for (int i = 0; i < nominators_n; i++) {
//                JUB_CHAR_PTR nominator = (JUB_CHAR_PTR)root["DOT"]["staking_call"]["nominate"]["id"][i].asCString();
//                nominators.push_back(nominator);
                tx.staking.nominate.nominators[i] = (JUB_CHAR_PTR)root[coin]["staking_call"]["nominate"]["id"][i].asCString();
            }
            tx.staking.nominate.nominators_n = nominators_n;
        } break;
        default: {
            return JUBR_IMPL_NOT_SUPPORT;
        } break;
    }

    tx.genesisHash = (char *)root[coin]["genesisHash"].asCString();
    tx.blockHash = (char *)root[coin]["blockHash"].asCString();
    tx.specVersion = root[coin]["specVersion"].asUInt();
    tx.transaction_version = root[coin]["transaction_version"].asUInt();
    tx.blockNumber = root[coin]["era"]["blockNumber"].asUInt();
    tx.eraPeriod = root[coin]["era"]["eraPeriod"].asUInt();
    tx.tip = (char *)root[coin]["tip"].asCString();

    char* raw = nullptr;
    if (0 == coin.compare("DOT")) {
        rv = JUB_SignTransactionDOT(contextID, path, tx, &raw);
    }
    else {
        rv = JUB_SignTransactionKSM(contextID, path, tx, &raw);
    }
    if (JUBR_OK != rv) {
        cout << "JUB_SignTransactionDOT() return " << GetErrMsg(rv) << endl;
        return rv;
    }
    else {
        cout << "raw[" << strlen(raw)/2 << "]: "  << raw << endl;
        JUB_FreeMemory(raw);
    }

    return rv;
}
