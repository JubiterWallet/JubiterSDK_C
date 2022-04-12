//
//  JUB_SDK_test_ksm.hpp
//  JubSDKTest
//
//  Created by Administrator on 2021/3/29.
//  Copyright © 2021 ft. All rights reserved.
//

#ifndef JUB_SDK_test_ksm_hpp
#define JUB_SDK_test_ksm_hpp

#include <stdio.h>
#include <json/json.h>
#include "../../include/JUB_SDK_KSM.h"

void KSM_test(JUB_UINT16 deviceID, const char* json_file);

void  get_address_pubkey_KSM(JUB_UINT16 contextID);

#endif /* JUB_SDK_test_ksm_hpp */
