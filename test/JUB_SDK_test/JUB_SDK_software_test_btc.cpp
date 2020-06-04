//
//  JUB_SDK_software_test_btc.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/9/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_software_test_btc.hpp"
#include "JUB_SDK_test_btc.hpp"
#include "JUB_SDK_test_qtum.hpp"

#include "mSIGNA/stdutils/uchar_vector.h"
#include "JUB_SDK_main.h"

void software_test_btc(CONTEXT_CONFIG_BTC cfg, Json::Value root, bool isQRC20=false) {

    JUB_RV rv = JUBR_ERROR;

    JUB_CHAR_PTR mnemonic = nullptr;
    rv = JUB_GenerateMnemonic_soft(STRENGTH128, &mnemonic);
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

    rv = JUB_GenerateSeed_soft("gauge hole clog property soccer idea cycle stadium utility slice hold chief", "", seed, callback);
    if (rv != JUBR_OK) {
        cout << "JUB_GenerateSeed_soft error" << endl;
    }
    uchar_vector vSeed(seedLen);
    for (int i=0; i<seedLen; ++i) {
        vSeed[i] = seed[i];
    }
    cout << "seed: " << vSeed.getHex() << endl;
    cout << endl;

    JUB_CHAR_PTR masterXprv = nullptr;
    rv = JUB_SeedToMasterPrivateKey_soft(seed, seedLen,
                                         JUB_ENUM_CURVES::SECP256K1,
                                         &masterXprv);
    if (rv == JUBR_OK) {
        cout << "MasterPrivateKey: " << masterXprv << endl;
    }

    JUB_UINT16 contextID;
    rv = JUB_CreateContextBTC_soft(cfg, masterXprv, &contextID);
    if (rv != JUBR_OK) {
        cout << "JUB_CreateContextBTC_soft return " << rv << endl;
    }

    JUB_CHAR_PTR mainXpub;
    rv = JUB_GetMainHDNodeBTC(contextID, &mainXpub);
    if (rv == JUBR_OK) {
        cout << "JUB_GetMainHDNodeBTC: " << mainXpub << endl;
        JUB_FreeMemory(mainXpub);
    }

    BIP44_Path path;
    path.change = BOOL_FALSE;
    path.addressIndex = 0;
    JUB_CHAR_PTR  xpub = nullptr;
    rv = JUB_GetHDNodeBTC(contextID, path, &xpub);
    if (rv == JUBR_OK) {
        cout << "JUB_GetHDNodeBTC: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }

    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressBTC(contextID, path, BOOL_FALSE, &address);
    if(rv == JUBR_OK) {
        cout << "JUB_GetAddressBTC: " << address << endl;
        rv = JUB_CheckAddressBTC(contextID, address);
        cout << "JUB_CheckAddressBTC() return " << GetErrMsg(rv) << endl;
        JUB_FreeMemory(address);
    }

//    rv = JUB_CheckAddressBTC(contextID,"bc1qwmde2qhsgvzlnmju48tp4kdvqxdaaxr0vx9g27");
//    if(rv == JUBR_OK){cout << "check BTC segwit address success"<<endl;}

    if (isQRC20) {
        rv = transactionQTUM_proc(contextID, root);
    }
    else if (COINUSDT == cfg.coinType) {
        rv = transactionUSDT_proc(contextID, root);
    }
    else {
        rv = transaction_proc(contextID, root);
    }
}

void software_test_btc() {

    while (true) {
        cout << "-------------------------------------------" << endl;
        cout << "|********** Jubiter Wallet Test **********|" << endl;
        cout << "|   2. LTC_test.                          |" << endl;
        cout << "|   5.DASH_test.                          |" << endl;
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

        std::string json_file = "json/";
        bool isQRC20 = false;
        CONTEXT_CONFIG_BTC cfg;
        switch (choice) {
            case 2:
            {
                cfg.coinType = COINLTC;
                cfg.transType = p2sh_p2wpkh;
                json_file += "testLTC.json";
                break;
            }
            case 5:
            {
                cfg.coinType = COINDASH;
                cfg.transType = p2pkh;
                json_file += "testDASH.json";
                break;
            }
            case 31:
            {
                cfg.coinType = COINBTC;
                cfg.transType = p2pkh;
                json_file += "testBTC44.json";
                break;
            }
            case 32:
            {
                cfg.coinType = COINBTC;
                cfg.transType = p2sh_p2wpkh;
                json_file += "testBTC49.json";
                break;
            }
            case 39:
            {
                cfg.coinType = COINUSDT;
                cfg.transType = p2pkh;
                json_file += "testUSDT.json";
                break;
            }
            case 145:
            {
                cfg.coinType = COINBCH;
                cfg.transType = p2pkh;
                json_file += "testBCH.json";
                break;
            }
            case 88:
            {
                cfg.coinType = COINQTUM;
                cfg.transType = p2pkh;
                json_file += "testQTUM_qrc20.json";
                isQRC20 = true;
                break;
            }
            case 2301:
            {
                cfg.coinType = COINQTUM;
                cfg.transType = p2pkh;
                json_file += "testQTUM.json";
                break;
            }
            case 0:
                exit(0);
            default:
                continue;
        }

        Json::Value root = readJSON(json_file.c_str());

        cfg.mainPath = (char*)root["main_path"].asCString();

        software_test_btc(cfg, root, isQRC20);
    }
}
