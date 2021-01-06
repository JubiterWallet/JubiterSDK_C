//
//  JUB_SDK_software_test_trx.cpp
//  JubSDKTest
//
//  Created by panmin on 2020/9/11.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include "JUB_SDK_software_test_trx.hpp"
#include "JUB_SDK_test_trx.hpp"

#include "mSIGNA/stdutils/uchar_vector.h"
#include "JUB_SDK_main.h"


void software_test_trx(const char* json_file) {

    JUB_RV rv = JUBR_ERROR;

    Json::Value root = readJSON(json_file);

//    JUB_CHAR_PTR mnemonic = nullptr;
//
//    rv = JUB_GenerateMnemonic_soft(STRENGTH128, &mnemonic);
//    cout << "JUB_GenerateMnemonic_soft() return " << GetErrMsg(rv) << endl;
//    if(JUBR_OK == rv) {
//        cout << "Generate mnemonic: " << mnemonic << endl;
//    }

    JUB_CHAR_CPTR mnemonic = "gauge hole clog property soccer idea cycle stadium utility slice hold chief";

    rv = JUB_CheckMnemonic(mnemonic);
    cout << "JUB_CheckMnemonic() return " << GetErrMsg(rv) << endl;
    if(JUBR_OK != rv) {
        return;
    }
    JUB_BYTE seed[64] = {0,};
    JUB_UINT16 seedLen = sizeof(seed)/sizeof(JUB_BYTE);
    auto callback = [](JUB_UINT32 current, JUB_UINT32 total) -> void {
        cout << ".";
    };

    rv = JUB_GenerateSeed_soft(mnemonic, "", seed, callback);
    cout << "JUB_GenerateSeed_soft() return " << GetErrMsg(rv) << endl;
    if (rv != JUBR_OK) {
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

    CONTEXT_CONFIG_TRX cfgeth;
    cfgeth.mainPath = (char*)root["main_path"].asCString();
    JUB_UINT16 contextID = 0;
    rv = JUB_CreateContextTRX_soft(cfgeth, masterXprv, &contextID);
    cout << "JUB_CreateContextTRX_soft() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        JUB_FreeMemory(masterXprv);
        return;
    }
    JUB_FreeMemory(masterXprv);

    JUB_CHAR_PTR xpub = nullptr;
    rv = JUB_GetMainHDNodeTRX(contextID, JUB_ENUM_PUB_FORMAT::HEX, &xpub);
    cout << "JUB_GetMainHDNodeTRX() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "main xpub in HEX: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["TRX"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["TRX"]["bip32_path"]["addressIndex"].asInt();
    rv = JUB_GetHDNodeTRX(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &xpub);
    cout << "JUB_GetHDNodeTRX() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK == rv) {
        cout << "xpub in HEX: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }

    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressTRX(contextID, path, BOOL_FALSE, &address);
    cout << "JUB_GetAddressTRX() return " << GetErrMsg(rv) << endl;
    std::string addr = "";
    if(JUBR_OK == rv) {
        cout << "address: " << address << endl;
        addr = std::string(address);
        JUB_FreeMemory(address);
    }

    JUB_CHAR_PTR addrInHex = nullptr;
    rv = JUB_CheckAddressTRX(contextID, addr.c_str(), &addrInHex);
    if(JUBR_OK == rv) {
        cout << "address in hex: " << addrInHex << endl;
        JUB_FreeMemory(addrInHex);
    }

    for (int i=1; i<=3; ++i) {
        switch (i) {
        case 1:
            std::cout << "transfer contract test..." << std::endl;
            break;
        case 2:
            std::cout << "transfer asset contract test..." << std::endl;
            break;
        case 3:
            std::cout << "trigger smart contract test..." << std::endl;
            break;
        }
        rv = transaction_proc_TRX(contextID, root, i);
        if (JUBR_OK != rv) {
            return;
        }
    }
}
