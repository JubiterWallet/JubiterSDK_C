//
//  JUB_SDK_software_test_fil.cpp
//  JubSDKTest
//
//  Created by panmin on 2020/11/13.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
#include "JUB_SDK_test_fil.hpp"
#include "JUB_SDK_software_test_fil.hpp"

#include "mSIGNA/stdutils/uchar_vector.h"
#include "JUB_SDK_main.h"

#include "Base58.h"

void software_test_fil(const char* json_file) {

    Json::Value root = readJSON(json_file);

    JUB_RV rv = JUBR_ERROR;

/*    JUB_CHAR_PTR mnemonic = nullptr;
    rv = JUB_GenerateMnemonic_soft(STRENGTH128, &mnemonic);
    if(rv == JUBR_OK) {
        cout << mnemonic << endl;
    }

    rv = JUB_CheckMnemonic(mnemonic);
    if(rv != JUBR_OK) {
        cout << "JUB_CheckMnemonic return" << rv << endl;
    }
*/
    JUB_BYTE seed[64] = {0,};
    JUB_UINT16 seedLen = sizeof(seed)/sizeof(JUB_BYTE);
    auto callback = [](JUB_UINT32 current, JUB_UINT32 total) -> void {
        cout << ".";
    };

    JUB_CHAR_CPTR mnemonic = "gauge hole clog property soccer idea cycle stadium utility slice hold chief";
//    JUB_CHAR_CPTR mnemonic = "ensure token dress jar donate recipe once blue chief honey whip enhance";
    rv = JUB_GenerateSeed_soft(mnemonic, "", seed, callback);
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
//                                         JUB_ENUM_CURVES::ED25519,
                                         &masterXprv);
    if (rv == JUBR_OK) {
        cout << masterXprv << endl;
    }

    CONTEXT_CONFIG_XRP cfg;
    cfg.mainPath = (char*)root["main_path"].asCString();
    JUB_UINT16 contextID;
    rv = JUB_CreateContextFIL_soft(cfg, masterXprv, &contextID);
    if (rv != JUBR_OK) {
        cout << "JUB_CreateContextFIL_soft return " << rv << endl;
        return;
    }

    JUB_CHAR_PTR mainXpub = nullptr;
    rv = JUB_GetMainHDNodeFIL(contextID, JUB_ENUM_PUB_FORMAT::HEX, &mainXpub);
    if (JUBR_OK != rv) {
        cout << "JUB_GetMainHDNodeFIL return " << rv << endl;
        return;
    }
    cout << "JUB_GetMainHDNodeFIL return " << mainXpub << endl;
    JUB_FreeMemory(mainXpub);

    rv = JUB_GetMainHDNodeFIL(contextID, JUB_ENUM_PUB_FORMAT::XPUB, &mainXpub);
    if (JUBR_OK != rv) {
        cout << "JUB_GetMainHDNodeFIL return " << rv << endl;
        return;
    }
    cout << "JUB_GetMainHDNodeFIL return " << mainXpub << endl;
    JUB_FreeMemory(mainXpub);

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["FIL"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["FIL"]["bip32_path"]["addressIndex"].asInt();
    JUB_CHAR_PTR pub = nullptr;
    rv = JUB_GetHDNodeFIL(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &pub);
    if (rv != JUBR_OK) {
        cout << "JUB_GetHDNodeFIL return " << rv << endl;
        return;
    }
    cout << "JUB_GetHDNodeFIL return " << pub << endl;
    JUB_FreeMemory(pub);

    rv = JUB_GetHDNodeFIL(contextID, JUB_ENUM_PUB_FORMAT::XPUB, path, &pub);
    if (rv != JUBR_OK) {
        cout << "JUB_GetHDNodeFIL return " << rv << endl;
        return;
    }
    cout << "JUB_GetHDNodeFIL return " << pub << endl;
    JUB_FreeMemory(pub);

    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressFIL(contextID,
                           path,
                           JUB_ENUM_BOOL::BOOL_FALSE,
                           &address);
    if (rv != JUBR_OK) {
        cout << "JUB_GetAddressFIL return " << rv << endl;
        return;
    }
    cout << "JUB_GetAddressFIL return " << address << endl;
    JUB_FreeMemory(address);

    rv = transaction_proc_FIL(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}
