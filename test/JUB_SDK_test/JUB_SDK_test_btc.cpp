//
//  JUB_SDK_test_btc.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/9/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
#include "JUB_SDK_test_dev.hpp"
#include "JUB_SDK_test_btc.hpp"

#include "JUB_SDK_main.h"
#include "mSIGNA/stdutils/uchar_vector.h"

void BTC_test(JUB_UINT16 deviceID, const char* json_file, JUB_ENUM_COINTYPE_BTC coinType) {

    JUB_RV rv = JUBR_ERROR;

    JUB_ENUM_COMMODE commode;
    JUB_ENUM_DEVICE deviceClass;
    rv = JUB_GetDeviceType(deviceID,
                           &commode, &deviceClass);
    cout << "[-] JUB_GetDeviceType() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }

    switch (commode) {
    case JUB_ENUM_COMMODE::HID:
    case JUB_ENUM_COMMODE::BLE:
    case JUB_ENUM_COMMODE::NFC:
    case JUB_ENUM_COMMODE::SIM:
    {
        char* appList;
        rv = JUB_EnumApplets(deviceID, &appList);
        cout << "[-] JUB_EnumApplets() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            return;
        }

        break;
    }
    case JUB_ENUM_COMMODE::SWI:
    default:
        break;
    }

    Json::Value root = readJSON(json_file);
    JUB_UINT16 contextID = 0;

    try {
        CONTEXT_CONFIG_BTC cfg;
        cfg.mainPath = (char*)root["main_path"].asCString();
        cfg.coinType = coinType;

        if (COINBCH == coinType) {
            cfg.transType = p2pkh;
        }
        else {
            if (root["p2sh-segwit"].asBool()) {
                cfg.transType = p2sh_p2wpkh;
            }
            else {
                cfg.transType = p2pkh;
            }
        }

        rv = JUB_CreateContextBTC(cfg, deviceID, &contextID);
        cout << "[-] JUB_CreateContextBTC() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            return;
        }
        cout << endl;
    }
    catch (...) {
        error_exit("Error format json file\n");
    }

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet BTC  ********|" << endl;
        cout << "| 1. get_address_test.               |" << endl;
        cout << "| 2. show_address_test.              |" << endl;
        cout << "| 3. transaction_test.               |" << endl;
        cout << "| 4. set_my_address_test.            |" << endl;
        cout << "| 5. set_timeout_test.               |" << endl;
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
            main_test();
        default:
            continue;
        }   // switch (choice) end
    }   // while (true) end
}

void get_address_test(JUB_UINT16 contextID, Json::Value root) {

    try {
        JUB_CHAR_PTR mainXpub;
        JUB_RV rv = JUB_GetMainHDNodeBTC(contextID, &mainXpub);
        cout << "[-] JUB_GetMainHDNodeBTC() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            return;
        }

        cout << "    Main xpub : " << mainXpub << endl;
        JUB_FreeMemory(mainXpub);

        int inputNumber = root["inputs"].size();
        for (int i = 0; i < inputNumber; i++) {
            JUB_CHAR_PTR xpub;

            BIP44_Path path;
            path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
            path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();

            JUB_RV rv = JUB_GetHDNodeBTC(contextID, path, &xpub);
            cout << "[-] JUB_GetHDNodeBTC() return " << GetErrMsg(rv) << endl;
            if (JUBR_OK != rv) {
                break;
            }

            cout << "    input " << i << " xpub : " << xpub << endl;
            JUB_FreeMemory(xpub);

            JUB_CHAR_PTR address;
            rv = JUB_GetAddressBTC(contextID, path, BOOL_FALSE, &address);
            cout << "[-] JUB_GetAddressBTC() return " << GetErrMsg(rv) << endl;
            if (JUBR_OK != rv) {
                break;
            }
            cout << "    input " << i << " address : " << address << endl;
            JUB_FreeMemory(address);
        }   // for (int i = 0; i < inputNumber; i++) end
        if (JUBR_OK != rv) {
            return;
        }
    }
    catch (...) {
        error_exit("Error format json file\n");
    }
}

void show_address_test(JUB_UINT16 contextID) {

    int change = 0;
    JUB_UINT64 index = 0;
    cout << "please input change level (non-zero means 1):" << endl;
    cin >> change;
    cout << "please input index " << endl;
    cin >> index;

    BIP44_Path path;
    path.change = JUB_ENUM_BOOL(change);
    path.addressIndex = index;

    JUB_CHAR_PTR address;
    JUB_RV rv = JUB_GetAddressBTC(contextID, path, BOOL_TRUE, &address);
    cout << "[-] JUB_GetAddressBTC() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "    show address: " << address << endl;
    rv = JUB_CheckAddressBTC(contextID, address);
    cout << "[-] JUB_CheckAddressBTC() return " << GetErrMsg(rv) << endl;

    JUB_FreeMemory(address);
}

