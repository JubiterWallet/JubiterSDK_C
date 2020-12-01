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
#include "Tron/Address.h"
//#include "Tron/Signer.h"
#include "PublicKey.h"
#include "PrivateKey.h"

//#include "xrp_pb_test.hpp"
//#include "xrp_pb_test.c"

void software_test_trx(const char* json_file) {
//    simple_pb_test();
//    trx_transfer_asset_pb_test();
//    trx_pb_test();
//    return;

    JUB_RV rv = JUBR_ERROR;

    //priv: 2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54
    TW::PrivateKey privateKey = TW::PrivateKey(uchar_vector("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
    const auto addr = TW::Tron::Address(publicKey);
    std::cout << "addr: " << addr.string() << std::endl;


    Json::Value root = readJSON(json_file);

//    JUB_CHAR_PTR mnemonic = nullptr;
//
//    rv = JUB_GenerateMnemonic_soft(STRENGTH128, &mnemonic);
//    if(rv == JUBR_OK) {
//        cout << "JUB_GenerateMnemonic_soft() return " << mnemonic << endl;
//    }

    JUB_CHAR_CPTR mnemonic = "gauge hole clog property soccer idea cycle stadium utility slice hold chief";

    rv = JUB_CheckMnemonic(mnemonic);
    if(rv != JUBR_OK) {
        cout << "JUB_CheckMnemonic() return" << rv << endl;
    }
    JUB_BYTE seed[64] = {0,};
    JUB_UINT16 seedLen = sizeof(seed)/sizeof(JUB_BYTE);
    auto callback = [](JUB_UINT32 current, JUB_UINT32 total) -> void {
        cout << ".";
    };

    rv = JUB_GenerateSeed_soft(mnemonic, "", seed, callback);
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

    CONTEXT_CONFIG_TRX cfgeth;
    cfgeth.mainPath = (char*)root["main_path"].asCString();
    JUB_UINT16 contextID = 0;
    rv = JUB_CreateContextTRX_soft(cfgeth, masterXprv, &contextID);
    if (rv != JUBR_OK) {
        cout << "JUB_CreateContextETH_soft() return " << rv << endl;
    }

    JUB_CHAR_PTR xpub = nullptr;
    rv = JUB_GetMainHDNodeTRX(contextID, JUB_ENUM_PUB_FORMAT::HEX, &xpub);
    if (rv != JUBR_OK) {
        cout << "JUB_GetMainHDNodeTRX() return " << rv << endl;
    }
    else {
        cout << "JUB_GetMainHDNodeTRX() in HEX: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }

    rv = JUB_GetMainHDNodeTRX(contextID, JUB_ENUM_PUB_FORMAT::XPUB, &xpub);
    if (rv != JUBR_OK) {
        cout << "JUB_GetMainHDNodeTRX() return " << rv << endl;
    }
    else {
        cout << "JUB_GetMainHDNodeTRX() in XPUB: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["TRX"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["TRX"]["bip32_path"]["addressIndex"].asInt();
    rv = JUB_GetHDNodeTRX(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &xpub);
    if (rv != JUBR_OK) {
        cout << "JUB_GetHDNodeTRX() return " << rv << endl;
    }
    else {
        cout << "JUB_GetHDNodeTRX() in HEX: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }

    rv = JUB_GetHDNodeTRX(contextID, JUB_ENUM_PUB_FORMAT::XPUB, path, &xpub);
    if (rv != JUBR_OK) {
        cout << "JUB_GetHDNodeTRX() return " << rv << endl;
    }
    else {
        cout << "JUB_GetHDNodeTRX() in XPUB: " << xpub << endl;
        JUB_FreeMemory(xpub);
    }

    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressTRX(contextID, path, BOOL_FALSE, &address);
    if(rv != JUBR_OK) {
        cout << "JUB_GetAddressTRX() return " << rv << endl;
    }
    else {
        cout << "JUB_GetAddressTRX() return " << address << endl;
        JUB_FreeMemory(address);
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
