//
//  JUB_SDK_software_test_eos.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/9/19.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
#include "JUB_SDK_test_eos.hpp"
#include "JUB_SDK_software_test_eos.hpp"

#include "mSIGNA/stdutils/uchar_vector.h"
#include "JUB_SDK_main.h"
#include "Base58.h"

void software_test_eos(const char* json_sw_file, const char* json_file) {

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
    if (rv != JUBR_OK) {
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

    CONTEXT_CONFIG_EOS cfg;
    cfg.mainPath = (char*)root["main_path"].asCString();
    JUB_UINT16 contextID = 0;
    rv = JUB_CreateContextEOS_soft(cfg, masterXprv, &contextID);
    cout << "[-] JUB_CreateContextEOS_soft() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        JUB_FreeMemory(masterXprv);
        return;
    }
    JUB_FreeMemory(masterXprv);
    cout << endl << endl;

    cout << "[----------------------------------- HD Node -----------------------------------]" << endl;
    JUB_CHAR_PTR xpub = nullptr;
    rv = JUB_GetMainHDNodeEOS(contextID, JUB_ENUM_PUB_FORMAT::HEX, &xpub);
    cout << "[-] JUB_GetMainHDNodeEOS() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "    main xpub in HEX: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }
	cout << endl;

    rv = JUB_GetMainHDNodeEOS(contextID, JUB_ENUM_PUB_FORMAT::XPUB, &xpub);
    cout << "[-] JUB_GetMainHDNodeEOS() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "    main xpub in xpub: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }
	cout << endl;

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)rootSW["bip32_path"]["change"].asBool();
    path.addressIndex = rootSW["bip32_path"]["addressIndex"].asInt();
//    BIP48_Path path;
//    path.network = 4;
//    path.role = JUB_ENUM_GRAPHENE_ROLE::ACTIVE;
//    path.accountIndex = 0;
//    path.keyIndex = 0;
    rv = JUB_GetHDNodeEOS(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &xpub);
    cout << "[-] JUB_GetHDNodeEOS() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "    xpub in HEX: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }
    cout << endl;

    rv = JUB_GetHDNodeEOS(contextID, JUB_ENUM_PUB_FORMAT::XPUB, path, &xpub);
    cout << "[-] JUB_GetHDNodeEOS() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "    xpub in xpub: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }
    cout << "[--------------------------------- HD Node end ---------------------------------]" << endl;
    cout << endl << endl;

    cout << "[----------------------------------- Address -----------------------------------]" << endl;
    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressEOS(contextID,
                           path,
                           JUB_ENUM_BOOL::BOOL_FALSE,
                           &address);
    cout << "[-] JUB_GetAddressEOS() return " << GetErrMsg(rv) << endl;
    if(JUBR_OK == rv) {
        cout << "    address: " << address << endl;
        JUB_FreeMemory(address);
    }
    cout << "[--------------------------------- Address end ---------------------------------]" << endl;
    cout << endl << endl;

    for (int i=1; i<=6; ++i) {
        switch (i) {
        case 1:
            cout << "[---------------------------------- Transfer test ------------------------------]" << endl;
            break;
        case 2:
            cout << "[-------------------------------- Transfer token test --------------------------]" << endl;
            break;
        case 3:
            cout << "[------------------------------------- Stake test ------------------------------]" << endl;
            break;
        case 4:
            cout << "[------------------------------------ Unstake test -----------------------------]" << endl;
            break;
        case 5:
            cout << "[------------------------------------ Buy RAM test -----------------------------]" << endl;
            break;
        case 6:
            cout << "[------------------------------------ Sell RAM test ----------------------------]" << endl;
            break;

        default:
            break;
        }
        rv = transaction_proc_EOS(contextID, root, i);
        if (JUBR_OK != rv) {
            return;
        }
        cout << endl << endl;
    }
}
