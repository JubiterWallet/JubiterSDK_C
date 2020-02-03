//
//  JUB_SDK_software_test_btc.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/9/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_software_test_btc.hpp"
#include "JUB_SDK_test_btc.hpp"

#include "mSIGNA/stdutils/uchar_vector.h"
#include "JUB_SDK_main.h"

void software_test_btc(CONTEXT_CONFIG_BTC cfg, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

    JUB_CHAR_PTR mnemonic = nullptr;
    rv = JUB_GenerateMnemonic(STRENGTH128, &mnemonic);
    if(rv == JUBR_OK) {
        cout << mnemonic << endl;
    }

    rv = JUB_CheckMnemonic(mnemonic);
    if(rv != JUBR_OK) {
        cout << "JUB_CheckMnemonic return" << rv << endl;
    }
    JUB_BYTE seed[64] = {0,};
    JUB_UINT16 seedLen = sizeof(seed)/sizeof(JUB_BYTE);
    auto callback = [](JUB_UINT32 current, JUB_UINT32 total) -> void {
        cout << ".";
    };

    rv = JUB_GenerateSeed("gauge hole clog property soccer idea cycle stadium utility slice hold chief", "", seed, callback);
    if (rv != JUBR_OK) {
        cout << "JUB_GenerateSeed error" << endl;
    }
    uchar_vector vSeed(seedLen);
    for (int i=0; i<seedLen; ++i) {
        vSeed[i] = seed[i];
    }
    cout << "seed: " << vSeed.getHex() << endl;
    cout << endl;

    JUB_CHAR_PTR masterXprv = nullptr;
    rv = JUB_SeedToMasterPrivateKey(seed, seedLen, secp256k1, &masterXprv);
    if (rv == JUBR_OK) {
        cout << masterXprv << endl;
    }

    JUB_UINT16 contextID;
    rv = JUB_CreateContextBTC_soft(cfg, masterXprv, &contextID);
    if (rv != JUBR_OK) {
        cout << "JUB_CreateContextBTC_soft return " << rv << endl;
    }

    BIP44_Path path;
    path.change = BOOL_FALSE;
    path.addressIndex = 0;
    JUB_CHAR_PTR  xpub = nullptr;
    rv = JUB_GetHDNodeBTC(contextID, path, &xpub);
    if (rv == JUBR_OK) {
        cout << xpub << endl;
        JUB_FreeMemory(xpub);
    }

    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressBTC(contextID, path, BOOL_FALSE, &address);
    if(rv == JUBR_OK) {
        cout << address << endl;
        JUB_FreeMemory(address);
    }

    rv = transaction_proc(contextID, root);
}

void software_test_btc() {

    while (true) {
        cout << "-------------------------------------------" << endl;
        cout << "|********** Jubiter Wallet Test **********|" << endl;
        cout << "|   2. LTC_test.                          |" << endl;
        cout << "|  31. BTC_test.                          |" << endl;
        cout << "|  32. BTC_segwit_test.                   |" << endl;
        cout << "|  39. BTC_USDT_test.                     |" << endl;
        cout << "| 145. BCH_test.                          |" << endl;
        cout << "|  88.QTUM_QRC20_test.                    |" << endl;
        cout << "|2301.QTUM_test.                          |" << endl;
        cout << "|   0. exit.                              |" << endl;
        cout << "-------------------------------------------" << endl;
        cout << "* Please enter your choice:" << endl;

        int choice = 0;
        cin >> choice;

        const char* json_file;
        CONTEXT_CONFIG_BTC cfg;
        switch (choice) {
            case 2:
            {
                cfg.coinType = COINLTC;
                cfg.transType = p2sh_p2wpkh;
                json_file = "json/testLTC.json";
                break;
            }
            case 31:
            {
                cfg.coinType = COINBTC;
                cfg.transType = p2pkh;
                json_file = "json/testBTC44.json";
                break;
            }
            case 32:
            {
                cfg.coinType = COINBTC;
                cfg.transType = p2sh_p2wpkh;
                json_file = "json/testBTC49.json";
                break;
            }
            case 39:
            {
                cfg.coinType = COINUSDT;
                cfg.transType = p2pkh;
                json_file = "json/testBTC44.json";
                break;
            }
            case 145:
            {
                cfg.coinType = COINBCH;
                cfg.transType = p2pkh;
                json_file = "json/testBCH.json";
                break;
            }
            case 88:
            {
                cfg.coinType = COINQTUM;
                cfg.transType = p2pkh;
                json_file = "json/testQTUM.json";
                break;
            }
            case 2301:
            {
                cfg.coinType = COINQTUM;
                cfg.transType = p2pkh;
                json_file = "json/testQTUM_qrc20.json";
                break;
            }
            case 0:
                exit(0);
            default:
                continue;
        }

        Json::Value root = readJSON(json_file);

        cfg.mainPath = (char*)root["main_path"].asCString();

        software_test_btc(cfg, root);
    }
}
