//
//  JUB_SDK_test_eth.hpp
//  JubSDKTest
//
//  Created by panmin on 2019/9/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_test_eth_hpp
#define JUB_SDK_test_eth_hpp

#include <stdio.h>
#include <json/json.h>

#include "../../include/JUB_SDK_ETH.h"
#include "../../include/JUB_SDK_BTC.h"


void ETH_test(JUB_UINT16 deviceID, const char* json_file);

void set_my_address_test_ETH(JUB_UINT16 contextID);
void  get_address_pubkey_ETH(JUB_UINT16 contextID);

void   transaction_test_ETH(JUB_UINT16 contextID, Json::Value root);
JUB_RV transaction_proc_ETH(JUB_UINT16 contextID, Json::Value root);
void   transaction_test_ERC20_ETH(JUB_UINT16 contextID, Json::Value root);
JUB_RV transaction_proc_ERC20_ETH(JUB_UINT16 contextID, Json::Value root);
void   transaction_test_contr_ETH(JUB_UINT16 contextID, Json::Value root);
JUB_RV transaction_proc_contr_ETH(JUB_UINT16 contextID, Json::Value root);
void   bytestring_test_ETH(JUB_UINT16 contextID, Json::Value root);
JUB_RV bytestring_proc_ETH(JUB_UINT16 contextID, Json::Value root);

#endif /* JUB_SDK_test_eth_hpp */
