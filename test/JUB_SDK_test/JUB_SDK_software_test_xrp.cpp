//
//  JUB_SDK_software_test_xrp.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/11/20.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
#include "JUB_SDK_software_test_xrp.hpp"

#include "mSIGNA/stdutils/uchar_vector.h"
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

void software_test_xrp() {
    JUB_RV rv = JUBR_ERROR;

/*    JUB_ENUM_EOS_PUB_FORMAT format = JUB_ENUM_EOS_PUB_FORMAT::EOS;
    JUB_CHAR_PTR mnemonic = nullptr;
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
    cfg.mainPath = (JUB_CHAR_PTR)"m/44'/144'/0'";
    JUB_UINT16 contextID;
    rv = JUB_CreateContextXRP_soft(cfg, masterXprv, &contextID);
    if (rv != JUBR_OK) {
        cout << "JUB_CreateContextXRP_soft return " << rv << endl;
        return ;
    }

    BIP44_Path path;
    path.change = BOOL_FALSE;
    path.addressIndex = 0;
    JUB_CHAR_PTR pub = nullptr;
    rv = JUB_GetHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &pub);
    if (rv != JUBR_OK) {
        cout << "JUB_GetHDNodeXRP return " << rv << endl;
        return ;
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

    JUB_CHAR_PTR mainXpub = nullptr;
    rv = JUB_GetMainHDNodeXRP(contextID,
                              JUB_ENUM_PUB_FORMAT::HEX,
                              &mainXpub);
    if (JUBR_OK != rv) {
        return ;
    }
    cout << "JUB_GetMainHDNodeXRP return " << mainXpub << endl;
    JUB_FreeMemory(mainXpub);

//typedef struct stPaymentXRP {
//    JUB_ENUM_XRP_PYMT_TYPE type;
//    JUB_CHAR_PTR invoiceID; // [Optional]
//    JUB_CHAR_PTR sendMax;   // [Optional]
//    JUB_CHAR_PTR deliverMin;// [Optional]
//    union {
//        JUB_PYMT_DXRP dxrp;
//        JUB_PYMT_DXRP fx;
//    };
//} JUB_PYMT_XRP;
//typedef struct stTxXRP {
//    JUB_CHAR_PTR account;
//    JUB_ENUM_XRP_TX_TYPE type;
//    JUB_CHAR_PTR fee;
//    JUB_CHAR_PTR sequence;
//    JUB_CHAR_PTR accountTxnID;
//    JUB_CHAR_PTR flags;
//    JUB_CHAR_PTR lastLedgerSequence;
//    JUB_CHAR_PTR memos;
//    JUB_CHAR_PTR sourceTag;
//    union {
//        JUB_PYMT_XRP pymt;
//    };
//} JUB_TX_XRP;
    JUB_TX_XRP xrp;
    xrp.type  = JUB_ENUM_XRP_TX_TYPE::PYMT;
    xrp.account  = (char*)"r9LqNeG6qHxjeUocjvVki2XR35weJ9mZgQ";
    xrp.fee   = (char*)"10";
    xrp.flags = (char*)"2147483648";
    xrp.sequence = (char*)"1";
    xrp.lastLedgerSequence = (char*)"0";
//typedef struct stDxrpPymt {
//    JUB_CHAR_PTR destination;
//    JUB_CHAR_PTR amount;
//    JUB_CHAR_PTR destinationTag;
//} JUB_PYMT_DXRP;
    xrp.pymt.type = JUB_ENUM_XRP_PYMT_TYPE::DXRP;
    xrp.pymt.destination    = (char*)"rHb9CJAWyB4rj91VRWn96DkukG4bwdtyTh";
    xrp.pymt.amount.value   = (char*)"1000";
    xrp.pymt.destinationTag = (char*)"0";
    char* raw = nullptr;
    rv = JUB_SignTransactionXRP(contextID,
                                path,
                                xrp,
                                &raw);
    if (JUBR_OK != rv) {
        cout << "JUB_SignTransactionXRP return " << rv << endl;
        return ;
    }
    cout << "JUB_SignTransactionXRP return " << raw << endl;
    JUB_FreeMemory(raw);
}
