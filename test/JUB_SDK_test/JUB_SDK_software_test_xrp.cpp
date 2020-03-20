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
//#include "HexCoding.h"
//#include "PublicKey.h"
//#include "PrivateKey.h"
//
//#include "EOS/Address.h"
//#include "EOS/Action.h"
//#include "EOS/Signer.h"
//#include "EOS/Transaction.h"
//#include "EOS/PackedTransaction.h"
//
//#include "Bravo/Address.h"
//#include "Bravo/Transaction.h"
//#include "Bravo/Operation.h"
//#include "Bravo/Serialization.h"
//#include "Bravo/Deserialization.h"

#include "Base58.h"

void software_test_xrp(const char* json_file) {

    Json::Value root = readJSON(json_file);

    JUB_RV rv = JUBR_ERROR;

    JUB_BYTE seed[64] = {0,};
    JUB_UINT16 seedLen = sizeof(seed)/sizeof(JUB_BYTE);
    auto callback = [](JUB_UINT32 current, JUB_UINT32 total) -> void {
        cout << ".";
    };
    JUB_CHAR_CPTR mnemonic = "gauge hole clog property soccer idea cycle stadium utility slice hold chief";
//    JUB_CHAR_CPTR mnemonic = "ensure token dress jar donate recipe once blue chief honey whip enhance";
    rv = JUB_GenerateSeed(mnemonic, "", seed, callback);
    if (rv != JUBR_OK) {
        cout << "JUB_GenerateSeed error" << endl;
    }
    uchar_vector vSeed(seedLen);
    for (int i=0; i<seedLen; ++i) {
        vSeed[i] = seed[i];
    }
    cout << "seed: " << vSeed.getHex() << endl;
    cout << endl;

    JUB_CHAR_PTR masterXprv = nullptr;
    rv = JUB_SeedToMasterPrivateKey(seed, seedLen,
                                    secp256k1,
//                                    ed25519,
                                    &masterXprv);
    if (rv == JUBR_OK) {
        cout << masterXprv << endl;
    }

    CONTEXT_CONFIG_XRP cfg;
    cfg.mainPath = (char*)root["main_path"].asCString();
    JUB_UINT16 contextID;
    rv = JUB_CreateContextXRP_soft(cfg, masterXprv, &contextID);
    if (rv != JUBR_OK) {
        cout << "JUB_CreateContextXRP_soft return " << rv << endl;
        return;
    }

    JUB_CHAR_PTR mainXpub = nullptr;
    rv = JUB_GetMainHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::HEX, &mainXpub);
    if (JUBR_OK != rv) {
        return;
    }
    cout << "JUB_GetMainHDNodeXRP return " << mainXpub << endl;
    JUB_FreeMemory(mainXpub);

    rv = JUB_GetMainHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::XPUB, &mainXpub);
    if (JUBR_OK != rv) {
        return;
    }
    cout << "JUB_GetMainHDNodeXRP return " << mainXpub << endl;
    JUB_FreeMemory(mainXpub);

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["XRP"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["XRP"]["bip32_path"]["addressIndex"].asInt();
    JUB_CHAR_PTR pub = nullptr;
    rv = JUB_GetHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &pub);
    if (rv != JUBR_OK) {
        cout << "JUB_GetHDNodeXRP return " << rv << endl;
        return;
    }
    cout << "JUB_GetHDNodeXRP return " << pub << endl;
    JUB_FreeMemory(pub);

    rv = JUB_GetHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::XPUB, path, &pub);
    if (rv != JUBR_OK) {
        cout << "JUB_GetHDNodeXRP return " << rv << endl;
        return;
    }
    cout << "JUB_GetHDNodeXRP return " << pub << endl;
    JUB_FreeMemory(pub);

    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressXRP(contextID,
                           path,
                           JUB_ENUM_BOOL::BOOL_FALSE,
                           &address);
    if (rv != JUBR_OK) {
        cout << "JUB_GetAddressXRP return " << rv << endl;
    }
    cout << "JUB_GetAddressXRP return " << address << endl;
    JUB_FreeMemory(address);

    rv = transaction_proc_XRP(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}
