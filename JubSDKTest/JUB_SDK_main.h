//
//  JUB_SDK_main.h
//  JubSDKTest
//
//  Created by panmin on 2019/9/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_main_h
#define JUB_SDK_main_h

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <thread>
#include <json/json.h>
//#include "JUB_SDK.h"
using namespace std;

void error_exit(const char* message);
Json::Value readJSON(const char* json_file);

#endif /* JUB_SDK_main_h */
