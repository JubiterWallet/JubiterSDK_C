//
//  JUB_SDK_test_eth.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/9/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
#include "JUB_SDK_test_dev.hpp"
#include "JUB_SDK_test_eth.hpp"

#include "JUB_SDK_main.h"


void ETH_test(JUB_UINT16 deviceID, JUB_CHAR_CPTR json_file) {

    JUB_RV rv = JUBR_ERROR;

    Json::Value root = readJSON(json_file);
    if (root.empty()) {
        return ;
    }

    JUB_UINT16 contextID = 0;

    CONTEXT_CONFIG_ETH cfg;
    cfg.mainPath = (JUB_CHAR_PTR)root["main_path"].asCString();
    cfg.chainID = root["chainID"].asInt();
    rv = JUB_CreateContextETH(cfg, deviceID, &contextID);
    cout << "[-] JUB_CreateContextETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << endl;

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet ETH  ********|" << endl;
        cout << "| 0.       show_address_pubkey_test. |" << endl;
        cout << "|                                    |" << endl;
        cout << "| 2.               transaction_test. |" << endl;
        cout << "|                                    |" << endl;
        cout << "| 3.         transaction_ERC20_test. |" << endl;
        cout << "|                                    |" << endl;
        cout << "| 4.        transaction_ERC721_test. |" << endl;
        cout << "|                                    |" << endl;
        cout << "| 5.       transaction_ERC1155_test. |" << endl;
        cout << "| 6. batch_transaction_ERC1155_test. |" << endl;
        cout << "|                                    |" << endl;
        cout << "| 7.         transaction_contr_test. |" << endl;
        cout << "| 8.                bytestring_test. |" << endl;
//        cout << "| 8.            set_my_address_test. |" << endl;
//        cout << "| 8.               set_timeout_test. |" << endl;
        cout << "| 9. return.                         |" << endl;
        cout << "--------------------------------------" << endl;
        cout << "* Please enter your choice:" << endl;

        int choice = 0;
        cin >> choice;

        switch (choice) {
        case 0:
            get_address_pubkey_ETH(contextID);
            break;
        case 2:
            transaction_test_ETH(contextID, root);
            break;
        case 3:
            transaction_test_ERC20_ETH(contextID, root);
            break;
        case 4:
            transaction_test_ERC721_ETH(contextID, root);
            break;
        case 5:
            transaction_test_ERC1155_ETH(contextID, root);
            break;
        case 6:
            batch_transaction_test_ERC1155_ETH(contextID, root);
            break;
        case 7:
            transaction_test_contr_ETH(contextID, root);
            break;
        case 8:
            bytestring_test_ETH(contextID, root);
            break;
//        case 8:
//            set_my_address_test_ETH(contextID);
//            break;
//        case 8:
//            set_timeout_test(contextID);
//            break;
        case 9:
            JUB_ClearContext(contextID);
            main_test();
        default:
            continue;
        }   // switch (choice) end
    }   // while (true) end
}


void set_my_address_test_ETH(JUB_UINT16 contextID) {

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
    rv = JUB_SetMyAddressETH(contextID, path, &address);
    cout << "[-] JUB_SetMyAddressETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    else {
        cout << "    set my address is : " << address << endl;
        JUB_FreeMemory(address);
    }
}


