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
    cout << "JUB_GenerateMnemonic_soft() return " << GetErrMsg(rv) << endl;
    if(JUBR_OK == rv) {
        cout << "Generate mnemonic: " << mnemonic << endl;
    }

    rv = JUB_CheckMnemonic(mnemonic);
    cout << "JUB_CheckMnemonic() return" << GetErrMsg(rv) << endl;
    JUB_FreeMemory(mnemonic);

    JUB_BYTE seed[64] = {0,};
    JUB_UINT16 seedLen = sizeof(seed)/sizeof(JUB_BYTE);
    auto callback = [](JUB_UINT32 current, JUB_UINT32 total) -> void {
        cout << ".";
    };

    rv = JUB_GenerateSeed_soft("gauge hole clog property soccer idea cycle stadium utility slice hold chief", "", seed, callback);
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
                                         &masterXprv);
    cout << "JUB_SeedToMasterPrivateKey_soft() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "master xprv: " << masterXprv << endl;
    }

    CONTEXT_CONFIG_ETH cfgeth;
    cfgeth.chainID = root["chainID"].asInt();
    cfgeth.mainPath = (char*)root["main_path"].asCString();
    JUB_UINT16 contextID = 0;
    rv = JUB_CreateContextETH_soft(cfgeth, masterXprv, &contextID);
    cout << "JUB_CreateContextETH_soft() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        JUB_FreeMemory(masterXprv);
        return;
    }
    JUB_FreeMemory(masterXprv);

    JUB_CHAR_PTR xpub = nullptr;
    rv = JUB_GetMainHDNodeETH(contextID, JUB_ENUM_PUB_FORMAT::HEX, &xpub);
    cout << "JUB_GetMainHDNodeETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "Main xpub : " << xpub << endl;
        JUB_FreeMemory(xpub);
    }

    rv = JUB_GetMainHDNodeETH(contextID, JUB_ENUM_PUB_FORMAT::XPUB, &xpub);
    cout << "JUB_GetMainHDNodeETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "main xpub: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["ETH"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["ETH"]["bip32_path"]["addressIndex"].asInt();
    rv = JUB_GetHDNodeETH(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &xpub);
    cout << "JUB_GetHDNodeETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "xpub in HEX: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }

    rv = JUB_GetHDNodeETH(contextID, JUB_ENUM_PUB_FORMAT::XPUB, path, &xpub);
    cout << "JUB_GetHDNodeETH() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "xpub in xpub: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }

    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressETH(contextID, path, BOOL_FALSE, &address);
    cout << "JUB_GetAddressETH() return " << GetErrMsg(rv) << endl;
    if(JUBR_OK == rv) {
        cout << "address: " << address << endl;
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

    rv = bytestring_proc_ETH(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}