void set_my_address_test_BTC(JUB_UINT16 contextID) {

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
    rv = JUB_SetMyAddressBTC(contextID, path, &address);
    cout << "[-] JUB_SetMyAddressBTC() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    else {
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
    }   // switch (choice) end

    rv = JUB_SetUnitBTC(contextID, unit);
    cout << "[-] JUB_SetUnitBTC() return " << GetErrMsg(rv) << endl;
    if (   JUBR_OK               != rv
        && JUBR_IMPL_NOT_SUPPORT != rv
        ) {
        return;
    }

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
    }   // switch (choice) end
    if (JUBR_OK != rv) {
        return;
    }

    rv = transaction_proc(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}

JUB_RV transaction_proc(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

    try {
        JUB_UINT32 version = root["ver"].asInt();

        std::vector<INPUT_BTC> inputs;
        std::vector<OUTPUT_BTC> outputs;
        int inputNumber = root["inputs"].size();

        for (int i = 0; i < inputNumber; i++) {
            INPUT_BTC input;
            input.type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
            input.preHash = (char*)root["inputs"][i]["preHash"].asCString();
            input.preIndex = root["inputs"][i]["preIndex"].asInt();
            input.path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
            input.path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();
            input.amount = root["inputs"][i]["amount"].asUInt64();
            input.nSequence = 0xffffffff;
            inputs.push_back(input);
        }

        int outputNumber = root["outputs"].size();

        for (int i = 0; i < outputNumber; i++) {
            OUTPUT_BTC output;
            output.type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
            output.stdOutput.address = (char*)root["outputs"][i]["address"].asCString();
            output.stdOutput.amount = root["outputs"][i]["amount"].asUInt64();
            output.stdOutput.changeAddress = (JUB_ENUM_BOOL)root["outputs"][i]["change_address"].asBool();
            if (output.stdOutput.changeAddress) {
                output.stdOutput.path.change = (JUB_ENUM_BOOL)root["outputs"][i]["bip32_path"]["change"].asBool();
                output.stdOutput.path.addressIndex = root["outputs"][i]["bip32_path"]["addressIndex"].asInt();
            }
            outputs.push_back(output);
        }

        char* raw = nullptr;
        rv = JUB_SignTransactionBTC(contextID, version, &inputs[0], (JUB_UINT16)inputs.size(), &outputs[0], (JUB_UINT16)outputs.size(), 0, &raw);
        cout << "[-] JUB_SignTransactionBTC() return " << GetErrMsg(rv) << endl;

        if (JUBR_USER_CANCEL == rv) {
            cout << "    User cancel the transaction !" << endl;
            return rv;
        }
        if (   JUBR_OK != rv
            || nullptr == raw
            ) {
            cout << "    error sign tx" << endl;
            return rv;
        }
        if (raw) {
            cout << "    BTC/LTC/DASH/... raw[" << strlen(raw) << "]: " << raw << endl;
            JUB_FreeMemory(raw);
        }
    }
    catch (...) {
        error_exit("Error format json file\n");
        return rv;
    }

    return rv;
}

