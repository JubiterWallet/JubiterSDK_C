//
//  JUB_SDK_test_trx.hpp
//  JubSDKTest
//
//  Created by panmin on 2020/9/11.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_test_trx_hpp
#define JUB_SDK_test_trx_hpp

#include <stdio.h>
#include <json/json.h>

#include "../../include/JUB_SDK_TRX.h"

#define TRC_20       20
#define TRC_721     721


void TRX_test(JUB_UINT16 deviceID, JUB_CHAR_CPTR json_file);

void  get_address_pubkey_TRX(JUB_UINT16 contextID);

void   transaction_test_TRX(JUB_UINT16 contextID, Json::Value root, int choice);
JUB_RV transaction_proc_TRX(JUB_UINT16 contextID, Json::Value root, int choice);
JUB_RV pack_contract_proc(JUB_UINT16 contextID, Json::Value root,
                          int choice,
                          std::string& packedContract,
                          int trc = TRC_20);

void   bytestring_test_TRX(JUB_UINT16 contextID, Json::Value root);
JUB_RV bytestring_proc_TRX(JUB_UINT16 contextID, Json::Value root);

#endif /* JUB_SDK_test_trx_hpp */
