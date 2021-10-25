//
//  JUB_SDK_test_ckb.cpp
//  JubSDKTest
//
//  Created by panmin on 2021/6/15.
//  Copyright © 2021 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
#include "JUB_SDK_test_dev.hpp"
#include "JUB_SDK_test_ckb.hpp"

#include "JUB_SDK_main.h"


void get_address_test_CKB(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

    cout << "[----------------------------------- HD Node -----------------------------------]" << endl;
    JUB_CHAR_PTR pubkey = nullptr;
    rv = JUB_GetMainHDNodeCKB(contextID, &pubkey);
    cout << "[-] JUB_GetMainHDNodeCKB() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "MainXpub in  HEX format:  " << pubkey << endl;
    JUB_FreeMemory(pubkey);
    cout << endl;

    int inputNumber = root["inputs"].size();
    for (int i = 0; i < inputNumber; i++) {
        JUB_CHAR_PTR xpub = nullptr;

        BIP44_Path path;
        path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
        path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();

        rv = JUB_GetHDNodeCKB(contextID, path, &xpub);
        cout << "[-] JUB_GetHDNodeCKB() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            break;
        }
        cout << "    input in  HEX" << i << " xpub : " << xpub << endl;
        JUB_FreeMemory(xpub);
        cout << endl;

        cout << "[----------------------------------- Address -----------------------------------]" << endl;
        JUB_CHAR_PTR address = nullptr;
        rv = JUB_GetAddressCKB(contextID, path, BOOL_FALSE, &address);
        cout << "[-] JUB_GetAddressCKB() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            break;
        }
        cout << "    input " << i << " address : " << address << endl;

        rv = JUB_CheckAddressCKB(contextID, address);
        cout << "[-] JUB_CheckAddressCKB() return " << GetErrMsg(rv) << endl;
        JUB_FreeMemory(address);
        if (JUBR_OK != rv) {
            break;
        }
        cout << "[--------------------------------- Address end ---------------------------------]" << endl;
        cout << endl << endl;
    }   // for (int i = 0; i < inputNumber; i++) end
    cout << "[--------------------------------- HD Node end ---------------------------------]" << endl;
    cout << endl << endl;

    if (JUBR_OK != rv) {
        return;
    }
}


void show_address_test_CKB(JUB_UINT16 contextID) {

    JUB_RV rv = JUBR_ERROR;

    int change = 0;
    JUB_UINT64 index = 0;
    cout << "please input change level (non-zero means 1):" << endl;
    cin >> change;
    cout << "please input index " << endl;
    cin >> index;

    BIP44_Path path;
    path.change = JUB_ENUM_BOOL(change);
    path.addressIndex = index;

    cout << "[----------------------------------- Address -----------------------------------]" << endl;
    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressCKB(contextID, path, BOOL_TRUE, &address);
    cout << "[-] JUB_GetAddressCKB() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "    show address: " << address << endl;

    rv = JUB_CheckAddressCKB(contextID, address);
    cout << "[-] JUB_CheckAddressCKB() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }

    JUB_INT32 format = 0, codeHashIndex = 0;
    JUB_CHAR_PTR codeHash = nullptr, args = nullptr;
    rv = JUB_IsValidAddressCKB(address,
                               &format,
                               &codeHashIndex,
                               &codeHash,
                               &args);
    cout << "[-] JUB_IsValidAddressCKB() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "address info(format, codeHashIndex): " << format << ", " << codeHashIndex << endl;
    cout << "address info(codeHash)[" << strlen(codeHash)/2 << "]: " << codeHash << endl;
    cout << "address info(    args)[" << strlen(args)/2 << "]: " << args << endl;
    JUB_FreeMemory(codeHash);
    JUB_FreeMemory(args);

    JUB_FreeMemory(address);
    cout << "[--------------------------------- Address end ---------------------------------]" << endl;
    cout << endl << endl;

    cout << "[----------------------------------- HD Node -----------------------------------]" << endl;
    JUB_CHAR_PTR xpub = nullptr;
    rv = JUB_GetHDNodeCKB(contextID, path, &xpub);
    cout << "[-] JUB_GetHDNodeCKB() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "            xpub: " << xpub << endl;
    JUB_FreeMemory(xpub);
    cout << "[--------------------------------- HD Node end ---------------------------------]" << endl;
}


