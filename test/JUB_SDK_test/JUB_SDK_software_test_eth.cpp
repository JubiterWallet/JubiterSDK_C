//
//  JUB_SDK_software_test_eth.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/9/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_software_test_eth.hpp"
#include "JUB_SDK_test_eth.hpp"

#include "mSIGNA/stdutils/uchar_vector.h"
#include "JUB_SDK_main.h"

void software_test_eth(const char* json_file) {

    Json::Value root = readJSON(json_file);

    JUB_CHAR_PTR mnemonic = nullptr;

    JUB_RV rv = JUB_GenerateMnemonic_soft(STRENGTH128, &mnemonic);
    if(rv == JUBR_OK) {
        cout << "JUB_GenerateMnemonic_soft() return " << mnemonic << endl;
    }

    rv = JUB_CheckMnemonic(mnemonic);
    if(rv != JUBR_OK) {
        cout << "JUB_CheckMnemonic() return" << rv << endl;
    }
    JUB_BYTE seed[64] = {0,};
    JUB_UINT16 seedLen = sizeof(seed)/sizeof(JUB_BYTE);
    auto callback = [](JUB_UINT32 current, JUB_UINT32 total) -> void {
        cout << ".";
    };

    rv = JUB_GenerateSeed_soft("gauge hole clog property soccer idea cycle stadium utility slice hold chief", "", seed, callback);
    if (rv != JUBR_OK) {
        cout << "JUB_GenerateSeed_soft() error" << endl;
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
                                         &masterXprv);
    if (rv == JUBR_OK) {
        cout << "JUB_SeedToMasterPrivateKey_soft() return " << masterXprv << endl;
    }

    CONTEXT_CONFIG_ETH cfgeth;
    cfgeth.chainID = root["chainID"].asInt();
    cfgeth.mainPath = (char*)root["main_path"].asCString();
    JUB_UINT16 contextID = 0;
    rv = JUB_CreateContextETH_soft(cfgeth, masterXprv, &contextID);
    if (rv != JUBR_OK) {
        cout << "JUB_CreateContextETH_soft() return " << rv << endl;
    }

    JUB_CHAR_PTR xpub = nullptr;
    rv = JUB_GetMainHDNodeETH(contextID, JUB_ENUM_PUB_FORMAT::HEX, &xpub);
    if (rv == JUBR_OK) {
        cout << "JUB_GetMainHDNodeETH() return " << xpub << endl;
        JUB_FreeMemory(xpub);
    }

    rv = JUB_GetMainHDNodeETH(contextID, JUB_ENUM_PUB_FORMAT::XPUB, &xpub);
    if (rv == JUBR_OK) {
        cout << "JUB_GetMainHDNodeETH() return " << xpub << endl;
        JUB_FreeMemory(xpub);
    }

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["ETH"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["ETH"]["bip32_path"]["addressIndex"].asInt();
    rv = JUB_GetHDNodeETH(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &xpub);
    if (rv == JUBR_OK) {
        cout << "JUB_GetHDNodeETH() return " << xpub << endl;
        JUB_FreeMemory(xpub);
    }

    rv = JUB_GetHDNodeETH(contextID, JUB_ENUM_PUB_FORMAT::XPUB, path, &xpub);
    if (rv == JUBR_OK) {
        cout << "JUB_GetHDNodeETH() return " << xpub << endl;
        JUB_FreeMemory(xpub);
    }

    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressETH(contextID, path, BOOL_FALSE, &address);
    if(rv == JUBR_OK) {
        cout << "JUB_GetAddressETH() return " << address << endl;
        JUB_FreeMemory(address);
    }

    rv = transaction_proc_ETH(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }

    rv = transaction_proc_ERC20_ETH(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}
