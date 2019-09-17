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

void ETH_test(const char* json_file) {

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

    CONTEXT_CONFIG_ETH cfg;
    cfg.mainPath = (char*)root["main_path"].asCString();
    cfg.chainID = root["chainID"].asInt();
    JUB_UINT16 contextID = 0;
    rv = JUB_CreateContextETH(cfg, deviceIDs[0], &contextID);
    if (JUBR_OK != rv) {
        cout << "JUB_CreateContextETH() return " << GetErrMsg(rv) << endl;
        return;
    }

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet ETH  ********|" << endl;
        cout << "| 1. show_address_pubkey_test.       |" << endl;
        cout << "| 2. transaction_test.               |" << endl;
        cout << "| 3. transaction_ERC20_test.         |" << endl;
        cout << "| 4. set_my_address_test.            |" << endl;
        cout << "| 5. set_timeout_test.               |" << endl;
        cout << "| 0. return.                         |" << endl;
        cout << "--------------------------------------" << endl;
        cout << "* Please enter your choice:" << endl;

        int choice = 0;
        cin >> choice;

        switch (choice) {
            case 1:
                get_address_pubkey_ETH(contextID);
                break;
            case 2:
                transaction_test_ETH(contextID, root);
                break;
            case 3:
                transaction_ERC20_ETH(contextID, root);
                break;
            case 4:
                set_my_address_test_ETH(contextID);
                break;
            case 5:
                set_timeout_test(contextID);
                break;
            case 0:
                main_test();
            default:
                continue;
        }
    }
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

    BIP32_Path path;
    path.change = JUB_ENUM_BOOL(change);
    path.addressIndex = index;

    JUB_CHAR_PTR address = nullptr;
    rv = JUB_SetMyAddressETH(contextID, path, &address);
    if (JUBR_OK != rv) {
        cout << "JUB_SetMyAddressETH() return " << GetErrMsg(rv) << endl;
        return;
    }
    else {
        cout << "set my address is : " << address << endl;
        JUB_FreeMemory(address);
    }
}

void get_address_pubkey_ETH(JUB_UINT16 contextID) {

    int change = 0;
    JUB_UINT64 index = 0;
    cout << "please input change level (non-zero means 1):" << endl;
    cin >> change;
    cout << "please input index " << endl;
    cin >> index;

    BIP32_Path path;
    path.change = JUB_ENUM_BOOL(change);
    path.addressIndex = index;

    char* pubkey = nullptr;
    JUB_RV rv = JUB_GetMainHDNodeETH(contextID, HEX, &pubkey);
    if (JUBR_OK != rv) {
        cout << "JUB_GetMainHDNodeETH() return " << GetErrMsg(rv) << endl;
        return;
    }

    cout << "MainXpub in  hex format :  " << pubkey << endl;
    JUB_FreeMemory(pubkey);

    pubkey = nullptr;
    rv = JUB_GetMainHDNodeETH(contextID, XPUB, &pubkey);
    if (JUBR_OK != rv) {
        cout << "JUB_GetMainHDNodeETH() return " << GetErrMsg(rv) << endl;
        return;
    }

    cout << "MainXpub in  xpub format :  " << pubkey << endl;
    JUB_FreeMemory(pubkey);

    pubkey = nullptr;
    rv = JUB_GetHDNodeETH(contextID, HEX, path, &pubkey);
    if (JUBR_OK != rv) {
        cout << "JUB_GetHDNodeETH() return " << GetErrMsg(rv) << endl;
        return;
    }

    cout << "pubkey in  hex format :  "<< pubkey << endl;
    JUB_FreeMemory(pubkey);

    pubkey = nullptr;
    rv = JUB_GetHDNodeETH(contextID, XPUB, path, &pubkey);
    if (JUBR_OK != rv) {
        cout << "JUB_GetHDNodeETH() return " << GetErrMsg(rv) << endl;
        return;
    }

    cout << "pubkey in xpub format :  " << pubkey << endl;
    JUB_FreeMemory(pubkey);

    char* address = nullptr;
    rv = JUB_GetAddressETH(contextID, path, BOOL_TRUE, &address);
    if (JUBR_OK != rv) {
        cout << "JUB_GetAddressETH() return " << GetErrMsg(rv) << endl;
        return;
    }
    cout << "address: " << address << endl;
    JUB_FreeMemory(address);
}

void transaction_test_ETH(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    BIP32_Path path;
    path.change = (JUB_ENUM_BOOL)root["ETH"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["ETH"]["bip32_path"]["addressIndex"].asUInt();

    //ETH Test
    uint32_t nonce = root["ETH"]["nonce"].asUInt();//.asDouble();
    uint32_t gasLimit = root["ETH"]["gasLimit"].asUInt();//.asDouble();
    char* gasPriceInWei = (char*)root["ETH"]["gasPriceInWei"].asCString();
    char* valueInWei = (char*)root["ETH"]["valueInWei"].asCString();
    char* to = (char*)root["ETH"]["to"].asCString();
    char* data = (char*)root["ETH"]["data"].asCString();

    char* raw = nullptr;
    rv = JUB_SignTransactionETH(contextID, path, nonce, gasLimit, gasPriceInWei, to, valueInWei, data, &raw);
    if (JUBR_OK != rv) {
        cout << "JUB_SignTransactionETH() return " << GetErrMsg(rv) << endl;
        return;
    }
    else {
        cout << "raw : " << raw << endl;
        JUB_FreeMemory(raw);
    }
}

//ERC-20 Test
void transaction_ERC20_ETH(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    uint32_t nonce = root["ERC20"]["nonce"].asUInt();//.asDouble();
    uint32_t gasLimit = root["ERC20"]["gasLimit"].asUInt();//.asDouble();
    char* gasPriceInWei = (char*)root["ERC20"]["gasPriceInWei"].asCString();
    char* to = (char*)root["ERC20"]["contract_address"].asCString();
    char* token_to = (char*)root["ERC20"]["token_to"].asCString();
    char* token_value = (char*)root["ERC20"]["token_value"].asCString();

    BIP32_Path path;
    path.change = (JUB_ENUM_BOOL)root["ERC20"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["ERC20"]["bip32_path"]["addressIndex"].asUInt();

    char* abi = nullptr;
    rv = JUB_BuildERC20AbiETH(contextID, token_to, token_value, &abi);
    if (JUBR_OK != rv) {
        cout << "JUB_BuildERC20AbiETH() return " << GetErrMsg(rv) << endl;
        return;
    }

    char* raw = nullptr;
    rv = JUB_SignTransactionETH(contextID, path, nonce, gasLimit, gasPriceInWei, to, nullptr, abi, &raw);

    JUB_FreeMemory(abi);
    if (JUBR_OK != rv) {
        cout << "JUB_SignTransactionETH() return " << GetErrMsg(rv) << endl;
        return;
    }
    else {
        cout << raw << endl;
        JUB_FreeMemory(raw);
    }
}
