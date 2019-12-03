//
//  JUB_SDK_software_test_eth.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/9/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_software_test_eth.hpp"

#include "mSIGNA/stdutils/uchar_vector.h"
#include "JUB_SDK_main.h"

void software_test_eth() {
    JUB_CHAR_PTR mnemonic = nullptr;

    JUB_RV rv = JUB_GenerateMnemonic(STRENGTH128, &mnemonic);
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

    JUB_CHAR_PTR masterXprv = nullptr;
    rv = JUB_SeedToMasterPrivateKey(seed, seedLen, secp256k1, &masterXprv);
    if (rv == JUBR_OK) {
        cout << masterXprv << endl;
    }

    CONTEXT_CONFIG_ETH cfgeth;
    cfgeth.chainID = 1;
    cfgeth.mainPath = (JUB_CHAR_PTR)"m/44'/60'/0'";
    JUB_UINT16 contextID = 0;
    rv = JUB_CreateContextETH_soft(cfgeth, masterXprv, &contextID);
    if (rv != JUBR_OK) {
        cout << "JUB_CreateContextETH_soft return " << rv << endl;
    }

    BIP44_Path path;
    path.change = BOOL_FALSE;
    path.addressIndex = 0;
    JUB_CHAR_PTR xpub = nullptr;
        rv = JUB_GetHDNodeETH(contextID, JUB_ENUM_ETH_PUB_FORMAT::HEX, path, &xpub);
    if (rv == JUBR_OK) {
        cout << xpub << endl;
        JUB_FreeMemory(xpub);
    }

    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressETH(contextID, path, BOOL_FALSE, &address);
    if(rv == JUBR_OK){
        cout << address << endl;
        JUB_FreeMemory(address);
    }

//    JUB_UINT16 deviceIDs[MAX_DEVICE] = {0,};
//    fill_n(deviceIDs, MAX_DEVICE, 0xffff);
//    rv = JUB_ListDeviceHid(deviceIDs);
//
//    rv = JUB_ConnetDeviceHid(deviceIDs[0]);
//
//    CONTEXT_CONFIG_ETH cfg_eth;
//    cfg_eth.mainPath = "m/44'/60'/0'";
//    cfg_eth.chainID = 1;
//    JUB_UINT16 contextID_hard = 0;
//    rv = JUB_CreateContextETH(cfg_eth, deviceIDs[0], &contextID_hard);
//    if (JUBR_OK != rv) {
//        cout << "JUB_CreateContextETH() return " << GetErrMsg(rv) << endl;
//        return;
//    }

    uint32_t nonce = 5;//.asDouble();
    uint32_t gasLimit = 21000;//.asDouble();
    char* gasPriceInWei = "10000000000";
    char* valueInWei = "500000000000000";
    char* to = "0xef31DEc147DCDcd64F6a0ABFA7D441B62A216BC9";
    char* data = "4A75626974657257616C6C6574";

    char* raw = nullptr;
    rv = JUB_SignTransactionETH(contextID,
                                path, nonce,
                                gasLimit, gasPriceInWei,
                                to,
                                valueInWei, data,
                                &raw);
    if (JUBR_OK != rv) {
        cout << "JUB_SignTransactionETH() return " << GetErrMsg(rv) << endl;
        return;
    }
    else {
        cout << "raw : " << raw << endl;
        JUB_FreeMemory(raw);
    }
}
