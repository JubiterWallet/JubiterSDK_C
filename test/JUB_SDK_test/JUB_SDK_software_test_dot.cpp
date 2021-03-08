//
//  JUB_SDK_software_test_dot.cpp
//  JubSDKTest
//
//  Created by panmin on 2020/11/13.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
//#include "JUB_SDK_test_dot.hpp"
#include "JUB_SDK_software_test_dot.hpp"

#include "mSIGNA/stdutils/uchar_vector.h"
#include "JUB_SDK_main.h"

#include "Base58.h"

void software_test_dot(const char* json_file) {

//    Json::Value root = readJSON(json_file);

    JUB_RV rv = JUBR_ERROR;

    JUB_CHAR_PTR mnemonic = "chief menu kingdom stereo hope hazard into island bag trick egg route";

    rv = JUB_CheckMnemonic(mnemonic);
    cout << "JUB_CheckMnemonic() return" << GetErrMsg(rv) << endl;
    JUB_FreeMemory(mnemonic);

    JUB_BYTE seed[64] = {0,};
    JUB_UINT16 seedLen = sizeof(seed)/sizeof(JUB_BYTE);
    auto callback = [](JUB_UINT32 current, JUB_UINT32 total) -> void {
        cout << ".";
    };

    rv = JUB_GenerateSeed_soft(mnemonic, "", seed, callback);
    cout << "JUB_GenerateSeed_soft() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    uchar_vector vSeed(seedLen);
    for (int i=0; i<seedLen; ++i) {
        vSeed[i] = seed[i];
    }
    cout << "seed: " << vSeed.getHex() << endl;
    cout << endl;

    JUB_CHAR_PTR masterXprv = nullptr;
    rv = JUB_SeedToMasterPrivateKey_soft(seed, seedLen,
                                         JUB_ENUM_CURVES::ED25519,
                                         &masterXprv);
    cout << "JUB_SeedToMasterPrivateKey_soft() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "master xprv: " << masterXprv << endl;
    }

    CONTEXT_CONFIG_DOT cfg;
//    cfg.mainPath = (char*)root["main_path"].asCString();
    cfg.mainPath = "m/44'/354'/0'";
    JUB_UINT16 contextID;
    rv = JUB_CreateContextDOT_soft(cfg, masterXprv, &contextID);
    if (rv != JUBR_OK) {
        cout << "JUB_CreateContextDOT_soft return " << rv << endl;
        return;
    }

    JUB_CHAR_PTR mainXpub = nullptr;
    rv = JUB_GetMainHDNodeDOT(contextID, JUB_ENUM_PUB_FORMAT::HEX, &mainXpub);
    if (JUBR_OK != rv) {
        cout << "JUB_GetMainHDNodeDOT return " << rv << endl;
        return;
    }
    cout << "JUB_GetMainHDNodeDOT return " << mainXpub << endl;
    JUB_FreeMemory(mainXpub);

    rv = JUB_GetMainHDNodeDOT(contextID, JUB_ENUM_PUB_FORMAT::XPUB, &mainXpub);
    if (JUBR_OK != rv) {
        cout << "JUB_GetMainHDNodeDOT return " << rv << endl;
        return;
    }
    cout << "JUB_GetMainHDNodeDOT return " << mainXpub << endl;
    JUB_FreeMemory(mainXpub);

    BIP44_Path path;
    path.change = JUB_ENUM_BOOL::BOOL_FALSE;
    path.addressIndex = 0;
    JUB_CHAR_PTR pub = nullptr;
    rv = JUB_GetHDNodeDOT(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &pub);
    if (rv != JUBR_OK) {
        cout << "JUB_GetHDNodeDOT return " << rv << endl;
        return;
    }
    cout << "JUB_GetHDNodeDOT return " << pub << endl;
    JUB_FreeMemory(pub);

    rv = JUB_GetHDNodeDOT(contextID, JUB_ENUM_PUB_FORMAT::XPUB, path, &pub);
        if (rv != JUBR_OK) {
        cout << "JUB_GetHDNodeDOT return " << rv << endl;
        return;
    }
    cout << "JUB_GetHDNodeDOT return " << pub << endl;
    JUB_FreeMemory(pub);

    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressDOT(contextID,
                           path,
                           JUB_ENUM_BOOL::BOOL_FALSE,
                           &address);
    if (rv != JUBR_OK) {
        cout << "JUB_GetAddressDOT return " << rv << endl;
        return;
    }
    cout << "JUB_GetAddressDOT return " << address << endl;
    JUB_FreeMemory(address);

}
