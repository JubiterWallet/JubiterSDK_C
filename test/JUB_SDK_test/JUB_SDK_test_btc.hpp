//
//  JUB_SDK_test_btc.hpp
//  JubSDKTest
//
//  Created by panmin on 2019/9/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_test_btc_hpp
#define JUB_SDK_test_btc_hpp

#include <stdio.h>
#include <json/json.h>

#include "../../include/JUB_SDK_BTC.h"

#include "JUB_SDK_test_usdt.hpp"

void BTC_test(JUB_UINT16 deviceID, JUB_CHAR_CPTR json_file, JUB_ENUM_COINTYPE_BTC coinType);

void  get_address_test(JUB_UINT16 contextID, Json::Value root);
void show_address_test(JUB_UINT16 contextID);
void set_my_address_test_BTC(JUB_UINT16 contextID);

void   transaction_test(JUB_UINT16 contextID, Json::Value root);
JUB_RV transaction_proc(JUB_UINT16 contextID, Json::Value root);

#endif /* JUB_SDK_test_btc_hpp */
