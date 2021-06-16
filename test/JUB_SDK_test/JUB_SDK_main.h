//
//  JUB_SDK_main.h
//  JubSDKTest
//
//  Created by panmin on 2019/9/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_main_h
#define JUB_SDK_main_h

#define SW 1
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <thread>
#include <json/json.h>
using namespace std;

std::string GetErrMsg(JUB_RV rv);
void error_exit(JUB_CHAR_CPTR message);
Json::Value readJSON(JUB_CHAR_CPTR json_file);

#endif /* JUB_SDK_main_h */
