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
        }
        outputs.push_back(output);
    }

    OUTPUT_BTC QRC20_output;
    JUB_CHAR_CPTR contractAddress = (JUB_CHAR_PTR)root["QRC20_contractAddr"].asCString();
    JUB_UINT8 decimal = root["QRC20_decimal"].asUInt64();
    JUB_CHAR_CPTR symbol = (JUB_CHAR_PTR)root["QRC20_symbol"].asCString();
    JUB_UINT64 gasLimit = root["gasLimit"].asUInt64();
    JUB_UINT64 gasPrice = root["gasPrice"].asUInt64();
    JUB_CHAR_CPTR to = (JUB_CHAR_PTR)root["QRC20_to"].asCString();
    JUB_CHAR_CPTR value = (JUB_CHAR_PTR)root["QRC20_amount"].asCString();
    rv = JUB_BuildQRC20Outputs(contextID,
                               contractAddress, decimal, symbol,
                               gasLimit, gasPrice,
                               to, value,
                               &QRC20_output);
    cout << "[-] JUB_BuildQRC20Outputs() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return rv;
    }
    outputs.emplace_back(QRC20_output);

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
        cout << "    QTUM raw[" << strlen(raw) << "]: " << raw << endl;
        JUB_FreeMemory(raw);
    }

    return rv;
}

void QTUM_test(JUB_UINT16 deviceID, JUB_CHAR_CPTR json_file) {

    JUB_RV rv = JUBR_ERROR;

    Json::Value root = readJSON(json_file);
    if (root.empty()) {
        return ;
    }

    JUB_UINT16 contextID = 0;

    CONTEXT_CONFIG_BTC cfg;
    cfg.mainPath = (JUB_CHAR_PTR)root["main_path"].asCString();
    cfg.coinType = COINQTUM;
    cfg.transType = p2pkh;

    rv = JUB_CreateContextBTC(cfg, deviceID, &contextID);
    cout << "[-] JUB_CreateContextBTC() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << endl;

    while (true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet QTUM ********|" << endl;
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
            transactionQTUM_test(contextID, root);
            break;
        case 9:
            JUB_ClearContext(contextID);
            main_test();
        default:
            continue;
        }   // switch (choice) end
    }   // while (true) end
}
