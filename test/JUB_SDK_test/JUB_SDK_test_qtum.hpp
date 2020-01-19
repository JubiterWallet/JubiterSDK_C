//
//  JUB_SDK_test_qtum.hpp
//  JubSDKTest
//
//  Created by panmin on 2020/1/19.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_test_qtum_hpp
#define JUB_SDK_test_qtum_hpp

#include <stdio.h>
#include <json/json.h>

#include "../../include/JUB_SDK_BTC.h"

void QTUM_test(JUB_UINT16 deviceID, const char* json_file);
void transactionQTUM_test(JUB_UINT16 contextID, Json::Value root);


#endif /* JUB_SDK_test_qtum_hpp */
