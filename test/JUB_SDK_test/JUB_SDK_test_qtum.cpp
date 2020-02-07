//
//  JUB_SDK_test_qtum.cpp
//  JubSDKTest
//
//  Created by panmin on 2020/1/19.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
#include "JUB_SDK_test_dev.hpp"
#include "JUB_SDK_test_btc.hpp"
#include "JUB_SDK_test_qtum.hpp"

#include "JUB_SDK_main.h"


void transactionQTUM_test(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    rv = transactionQTUM_proc(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}

JUB_RV transactionQTUM_proc(JUB_UINT16 contextID, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

    try {
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

        OUTPUT_BTC QRC20_output;
        JUB_CHAR_CPTR contractAddress = (char*)root["QRC20_contractAddr"].asCString();
        JUB_UINT8 decimal = root["QRC20_decimal"].asUInt64();
        JUB_CHAR_CPTR symbol = (char*)root["QRC20_symbol"].asCString();
        JUB_UINT64 gasLimit = root["gasLimit"].asUInt64();
        JUB_UINT64 gasPrice = root["gasPrice"].asUInt64();
        JUB_CHAR_CPTR to = (char*)root["QRC20_to"].asCString();
        JUB_CHAR_CPTR value = (char*)root["QRC20_amount"].asCString();
        rv = JUB_BuildQRC20Outputs(contextID,
                                   contractAddress, decimal, symbol,
                                   gasLimit, gasPrice,
                                   to, value,
                                   &QRC20_output);
        if (JUBR_OK != rv) {
            cout << "JUB_BuildUSDTOutputs() return " << GetErrMsg(rv) << endl;
            return rv;
        }
        outputs.emplace_back(QRC20_output);

        char* raw = nullptr;
        rv = JUB_SignTransactionBTC(contextID, &inputs[0], (JUB_UINT16)inputs.size(), &outputs[0], (JUB_UINT16)outputs.size(), 0, &raw);
        cout << "JUB_SignTransactionBTC() return " << GetErrMsg(rv) << endl;

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
            cout << raw;
            JUB_FreeMemory(raw);
        }
    }
    catch (...) {
        error_exit("Error format json file\n");
    }

    return rv;
}

void QTUM_test(JUB_UINT16 deviceID, const char* json_file) {

    JUB_RV rv = JUBR_ERROR;

    Json::Value root = readJSON(json_file);
    JUB_UINT16 contextID = 0;

    try {
        CONTEXT_CONFIG_BTC cfg;
        cfg.mainPath = (char*)root["main_path"].asCString();
        cfg.coinType = COINQTUM;
        cfg.transType = p2pkh;

        rv = JUB_CreateContextBTC(cfg, deviceID, &contextID);
        if (JUBR_OK != rv) {
            cout << "JUB_CreateContextBTC() return " << GetErrMsg(rv) << endl;
            return;
        }
    }
    catch (...) {
        error_exit("Error format json file\n");
    }

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet QTUM ********|" << endl;
        cout << "| 1. get_address_test.               |" << endl;
        cout << "| 2. show_address_test.              |" << endl;
        cout << "| 3. transaction_test.               |" << endl;
        cout << "| 4. set_my_address_test.            |" << endl;
        cout << "| 5. set_timeout_test.               |" << endl;
        cout << "| 0. return.                         |" << endl;
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
                transactionQTUM_test(contextID, root);
                break;
            case 4:
                set_my_address_test_BTC(contextID);
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
