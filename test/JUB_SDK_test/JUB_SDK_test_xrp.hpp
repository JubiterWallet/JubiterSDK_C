//
//  JUB_SDK_test_xrp.hpp
//  JubSDKTest
//
//  Created by panmin on 2019/12/03.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_test_xrp_hpp
#define JUB_SDK_test_xrp_hpp

#include <stdio.h>
#include <json/json.h>

#include "../../include/JUB_SDK_XRP.h"

void XRP_test(const char* json_file);

void set_my_address_test_XRP(JUB_UINT16 contextID);
void  get_address_pubkey_XRP(JUB_UINT16 contextID);

void   transaction_test_XRP(JUB_UINT16 contextID, Json::Value root);
JUB_RV transaction_proc_XRP(JUB_UINT16 contextID, Json::Value root);

#endif /* JUB_SDK_test_xrp_hpp */
