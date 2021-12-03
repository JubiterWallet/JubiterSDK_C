//
//  JUB_SDK_test_btc.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/9/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_test_btc.hpp"
#include "JUB_SDK_test.h"
#include "JUB_SDK_test_dev.hpp"

#include "JUB_SDK_main.h"
#include "mSIGNA/stdutils/uchar_vector.h"
#include <cstring>
#include <iostream>
#include <vector>

void BTC_test(JUB_UINT16 deviceID, JUB_CHAR_CPTR json_file, JUB_ENUM_COINTYPE_BTC coinType) {

    JUB_RV rv = JUBR_ERROR;

    Json::Value root = readJSON(json_file);
    if (root.empty()) {
        return;
    }

    JUB_UINT16 contextID = 0;

    CONTEXT_CONFIG_BTC cfg;
    cfg.mainPath = (JUB_CHAR_PTR)root["main_path"].asCString();
    cfg.coinType = coinType;

    if (COINBCH == coinType) {
        cfg.transType = p2pkh;
    } else {
        if (root["p2sh-segwit"].asBool()) {
            cfg.transType = p2sh_p2wpkh;
        } else if (root["p2pkh-segwit"].asBool()) {
            cfg.transType = p2wpkh;
        } else if (root["p2tr"].asBool()) {
            cfg.transType = p2tr;
        } else {
            cfg.transType = p2pkh;
        }
    }

    rv = JUB_CreateContextBTC(cfg, deviceID, &contextID);
    cout << "[-] JUB_CreateContextBTC() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << endl;

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet BTC  ********|" << endl;
        cout << "| 1.    get_address_test.            |" << endl;
        cout << "| 2.   show_address_test.            |" << endl;
        cout << "|                                    |" << endl;
        cout << "| 3.    transaction_test.            |" << endl;
        cout << "|                                    |" << endl;
        cout << "| 4. set_my_address_test.            |" << endl;
        cout << "| 5.    set_timeout_test.            |" << endl;
        cout << "|                                    |" << endl;
        cout << "| 9. return.                         |" << endl;
        cout << "--------------------------------------" << endl;
        cout << "* Please enter your choice:" << endl;

        int choice = 0;
        cin >> choice;

        switch (choice) {
        case 1:
            get_address_test(contextID, root);
            break;
        case 2:
            show_address_test(contextID);
            break;
        case 3:
            transaction_test(contextID, root);
            break;
        case 4:
            set_my_address_test_BTC(contextID);
            break;
        case 5:
            set_timeout_test(contextID);
            break;
        case 9:
            JUB_ClearContext(contextID);
            main_test();
        default:
            continue;
        } // switch (choice) end
    }     // while (true) end
}

void get_address_test(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

    cout << "[----------------------------------- HD Node -----------------------------------]" << endl;
    JUB_CHAR_PTR pubkey = nullptr;
    rv                  = JUB_GetMainHDNodeBTC(contextID, &pubkey);
    cout << "[-] JUB_GetMainHDNodeBTC() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "MainXpub in  HEX format:  " << pubkey << endl;
    JUB_FreeMemory(pubkey);
    cout << endl;

    int inputNumber = root["inputs"].size();
    for (int i = 0; i < inputNumber; i++) {
        JUB_CHAR_PTR xpub;

        BIP44_Path path;
        path.change       = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
        path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();

        JUB_RV rv = JUB_GetHDNodeBTC(contextID, path, &xpub);
        cout << "[-] JUB_GetHDNodeBTC() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            break;
        }
        cout << "    input " << i << " xpub : " << xpub << endl;
        JUB_FreeMemory(xpub);
        cout << endl;

        cout << "[----------------------------------- Address -----------------------------------]" << endl;
        JUB_CHAR_PTR address = nullptr;
        rv                   = JUB_GetAddressBTC(contextID, path, BOOL_FALSE, &address);
        cout << "[-] JUB_GetAddressBTC() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            break;
        }
        cout << "    input " << i << " address : " << address << endl;
        JUB_FreeMemory(address);
        cout << "[--------------------------------- Address end ---------------------------------]" << endl;
        cout << endl << endl;
    } // for (int i = 0; i < inputNumber; i++) end
    cout << "[--------------------------------- HD Node end ---------------------------------]" << endl;
    cout << endl << endl;

    if (JUBR_OK != rv) {
        return;
    }
}

void show_address_test(JUB_UINT16 contextID) {

    JUB_RV rv = JUBR_ERROR;

    int change       = 0;
    JUB_UINT64 index = 0;
    cout << "please input change level (non-zero means 1):" << endl;
    cin >> change;
    cout << "please input index " << endl;
    cin >> index;

    BIP44_Path path;
    path.change       = JUB_ENUM_BOOL(change);
    path.addressIndex = index;

    cout << "[----------------------------------- Address -----------------------------------]" << endl;
    JUB_CHAR_PTR address = nullptr;
    rv                   = JUB_GetAddressBTC(contextID, path, BOOL_FALSE, &address);
    cout << "[-] JUB_GetAddressBTC() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "         address: " << address << endl;
    JUB_FreeMemory(address);

    rv = JUB_GetAddressBTC(contextID, path, BOOL_TRUE, &address);
    cout << "[-] JUB_GetAddressBTC() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "    show address: " << address << endl;

    rv = JUB_CheckAddressBTC(contextID, address);
    cout << "[-] JUB_CheckAddressBTC() return " << GetErrMsg(rv) << endl;

    JUB_FreeMemory(address);
    cout << "[--------------------------------- Address end ---------------------------------]" << endl;
    cout << endl << endl;
}