void get_address_pubkey_ETH(JUB_UINT16 contextID) {

    JUB_RV rv = JUBR_ERROR;

    cout << "[----------------------------------- HD Node -----------------------------------]" << endl;
    JUB_CHAR_PTR pubkey = nullptr;
    rv = JUB_GetMainHDNodeETH(contextID, JUB_ENUM_PUB_FORMAT::HEX, &pubkey);
    cout << "[-] JUB_GetMainHDNodeETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "MainXpub in  HEX format:  " << pubkey << endl;
    JUB_FreeMemory(pubkey);
    cout << endl;

    pubkey = nullptr;
    rv = JUB_GetMainHDNodeETH(contextID, JUB_ENUM_PUB_FORMAT::XPUB, &pubkey);
    cout << "[-] JUB_GetMainHDNodeETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "MainXpub in XPUB format:  " << pubkey << endl;
    JUB_FreeMemory(pubkey);
    cout << endl;

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
    rv = JUB_GetHDNodeETH(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &pubkey);
    cout << "[-] JUB_GetHDNodeETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "  pubkey in  HEX format :  "<< pubkey << endl;
    JUB_FreeMemory(pubkey);
    cout << endl;

    pubkey = nullptr;
    rv = JUB_GetHDNodeETH(contextID, JUB_ENUM_PUB_FORMAT::XPUB, path, &pubkey);
    cout << "[-] JUB_GetHDNodeETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "  pubkey in XPUB format :  " << pubkey << endl;
    JUB_FreeMemory(pubkey);
    cout << "[--------------------------------- HD Node end ---------------------------------]" << endl;
    cout << endl << endl;

    cout << "[----------------------------------- Address -----------------------------------]" << endl;
    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressETH(contextID, path, BOOL_FALSE, &address);
    cout << "[-] JUB_GetAddressETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "         address: " << address << endl;
    JUB_FreeMemory(address);

    address = nullptr;
    rv = JUB_GetAddressETH(contextID, path, BOOL_TRUE, &address);
    cout << "[-] JUB_GetAddressETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "    show address: " << address << endl;
    JUB_FreeMemory(address);
    cout << "[--------------------------------- Address end ---------------------------------]" << endl;
    cout << endl << endl;
}


