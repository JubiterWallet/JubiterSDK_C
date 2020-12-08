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

void software_test_eos(const char* json_file) {

    Json::Value root = readJSON(json_file);

    JUB_RV rv = JUBR_ERROR;

/*    JUB_CHAR_PTR mnemonic = nullptr;
    JUB_RV rv = JUB_GenerateMnemonic_soft(STRENGTH128, &mnemonic);
    cout << "JUB_GenerateMnemonic_soft() return " << GetErrMsg(rv) << endl;
    if(JUBR_OK == rv) {
        cout << "Generate mnemonic: " << mnemonic << endl;
    }

    rv = JUB_CheckMnemonic(mnemonic);
    cout << "JUB_CheckMnemonic() return " << GetErrMsg(rv) << endl;
    JUB_FreeMemory(mnemonic);
*/
    JUB_BYTE seed[64] = {0,};
    JUB_UINT16 seedLen = sizeof(seed)/sizeof(JUB_BYTE);
    auto callback = [](JUB_UINT32 current, JUB_UINT32 total) -> void {
        cout << ".";
    };

    JUB_CHAR_CPTR mnemonic = "gauge hole clog property soccer idea cycle stadium utility slice hold chief";
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
                                         JUB_ENUM_CURVES::SECP256K1,
//                                         JUB_ENUM_CURVES::ED25519,
                                         &masterXprv);
    cout << "JUB_SeedToMasterPrivateKey_soft() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "master xprv: " << masterXprv << endl;
    }

    CONTEXT_CONFIG_EOS cfg;
    cfg.mainPath = (char*)root["main_path"].asCString();
    JUB_UINT16 contextID;
    rv = JUB_CreateContextEOS_soft(cfg, masterXprv, &contextID);
    cout << "JUB_CreateContextEOS_soft() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        JUB_FreeMemory(masterXprv);
        return;
    }
    JUB_FreeMemory(masterXprv);

    JUB_CHAR_PTR mainPub = nullptr;
    rv = JUB_GetMainHDNodeEOS(contextID, JUB_ENUM_PUB_FORMAT::HEX, &mainPub);
    cout << "JUB_GetMainHDNodeEOS() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "main xpub in HEX: " << mainPub << endl;
        JUB_FreeMemory(mainPub);
    }

    rv = JUB_GetMainHDNodeEOS(contextID, JUB_ENUM_PUB_FORMAT::XPUB, &mainPub);
    cout << "JUB_GetMainHDNodeEOS() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "main xpub in xpub: " << mainPub << endl;
        JUB_FreeMemory(mainPub);
    }

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["EOS"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["EOS"]["bip32_path"]["addressIndex"].asInt();
//    BIP48_Path path;
//    path.network = 4;
//    path.role = JUB_ENUM_GRAPHENE_ROLE::ACTIVE;
//    path.accountIndex = 0;
//    path.keyIndex = 0;
    JUB_CHAR_PTR pub = nullptr;
    rv = JUB_GetHDNodeEOS(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &pub);
    cout << "JUB_GetHDNodeEOS() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "xpub in HEX: " << pub << endl;
        JUB_FreeMemory(pub);
    }

    rv = JUB_GetHDNodeEOS(contextID, JUB_ENUM_PUB_FORMAT::XPUB, path, &pub);
    cout << "JUB_GetHDNodeEOS() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "xpub in xpub: " << pub << endl;
        JUB_FreeMemory(pub);
    }

    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressEOS(contextID,
                           path,
                           JUB_ENUM_BOOL::BOOL_FALSE,
                           &address);
    cout << "JUB_GetAddressEOS() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "address: " << address << endl;
        JUB_FreeMemory(address);
    }

    for (int i=1; i<=6; ++i) {
        switch (i) {
        case 1:
            std::cout << "transfer test..." << std::endl;
            break;
        case 2:
            std::cout << "transfer token test..." << std::endl;
            break;
        case 3:
            std::cout << "stake test..." << std::endl;
            break;
        case 4:
            std::cout << "unstake test..." << std::endl;
            break;
        case 5:
            std::cout << "buy ram test..." << std::endl;
            break;
        case 6:
            std::cout << "sell ram test..." << std::endl;
            break;

        default:
            break;
        }
        rv = transaction_proc_EOS(contextID, root, i);
        if (JUBR_OK != rv) {
            return;
        }
    }
}
