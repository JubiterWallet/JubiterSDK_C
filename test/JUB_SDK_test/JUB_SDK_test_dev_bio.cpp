//
//  JUB_SDK_test_dev_bio.cpp
//  JubSDKTest
//
//  Created by panmin on 2020/7/29.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
#include "JUB_SDK_test_dev.hpp"
#include "../../include/JUB_SDK_COMM.h"
#include "../../include/JUB_SDK_DEV_BIO.h"

#include "JUB_SDK_main.h"

void device_fgpt_enroll_test(JUB_UINT16 deviceID, int fgptIndex) {

    JUB_RV rv = JUBR_ERROR;

    JUB_BYTE fgptID = fgptIndex;
    JUB_ULONG times = 0;
    JUB_BYTE modalityID = 0;
    rv = JUB_EnrollFingerprint(deviceID, &fgptID, &times, &modalityID);
    std::cout << "JUB_EnrollFingerprint return " << rv << std::endl;
    if (JUBR_OK == rv) {
        std::cout << "The total number of times that need to enroll for _NO."
                  << modalityID
                  << "_ fingerprint is _" << fgptID << "/" << times << "_." << std::endl;
    }

    JUB_BYTE _fgptID = 0;
    JUB_ULONG _times = 0;
    JUB_BYTE _modalityID = 0;
    for (JUB_BYTE i=fgptID; i<times; ++i) {
        if (1 == i) {
            _fgptID = fgptID;
            _times = times;
            _modalityID = modalityID;
        }
        rv = JUB_EnrollFingerprint(deviceID, &_fgptID, &_times, &_modalityID);
        std::cout << "JUB_EnrollFingerprint return " << rv << std::endl;
        if (JUBR_OK == rv) {
            std::cout << "The total number of times that need to enroll for _NO."
                      << _modalityID
                      << "_ fingerprint is _" << _fgptID << "/" << _times << "_." << std::endl;
        }
    }
}


void device_fgpt_test(JUB_UINT16 deviceID) {

    JUB_RV rv = JUBR_ERROR;

    while (true) {
        cout << "|----------------------------------|" << endl;
        cout << "|******* G2 fingerprint test ******|" << endl;
        cout << "| 1. Enumerate Fingerprint.        |" << endl;
        cout << "| 2. Enroll    Fingerprint.        |" << endl;
        cout << "| 3. Delete    Fingerprint.        |" << endl;
        cout << "| 4. Erase     Fingerprint.        |" << endl;
        cout << "|                                  |" << endl;
        cout << "| 9. return.                       |" << endl;
        cout << "|----------------------------------|" << endl;
        cout << "* Please enter your choice:         " << endl;

        int choice = 0;
        std::cin >> choice;
        if (0 == choice) {
            exit(0);
        }

        switch (choice) {
        case 1:
        {
            JUB_CHAR_PTR fgptList;
            rv = JUB_EnumFingerprint(deviceID, &fgptList);
            std::cout << "JUB_EnumFingerprint return " << rv << std::endl;
            if (JUBR_OK == rv) {
                std::cout << "finger print list: " << fgptList << std::endl;
                JUB_FreeMemory(fgptList);
            }
            break;
        }
        case 2:
        {
            int fgptIndex = 0;
            std::cout << "Please enter your fingerprint index for enroll:" << std::endl;
            std::cin >> fgptIndex;
            device_fgpt_enroll_test(deviceID, fgptIndex);
            break;
        }
        case 3:
        {
            int fgptID = 0;
            std::cout << "Please enter your fingerprint ID for delete:" << std::endl;
            std::cin >> fgptID;
            rv = JUB_DeleteFingerprint(deviceID, fgptID);
            std::cout << "JUB_DeleteFingerprint return " << rv << std::endl;
            break;
        }
        case 4:
        {
            char ch;
            std::cout << "Erase all fingerprint? Y:N" << std::endl;
            std::cin >> ch;
            if ('Y' == ch) {
                rv = JUB_EraseFingerprint(deviceID);
                std::cout << "JUB_EraseFingerprint return " << rv << std::endl;
            }
            break;
        }
        case 9:
            main_test();
            break;
        default:
            continue;
        }   // switch (choice) end
    }   // while (true) end
}
