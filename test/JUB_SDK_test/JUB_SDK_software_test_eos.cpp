//
//  JUB_SDK_software_test_eos.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/9/19.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
#include "JUB_SDK_software_test_eos.hpp"

#include "mSIGNA/stdutils/uchar_vector.h"
#include "HexCoding.h"
#include "PublicKey.h"
#include "PrivateKey.h"

#include "EOS/Address.h"
#include "EOS/Action.h"
#include "EOS/Signer.h"
#include "EOS/Transaction.h"
#include "EOS/PackedTransaction.h"

#include "Bravo/Address.h"
#include "Bravo/Transaction.h"
#include "Bravo/Operation.h"
#include "Bravo/Serialization.h"
#include "Bravo/Deserialization.h"

#include "Base58.h"

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
    cfg.mainPath = (JUB_CHAR_PTR)"m/48'";//"m/44'/194'/0'";
    JUB_UINT16 contextID;
    rv = JUB_CreateContextEOS_soft(cfg, masterXprv, &contextID);
    if (rv != JUBR_OK) {
        cout << "JUB_CreateContextBTC_soft return " << rv << endl;
    }

//    BIP44_Path path;
//    path.change = BOOL_FALSE;
//    path.addressIndex = 0;
    BIP48_Path path;
    path.network = 4;
    path.role = JUB_ENUM_GRAPHENE_ROLE::ACTIVE;
    path.accountIndex = 0;
    path.keyIndex = 0;
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

    char* referenceBlockId = (char*)"000046dc08ad384ca452d92c59348aba888fcbb6ef1ebffc3181617706664d4c";
    int32_t referenceBlockTime = 1554121728;

    char* currency = (char*)"eosio.token1";
    char* from = (char*)"ypqazokngspt1";
    char* to = (char*)"godice.e1";
    char* asset = (char*)"0.1000 EOS";
    char* memo = (char*)"20,,";
    char* rawInJSON = nullptr;
    rv = JUB_SignTransactionEOS(contextID,
                                path,
                                referenceBlockId,
                                referenceBlockTime,
                                currency,
                                from,
                                to,
                                asset,
                                memo,
                                &rawInJSON);
    cout << "JUB_SignTransactionEOS return " << rawInJSON << endl;
    JUB_FreeMemory(rawInJSON);
}
