//
//  JUB_SDK_test_eos.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/11/11.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
#include "JUB_SDK_test_dev.hpp"
#include "JUB_SDK_test_eos.hpp"

#include "JUB_SDK_main.h"
#include <time.h>


void EOS_test(JUB_UINT16 deviceID, const char* json_file) {

    JUB_RV rv = JUBR_ERROR;

    Json::Value root = readJSON(json_file);
    if (root.empty()) {
        return ;
    }

    JUB_UINT16 contextID = 0;

    CONTEXT_CONFIG_EOS cfg;
    cfg.mainPath = (char*)root["main_path"].asCString();
    rv = JUB_CreateContextEOS(cfg, deviceID, &contextID);
    cout << "[-] JUB_CreateContextEOS() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << endl;

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet EOS  ********|" << endl;
        cout << "|  0. show_address_pubkey_test.      |" << endl;
        cout << "|                                    |" << endl;
        cout << "| 21.       transfer_test.           |" << endl;
        cout << "| 22. transfer_token_test.           |" << endl;
        cout << "| 23.          stake_test.           |" << endl;
        cout << "| 24.        unstake_test.           |" << endl;
        cout << "| 25.        buy_ram_test.           |" << endl;
        cout << "| 26.       sell_ram_test.           |" << endl;
        cout << "|                                    |" << endl;
        cout << "| 9 . return.                        |" << endl;
        cout << "--------------------------------------" << endl;
        cout << "* Please enter your choice:" << endl;

        int choice = 0;
        cin >> choice;

        switch (choice) {
        case 0:
            get_address_pubkey_EOS(contextID);
            break;
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
            transaction_test_EOS(contextID, root, choice);
            break;
        case 9:
            JUB_ClearContext(contextID);
            main_test();
        default:
            continue;
        }   // switch (choice) end
    }   // while (true) end
}


void get_address_pubkey_EOS(JUB_UINT16 contextID) {

    JUB_RV rv = JUBR_ERROR;

    cout << "[----------------------------------- HD Node -----------------------------------]" << endl;
    JUB_CHAR_PTR pubkey = nullptr;
    rv = JUB_GetMainHDNodeEOS(contextID, JUB_ENUM_PUB_FORMAT::HEX, &pubkey);
    cout << "[-] JUB_GetMainHDNodeEOS() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "MainXpub in  HEX format:  " << pubkey << endl;
    JUB_FreeMemory(pubkey);
    cout << endl;

    pubkey = nullptr;
    rv = JUB_GetMainHDNodeEOS(contextID, JUB_ENUM_PUB_FORMAT::XPUB, &pubkey);
    cout << "[-] JUB_GetMainHDNodeEOS() return " << GetErrMsg(rv) << endl;
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
    rv = JUB_GetHDNodeEOS(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &pubkey);
    cout << "[-] JUB_GetHDNodeEOS() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "  pubkey in  HEX format :  "<< pubkey << endl;
    JUB_FreeMemory(pubkey);
    cout << endl;

    pubkey = nullptr;
    rv = JUB_GetHDNodeEOS(contextID, JUB_ENUM_PUB_FORMAT::XPUB, path, &pubkey);
    cout << "[-] JUB_GetHDNodeEOS() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "  pubkey in XPUB format :  " << pubkey << endl;
        JUB_FreeMemory(pubkey);
    }
    cout << "[--------------------------------- HD Node end ---------------------------------]" << endl;
    cout << endl << endl;

    cout << "[----------------------------------- Address -----------------------------------]" << endl;
    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressEOS(contextID, path, BOOL_FALSE, &address);
    cout << "[-] JUB_GetAddressEOS() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "         address: " << address << endl;
    JUB_FreeMemory(address);

    address = nullptr;
    rv = JUB_GetAddressEOS(contextID, path, BOOL_TRUE, &address);
    cout << "[-] JUB_GetAddressEOS() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "    show address: " << address << endl;

    rv = JUB_CheckAddressEOS(contextID, address);
    cout << "[-] JUB_CheckAddressEOS() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }

    JUB_FreeMemory(address);
    cout << "[--------------------------------- Address end ---------------------------------]" << endl;
    cout << endl << endl;
}


void transaction_test_EOS(JUB_UINT16 contextID, Json::Value root, int choice) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    rv = transaction_proc_EOS(contextID, root, (choice-20));
    if (JUBR_OK != rv) {
        return;
    }
}


