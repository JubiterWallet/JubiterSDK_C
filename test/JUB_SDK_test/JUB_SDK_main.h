//
//  JUB_SDK_main.h
//  JubSDKTest
//
//  Created by ft on 2019/9/17.
//  Copyright © 2019 ft. All rights reserved.
//

#ifndef JUB_SDK_main_h
#define JUB_SDK_main_h

#include <vector>
#include <iostream>
#include <json/json.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <cstring>
#include <thread>
using namespace std;

std::string GetErrMsg(JUB_RV rv);
void error_exit(const char* message);

#endif /* JUB_SDK_main_h */
