//
//  JUB_SDK_software_test_eos.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/9/19.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
#include "JUB_SDK_software_test_eos.hpp"

void software_test_eos() {
    JUB_RV rv = JUBR_ERROR;

    JUB_ENUM_EOS_PUB_FORMAT format = JUB_ENUM_EOS_PUB_FORMAT::EOS;
/*    JUB_CHAR_PTR mnemonic = nullptr;
    JUB_RV rv = JUB_GenerateMnemonic(STRENGTH128, &mnemonic);
    if(rv == JUBR_OK) {
        cout << mnemonic << endl;
    }

    rv = JUB_CheckMnemonic(mnemonic);
    if(rv != JUBR_OK) {
        cout << "JUB_CheckMnemonic return" << rv << endl;
    }
*/
    JUB_BYTE seed[64] = {0,};
    auto callback = [](JUB_UINT32 current, JUB_UINT32 total) -> void {
        cout << ".";
    };
    rv = JUB_GenerateSeed("gauge hole clog property soccer idea cycle stadium utility slice hold chief", "", seed, callback);
    if (rv != JUBR_OK) {
        cout << "JUB_GenerateSeed error" << endl;
    }
    cout << endl;

    JUB_CHAR_PTR masterXprv = nullptr;
    rv = JUB_SeedToMasterPrivateKey(seed, 64,
                                    secp256k1,
//                                    ed25519,
                                    &masterXprv);
    if (rv == JUBR_OK) {
        cout << masterXprv << endl;
    }

    CONTEXT_CONFIG_EOS cfg;
    cfg.mainPath = (JUB_CHAR_PTR)"m/44'/194'/0'";//"m/48'/4'/0'/0'/0'";
    JUB_UINT16 contextID;
    rv = JUB_CreateContextEOS_soft(cfg, masterXprv, &contextID);
    if (rv != JUBR_OK) {
        cout << "JUB_CreateContextBTC_soft return " << rv << endl;
    }

    BIP44_Path path;
    path.change = BOOL_FALSE;
    path.addressIndex = 0;
    JUB_CHAR_PTR pub = nullptr;
    rv = JUB_GetHDNodeEOS(contextID, format, path, &pub);
    if (rv != JUBR_OK) {
        cout << "JUB_GetHDNodeEOS return " << rv << endl;
    }
    cout << "JUB_GetHDNodeEOS return " << pub << endl;
    JUB_FreeMemory(pub);

    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressEOS(contextID,
                           path,
                           JUB_ENUM_BOOL::BOOL_FALSE,
                           &address);
    if (rv != JUBR_OK) {
        cout << "JUB_GetAddressEOS return " << rv << endl;
    }
    cout << "JUB_GetHDNodeEOS return " << address << endl;
    JUB_FreeMemory(address);

    JUB_CHAR_PTR mainXpub = nullptr;
    rv = JUB_GetMainHDNodeEOS(contextID,
                              format,
                              &mainXpub);
    cout << "JUB_GetMainHDNodeEOS return " << mainXpub << endl;
    JUB_FreeMemory(mainXpub);
}
