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

void software_test_xrp(const char* json_file) {

    Json::Value root = readJSON(json_file);

    JUB_RV rv = JUBR_ERROR;

/*    JUB_CHAR_PTR mnemonic = nullptr;
    rv = JUB_GenerateMnemonic_soft(STRENGTH128, &mnemonic);
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
//    JUB_CHAR_CPTR mnemonic = "ensure token dress jar donate recipe once blue chief honey whip enhance";
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

    CONTEXT_CONFIG_XRP cfg;
    cfg.mainPath = (char*)root["main_path"].asCString();
    JUB_UINT16 contextID;
    rv = JUB_CreateContextXRP_soft(cfg, masterXprv, &contextID);
    cout << "JUB_CreateContextXRP_soft() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        JUB_FreeMemory(masterXprv);
        return;
    }
    JUB_FreeMemory(masterXprv);

    JUB_CHAR_PTR mainXpub = nullptr;
    rv = JUB_GetMainHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::HEX, &mainXpub);
    cout << "JUB_GetMainHDNodeXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "main xpub in HEX: " << mainXpub << endl;
        JUB_FreeMemory(mainXpub);
    }

    rv = JUB_GetMainHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::XPUB, &mainXpub);
    cout << "JUB_GetMainHDNodeXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "main xpub in XPUB: " << mainXpub << endl;
        JUB_FreeMemory(mainXpub);
    }

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["XRP"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["XRP"]["bip32_path"]["addressIndex"].asInt();
    JUB_CHAR_PTR pub = nullptr;
    rv = JUB_GetHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &pub);
    cout << "JUB_GetHDNodeXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "xpub in HEX: " << pub << endl;
        JUB_FreeMemory(pub);
    }

    rv = JUB_GetHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::XPUB, path, &pub);
    cout << "JUB_GetHDNodeXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "xpub in XPUB: " << pub << endl;
        JUB_FreeMemory(pub);
    }

    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressXRP(contextID,
                           path,
                           JUB_ENUM_BOOL::BOOL_FALSE,
                           &address);
    cout << "JUB_GetAddressXRP() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "address: " << address << endl;
        JUB_FreeMemory(address);
    }

    rv = transaction_proc_XRP(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}
