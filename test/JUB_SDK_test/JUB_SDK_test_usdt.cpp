//
//  JUB_SDK_test_usdt.cpp
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


JUB_RV transactionUSDT_proc(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

    JUB_UINT32 version = root["ver"].asInt();

    std::vector<INPUT_BTC> inputs;
    std::vector<OUTPUT_BTC> outputs;
    int inputNumber = root["inputs"].size();

    for (int i = 0; i < inputNumber; i++) {
        INPUT_BTC input;
        input.type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
        input.preHash = (JUB_CHAR_PTR)root["inputs"][i]["preHash"].asCString();
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
        output.stdOutput.address = (JUB_CHAR_PTR)root["outputs"][i]["address"].asCString();
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
    rv = JUB_BuildUSDTOutputs(contextID, (JUB_CHAR_PTR)root["USDT_to"].asCString(), root["USDT_amount"].asUInt64(), USDT_outputs);
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

    JUB_CHAR_PTR raw = nullptr;
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

    return JUBR_OK;
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


void USDT_test(JUB_UINT16 deviceID, JUB_CHAR_CPTR json_file) {

    JUB_RV rv = JUBR_ERROR;

    Json::Value root = readJSON(json_file);
    if (root.empty()) {
        return ;
    }

    JUB_UINT16 contextID = 0;

    CONTEXT_CONFIG_BTC cfg;
    cfg.mainPath = (JUB_CHAR_PTR)root["main_path"].asCString();
    cfg.coinType = COINUSDT;
    cfg.transType = p2pkh;

    rv = JUB_CreateContextBTC(cfg, deviceID, &contextID);
    cout << "[-] JUB_CreateContextBTC() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << endl;

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet USDT ********|" << endl;
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
            get_address_test(contextID, root);
            break;
        case 2:
            show_address_test(contextID);
            break;
        case 3:
            transactionUSDT_test(contextID, root);
            break;
        case 9:
            JUB_ClearContext(contextID);
            main_test();
        default:
            continue;
        }   // switch (choice) end
    }   // while (true) end
}
