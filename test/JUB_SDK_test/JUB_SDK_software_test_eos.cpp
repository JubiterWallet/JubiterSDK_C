//
//  JUB_SDK_software_test_eos.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/9/19.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
#include "JUB_SDK_software_test_eos.hpp"

//#include "mSIGNA/stdutils/uchar_vector.h"
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
    rv = JUB_GenerateSeed(
                          "gauge hole clog property soccer idea cycle stadium utility slice hold chief",
//                          "ensure token dress jar donate recipe once blue chief honey whip enhance",
                          "", seed, callback);
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
//    cfg.mainPath = (JUB_CHAR_PTR)"m/48'";
    cfg.mainPath = (JUB_CHAR_PTR)"m/44'/194'/0'";
    JUB_UINT16 contextID;
    rv = JUB_CreateContextEOS_soft(cfg, masterXprv, &contextID);
    if (rv != JUBR_OK) {
        cout << "JUB_CreateContextBTC_soft return " << rv << endl;
        return ;
    }

    BIP44_Path path;
    path.change = BOOL_FALSE;
    path.addressIndex = 0;
//    BIP48_Path path;
//    path.network = 4;
//    path.role = JUB_ENUM_GRAPHENE_ROLE::ACTIVE;
//    path.accountIndex = 0;
//    path.keyIndex = 0;
//    JUB_CHAR_PTR pub = nullptr;
//    rv = JUB_GetHDNodeEOS(contextID, format, path, &pub);
//    if (rv != JUBR_OK) {
//        cout << "JUB_GetHDNodeEOS return " << rv << endl;
//        return ;
//    }
//    cout << "JUB_GetHDNodeEOS return " << pub << endl;
//    JUB_FreeMemory(pub);
//
    JUB_CHAR_PTR address = nullptr;
    rv = JUB_GetAddressEOS(contextID,
                           path,
                           JUB_ENUM_BOOL::BOOL_FALSE,
                           &address);
    if (rv != JUBR_OK) {
        cout << "JUB_GetAddressEOS return " << rv << endl;
    }
    cout << "JUB_GetAddressEOS return " << address << endl;
    JUB_FreeMemory(address);

//    JUB_CHAR_PTR mainXpub = nullptr;
//    rv = JUB_GetMainHDNodeEOS(contextID,
//                              format,
//                              &mainXpub);
//    if (JUBR_OK != rv) {
//        return ;
//    }
//    cout << "JUB_GetMainHDNodeEOS return " << mainXpub << endl;
//    JUB_FreeMemory(mainXpub);

//    typedef struct stActionEOS {
//        JUB_ENUM_EOS_ACTION_TYPE type;
//        JUB_CHAR_PTR currency;
//        JUB_CHAR_PTR name;
//        union {
//            JUB_ACTION_TRANSFER transfer;
//            JUB_ACTION_DELEGATE delegate;
//            JUB_ACTION_BUYRAM buyRam;
//        };
//    } JUB_ACTION_EOS;
    JUB_ACTION_EOS action;

//    typedef struct stTransferAction {
//        JUB_CHAR_PTR from;
//        JUB_CHAR_PTR to;
//        JUB_CHAR_PTR asset;
//        JUB_CHAR_PTR memo;
//    } JUB_ACTION_TRANSFER;
    action.type = JUB_ENUM_EOS_ACTION_TYPE::XFER;
    action.currency = (char*)"metpacktoken";
    action.name     = (char*)"transfer";
    action.transfer.from  = (char*)"abcdenero123";
    action.transfer.to    = (char*)"gsy123451111";
    action.transfer.asset = (char*)"14.285 MPT";
    action.transfer.memo  = (char*)"@gmail.com";

//    typedef struct stDelegateAction {
//        JUB_CHAR_PTR from;
//        JUB_CHAR_PTR receiver;
//        JUB_CHAR_PTR unstakeNetQty; // unstake_net_quantity
//        JUB_CHAR_PTR unstakeCpuQty; // unstake_cpu_quantity
//    } JUB_ACTION_DELEGATE;
/*    action.type = JUB_ENUM_EOS_ACTION_TYPE::DELE;
    action.currency = (char*)"eosio";
    action.name     = (char*)"delegatebw";
    action.delegate.from     = (char*)"ftsafetest55";
    action.delegate.receiver = (char*)"ftsafetest55";
    action.delegate.unstakeCpuQty = (char*)"0.7000 EOS";
    action.delegate.unstakeNetQty = (char*)"0.3000 EOS";
*/
//    typedef struct stBuyRamAction {
//        JUB_CHAR_PTR payer;
//        JUB_CHAR_PTR quant;
//        JUB_CHAR_PTR receiver;
//    } JUB_ACTION_BUYRAM;
/*    action.type = JUB_ENUM_EOS_ACTION_TYPE::BUYRAM;
    action.currency = (char*)"eosio";
    action.name     = (char*)"buyram";
    action.buyRam.payer    = (char*)"punkneverdie";
    action.buyRam.receiver = (char*)"punkneverdie";
    action.buyRam.quant    = (char*)"0.4000 EOS";
*/
    JUB_CHAR_PTR actionsInJSON = nullptr;
    rv = JUB_BuildActionEOS(contextID,
                            &action, sizeof(action)/sizeof(JUB_ACTION_EOS),
                            &actionsInJSON);
    if (JUBR_OK != rv) {
        cout << "JUB_BuildActionEOS return " << rv << endl;
        return ;
    }
    cout << "JUB_BuildActionEOS return " << actionsInJSON << endl;

    char* chainID = (char*)"";
    char* expiration = (char*)"900";
    char* referenceBlockId = (char*)"05264c4d25b8e54b6bed3189f1b866c39ffb260ccfed170f0622e7386139a53a";
    char* referenceBlockTime = (char*)"1572004203";
    char* rawInJSON = nullptr;
    rv = JUB_SignTransactionEOS(contextID,
                                path,
                                chainID,
                                expiration,
                                referenceBlockId,
                                referenceBlockTime,
                                actionsInJSON,
                                &rawInJSON);
    if (JUBR_OK != rv) {
        cout << "JUB_SignTransactionEOS return " << rv << endl;
        return ;
    }
    cout << "JUB_SignTransactionEOS return " << rawInJSON << endl;
    JUB_FreeMemory(actionsInJSON);
    JUB_FreeMemory(rawInJSON);
}
