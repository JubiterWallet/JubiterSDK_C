//
//  JUB_SDK_software_test_hcash.cpp
//  JubSDKTest
//
//  Created by panmin on 2020/1/19.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include "JUB_SDK_software_test_hcash.hpp"
#include "JUB_SDK_test_hcash.hpp"

#include "mSIGNA/stdutils/uchar_vector.h"
#include "JUB_SDK_main.h"

void software_test_hcash(const char* json_sw_file, CONTEXT_CONFIG_HC cfg, Json::Value root) {

    JUB_RV rv = JUBR_ERROR;

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
    cout << endl;

    // Special handling of hcash seed
//    seed := bip39.NewSeed(mnemonic, "")
//    hcSeed32 := make([]byte, 32)
//    for i := 0; i < 32; i++ {
//        hcSeed32[i] = byte((uint16(seed[i]) + uint16(seed[64-i-1])) >> 1)
//    }
//    masterKey, _ := bip32.NewMasterKey(hcSeed32)
    JUB_BYTE hcSeed32[32] = {0,};
    JUB_UINT16 hcSeed32Len = sizeof(hcSeed32)/sizeof(JUB_BYTE);
    for (int i=0; i<hcSeed32Len; ++i) {
        hcSeed32[i] = ((uint16_t(seed[i]) + uint16_t(seed[seedLen-i-1])) >> 1);
    }
    uchar_vector vHCSeed(hcSeed32Len);
    for (int i=0; i<hcSeed32Len; ++i) {
        vHCSeed[i] = hcSeed32[i];
    }
    cout << "    Generate Hcash seed: " << vHCSeed.getHex() << endl;
    cout << "[------------------------------ Generate Seed end ------------------------------]" << endl;
    cout << endl;

    JUB_CHAR_PTR masterXprv = nullptr;
    rv = JUB_SeedToMasterPrivateKey_soft(hcSeed32, hcSeed32Len,
                                         JUB_ENUM_CURVES::SECP256K1,
                                         &masterXprv);
    cout << "[-] JUB_SeedToMasterPrivateKey_soft() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "    Seed to master xprv: " << masterXprv << endl;
    }
    cout << endl << endl;

    JUB_UINT16 contextID = 0;
    rv = JUB_CreateContextHC_soft(cfg, masterXprv, &contextID);
    cout << "[-] JUB_CreateContextHC_soft() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        JUB_FreeMemory(masterXprv);
        return;
    }
    JUB_FreeMemory(masterXprv);
    cout << endl << endl;

    cout << "[----------------------------------- HD Node -----------------------------------]" << endl;
    JUB_CHAR_PTR xpub = nullptr;
    rv = JUB_GetMainHDNodeHC(contextID, &xpub);
    cout << "[-] JUB_GetMainHDNodeHC() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "    Main xpub in HEX: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }
	cout << endl;

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)rootSW["bip32_path"]["change"].asBool();
    path.addressIndex = rootSW["bip32_path"]["addressIndex"].asInt();
    rv = JUB_GetHDNodeHC(contextID, path, &xpub);
    cout << "[-] JUB_GetHDNodeHC() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "    dpub in HEX: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }
    cout << endl;
    cout << "[--------------------------------- HD Node end ---------------------------------]" << endl;
    cout << endl << endl;

    cout << "[----------------------------------- Address -----------------------------------]" << endl;
    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressHC(contextID, path, BOOL_FALSE, &address);
    cout << "[-] JUB_GetAddressHC() return " << GetErrMsg(rv) << endl;
    if(JUBR_OK == rv) {
        cout << "    address: " << address << endl;
        JUB_FreeMemory(address);
    }
    cout << "[--------------------------------- Address end ---------------------------------]" << endl;
    cout << endl << endl;

    rv = transactionHC_proc(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}

void software_test_hcash(const char* json_sw_file, const char* json_file) {

    Json::Value root = readJSON(json_file);

    CONTEXT_CONFIG_HC cfg;
    cfg.mainPath = (char*)root["main_path"].asCString();

    software_test_hcash(json_sw_file, cfg, root);
}