void set_my_address_test_BTC(JUB_UINT16 contextID) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    int change       = 0;
    JUB_UINT64 index = 0;
    cout << "please input change level (non-zero means 1):" << endl;
    cin >> change;
    cout << "please input index " << endl;
    cin >> index;

    BIP44_Path path;
    path.change       = JUB_ENUM_BOOL(change);
    path.addressIndex = index;

    JUB_CHAR_PTR address = nullptr;
    rv                   = JUB_SetMyAddressBTC(contextID, path, &address);
    cout << "[-] JUB_SetMyAddressBTC() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    } else {
        cout << "    set my address is : " << address << endl;
        JUB_FreeMemory(address);
    }
}

void transaction_test(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

    JUB_ENUM_BTC_UNIT_TYPE unit = mBTC;

    cout << "Please input BTCunit on JubiterBLD" << endl;
    cout << "1: BTC" << endl;
    cout << "2: cBTC" << endl;
    cout << "3: mBTC" << endl;
    cout << "4: uBTC" << endl;
    cout << "5: Satoshi" << endl;

    int choice = 0;
    cin >> choice;

    switch (choice) {
    case 2:
        unit = cBTC;
        break;
    case 3:
        unit = mBTC;
        break;
    case 4:
        unit = uBTC;
        break;
    case 5:
        unit = Satoshi;
        break;
    case 1:
    default:
        unit = BTC;
        break;
    } // switch (choice) end

    rv = JUB_SetUnitBTC(contextID, unit);
    cout << "[-] JUB_SetUnitBTC() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv && JUBR_IMPL_NOT_SUPPORT != rv) {
        return;
    }

    JUB_ENUM_DEVICE deviceClass = JUB_ENUM_DEVICE::DEVICE_NS_ITEM;
    rv                          = JUB_GetContextType(contextID, &deviceClass);
    if (JUBR_OK != rv) {
        return;
    }

    if (JUB_ENUM_DEVICE::VD != deviceClass) {
        cout << "Please chose the way of verify PIN:" << endl;
        cout << "1: PIN" << endl;
        cout << "2: Fingerprint" << endl;
        choice = 0;
        cin >> choice;

        switch (choice) {
        case 2:
            rv = verify_fgpt(contextID);
            break;
        case 1:
        default:
            rv = verify_pin(contextID);
            break;
        } // switch (choice) end
        if (JUBR_OK != rv) {
            return;
        }
    }

    rv = transaction_proc(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}

JUB_RV transaction_proc(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

    JUB_UINT32 version = root["ver"].asInt();

    std::vector<INPUT_BTC> inputs;
    std::vector<OUTPUT_BTC> outputs;
    int inputNumber = root["inputs"].size();

    for (int i = 0; i < inputNumber; i++) {
        INPUT_BTC input;
        input.type              = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
        input.preHash           = (JUB_CHAR_PTR)root["inputs"][i]["preHash"].asCString();
        input.preIndex          = root["inputs"][i]["preIndex"].asInt();
        input.path.change       = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
        input.path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();
        input.amount            = root["inputs"][i]["amount"].asUInt64();
        input.nSequence         = 0xffffffff;
        inputs.push_back(input);
    }

    int outputNumber = root["outputs"].size();

    for (int i = 0; i < outputNumber; i++) {
        OUTPUT_BTC output;
        output.type                    = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
        output.stdOutput.address       = (JUB_CHAR_PTR)root["outputs"][i]["address"].asCString();
        output.stdOutput.amount        = root["outputs"][i]["amount"].asUInt64();
        output.stdOutput.changeAddress = (JUB_ENUM_BOOL)root["outputs"][i]["change_address"].asBool();
        if (output.stdOutput.changeAddress) {
            output.stdOutput.path.change       = (JUB_ENUM_BOOL)root["outputs"][i]["bip32_path"]["change"].asBool();
            output.stdOutput.path.addressIndex = root["outputs"][i]["bip32_path"]["addressIndex"].asInt();
        }
        outputs.push_back(output);
    }

    JUB_CHAR_PTR raw = nullptr;
    rv               = JUB_SignTransactionBTC(contextID, version, &inputs[0], (JUB_UINT16)inputs.size(), &outputs[0],
                                              (JUB_UINT16)outputs.size(), 0, &raw);
    cout << "[-] JUB_SignTransactionBTC() return " << GetErrMsg(rv) << endl;

    if (JUBR_USER_CANCEL == rv) {
        cout << "    User cancel the transaction !" << endl;
        return rv;
    }
    if (JUBR_OK != rv || nullptr == raw) {
        cout << "    error sign tx" << endl;
        return rv;
    }
    if (raw) {
        cout << "    BTC/LTC/DASH/... raw[" << strlen(raw) << "]: " << raw << endl;
        JUB_FreeMemory(raw);
    }

    return rv;
}