void transactionUSDT_test(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    rv = transactionUSDT_proc(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}

JUB_RV transactionUSDT_proc(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

    try {
        JUB_UINT32 version = root["ver"].asInt();

        std::vector<INPUT_BTC> inputs;
        std::vector<OUTPUT_BTC> outputs;
        int inputNumber = root["inputs"].size();

        for (int i = 0; i < inputNumber; i++) {
            INPUT_BTC input;
            input.type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
            input.preHash = (char*)root["inputs"][i]["preHash"].asCString();
            input.preIndex = root["inputs"][i]["preIndex"].asInt();
            input.path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
            input.path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();
            input.amount = root["inputs"][i]["amount"].asUInt64();
            input.nSequence = 0xffffffff;
            inputs.push_back(input);
        }

        int outputNumber = root["outputs"].size();

        for (int i = 0; i < outputNumber; i++) {
            OUTPUT_BTC output;
            output.type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
            output.stdOutput.address = (char*)root["outputs"][i]["address"].asCString();
            output.stdOutput.amount = root["outputs"][i]["amount"].asUInt64();
            output.stdOutput.changeAddress = (JUB_ENUM_BOOL)root["outputs"][i]["change_address"].asBool();
            if (output.stdOutput.changeAddress) {
                output.stdOutput.path.change = (JUB_ENUM_BOOL)root["outputs"][i]["bip32_path"]["change"].asBool();
                output.stdOutput.path.addressIndex = root["outputs"][i]["bip32_path"]["addressIndex"].asInt();

                JUB_CHAR_PTR selfAddress;
                rv = JUB_GetAddressBTC(contextID, output.stdOutput.path, JUB_ENUM_BOOL::BOOL_FALSE, &selfAddress);
                cout << "[-] JUB_GetAddressBTC() return " << GetErrMsg(rv) << endl;
                if (JUBR_OK != rv) {
                    return rv;
                }
                output.stdOutput.address = selfAddress;
            }
            outputs.push_back(output);
        }

        OUTPUT_BTC USDT_outputs[2] = {};
        rv = JUB_BuildUSDTOutputs(contextID, (char*)root["USDT_to"].asCString(), root["USDT_amount"].asUInt64(), USDT_outputs);
        cout << "[-] JUB_BuildUSDTOutputs() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            return rv;
        }
        for (int i=0; i<2; ++i) {
            switch (USDT_outputs[i].type) {
            case JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH:
                std::cout << "    JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH:" << std::endl;
                std::cout << "    address: " << USDT_outputs[i].stdOutput.address << std::endl;
                std::cout << "    amount: " << USDT_outputs[i].stdOutput.amount << std::endl;
                std::cout << "    change: " << USDT_outputs[i].stdOutput.changeAddress << std::endl;
                std::cout << "    addressIndex: " << USDT_outputs[i].stdOutput.path.addressIndex << std::endl;
                if (USDT_outputs[i].stdOutput.changeAddress) {
                    JUB_CHAR_PTR selfAddress;
                    rv = JUB_GetAddressBTC(contextID, USDT_outputs[i].stdOutput.path, JUB_ENUM_BOOL::BOOL_FALSE, &selfAddress);
                    cout << "[-] JUB_GetAddressBTC() return " << GetErrMsg(rv) << endl;
                    if (JUBR_OK != rv) {
                        return rv;
                    }
                    USDT_outputs[i].stdOutput.address = selfAddress;
                }
                break;
            case JUB_ENUM_SCRIPT_BTC_TYPE::RETURN0:
                std::cout << "    JUB_ENUM_SCRIPT_BTC_TYPE::RETURN0:" << std::endl;
                std::cout << "    return0: " << uchar_vector(USDT_outputs[i].return0.data, USDT_outputs[i].return0.dataLen).getHex() << std::endl;
                std::cout << "    amount: " << USDT_outputs[i].return0.amount << std::endl;
                break;
            default:
                break;
            }
        }
        outputs.emplace_back(USDT_outputs[0]);
        outputs.emplace_back(USDT_outputs[1]);

        char* raw = nullptr;
        rv = JUB_SignTransactionBTC(contextID, version, &inputs[0], (JUB_UINT16)inputs.size(), &outputs[0], (JUB_UINT16)outputs.size(), 0, &raw);
        cout << "[-] JUB_SignTransactionBTC() return " << GetErrMsg(rv) << endl;

        if (JUBR_USER_CANCEL == rv) {
            cout << "    User cancel the transaction !" << endl;
            return rv;
        }
        if (   JUBR_OK != rv
            || nullptr == raw
            ) {
            cout << "    error sign tx" << endl;
            return rv;
        }
        if (raw) {
            cout << "    USDT raw[" << strlen(raw) << "]: " << raw << endl;
            JUB_FreeMemory(raw);
        }
    }
    catch (...) {
        error_exit("Error format json file\n");
    }

    return JUBR_OK;
}

void USDT_test(JUB_UINT16 deviceID, const char* json_file) {

    JUB_RV rv = JUBR_ERROR;

    JUB_ENUM_COMMODE commode;
    JUB_ENUM_DEVICE deviceClass;
    rv = JUB_GetDeviceType(deviceID,
                           &commode, &deviceClass);
    cout << "[-] JUB_GetDeviceType() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }

    switch (commode) {
    case JUB_ENUM_COMMODE::HID:
    case JUB_ENUM_COMMODE::BLE:
    case JUB_ENUM_COMMODE::NFC:
    case JUB_ENUM_COMMODE::SIM:
    {
        char* appList;
        rv = JUB_EnumApplets(deviceID, &appList);
        cout << "[-] JUB_EnumApplets() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            return;
        }

        break;
    }
    case JUB_ENUM_COMMODE::SWI:
    default:
        break;
    }

    Json::Value root = readJSON(json_file);
    JUB_UINT16 contextID = 0;

    try {
        CONTEXT_CONFIG_BTC cfg;
        cfg.mainPath = (char*)root["main_path"].asCString();
        cfg.coinType = COINUSDT;
        cfg.transType = p2pkh;

        rv = JUB_CreateContextBTC(cfg, deviceID, &contextID);
        cout << "[-] JUB_CreateContextBTC() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            return;
        }
        cout << endl;
    }
    catch (...) {
        error_exit("Error format json file\n");
    }

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet USDT ********|" << endl;
        cout << "| 1. get_address_test.               |" << endl;
        cout << "| 2. show_address_test.              |" << endl;
        cout << "| 3. transaction_test.               |" << endl;
        cout << "| 4. set_my_address_test.            |" << endl;
        cout << "| 5. set_timeout_test.               |" << endl;
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
            transactionUSDT_test(contextID, root);
            break;
        case 4:
            set_my_address_test_BTC(contextID);
            break;
        case 5:
            set_timeout_test(contextID);
            break;
        case 9:
            main_test();
        default:
            continue;
        }   // switch (choice) end
    }   // while (true) end
}
