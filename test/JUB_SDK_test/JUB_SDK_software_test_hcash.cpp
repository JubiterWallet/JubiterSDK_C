//
//  JUB_SDK_software_test_hcash.cpp
//  JubSDKTest
//
//  Created by panmin on 2020/1/19.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include "JUB_SDK_software_test_hcash.hpp"

#include "mSIGNA/stdutils/uchar_vector.h"
#include "JUB_SDK_main.h"

void software_test_hcash() {

    JUB_RV rv = JUBR_ERROR;

    JUB_CHAR_PTR mnemonic = nullptr;
    rv = JUB_GenerateMnemonic(STRENGTH128, &mnemonic);
    if(rv == JUBR_OK) {
        cout << mnemonic << endl;
    }

    rv = JUB_CheckMnemonic(mnemonic);
    if(rv != JUBR_OK) {
        cout << "JUB_CheckMnemonic return" << rv << endl;
    }
    JUB_BYTE seed[64] = {0,};
    JUB_UINT16 seedLen = sizeof(seed)/sizeof(JUB_BYTE);
    auto callback = [](JUB_UINT32 current, JUB_UINT32 total) -> void {
        cout << ".";
    };

    rv = JUB_GenerateSeed("gauge hole clog property soccer idea cycle stadium utility slice hold chief", "", seed, callback);
    if (rv != JUBR_OK) {
        cout << "JUB_GenerateSeed error" << endl;
    }
    uchar_vector vSeed(seedLen);
    for (int i=0; i<seedLen; ++i) {
        vSeed[i] = seed[i];
    }
    cout << "seed: " << vSeed.getHex() << endl;
    cout << endl;

    // Special handling of hcash seed
//    seed := bip39.NewSeed(mnemonic, "")
//    hcSeed32 := make([]byte, 32)
//    for i := 0; i < 32; i++ {
//        hcSeed32[i] = byte((uint16(seed[i]) + uint16(seed[64-i-1])) >> 1)
//    }
//    masterKey, _ := bip32.NewMasterKey(hcSeed32)
    JUB_BYTE hcSeed32[32] = {0,};
    JUB_UINT16 hcSeed32Len = sizeof(hcSeed32)/sizeof(JUB_BYTE);
    for (int i=0; i<hcSeed32Len; ++i) {
        hcSeed32[i] = ((uint16_t(seed[i]) + uint16_t(seed[seedLen-i-1])) >> 1);
    }
    uchar_vector vHCSeed(hcSeed32Len);
    for (int i=0; i<hcSeed32Len; ++i) {
        vHCSeed[i] = hcSeed32[i];
    }
    cout << "hc seed: " << vHCSeed.getHex() << endl;
    cout << endl;

    JUB_CHAR_PTR masterXprv = nullptr;
    rv = JUB_SeedToMasterPrivateKey(hcSeed32, hcSeed32Len, secp256k1, &masterXprv);
    if (rv == JUBR_OK) {
        cout << masterXprv << endl;
    }

    CONTEXT_CONFIG_HC cfg;
    cfg.mainPath = (JUB_CHAR_PTR)"m/44'/171'/0'";

    JUB_UINT16 contextID;
    rv = JUB_CreateContextHC_soft(cfg, masterXprv, &contextID);
    if (rv != JUBR_OK) {
        cout << "JUB_CreateContextHC_soft return " << rv << endl;
    }

    BIP44_Path path;
    path.change = BOOL_FALSE;
    path.addressIndex = 0;
    JUB_CHAR_PTR  xpub = nullptr;
    rv = JUB_GetHDNodeHC(contextID, path, &xpub);
    if (rv == JUBR_OK) {
        cout << xpub << endl;
        JUB_FreeMemory(xpub);
    }

    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressHC(contextID, path, BOOL_FALSE, &address);
    if(rv == JUBR_OK){
        cout << address << endl;
        JUB_FreeMemory(address);
    }
}
