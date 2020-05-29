//
//  JUB_SDK_main.h
//  JubSDKTest
//
//  Created by ft on 2019/9/17.
//  Copyright © 2019 ft. All rights reserved.
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
void error_exit(const char* message);
Json::Value readJSON(const char* json_file);

#endif /* JUB_SDK_main_h */