void transaction_test_ETH(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    rv = transaction_proc_ETH(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}


JUB_RV transaction_proc_ETH(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["ETH"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["ETH"]["bip32_path"]["addressIndex"].asUInt();

    //ETH Test
    uint32_t    nonce = root["ETH"]["nonce"].asUInt();//.asDouble();
    uint32_t gasLimit = root["ETH"]["gasLimit"].asUInt();//.asDouble();
    JUB_CHAR_PTR gasPriceInWei = (JUB_CHAR_PTR)root["ETH"]["gasPriceInWei"].asCString();
    JUB_CHAR_PTR    valueInWei = (JUB_CHAR_PTR)root["ETH"]["valueInWei"].asCString();
    JUB_CHAR_PTR   to = (JUB_CHAR_PTR)root["ETH"]["to"].asCString();
    JUB_CHAR_PTR data = (JUB_CHAR_PTR)root["ETH"]["data"].asCString();

    JUB_CHAR_PTR raw = nullptr;
    rv = JUB_SignTransactionETH(contextID, path, nonce, gasLimit, gasPriceInWei, to, valueInWei, data, &raw);
    cout << "[-] JUB_SignTransactionETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return rv;
    }
    else {
        cout << "    ETH raw[" << strlen(raw) << "]: "  << raw << endl;
        JUB_FreeMemory(raw);
    }

    return rv;
}


//ERC-20 Test
void transaction_test_ERC20_ETH(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    rv = transaction_proc_ERC20_ETH(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}


JUB_RV transaction_proc_ERC20_ETH(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

    JUB_CHAR_PTR tokenName = (JUB_CHAR_PTR)root["ERC20"]["tokenName"].asCString();
    JUB_UINT16 unitDP = root["ERC20"]["dp"].asUInt();
    JUB_CHAR_PTR contractAddress = (JUB_CHAR_PTR)root["ERC20"]["contract_address"].asCString();

    rv = JUB_SetERC20TokenETH(contextID,
                              tokenName, unitDP, contractAddress);
    cout << "[-] JUB_SetERC20TokenETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return rv;
    }

    JUB_CHAR_PTR to = (JUB_CHAR_PTR)root["ERC20"]["contract_address"].asCString();
    JUB_CHAR_PTR token_to = (JUB_CHAR_PTR)root["ERC20"]["token_to"].asCString();
    JUB_CHAR_PTR token_value = (JUB_CHAR_PTR)root["ERC20"]["token_value"].asCString();
    JUB_CHAR_PTR abi = nullptr;
    rv = JUB_BuildERC20TransferAbiETH(contextID,
                                      token_to, token_value, &abi);
    cout << "[-] JUB_BuildERC20TransferAbiETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return rv;
    }

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["ERC20"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["ERC20"]["bip32_path"]["addressIndex"].asUInt();
    uint32_t nonce = root["ERC20"]["nonce"].asUInt();//.asDouble();
    uint32_t gasLimit = root["ERC20"]["gasLimit"].asUInt();//.asDouble();
    JUB_CHAR_PTR gasPriceInWei = (JUB_CHAR_PTR)root["ERC20"]["gasPriceInWei"].asCString();
    JUB_CHAR_PTR valueInWei = nullptr; //"" and "0" ara also OK
    JUB_CHAR_PTR raw = nullptr;
    rv = JUB_SignTransactionETH(contextID, path, nonce, gasLimit, gasPriceInWei, to, valueInWei, abi, &raw);
    cout << "[-] JUB_SignTransactionETH() return " << GetErrMsg(rv) << endl;
    JUB_FreeMemory(abi);
    if (JUBR_OK != rv) {
        return rv;
    }
    else {
        cout << raw << endl;
        JUB_FreeMemory(raw);
    }

    return rv;
}


//ERC-721 Test
void transaction_test_ERC721_ETH(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    rv = transaction_proc_ERC721_ETH(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}


JUB_RV transaction_proc_ERC721_ETH(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

//    rv = JUB_GetAddressETH(contextID, path, JUB_ENUM_BOOL::BOOL_FALSE, &token_from);
//    cout << "[-] JUB_GetAddressETH() return " << GetErrMsg(rv) << endl;
//    if (JUBR_OK != rv) {
//        return rv;
//    }

    JUB_CHAR_PTR tokenName = (JUB_CHAR_PTR)root["ERC721"]["tokenName"].asCString();
    JUB_CHAR_PTR contractAddress = (JUB_CHAR_PTR)root["ERC721"]["contract_address"].asCString();
    rv = JUB_SetERC721TokenETH(contextID,
                               tokenName, contractAddress);
    cout << "[-] JUB_SetERC721TokenETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return rv;
    }

    JUB_CHAR_PTR token_from = (JUB_CHAR_PTR)root["ERC721"]["token_from"].asCString();
    JUB_CHAR_PTR to = (JUB_CHAR_PTR)root["ERC721"]["contract_address"].asCString();
    JUB_CHAR_PTR token_to = (JUB_CHAR_PTR)root["ERC721"]["token_to"].asCString();
    JUB_CHAR_PTR tokenID = (JUB_CHAR_PTR)root["ERC721"]["tokenID"].asCString();
    JUB_CHAR_PTR abi = nullptr;
    rv = JUB_BuildERC721TransferAbiETH(contextID,
                                       token_from, token_to, tokenID,
                                       &abi);
    cout << "[-] JUB_BuildERC721TransferAbiETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return rv;
    }
    cout << "ERC-721 abi[" << strlen(abi)/2 << "]: " << abi << std::endl;

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["ERC721"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["ERC721"]["bip32_path"]["addressIndex"].asUInt();

    uint32_t nonce = root["ERC721"]["nonce"].asUInt();//.asDouble();
    uint32_t gasLimit = root["ERC721"]["gasLimit"].asUInt();//.asDouble();
    JUB_CHAR_PTR gasPriceInWei = (JUB_CHAR_PTR)root["ERC721"]["gasPriceInWei"].asCString();
    JUB_CHAR_PTR valueInWei = nullptr; //"" and "0" ara also OK
    JUB_CHAR_PTR raw = nullptr;
    rv = JUB_SignTransactionETH(contextID, path, nonce, gasLimit, gasPriceInWei, to, valueInWei, abi, &raw);
    cout << "[-] JUB_SignTransactionETH() return " << GetErrMsg(rv) << endl;
    JUB_FreeMemory(abi);
    if (JUBR_OK != rv) {
        return rv;
    }
    else {
        cout << raw << endl;
        JUB_FreeMemory(raw);
    }

    return rv;
}


//ERC-1155 Test
void transaction_test_ERC1155_ETH(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    rv = transaction_proc_ERC1155_ETH(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}


JUB_RV transaction_proc_ERC1155_ETH(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

    JUB_CHAR_PTR tokenName = (JUB_CHAR_PTR)root["ERC1155"]["tokenName"].asCString();
    JUB_CHAR_PTR contractAddress = (JUB_CHAR_PTR)root["ERC1155"]["contract_address"].asCString();
    rv = JUB_SetERC721TokenETH(contextID,
                               tokenName, contractAddress);
    cout << "[-] JUB_SetERC721TokenETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return rv;
    }

    JUB_CHAR_PTR token_from = (JUB_CHAR_PTR)root["ERC1155"]["token_from"].asCString();
    JUB_CHAR_PTR to = (JUB_CHAR_PTR)root["ERC1155"]["contract_address"].asCString();
    JUB_CHAR_PTR token_to = (JUB_CHAR_PTR)root["ERC1155"]["token_to"].asCString();
    JUB_CHAR_PTR tokenID = (JUB_CHAR_PTR)root["ERC1155"]["tokenID"].asCString();
    JUB_CHAR_PTR tokenValue = (JUB_CHAR_PTR)root["ERC1155"]["token_value"].asCString();
    JUB_CHAR_PTR tokenData = (JUB_CHAR_PTR)root["ERC1155"]["data"].asCString();
    JUB_CHAR_PTR abi = nullptr;
    rv = JUB_BuildERC1155TransferAbiETH(contextID,
                                        token_from, token_to, tokenID, tokenValue, tokenData,
                                        &abi);
    cout << "[-] JUB_BuildERC1155TransferAbiETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return rv;
    }
    cout << "ERC-1155 abi[" << strlen(abi)/2 << "]: " << abi << std::endl;

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["ERC1155"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["ERC1155"]["bip32_path"]["addressIndex"].asUInt();

    uint32_t nonce = root["ERC1155"]["nonce"].asUInt();//.asDouble();
    uint32_t gasLimit = root["ERC1155"]["gasLimit"].asUInt();//.asDouble();
    JUB_CHAR_PTR gasPriceInWei = (JUB_CHAR_PTR)root["ERC1155"]["gasPriceInWei"].asCString();
    JUB_CHAR_PTR valueInWei = nullptr; //"" and "0" ara also OK
    JUB_CHAR_PTR raw = nullptr;
    rv = JUB_SignTransactionETH(contextID, path, nonce, gasLimit, gasPriceInWei, to, valueInWei, abi, &raw);
    cout << "[-] JUB_SignTransactionETH() return " << GetErrMsg(rv) << endl;
    JUB_FreeMemory(abi);
    if (JUBR_OK != rv) {
        return rv;
    }
    else {
        cout << raw << endl;
        JUB_FreeMemory(raw);
    }

    return rv;
}


//ERC-1155 Batch Test
void batch_transaction_test_ERC1155_ETH(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    rv = batch_transaction_proc_ERC1155_ETH(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}


JUB_RV batch_transaction_proc_ERC1155_ETH(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

    JUB_CHAR_PTR tokenName = (JUB_CHAR_PTR)root["ERC1155Batch"]["tokenName"].asCString();
    JUB_CHAR_PTR contractAddress = (JUB_CHAR_PTR)root["ERC1155Batch"]["contract_address"].asCString();
    rv = JUB_SetERC721TokenETH(contextID,
                               tokenName, contractAddress);
    cout << "[-] JUB_SetERC721TokenETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return rv;
    }

    JUB_CHAR_PTR token_from = (JUB_CHAR_PTR)root["ERC1155Batch"]["token_from"].asCString();
    JUB_CHAR_PTR to = (JUB_CHAR_PTR)root["ERC1155Batch"]["contract_address"].asCString();
    JUB_CHAR_PTR token_to = (JUB_CHAR_PTR)root["ERC1155Batch"]["token_to"].asCString();
    std::vector<JUB_CHAR_CPTR> tokenIDs;
    int tokenIDNumber = root["ERC1155Batch"]["tokenIDs"].size();
    for (int i = 0; i < tokenIDNumber; i++) {
        tokenIDs.push_back((JUB_CHAR_PTR)root["ERC1155Batch"]["tokenIDs"][i].asCString());
    }
    std::vector<JUB_CHAR_CPTR> values;
    int valueNumber = root["ERC1155Batch"]["token_values"].size();
    for (int i = 0; i < valueNumber; i++) {
        values.push_back((JUB_CHAR_PTR)root["ERC1155Batch"]["token_values"][i].asCString());
    }
    JUB_CHAR_PTR tokenData = (JUB_CHAR_PTR)root["ERC1155Batch"]["data"].asCString();

    JUB_CHAR_PTR abi = nullptr;
    rv = JUB_BuildERC1155BatchTransferAbiETH(contextID,
                                             token_from, token_to,
                                             &tokenIDs[0], tokenIDs.size(),
                                             &values[0], values.size(),
                                             tokenData,
                                             &abi);
    cout << "[-] JUB_BuildERC1155BatchTransferAbiETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return rv;
    }
    cout << "ERC-1155 abi[" << strlen(abi)/2 << "]: " << abi << std::endl;

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["ERC1155Batch"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["ERC1155Batch"]["bip32_path"]["addressIndex"].asUInt();

    uint32_t nonce = root["ERC1155Batch"]["nonce"].asUInt();//.asDouble();
    uint32_t gasLimit = root["ERC1155Batch"]["gasLimit"].asUInt();//.asDouble();
    JUB_CHAR_PTR gasPriceInWei = (JUB_CHAR_PTR)root["ERC1155Batch"]["gasPriceInWei"].asCString();
    JUB_CHAR_PTR valueInWei = nullptr; //"" and "0" ara also OK
    JUB_CHAR_PTR raw = nullptr;
    rv = JUB_SignTransactionETH(contextID, path, nonce, gasLimit, gasPriceInWei, to, valueInWei, abi, &raw);
    cout << "[-] JUB_SignTransactionETH() return " << GetErrMsg(rv) << endl;
    JUB_FreeMemory(abi);
    if (JUBR_OK != rv) {
        return rv;
    }
    else {
        cout << raw << endl;
        JUB_FreeMemory(raw);
    }

    return rv;
}


//contract Test
void transaction_test_contr_ETH(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    rv = transaction_proc_contr_ETH(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}


JUB_RV transaction_proc_contr_ETH(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["contract"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["contract"]["bip32_path"]["addressIndex"].asUInt();

    uint32_t nonce = root["contract"]["nonce"].asUInt();//.asDouble();
    uint32_t gasLimit = root["contract"]["gasLimit"].asUInt();//.asDouble();
    JUB_CHAR_PTR gasPriceInWei = (JUB_CHAR_PTR)root["contract"]["gasPriceInWei"].asCString();
    JUB_CHAR_PTR valueInWei = nullptr; //"" and "0" ara also OK
    JUB_CHAR_PTR to   = (JUB_CHAR_PTR)root["contract"]["to"].asCString();
    JUB_CHAR_PTR abi  = (JUB_CHAR_PTR)root["contract"]["data"].asCString();;

    JUB_CHAR_PTR raw = nullptr;
    rv = JUB_SignContractETH(contextID, path, nonce, gasLimit, gasPriceInWei, to, valueInWei, abi, &raw);
    if (JUBR_OK != rv) {
        cout << "[-] JUB_SignContractETH() return " << GetErrMsg(rv) << endl;
        return rv;
    }
    else {
        cout << "    raw[" << strlen(raw) << "]: " << raw << endl;
        JUB_FreeMemory(raw);
    }

    return rv;
}


//bytestring Test
void bytestring_test_ETH(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    rv = bytestring_proc_ETH(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}


JUB_RV bytestring_proc_ETH(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["ETH"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["ETH"]["bip32_path"]["addressIndex"].asUInt();

    //ETH Test
    JUB_CHAR_PTR data = (JUB_CHAR_PTR)root["Bytestring"]["data"].asCString();

    JUB_CHAR_PTR raw = nullptr;
    rv = JUB_SignBytestringETH(contextID, path, data, &raw);
    cout << "[-] JUB_SignBytestringETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return rv;
    }
    else {
        cout << "    raw[" << strlen(raw) << "]: "  << raw << endl;
        JUB_FreeMemory(raw);
    }

    return rv;
}


//uniswap Test
void uniswap_test_ETH(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    rv = uniswap_proc_ETH(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}


JUB_RV uniswap_proc_ETH(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

    JUB_CHAR_PTR uniswip = (JUB_CHAR_PTR)root["Uniswap"]["method"].asCString();

    JUB_UINT16 count = root["Uniswap"][uniswip]["tokens"].size();
    ERC20_TOKEN_INFO* tokens = new ERC20_TOKEN_INFO[count];
    memset(tokens, 0, count);
    for (JUB_UINT16 i=0; i<count; ++i) {
        tokens[i].contractAddress = (JUB_CHAR_PTR)root["Uniswap"][uniswip]["tokens"][i]["contract_address"].asCString();
        tokens[i].unitDP          = root["Uniswap"][uniswip]["tokens"][i]["dp"].asUInt();
        tokens[i].tokenName       = (JUB_CHAR_PTR)root["Uniswap"][uniswip]["tokens"][i]["tokenName"].asCString();
    }
    if (1 == count) {
        rv = JUB_SetERC20TokenETH(contextID,
                                  tokens[0].tokenName, tokens[0].unitDP, tokens[0].contractAddress);
        cout << "[-] JUB_SetERC20TokenETH() return " << GetErrMsg(rv) << endl;
    }
    else {
        rv = JUB_SetERC20TokensETH(contextID,
                                   tokens, count);
        cout << "[-] JUB_SetERC20TokensETH() return " << GetErrMsg(rv) << endl;
    }
    if (JUBR_OK != rv) {
        delete [] tokens; tokens = nullptr;
        return rv;
    }
    delete [] tokens; tokens = nullptr;

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["Uniswap"][uniswip]["bip32_path"]["change"].asBool();
    path.addressIndex = root["Uniswap"][uniswip]["bip32_path"]["addressIndex"].asUInt();

    uint32_t nonce    = root["Uniswap"][uniswip]["nonce"].asUInt();//.asDouble();
    uint32_t gasLimit = root["Uniswap"][uniswip]["gasLimit"].asUInt();//.asDouble();
    JUB_CHAR_PTR gasPriceInWei = (JUB_CHAR_PTR)root["Uniswap"][uniswip]["gasPriceInWei"].asCString();
    JUB_CHAR_PTR valueInWei = (JUB_CHAR_PTR)root["Uniswap"][uniswip]["valueInWei"].asCString();
    JUB_CHAR_PTR to   = (JUB_CHAR_PTR)root["Uniswap"][uniswip]["to"].asCString();
    JUB_CHAR_PTR abi  = (JUB_CHAR_PTR)root["Uniswap"][uniswip]["data"].asCString();

    JUB_CHAR_PTR raw = nullptr;
    rv = JUB_SignTransactionETH(contextID, path,
                                nonce, gasLimit, gasPriceInWei,
                                to, valueInWei, abi,
                                &raw);
    cout << "[-] JUB_SignTransactionETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return rv;
    }
    else {
        cout << "    raw[" << strlen(raw) << "]: "  << raw << endl;
        JUB_FreeMemory(raw);
    }

    return rv;
}
