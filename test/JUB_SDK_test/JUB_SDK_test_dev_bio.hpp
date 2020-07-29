//
//  JUB_SDK_test_dev_bio.hpp
//  JubSDKTest
//
//  Created by panmin on 2020/7/29.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_test_dev_bio_hpp
#define JUB_SDK_test_dev_bio_hpp

#include <stdio.h>
#include <iostream>     // std::cout
#include <sstream>      // std::istringstream
#include <vector>
#include "../../include/JUB_SDK.h"

using namespace std;

void device_fgpt_enroll_test(JUB_UINT16 deviceID, int fgptIndex);
void device_fgpt_test(JUB_UINT16 deviceID);

#endif /* JUB_SDK_test_dev_bio_hpp */