JUB_RV transaction_proc_EOS(JUB_UINT16 contextID, Json::Value root, int choice) {

    JUB_RV rv = JUBR_ERROR;

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["EOS"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["EOS"]["bip32_path"]["addressIndex"].asUInt();

    if (!root["EOS"]["actions"].isArray()) {
        return JUBR_ARGUMENTS_BAD;
    }

    int actIndex = 0;
    std::string sAction = "acts";
    if (   (1 == choice)
        || (2 == choice)
        ) {
        sAction = "actions";
    }
    switch (choice) {
    case 1:
        actIndex = 0;
        break;
    case 2:
        actIndex = 1;
        break;
    case 3:
        actIndex = 0;
        break;
    case 4:
        actIndex = 1;
        break;
    case 5:
        actIndex = 2;
        break;
    case 6:
        actIndex = 3;
        break;
    default:
        break;
    }

    std::vector<JUB_ACTION_EOS> actions;
    //EOS Test
//    for (Json::Value::iterator it = root["EOS"][sAction][actIndex].begin(); it != root["EOS"][sAction][actIndex].end(); ++it) {
        Json::Value::iterator it = root["EOS"][sAction].begin();
        for (int i=0; i<actIndex; ++i) {
            it ++;
        }

        JUB_ACTION_EOS action;

        action.type = (JUB_ENUM_EOS_ACTION_TYPE)(*it)["type"].asUInt();
        action.currency = (char*)(*it)["currency"].asCString();
        action.name     = (char*)(*it)["name"].asCString();

        std::string sType = std::to_string((unsigned int)action.type);
        switch (action.type) {
        case JUB_ENUM_EOS_ACTION_TYPE::XFER:
            action.transfer.from  = (char*)(*it)[sType]["from"].asCString();
            action.transfer.to    = (char*)(*it)[sType]["to"].asCString();
            action.transfer.asset = (char*)(*it)[sType]["asset"].asCString();
            action.transfer.memo  = (char*)(*it)[sType]["memo"].asCString();

            JUB_CHAR_PTR memoHash;
            rv = JUB_CalculateMemoHash(action.transfer.memo, &memoHash);
            cout << "[-] JUB_CalculateMemoHash() return " << GetErrMsg(rv) << endl;
            if (JUBR_OK == rv) {
                cout << "    memoHash: " << memoHash << std::endl;
            }
            break;
        case JUB_ENUM_EOS_ACTION_TYPE::DELE:
            action.delegate.from     = (char*)(*it)[sType]["from"].asCString();
            action.delegate.receiver = (char*)(*it)[sType]["receiver"].asCString();
            action.delegate.netQty   = (char*)(*it)[sType]["stake_net_quantity"].asCString();
            action.delegate.cpuQty   = (char*)(*it)[sType]["stake_cpu_quantity"].asCString();
            action.delegate.transfer = (*it)[sType]["transfer"].asBool();
            action.delegate.bStake = true;
            break;
        case JUB_ENUM_EOS_ACTION_TYPE::UNDELE:
            action.delegate.from     = (char*)(*it)[sType]["from"].asCString();
            action.delegate.receiver = (char*)(*it)[sType]["receiver"].asCString();
            action.delegate.netQty   = (char*)(*it)[sType]["unstake_net_quantity"].asCString();
            action.delegate.cpuQty   = (char*)(*it)[sType]["unstake_cpu_quantity"].asCString();
            action.delegate.transfer = (*it)[sType]["transfer"].asBool();
            action.delegate.bStake = false;
            break;
        case JUB_ENUM_EOS_ACTION_TYPE::BUYRAM:
            action.buyRam.payer    = (char*)(*it)[sType]["payer"].asCString();
            action.buyRam.quant    = (char*)(*it)[sType]["quant"].asCString();
            action.buyRam.receiver = (char*)(*it)[sType]["receiver"].asCString();
            break;
        case JUB_ENUM_EOS_ACTION_TYPE::SELLRAM:
            action.sellRam.account = (char*)(*it)[sType]["account"].asCString();
            action.sellRam.bytes   = (char*)(*it)[sType]["bytes"].asCString();
            break;
        case JUB_ENUM_EOS_ACTION_TYPE::NS_ITEM_EOS_ACTION_TYPE:
        default:
            return JUBR_ARGUMENTS_BAD;
        }   // switch (action.type) end
        actions.push_back(action);
//    }   // for (Json::Value::iterator it ... end

    size_t actionCnt = actions.size();
    JUB_ACTION_EOS_PTR pActions = new JUB_ACTION_EOS[actionCnt*sizeof(JUB_ACTION_EOS)+1];
    memset(pActions, 0x00, actionCnt*sizeof(JUB_ACTION_EOS)+1);
    int i=0;
    for (const JUB_ACTION_EOS& action:actions) {
        pActions[i] = action;
        ++i;
    }

    JUB_CHAR_PTR actionsInJSON = nullptr;
    rv = JUB_BuildActionEOS(contextID,
                            pActions, actionCnt,
                            &actionsInJSON);
    cout << "[-] JUB_BuildActionEOS() return " << GetErrMsg(rv) << endl;
    delete [] pActions; pActions = nullptr;
    if (JUBR_OK != rv) {
        return rv;
    }
    std::cout << "    Action is " << actionsInJSON << std::endl;
    char* chainID    = (char*)root["EOS"]["chainID"].asCString();
    char* expiration = (char*)root["EOS"]["expiration"].asCString();
    char* referenceBlockId = (char*)root["EOS"]["referenceBlockId"].asCString();

    char* refBlockT = (char*)root["EOS"]["referenceBlockTime"].asCString();
    int yy, month, dd, hh, mm, ss;
    sscanf(refBlockT, "%d-%d-%d %d:%d:%d",
           &yy, &month, &dd,
           &hh, &mm, &ss);
    tm refblocktime;
    refblocktime.tm_year = yy - 1900;
    refblocktime.tm_mon = month - 1;
    refblocktime.tm_mday = dd;
    refblocktime.tm_hour = hh;
    refblocktime.tm_min = mm;
    refblocktime.tm_sec = ss;
    refblocktime.tm_isdst = -1;
    time_t tRefblocktime = mktime(&refblocktime);

    time_t localTime;
    tRefblocktime += localtime(&localTime)->tm_gmtoff;
    char* referenceBlockTime = (char*)std::to_string(tRefblocktime).c_str();

    JUB_CHAR_PTR raw = nullptr;
    rv = JUB_SignTransactionEOS(contextID,
                                path,
                                chainID,
                                expiration,
                                referenceBlockId,
                                referenceBlockTime,
                                actionsInJSON,
                                &raw);
    cout << "[-] JUB_SignTransactionEOS() return " << GetErrMsg(rv) << endl;
    JUB_FreeMemory(actionsInJSON);
    if (JUBR_OK != rv) {
        return rv;
    }
    else {
        cout << "    EOS raw[" << strlen(raw)/2 << "]: "  << raw << endl;
        JUB_FreeMemory(raw);
    }

    return JUBR_OK;
}
