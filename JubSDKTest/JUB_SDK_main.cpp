//
//  JUB_SDK_main.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/9/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

//#include "JUB_SDK_test.h"
#include "JUB_SDK_main.h"
using namespace std;

std::string GetErrMsg(JUB_RV rv) {

    std::string errMsg;
    switch (rv) {
    case JUBR_OK:                   { errMsg = "JUBR_OK"; break; }
    case JUBR_ERROR:                { errMsg = "JUBR_ERROR"; break; }
    case JUBR_HOST_MEMORY:          { errMsg = "JUBR_HOST_MEMORY"; break; }
    case JUBR_ARGUMENTS_BAD:        { errMsg = "JUBR_ARGUMENTS_BAD"; break; }
    case JUBR_IMPL_NOT_SUPPORT:     { errMsg = "JUBR_IMPL_NOT_SUPPORT"; break; }
    case JUBR_MEMORY_NULL_PTR:      { errMsg = "JUBR_MEMORY_NULL_PTR"; break; }
    case JUBR_INVALID_MEMORY_PTR:   { errMsg = "JUBR_INVALID_MEMORY_PTR"; break; }
    case JUBR_REPEAT_MEMORY_PTR:    { errMsg = "JUBR_REPEAT_MEMORY_PTR"; break; }
    case JUBR_BUFFER_TOO_SMALL:     { errMsg = "JUBR_BUFFER_TOO_SMALL"; break; }

    default:                        { errMsg = "UNKNOWN ERROR."; break; }
    }   // switch (rv) end

    return errMsg;
}

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
