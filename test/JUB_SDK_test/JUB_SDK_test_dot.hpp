//
//  JUB_SDK_test_dot.hpp
//  JubSDKTest
//
//  Created by Administrator on 2021/3/29.
//  Copyright © 2021 ft. All rights reserved.
//

#ifndef JUB_SDK_test_dot_hpp
#define JUB_SDK_test_dot_hpp

#include <stdio.h>
#include <json/json.h>
#include "../../include/JUB_SDK_DOT.h"

void DOT_test(JUB_UINT16 deviceID, const char* json_file);

void  get_address_pubkey_DOT(JUB_UINT16 contextID, Json::Value root);

void   transaction_test_DOT(JUB_UINT16 contextID, Json::Value root);
JUB_RV transaction_proc_DOT(JUB_UINT16 contextID, Json::Value root);

#endif /* JUB_SDK_test_dot_hpp */
