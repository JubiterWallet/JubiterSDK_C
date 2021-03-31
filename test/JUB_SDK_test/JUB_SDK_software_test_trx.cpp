//
//  JUB_SDK_software_test_trx.cpp
//  JubSDKTest
//
//  Created by panmin on 2020/9/11.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include "JUB_SDK_software_test_trx.hpp"
#include "JUB_SDK_test_trx.hpp"

#include "mSIGNA/stdutils/uchar_vector.h"
#include "JUB_SDK_main.h"


void software_test_trx(const char* json_sw_file, const char* json_file) {

    JUB_RV rv = JUBR_ERROR;

    Json::Value root = readJSON(json_file);
    if (root.empty()) {
        return;
    }

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

    CONTEXT_CONFIG_TRX cfg;
    cfg.mainPath = (char*)root["main_path"].asCString();
    JUB_UINT16 contextID = 0;
    rv = JUB_CreateContextTRX_soft(cfg, masterXprv, &contextID);
    cout << "[-] JUB_CreateContextTRX_soft() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        JUB_FreeMemory(masterXprv);
        return;
    }
    JUB_FreeMemory(masterXprv);
    cout << endl << endl;

    cout << "[----------------------------------- HD Node -----------------------------------]" << endl;
    JUB_CHAR_PTR xpub = nullptr;
    rv = JUB_GetMainHDNodeTRX(contextID, JUB_ENUM_PUB_FORMAT::HEX, &xpub);
    cout << "[-] JUB_GetMainHDNodeTRX() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "    Main xpub in  HEX: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }
    cout << endl;

    rv = JUB_GetMainHDNodeTRX(contextID, JUB_ENUM_PUB_FORMAT::XPUB, &xpub);
    cout << "[-] JUB_GetMainHDNodeTRX() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "    Main xpub in XPUB: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }
    cout << endl;

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)rootSW["bip32_path"]["change"].asBool();
    path.addressIndex = rootSW["bip32_path"]["addressIndex"].asInt();
    rv = JUB_GetHDNodeTRX(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &xpub);
    cout << "[-] JUB_GetHDNodeTRX() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "    xpub in  HEX: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }
    cout << endl;

    rv = JUB_GetHDNodeTRX(contextID, JUB_ENUM_PUB_FORMAT::XPUB, path, &xpub);
    cout << "[-] JUB_GetHDNodeTRX() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "    xpub in XPUB: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }

    cout << "[--------------------------------- HD Node end ---------------------------------]" << endl;
    cout << endl << endl;

    cout << "[----------------------------------- Address -----------------------------------]" << endl;
    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressTRX(contextID, path, JUB_ENUM_BOOL::BOOL_FALSE, &address);
    cout << "[-] JUB_GetAddressTRX() return " << GetErrMsg(rv) << endl;
    std::string addr = "";
    if(JUBR_OK == rv) {
        cout << "    address: " << address << endl;
        addr = std::string(address);
        JUB_FreeMemory(address);
    }
    cout << endl;

    JUB_CHAR_PTR addrInHex = nullptr;
    rv = JUB_CheckAddressTRX(contextID, addr.c_str(), &addrInHex);
	cout << "[-] JUB_CheckAddressTRX() return " << GetErrMsg(rv) << endl;
    if(JUBR_OK == rv) {
        cout << "    address in hex: " << addrInHex << endl;
        JUB_FreeMemory(addrInHex);
    }
    cout << "[--------------------------------- Address end ---------------------------------]" << endl;
    cout << endl << endl;

    int choice = 0;
    for (int i=1; i<=5; ++i) {
        switch (i) {
        case 1:
            choice = JUB_ENUM_TRX_CONTRACT_TYPE::XFER_CONTRACT;
            cout << "[----------------------------- Transfer contract test --------------------------]" << endl;
            break;
        case 2:
            choice = JUB_ENUM_TRX_CONTRACT_TYPE::XFER_ASSET_CONTRACT;
            cout << "[-------------------------- Transfer asset contract test -----------------------]" << endl;
            break;
        case 3:
            choice = JUB_ENUM_TRX_CONTRACT_TYPE::TRIG_SMART_CONTRACT;
            cout << "[-------------------------- Trigger smart contract test ------------------------]" << endl;
            break;
        case 4:
            choice = JUB_ENUM_TRX_CONTRACT_TYPE::FRZ_BLA_CONTRACT;
            cout << "[-------------------------- freeze balance contract test -----------------------]" << endl;
            break;
        case 5:
            choice = JUB_ENUM_TRX_CONTRACT_TYPE::UNFRZ_BLA_CONTRACT;
            cout << "[------------------------ unfreeze balance contract test -----------------------]" << endl;
            break;
        }
        rv = transaction_proc_TRX(contextID, root, choice);
        if (JUBR_OK != rv) {
            return;
        }
        cout << endl << endl;
    }
}
