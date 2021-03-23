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


void software_test_btc(const char* json_sw_file, CONTEXT_CONFIG_BTC cfg, Json::Value root, bool isQRC20=false) {

    JUB_RV rv = JUBR_ERROR;
    std::string coinType = "44";

    cout << "[------------------------------ Generate Mnemonic ------------------------------]" << endl;
    JUB_CHAR_PTR mnemonic = nullptr;
    rv = JUB_GenerateMnemonic_soft(STRENGTH128, &mnemonic);
    cout << "[-] JUB_GenerateMnemonic_soft() return " << GetErrMsg(rv) << endl;
    if(JUBR_OK == rv) {
        cout << "    Generate mnemonic: " << mnemonic << endl;
    }
    cout << endl;

    rv = JUB_CheckMnemonic(mnemonic);
    cout << "[-] JUB_CheckMnemonic() return " << GetErrMsg(rv) << endl;
    if(JUBR_OK != rv) {
        return;
    }
    JUB_FreeMemory(mnemonic);
    cout << "[---------------------------- Generate Mnemonic end ----------------------------]" << endl;
    cout << endl << endl;

    cout << "[-------------------------------- Generate Seed --------------------------------]" << endl;
    Json::Value rootSW = readJSON(json_sw_file);
    mnemonic = (char*)rootSW["mnemonic"].asCString();
    cout << "    Read mnemonic: " << mnemonic << endl;
    cout << endl;

    rv = JUB_CheckMnemonic(mnemonic);
    cout << "[-] JUB_CheckMnemonic() return " << GetErrMsg(rv) << endl;
    if(JUBR_OK != rv) {
        return;
    }
    cout << endl;

    JUB_BYTE seed[64] = {0,};
    JUB_UINT16 seedLen = sizeof(seed)/sizeof(JUB_BYTE);
    auto callback = [](JUB_UINT32 current, JUB_UINT32 total) -> void {
        cout << ".";
    };
    rv = JUB_GenerateSeed_soft(mnemonic, "", seed, callback);
    cout << "[-] JUB_GenerateSeed_soft() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    uchar_vector vSeed(seedLen);
    for (int i=0; i<seedLen; ++i) {
        vSeed[i] = seed[i];
    }
    cout << "    Generate seed: " << vSeed.getHex() << endl;
    cout << "[------------------------------ Generate Seed end ------------------------------]" << endl;
    cout << endl;

    JUB_CHAR_PTR masterXprv = nullptr;
    rv = JUB_SeedToMasterPrivateKey_soft(seed, seedLen,
                                         JUB_ENUM_CURVES::SECP256K1,
                                         &masterXprv);
    cout << "[-] JUB_SeedToMasterPrivateKey_soft() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "    Seed to master xprv: " << masterXprv << endl;
    }
    cout << endl << endl;

    JUB_UINT16 contextID = 0;
    rv = JUB_CreateContextBTC_soft(cfg, masterXprv, &contextID);
    cout << "[-] JUB_CreateContextBTC_soft() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        JUB_FreeMemory(masterXprv);
        return;
    }
    JUB_FreeMemory(masterXprv);
    cout << endl << endl;

    cout << "[----------------------------------- HD Node -----------------------------------]" << endl;
    JUB_CHAR_PTR xpub = nullptr;
    rv = JUB_GetMainHDNodeBTC(contextID, &xpub);
    cout << "[-] JUB_GetMainHDNodeBTC() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "    main xpub in HEX: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }
	cout << endl;

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)rootSW["bip32_path"]["change"].asBool();
    path.addressIndex = rootSW["bip32_path"]["addressIndex"].asInt();
    rv = JUB_GetHDNodeBTC(contextID, path, &xpub);
    cout << "[-] JUB_GetHDNodeBTC() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "    xpub in HEX: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }
    cout << "[--------------------------------- HD Node end ---------------------------------]" << endl;
    cout << endl << endl;

    cout << "[----------------------------------- Address -----------------------------------]" << endl;
    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressBTC(contextID, path, BOOL_FALSE, &address);
    cout << "[-] JUB_GetAddressBTC() return " << GetErrMsg(rv) << endl;
    if(JUBR_OK == rv) {
        cout << "    address: " << address << endl;
        rv = JUB_CheckAddressBTC(contextID, address);
        cout << "[-] JUB_CheckAddressBTC() return " << GetErrMsg(rv) << endl;
        JUB_FreeMemory(address);
    }
    cout << endl;

//    rv = JUB_CheckAddressBTC(contextID,"bc1qwmde2qhsgvzlnmju48tp4kdvqxdaaxr0vx9g27");
//    if(JUBR_OK == rv){cout << "check BTC segwit address success"<<endl;}
    cout << "[--------------------------------- Address end ---------------------------------]" << endl;
    cout << endl << endl;

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

void software_test_btc(const char* json_sw_file) {

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
        }   // switch (choice) end

        Json::Value root = readJSON(json_file.c_str());

        cfg.mainPath = (char*)root["main_path"].asCString();

        software_test_btc(json_sw_file, cfg, root, isQRC20);
    }   // while (true) end
}
