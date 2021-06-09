//
//  JUB_SDK_test_usdt.hpp
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

void USDT_test(JUB_UINT16 deviceID, JUB_CHAR_CPTR json_file);

void   transactionUSDT_test(JUB_UINT16 contextID, Json::Value root);
JUB_RV transactionUSDT_proc(JUB_UINT16 contextID, Json::Value root);

#endif /* JUB_SDK_test_btc_hpp */
