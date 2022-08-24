//
//  JUB_SDK_test_sol.hpp
//  JubSDKTest
//
//  Created by panmin on 2022/7/20.
//  Copyright © 2022 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_test_sol_hpp
#define JUB_SDK_test_sol_hpp
#include "../../include/JUB_SDK_COMM.h"
#include <json/json.h>
#include <stdio.h>
#include <string>

void SOL_test(JUB_UINT16 deviceID, JUB_CHAR_CPTR json_file);

void get_address_pubkey_SOL(JUB_UINT16 contextID, std::string mainPath);

void transaction_test_SOL(JUB_UINT16 contextID, const Json::Value &root);
JUB_RV transaction_proc_SOL(JUB_UINT16 contextID, const Json::Value &root);
void transaction_test_SOL_token_transfer(JUB_UINT16 contextID, const Json::Value &root);
JUB_RV transaction_proc_SOL_token_transfer(JUB_UINT16 contextID, const Json::Value &root);
void transaction_test_SOL_associated_token_create(JUB_UINT16 contextID, const Json::Value &root);
JUB_RV transaction_proc_SOL_associated_token_create(JUB_UINT16 contextID, const Json::Value &root);
//
#endif /* JUB_SDK_test_sol_hpp */
