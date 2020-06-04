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

void software_test_hcash(CONTEXT_CONFIG_HC cfg, Json::Value root) {

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
    cout << "hc seed: " << vHCSeed.getHex() << endl;
    cout << endl;

    JUB_CHAR_PTR masterXprv = nullptr;
    rv = JUB_SeedToMasterPrivateKey_soft(hcSeed32, hcSeed32Len,
                                         JUB_ENUM_CURVES::SECP256K1,
                                         &masterXprv);
    if (rv == JUBR_OK) {
        cout << masterXprv << endl;
    }

    JUB_UINT16 contextID;
    rv = JUB_CreateContextHC_soft(cfg, masterXprv, &contextID);
    if (rv != JUBR_OK) {
        cout << "JUB_CreateContextHC_soft return " << rv << endl;
    }

    JUB_CHAR_PTR mainXpub;
    rv = JUB_GetMainHDNodeHC(contextID, &mainXpub);
    cout << "JUB_GetMainHDNodeHC() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "Main xpub : " << mainXpub << endl;
        JUB_FreeMemory(mainXpub);
    }

    BIP44_Path path;
    path.change = BOOL_FALSE;
    path.addressIndex = 0;
    JUB_CHAR_PTR  xpub = nullptr;
    rv = JUB_GetHDNodeHC(contextID, path, &xpub);
    cout << "JUB_GetHDNodeHC return " << GetErrMsg(rv) << endl;
    if (rv == JUBR_OK) {
        cout << "dpub: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }

    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressHC(contextID, path, BOOL_FALSE, &address);
    cout << "JUB_GetAddressHC return " << GetErrMsg(rv) << endl;
    if(rv == JUBR_OK) {
        cout << "address: " << address << endl;
        JUB_FreeMemory(address);
    }

    rv = transactionHC_proc(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}

void software_test_hcash() {

    std::string json_file = "json/";

    json_file += "testHCash.json";
    Json::Value root = readJSON(json_file.c_str());

    CONTEXT_CONFIG_HC cfg;
    cfg.mainPath = (char*)root["main_path"].asCString();

    software_test_hcash(cfg, root);
}
