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

    Json::Value root = readJSON(json_file);
    if (root.empty()) {
        return ;
    }

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
        cout << "|******** Jubiter Wallet TRX  ********|" << endl;
        cout << "| 0 . show_address_pubkey_test.       |" << endl;
        cout << "|                                     |" << endl;
        cout << "|  1.       transfer_contract_test.   |" << endl;
        cout << "|  2. transfer_asset_contract_test.   |" << endl;
        cout << "| 11.   freeze_balance_contract_test. |" << endl;
        cout << "| 12. unfreeze_balance_contract_test. |" << endl;
        cout << "| 31. trigger_smart_contr_erc20_test. |" << endl;
        cout << "| 32. trigger_smart_contr_trc20_test. |" << endl;
        cout << "| 33. trigger_smart_contr_trc721_test.|" << endl;
        cout << "|                                     |" << endl;
        cout << "| 3 . set_my_address_test.            |" << endl;
        cout << "| 4 . set_timeout_test.               |" << endl;
        cout << "|                                     |" << endl;
        cout << "| 9 . return.                         |" << endl;
        cout << " ------------------------------------- " << endl;
        cout << "* Please enter your choice:" << endl;

        int choice = 0;
        cin >> choice;

        switch (choice) {
        case 0:
            get_address_pubkey_TRX(contextID);
            break;
        case JUB_ENUM_TRX_CONTRACT_TYPE::XFER_CONTRACT:
        case JUB_ENUM_TRX_CONTRACT_TYPE::XFER_ASSET_CONTRACT:
        case JUB_ENUM_TRX_CONTRACT_TYPE::FRZ_BLA_CONTRACT:
        case JUB_ENUM_TRX_CONTRACT_TYPE::UNFRZ_BLA_CONTRACT:
        case JUB_ENUM_TRX_CONTRACT_TYPE::TRIG_SMART_CONTRACT:
        case 32:
        case 33:
            transaction_test_TRX(contextID, root, choice);
            break;
        case 9:
            JUB_ClearContext(contextID);
            main_test();
        default:
            continue;
        }   // switch (choice) end
    }   // while (true) end
}


