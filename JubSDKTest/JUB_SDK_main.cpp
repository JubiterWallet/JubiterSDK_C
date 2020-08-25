//
//  JUB_SDK_main.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/9/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_main.h"
using namespace std;

void error_exit(const char* message) {

    cout << message << endl;
    cout << "press any key to exit" << endl;
    char str[9] = { 0, };
    cin >> str;
    exit(0);
}

Json::Value readJSON(const char* json_file) {
    Json::CharReaderBuilder builder;
    Json::Value root;
    ifstream in(json_file, ios::binary);
    if (!in.is_open()) {
        error_exit("Error opening json file\n");
    }
    JSONCPP_STRING errs;
    if (!parseFromStream(builder, in, &root, &errs)) {
        error_exit("Error parse json file\n");
    }

    return root;
}
