//
//  JUB_SDK_software_test_xrp.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/11/20.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
#include "JUB_SDK_test_xrp.hpp"
#include "JUB_SDK_software_test_xrp.hpp"

#include "mSIGNA/stdutils/uchar_vector.h"
#include "JUB_SDK_main.h"

#include "Base58.h"

void software_test_xrp(const char* json_sw_file, const char* json_file) {

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
//                                         JUB_ENUM_CURVES::ED25519,
                                         &masterXprv);
    cout << "[-] JUB_SeedToMasterPrivateKey_soft() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "    Seed to master xprv: " << masterXprv << endl;
    }
    cout << endl << endl;

    CONTEXT_CONFIG_XRP cfg;
    cfg.mainPath = (char*)root["main_path"].asCString();
    JUB_UINT16 contextID = 0;
    rv = JUB_CreateContextXRP_soft(cfg, masterXprv, &contextID);
    cout << "[-] JUB_CreateContextXRP_soft() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        JUB_FreeMemory(masterXprv);
        return;
    }
    JUB_FreeMemory(masterXprv);
    cout << endl << endl;

    cout << "[----------------------------------- HD Node -----------------------------------]" << endl;
    JUB_CHAR_PTR xpub = nullptr;
    rv = JUB_GetMainHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::HEX, &xpub);
    cout << "[-] JUB_GetMainHDNodeXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "    Main xpub in HEX: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }
	cout << endl;

    rv = JUB_GetMainHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::XPUB, &xpub);
    cout << "[-] JUB_GetMainHDNodeXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "    Main xpub in XPUB: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }
	cout << endl;

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)rootSW["bip32_path"]["change"].asBool();
    path.addressIndex = rootSW["bip32_path"]["addressIndex"].asInt();
    rv = JUB_GetHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &xpub);
    cout << "[-] JUB_GetHDNodeXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "    xpub in HEX: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }
    cout << endl;

    rv = JUB_GetHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::XPUB, path, &xpub);
    cout << "[-] JUB_GetHDNodeXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "xpub in XPUB: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }
    cout << "[--------------------------------- HD Node end ---------------------------------]" << endl;
    cout << endl << endl;

    cout << "[----------------------------------- Address -----------------------------------]" << endl;
    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressXRP(contextID,
                           path,
                           JUB_ENUM_BOOL::BOOL_FALSE,
                           &address);
    cout << "[-] JUB_GetAddressXRP() return " << GetErrMsg(rv) << endl;
    if(JUBR_OK == rv) {
        cout << "    address: " << address << endl;
        JUB_FreeMemory(address);
    }
    cout << "[--------------------------------- Address end ---------------------------------]" << endl;
    cout << endl << endl;

    rv = transaction_proc_XRP(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}