void get_address_pubkey_TRX(JUB_UINT16 contextID) {

    JUB_RV rv = JUBR_ERROR;

    cout << "[----------------------------------- HD Node -----------------------------------]" << endl;
    JUB_CHAR_PTR pubkey = nullptr;
    rv = JUB_GetMainHDNodeTRX(contextID, JUB_ENUM_PUB_FORMAT::HEX, &pubkey);
    cout << "[-] JUB_GetMainHDNodeTRX() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "MainXpub in  HEX format:  " << pubkey << endl;
    JUB_FreeMemory(pubkey);
    cout << endl;

    pubkey = nullptr;
    rv = JUB_GetMainHDNodeTRX(contextID, JUB_ENUM_PUB_FORMAT::XPUB, &pubkey);
    cout << "[-] JUB_GetMainHDNodeTRX() return " << GetErrMsg(rv) << endl;
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
    rv = JUB_GetHDNodeTRX(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &pubkey);
    cout << "[-] JUB_GetHDNodeTRX() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        cout << "  pubkey in  HEX format :  "<< pubkey << endl;
        JUB_FreeMemory(pubkey);
        cout << endl;
    }

    pubkey = nullptr;
    rv = JUB_GetHDNodeTRX(contextID, JUB_ENUM_PUB_FORMAT::XPUB, path, &pubkey);
    cout << "[-] JUB_GetHDNodeTRX() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "  pubkey in XPUB format :  " << pubkey << endl;
        JUB_FreeMemory(pubkey);
    }
    cout << "[--------------------------------- HD Node end ---------------------------------]" << endl;
    cout << endl << endl;

    cout << "[----------------------------------- Address -----------------------------------]" << endl;
    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressTRX(contextID, path, BOOL_FALSE, &address);
    cout << "[-] JUB_GetAddressTRX() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "         address: " << address << endl;
    JUB_FreeMemory(address);

    address = nullptr;
    rv = JUB_GetAddressTRX(contextID, path, BOOL_TRUE, &address);
    cout << "[-] JUB_GetAddressTRX() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "    show address: " << address << endl;

    JUB_CHAR_PTR addrInHex = nullptr;
    rv = JUB_CheckAddressTRX(contextID, address, &addrInHex);
    cout << "[-] JUB_CheckAddressTRX() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }

    cout << "  address in hex: " << addrInHex << endl;
    JUB_FreeMemory(address);
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
//         NS_ITEM_TRX_CONTRACT =  0,
//                XFER_CONTRACT =  1, // TransferContract(balance_contract.proto)
//          XFER_ASSET_CONTRACT =  2, // TransferAssetContract(asset_issue_contract.proto)
//             FRZ_BLA_CONTRACT = 11, // TransferContract(balance_contract.proto)
//           UNFRZ_BLA_CONTRACT = 12, // TransferContract(balance_contract.proto)
//        CREATE_SMART_CONTRACT = 30, // CreateSmartContract(smart_contract.proto)
//          TRIG_SMART_CONTRACT = 31, // TriggerSmartContract(smart_contract.proto)
//    } JUB_ENUM_TRX_CONTRACT_TYPE;
    int trc = 0;
    if (32 == choice) {
        trc = TRC_20;
        choice = JUB_ENUM_TRX_CONTRACT_TYPE::TRIG_SMART_CONTRACT;
    }
    else if (33 == choice) {
        trc = TRC_721;
        choice = JUB_ENUM_TRX_CONTRACT_TYPE::TRIG_SMART_CONTRACT;
    }

    std::string packedContractInPb;
    rv = pack_contract_proc(contextID, root,
                            choice,
                            packedContractInPb,
                            trc);
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
                          int trc) {

    JUB_RV rv = JUBR_OK;

    if (JUB_ENUM_TRX_CONTRACT_TYPE::XFER_ASSET_CONTRACT == choice) {
        string assetName = (JUB_CHAR_PTR)root["TRX"]["TRC10"]["assetName"].asCString();
        string assetID = (JUB_CHAR_PTR)root["TRX"]["TRC10"]["assetID"].asCString();
        JUB_UINT16 unitDP = root["TRX"]["TRC10"]["dp"].asUInt64();
        rv = JUB_SetTRC10Asset(contextID,
                               assetName.c_str(),
                               unitDP,
                               assetID.c_str());
        cout << "[-] JUB_SetTRC10Asset() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            return rv;
        }
        cout << endl;
    }

    JUB_CHAR_PTR trcAbi = nullptr;
    string contractAddress = "";
    switch (trc) {
    case TRC_20:
    {
        contractAddress   = (JUB_CHAR_PTR)root["TRX"]["TRC20"]["contract_address"].asCString();
        string tokenName  = (JUB_CHAR_PTR)root["TRX"]["TRC20"]["tokenName"].asCString();
        JUB_UINT16 unitDP = root["TRX"]["TRC20"]["dp"].asUInt64();

        rv = JUB_SetTRC20Token(contextID,
                               tokenName.c_str(),
                               unitDP,
                               contractAddress.c_str());
        cout << "[-] JUB_SetTRC20Token() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            return rv;
        }
        cout << endl;

        string tokenTo    = (JUB_CHAR_PTR)root["TRX"]["TRC20"]["token_to"].asCString();
        string tokenValue = (JUB_CHAR_PTR)root["TRX"]["TRC20"]["token_value"].asCString();
        rv = JUB_BuildTRC20TransferAbi(contextID,
                                       tokenTo.c_str(), tokenValue.c_str(),
                                       &trcAbi);
        cout << "[-] JUB_BuildTRC20TransferAbi() return " << GetErrMsg(rv) << endl;
        cout << "TRC-20  [" << strlen(trcAbi)/2 << "]: " << trcAbi << std::endl;
        break;
    }
    case TRC_721:
    {
        contractAddress   = (JUB_CHAR_PTR)root["TRX"]["TRC721"]["contract_address"].asCString();
        string tokenName  = (JUB_CHAR_PTR)root["TRX"]["TRC721"]["tokenName"].asCString();
        rv = JUB_SetTRC721Token(contextID,
                               tokenName.c_str(),
                               contractAddress.c_str());
        cout << "[-] JUB_SetTRC721Token() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            return rv;
        }
        cout << endl;

        string tokenFrom  = (JUB_CHAR_PTR)root["TRX"]["TRC721"]["token_from"].asCString();
        string tokenTo    = (JUB_CHAR_PTR)root["TRX"]["TRC721"]["token_to"].asCString();
        string tokenID    = (JUB_CHAR_PTR)root["TRX"]["TRC721"]["tokenID"].asCString();
        rv = JUB_BuildTRC721TransferAbi(contextID,
                                        tokenFrom.c_str(), tokenTo.c_str(), tokenID.c_str(),
                                        &trcAbi);
        cout << "[-] JUB_BuildTRC721TransferAbi() return " << GetErrMsg(rv) << endl;
        cout << "TRC-721 [" << strlen(trcAbi)/2 << "]: " << trcAbi << std::endl;
        break;
    }
    default:
        break;
    }
    if (JUBR_OK != rv) {
        return rv;
    }
    cout << endl;

    JUB_TX_TRX tx;
    tx.ref_block_bytes = (char*)root["TRX"]["pack"]["ref_block_bytes"].asCString();
    tx.ref_block_num = 0;
    tx.ref_block_hash = (char*)root["TRX"]["pack"]["ref_block_hash"].asCString();
    tx.data = nullptr;
    tx.expiration = (char*)root["TRX"]["pack"]["expiration"].asCString();
    tx.timestamp = (char*)root["TRX"]["pack"]["timestamp"].asCString();
    tx.fee_limit = (char*)"0";

    JUB_CONTRACT_TRX contrTRX;
    std::string strChoice = std::to_string((unsigned int)choice);
    const char* sType = strChoice.c_str();
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
    case JUB_ENUM_TRX_CONTRACT_TYPE::FRZ_BLA_CONTRACT:
    {
        contrTRX.freezeBalance.owner_address = (char*)root["TRX"]["contracts"]["owner_address"].asCString();
        contrTRX.freezeBalance.frozen_balance = root["TRX"]["contracts"][sType]["frozen_balance"].asUInt64();
        contrTRX.freezeBalance.frozen_duration = root["TRX"]["contracts"][sType]["frozen_duration"].asUInt64();
        contrTRX.freezeBalance.resource = (JUB_ENUM_RESOURCE_CODE)root["TRX"]["contracts"][sType]["resource"].asUInt64();
        contrTRX.freezeBalance.receiver_address = (char*)root["TRX"]["contracts"][sType]["receiver_address"].asCString();
        break;
    }
    case JUB_ENUM_TRX_CONTRACT_TYPE::UNFRZ_BLA_CONTRACT:
    {
        contrTRX.unfreezeBalance.owner_address = (char*)root["TRX"]["contracts"]["owner_address"].asCString();
        contrTRX.unfreezeBalance.resource = (JUB_ENUM_RESOURCE_CODE)root["TRX"]["contracts"][sType]["resource"].asUInt64();
        contrTRX.unfreezeBalance.receiver_address = (char*)root["TRX"]["contracts"][sType]["receiver_address"].asCString();
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
        switch (trc) {
        case TRC_20:
        case TRC_721:
            contrTRX.triggerSmart.contract_address = (char*)contractAddress.c_str();
            contrTRX.triggerSmart.data = trcAbi;
            break;
        default:
            contrTRX.triggerSmart.contract_address = (char*)root["TRX"]["contracts"][sType]["contract_address"].asCString();
            contrTRX.triggerSmart.data = (char*)root["TRX"]["contracts"][sType]["data"].asCString();
            break;
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
