//
//  JUB_SDK_test_trx.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/9/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#include "JUB_SDK_test.h"
#include "JUB_SDK_test_dev.hpp"
#include "JUB_SDK_test_trx.hpp"

#include "JUB_SDK_main.h"

void TRX_test(JUB_UINT16 deviceID, const char* json_file) {

    JUB_RV rv = JUBR_ERROR;

    char* appList;
    rv = JUB_EnumApplets(deviceID, &appList);
    cout << "[-] JUB_EnumApplets() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }

    Json::Value root = readJSON(json_file);
    JUB_UINT16 contextID = 0;

    CONTEXT_CONFIG_TRX cfg;
    cfg.mainPath = (char*)root["main_path"].asCString();
    rv = JUB_CreateContextTRX(cfg, deviceID, &contextID);
    cout << "[-] JUB_CreateContextTRX() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
	cout << endl;

    while (true) {
        cout << " ------------------------------------- " << endl;
        cout << "|******* Jubiter Wallet TRX  ********|" << endl;
        cout << "| 1 . show_address_pubkey_test.      |" << endl;
        cout << "|                                    |" << endl;
        cout << "| 21.       transfer_contract_test.  |" << endl;
        cout << "| 22. transfer_asset_contract_test.  |" << endl;
        cout << "| 23. trigger_smart_contr_erc20_test.|" << endl;
        cout << "| 24. trigger_smart_contr_trc20_test.|" << endl;
        cout << "|                                    |" << endl;
        cout << "| 3 . set_my_address_test.           |" << endl;
        cout << "| 4 . set_timeout_test.              |" << endl;
        cout << "|                                    |" << endl;
        cout << "| 9 . return.                        |" << endl;
        cout << " ------------------------------------ " << endl;
        cout << "* Please enter your choice:" << endl;

        int choice = 0;
        cin >> choice;

        switch (choice) {
        case 1:
            get_address_pubkey_TRX(contextID);
            break;
        case 21:
        case 22:
        case 23:
        case 24:
            transaction_test_TRX(contextID, root, choice);
            break;
        case 3:
            set_my_address_test_TRX(contextID);
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

void set_my_address_test_TRX(JUB_UINT16 contextID) {

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
    rv = JUB_SetMyAddressTRX(contextID, path, &address);
    cout << "[-] JUB_SetMyAddressTRX() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    else {
        cout << "    set my address is : " << address << endl;
        JUB_FreeMemory(address);
    }
}

void get_address_pubkey_TRX(JUB_UINT16 contextID) {

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
    JUB_RV rv = JUB_GetMainHDNodeTRX(contextID, JUB_ENUM_PUB_FORMAT::HEX, &xpub);
    cout << "[-] JUB_GetMainHDNodeTRX() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }

    cout << "    Main xpub in HEX: " << xpub << endl;
    JUB_FreeMemory(xpub);
    cout << endl;

    xpub = nullptr;
    rv = JUB_GetHDNodeTRX(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &xpub);
    cout << "[-] JUB_GetHDNodeTRX() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }

    cout << "    xpub in HEX: " << xpub << endl;
    JUB_FreeMemory(xpub);
    cout << "[--------------------------------- HD Node end ---------------------------------]" << endl;
    cout << endl << endl;

    cout << "[----------------------------------- Address -----------------------------------]" << endl;
    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressTRX(contextID, path, BOOL_TRUE, &address);
    cout << "[-] JUB_GetAddressTRX() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "    show address: " << address << endl << endl;

    char* addrInHex = nullptr;
    rv = JUB_CheckAddressTRX(contextID, address, &addrInHex);
    cout << "[-] JUB_CheckAddressTRX() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }

    cout << "    address in hex: " << addrInHex << endl;
    JUB_FreeMemory(addrInHex);
    cout << "[--------------------------------- Address end ---------------------------------]" << endl;
    cout << endl << endl;
}

void transaction_test_TRX(JUB_UINT16 contextID, Json::Value root, int choice) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    rv = transaction_proc_TRX(contextID, root, choice);
    if (JUBR_OK != rv) {
        return;
    }
}

JUB_RV transaction_proc_TRX(JUB_UINT16 contextID, Json::Value root, int choice) {

    JUB_RV rv = JUBR_ERROR;

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["TRX"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["TRX"]["bip32_path"]["addressIndex"].asUInt();

    //TRX Test
//    typedef enum {
//                XFER_CONTRACT =  1, // TransferContract(balance_contract.proto)
//          XFER_ASSET_CONTRACT =  2, // TransferAssetContract(asset_issue_contract.proto)
//        CREATE_SMART_CONTRACT = 30, // CreateSmartContract(smart_contract.proto)
//          TRIG_SMART_CONTRACT = 31, // TriggerSmartContract(smart_contract.proto)
//         NS_ITEM_TRX_CONTRACT
//    } JUB_ENUM_TRX_CONTRACT_TYPE;
    bool bERC20 = false;
    if (   21 == choice
        || 22 == choice
        ) {
        choice -= 20;
    }
    else if (   23 == choice
             || 24 == choice
             ) {
        if (23 == choice) {
            bERC20 = true;
        }
        choice = 31;
    }

    std::string packedContractInPb;
    rv = pack_contract_proc(contextID, root,
                            choice,
                            packedContractInPb,
                            bERC20);
    if (JUBR_OK != rv) {
        return rv;
    }
    std::cout << "    Packed Contract[" << (packedContractInPb.length()/2) << "]: " << packedContractInPb << std::endl;

    char* raw = nullptr;
    rv = JUB_SignTransactionTRX(contextID, path,
                                packedContractInPb.c_str(),
                                &raw);
    cout << "[-] JUB_SignTransactionTRX() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return rv;
    }
    else {
        cout << "    TRX raw[" << strlen(raw) << "]: "  << raw << endl;
        JUB_FreeMemory(raw);
    }
    cout << endl;

    return rv;
}


JUB_RV pack_contract_proc(JUB_UINT16 contextID, Json::Value root,
                          int choice,
                          std::string& packedContract,
                          bool bERC20) {

    JUB_RV rv = JUBR_OK;

    JUB_CHAR_PTR trc20Abi = nullptr;
    string contractAddress = (char*)root["TRX"]["TRC20"]["contract_address"].asCString();
    if (bERC20) {
        string tokenName = (char*)root["TRX"]["TRC20"]["tokenName"].asCString();
        JUB_UINT16 unitDP = root["TRX"]["TRC20"]["dp"].asUInt64();
        string tokenTo = (char*)root["TRX"]["TRC20"]["token_to"].asCString();
        string tokenValue = (char*)root["TRX"]["TRC20"]["token_value"].asCString();

        rv = JUB_BuildTRC20Abi(contextID,
                               tokenName.c_str(),
                               unitDP,
                               contractAddress.c_str(),
                               tokenTo.c_str(), tokenValue.c_str(),
                               &trc20Abi);
        cout << "[-] JUB_BuildTRC20Abi() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            return rv;
        }
        cout << endl;
    }

    JUB_TX_TRX tx;
    tx.ref_block_bytes = (char*)root["TRX"]["pack"]["ref_block_bytes"].asCString();
    tx.ref_block_num = 0;
    tx.ref_block_hash = (char*)root["TRX"]["pack"]["ref_block_hash"].asCString();
    tx.data = nullptr;
    tx.expiration = (char*)root["TRX"]["pack"]["expiration"].asCString();
    tx.timestamp = (char*)root["TRX"]["pack"]["timestamp"].asCString();
    tx.fee_limit = (char*)"0";

    JUB_CONTRACT_TRX contrTRX;
    const char* sType = std::to_string((unsigned int)choice).c_str();
    switch ((JUB_ENUM_TRX_CONTRACT_TYPE)choice) {
    case JUB_ENUM_TRX_CONTRACT_TYPE::XFER_CONTRACT:
    {
        contrTRX.transfer.owner_address = (char*)root["TRX"]["contracts"]["owner_address"].asCString();
        contrTRX.transfer.to_address    = (char*)root["TRX"]["contracts"][sType]["to_address"].asCString();
        contrTRX.transfer.amount = root["TRX"]["contracts"][sType]["amount"].asUInt64();
        break;
    }
    case JUB_ENUM_TRX_CONTRACT_TYPE::XFER_ASSET_CONTRACT:
    {
        contrTRX.transferAsset.asset_name = (char*)root["TRX"]["contracts"][sType]["asset_name"].asCString();
        contrTRX.transferAsset.owner_address = (char*)root["TRX"]["contracts"]["owner_address"].asCString();
        contrTRX.transferAsset.to_address = (char*)root["TRX"]["contracts"][sType]["to_address"].asCString();
        contrTRX.transferAsset.amount = root["TRX"]["contracts"][sType]["amount"].asUInt64();
        break;
    }
    case JUB_ENUM_TRX_CONTRACT_TYPE::CREATE_SMART_CONTRACT:
    {
        contrTRX.createSmart.bytecode = (char*)root["TRX"]["contracts"][sType]["bytecode"].asCString();
        contrTRX.createSmart.call_token_value = root["TRX"]["contracts"][sType]["call_token_value"].asUInt64();
        contrTRX.createSmart.token_id = root["TRX"]["contracts"][sType]["token_id"].asUInt64();
        break;
    }
    case JUB_ENUM_TRX_CONTRACT_TYPE::TRIG_SMART_CONTRACT:
    {
        tx.fee_limit = (char*)root["TRX"]["contracts"][sType]["fee_limit"].asCString();
        contrTRX.triggerSmart.owner_address = (char*)root["TRX"]["contracts"]["owner_address"].asCString();
        if (bERC20) {
            contrTRX.triggerSmart.contract_address = (char*)contractAddress.c_str();
            contrTRX.triggerSmart.data = trc20Abi;
        }
        else {
            contrTRX.triggerSmart.contract_address = (char*)root["TRX"]["contracts"][sType]["contract_address"].asCString();
            contrTRX.triggerSmart.data = (char*)root["TRX"]["contracts"][sType]["data"].asCString();
        }
        contrTRX.triggerSmart.call_value = root["TRX"]["contracts"][sType]["call_value"].asUInt64();
        contrTRX.triggerSmart.call_token_value = root["TRX"]["contracts"][sType]["call_token_value"].asUInt64();
        contrTRX.triggerSmart.token_id = root["TRX"]["contracts"][sType]["token_id"].asUInt64();
        break;
    }
    default:
        rv = JUBR_ARGUMENTS_BAD;
        break;
    }
    if (JUBR_OK != rv) {
        return rv;
    }
    contrTRX.type = (JUB_ENUM_TRX_CONTRACT_TYPE)choice;

    tx.contracts = &contrTRX;
    tx.contractCount = 1;

    JUB_CHAR_PTR packContractInPb = nullptr;
    rv = JUB_PackContractTRX(contextID, tx, &packContractInPb);
    cout << "[-] JUB_PackContractTRX() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        packedContract = packContractInPb;
    }
    cout << endl;

    return rv;
}
