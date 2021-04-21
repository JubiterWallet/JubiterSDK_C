//
//  JUB_SDK_test_dev_swi.hpp
//  JubSDKTest
//
//  Created by panmin on 2021/4/21.
//  Copyright © 2021 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_test_dev_swi_hpp
#define JUB_SDK_test_dev_swi_hpp

#include <stdio.h>
#include <json/json.h>

#include "../../include/JUB_SDK.h"

JUB_RV menu_opt_swi(const int choice, const Json::Value& root, JUB_UINT16_PTR pDeviceID);

#endif /* JUB_SDK_test_dev_swi_hpp */
