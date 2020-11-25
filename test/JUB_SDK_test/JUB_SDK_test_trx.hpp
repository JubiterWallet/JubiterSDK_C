//
//  JUB_SDK_test_trx.hpp
//  JubSDKTest
//
//  Created by panmin on 2020/9/11.
//  Copyright © 2020 ft. All rights reserved.
//

#ifndef JUB_SDK_test_trx_hpp
#define JUB_SDK_test_trx_hpp

#include <stdio.h>
#include <json/json.h>

#include "../../include/JUB_SDK_TRX.h"


void TRX_test(const char* json_file);

void set_my_address_test_TRX(JUB_UINT16 contextID);
void get_address_pubkey_TRX(JUB_UINT16 contextID);

void transaction_test_TRX(JUB_UINT16 contextID, Json::Value root);
JUB_RV transaction_proc_TRX(JUB_UINT16 contextID, Json::Value root);
JUB_RV pack_contract_proc(JUB_UINT16 contextID, Json::Value root,
                          int choice,
                          std::string& packedContract);
void transaction_test_TRC20(JUB_UINT16 contextID, Json::Value root);
JUB_RV transaction_proc_TRC20(JUB_UINT16 contextID, Json::Value root);

#endif /* JUB_SDK_test_trx_hpp */
