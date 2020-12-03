//
//  JUB_SDK_test_dev.hpp
//  JubSDKTest
//
//  Created by panmin on 2019/9/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_test_dev_hpp
#define JUB_SDK_test_dev_hpp

#include <stdio.h>
#include <iostream>     // std::cout
#include <sstream>      // std::istringstream
#include <vector>
#include "../../include/JUB_SDK.h"

using namespace std;

vector<string> split(const string &str, char delim, bool bSkipEmpty = true);

JUB_RV verify_pin(JUB_UINT16 contextID);
JUB_RV verify_fgpt(JUB_UINT16 contextID);

void get_device_info_test(JUB_UINT16 deviceID);
void     set_timeout_test(JUB_UINT16 contextID);

void send_apdu_test(JUB_UINT16 deviceID);

#endif /* JUB_SDK_test_dev_hpp */
