//
//  JUB_SDK_main.cpp
//  JubSDKTest
//
//  Created by panmin on 2019/9/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
#define SW 0
#if SW
#include "JUB_SDK_software_test.hpp"
#endif
using namespace std;

std::string GetErrMsg(JUB_RV rv) {

    std::string errMsg;
    switch (rv) {
        case JUBR_OK:                       { errMsg = "JUBR_OK"; break; }
        case JUBR_ERROR:                    { errMsg = "JUBR_ERROR"; break; }
        case JUBR_HOST_MEMORY:              { errMsg = "JUBR_HOST_MEMORY"; break; }
        case JUBR_ARGUMENTS_BAD:            { errMsg = "JUBR_ARGUMENTS_BAD"; break; }
        case JUBR_IMPL_NOT_SUPPORT:         { errMsg = "JUBR_IMPL_NOT_SUPPORT"; break; }
        case JUBR_MEMORY_NULL_PTR:          { errMsg = "JUBR_MEMORY_NULL_PTR"; break; }
        case JUBR_INVALID_MEMORY_PTR:       { errMsg = "JUBR_INVALID_MEMORY_PTR"; break; }
        case JUBR_REPEAT_MEMORY_PTR:        { errMsg = "JUBR_REPEAT_MEMORY_PTR"; break; }
        case JUBR_BUFFER_TOO_SMALL:         { errMsg = "JUBR_BUFFER_TOO_SMALL"; break; }

        default:                            { errMsg = "UNKNOWN ERROR."; break; }
    }

    return errMsg;
}

void error_exit(const char* message) {

    cout << message << endl;
    cout << "press any key to exit" << endl;
    char str[9] = { 0, };
    cin >> str;
    exit(0);
}

int main() {

    while(true) {
        cout << "--------------------------------------" << endl;
        cout << "|******* Jubiter Wallet Test ********|" << endl;
        cout << "|  1. hardware_test.                 |" << endl;
#if SW
        cout << "|  2. software_test.                 |" << endl;
#endif
        cout << "|  0. exit.                          |" << endl;
        cout << "--------------------------------------" << endl;
        cout << "* Please enter your choice:" << endl;

        int choice = 0;
        cin >> choice;
        switch (choice) {
            case 1:
                main_test();
                break;
#if SW
            case 2:
                software_test();
                break;
#endif
            case 0:
                exit(0);
            default:
                continue;
        }
    }

    return 0;
}