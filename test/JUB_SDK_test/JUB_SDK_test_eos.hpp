//
//  JUB_SDK_test_eos.hpp
//  JubSDKTest
//
//  Created by panmin on 2019/11/11.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_test_eos_hpp
#define JUB_SDK_test_eos_hpp

#include <stdio.h>
#include <json/json.h>

#include "../../include/JUB_SDK_EOS.h"


void EOS_test(JUB_UINT16 deviceID, const char* json_file);

void  get_address_pubkey_EOS(JUB_UINT16 contextID);

void   transaction_test_EOS(JUB_UINT16 contextID, Json::Value root, int choice);
JUB_RV transaction_proc_EOS(JUB_UINT16 contextID, Json::Value root, int choice);

#endif /* JUB_SDK_test_eos_hpp */