void transactionCKB_test(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    rv = transactionCKB_proc(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}


JUB_RV transactionCKB_proc(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

    JUB_UINT32 version = root["ver"].asInt();

    JUB_UINT16 depNumber = root["deps"].size();
    JUB_UINT16 inputNumber = root["inputs"].size();
    JUB_UINT16 outputNumber = root["outputs"].size();

    std::vector<CELL_DEP> deps;
    std::vector<CELL_INPUT> inputs;
    std::vector<CELL_OUTPUT> outputs;

    deps.clear();
    for (int i = 0; i < depNumber; i++) {
        CELL_DEP dep;
        dep.type = root["deps"][i]["depType"].asInt();
        dep.txHash = (JUB_CHAR_PTR)root["deps"][i]["outPoint"]["txHash"].asCString();
        dep.index  = root["deps"][i]["outPoint"]["index"].asUInt();
        deps.push_back(dep);
    }

    inputs.clear();
    for (int i = 0; i < inputNumber; i++) {
        CELL_INPUT input;
        input.preHash = (JUB_CHAR_PTR)root["inputs"][i]["previousOutput"]["txHash"].asCString();
        input.preIndex = root["inputs"][i]["previousOutput"]["index"].asUInt();

        input.since = atoi((JUB_CHAR_PTR)root["inputs"][i]["since"].asCString());

        input.capacity = root["inputs"][i]["capacity"].asUInt64();

        input.lock.codeHash = (JUB_CHAR_PTR)root["inputs"][i]["lock"]["codeHash"].asCString();
        input.lock.hashType = root["inputs"][i]["lock"]["hashType"].asInt();
        input.lock.args = (JUB_CHAR_PTR)root["inputs"][i]["lock"]["args"].asCString();

        input.path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
        input.path.addressIndex = (JUB_UINT64)root["inputs"][i]["bip32_path"]["addressIndex"].asInt();

        inputs.push_back(input);
    }

    outputs.clear();
    for (int i = 0; i < outputNumber; i++) {
        CELL_OUTPUT output;
        output.capacity = root["outputs"][i]["capacity"].asUInt64();

        output.lock.codeHash = (JUB_CHAR_PTR)root["outputs"][i]["lock"]["codeHash"].asCString();
        output.lock.hashType = root["outputs"][i]["lock"]["hashType"].asInt();
        output.lock.args = (JUB_CHAR_PTR)root["outputs"][i]["lock"]["args"].asCString();

        if (!root["outputs"][i]["type"].empty()) {
            output.type.codeHash = (JUB_CHAR_PTR)root["outputs"][i]["type"]["codeHash"].asCString();
            output.type.hashType = root["outputs"][i]["type"]["hashType"].asInt();
            output.type.args = (JUB_CHAR_PTR)root["outputs"][i]["type"]["args"].asCString();
        }
        else {
            output.type.codeHash = nullptr;
            output.type.hashType = 0;
            output.type.args = nullptr;
        }

        output.data = (JUB_CHAR_PTR)root["outputs"][i]["data"].asCString();

        outputs.push_back(output);
    }

    char* raw = nullptr;
    rv = JUB_SignTransactionCKB(contextID,
                                version,
                                &deps[0],       depNumber,
                                &inputs[0],   inputNumber,
                                &outputs[0], outputNumber,
                                &raw);
    cout << "[-] JUB_SignTransactionCKB() return " << GetErrMsg(rv) << endl;

    if (JUBR_USER_CANCEL == rv) {
        cout << "User cancel the transaction !" << endl;
        return rv;
    }
    if (   JUBR_OK != rv
        || nullptr == raw
        ) {
        cout << "error sign tx" << endl;
        return rv;
    }
    if (raw) {
        cout << "Nervos CKB raw[" << strlen(raw) << "]: " << raw << endl;
        JUB_FreeMemory(raw);
    }

    return rv;
}


void CKB_test(JUB_UINT16 deviceID, JUB_CHAR_CPTR json_file) {

    JUB_RV rv = JUBR_ERROR;

    Json::Value root = readJSON(json_file);
    if (root.empty()) {
        return ;
    }

    JUB_UINT16 contextID = 0;

    CONTEXT_CONFIG_CKB cfg;
    cfg.mainPath = (char*)root["main_path"].asCString();
    cfg.mainNet = (JUB_ENUM_BOOL)root["main_net"].asBool();

    rv = JUB_CreateContextCKB(cfg, deviceID, &contextID);
    cout << "[-] JUB_CreateContextCKB() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << endl;

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|***** Jubiter Wallet Nervos CKB ****|" << endl;
        cout << "| 1.    get_address_test.            |" << endl;
        cout << "| 2.   show_address_test.            |" << endl;
        cout << "|                                    |" << endl;
        cout << "| 3.    transaction_test.            |" << endl;
        cout << "|                                    |" << endl;
        cout << "| 9. return.                         |" << endl;
        cout << "--------------------------------------" << endl;
        cout << "* Please enter your choice:" << endl;

        int choice = 0;
        cin >> choice;

        switch (choice) {
        case 1:
            get_address_test_CKB(contextID, root);
            break;
        case 2:
            show_address_test_CKB(contextID);
            break;
        case 3:
            transactionCKB_test(contextID, root);
            break;
        case 9:
            JUB_ClearContext(contextID);
            main_test();
        default:
            continue;
        }   // switch (choice) end
    }   // while (true) end
}
