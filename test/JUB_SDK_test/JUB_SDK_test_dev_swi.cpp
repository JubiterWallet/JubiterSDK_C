//
//  JUB_SDK_test_dev_swi.cpp
//  JubSDKTest
//
//  Created by panmin on 2021/4/21.
//  Copyright © 2021 JuBiter. All rights reserved.
//

#include "JUB_SDK_test.h"
#include "JUB_SDK_test_dev_swi.hpp"
#include "JUB_SDK_main.h"

JUB_RV menu_opt_swi(const int choice, const Json::Value& root, JUB_UINT16_PTR pDeviceID) {

    JUB_RV rv = JUBR_ERROR;

//    cout << "| 21. device test via SWI(software impl, generate mnemonic) |" << endl;
//    cout << "| 22. device test via SWI(software impl, import mnemonic)   |" << endl;
//    cout << "| 23. device test via SWI(software impl, import xpriv)      |" << endl;
//    cout << "| 24. device test via SWI(software impl, import xpub)       |" << endl;

    std::cout << "[--------------------------------- Connect SWI ---------------------------------]" << std::endl;
    rv = JUB_ConnetDeviceSWI(pDeviceID);
    cout << "[-] JUB_ConnetDeviceSWI() return " << GetErrMsg(rv) << endl;
    if(JUBR_OK != rv) {
        return rv;
    }
    cout << "Software Interface deviceID: " << (*pDeviceID) << endl;
    cout << endl;
    cout << "[------------------------------- Connect SWI end -------------------------------]" << endl;

    switch (choice) {
    case 0:
    {
        return JUBR_OK;
    }
    case 2:
    case 21:
    {
        JUB_CHAR_PTR mnemonic = nullptr;
        JUB_ENUM_MNEMONIC_STRENGTH strength = JUB_ENUM_MNEMONIC_STRENGTH::STRENGTH128;
        switch (root["strength"].asInt()) {
        case 12: break;
        case 18: strength = JUB_ENUM_MNEMONIC_STRENGTH::STRENGTH192; break;
        case 24: strength = JUB_ENUM_MNEMONIC_STRENGTH::STRENGTH256; break;
        default: rv = JUBR_ARGUMENTS_BAD; break;
        }
        if (JUBR_OK != rv) {
            break;
        }
        cout << "[------------------------------ Generate Mnemonic ------------------------------]" << endl;
        rv = JUB_GenerateMnemonic(strength, &mnemonic);
        cout << "[-] JUB_GenerateMnemonic() return " << GetErrMsg(rv) << endl;
        if(JUBR_OK != rv) {
            break;
        }
        cout << "    Generate mnemonic: " << mnemonic << endl;
        cout << endl;

        rv = JUB_CheckMnemonic(mnemonic);
        cout << "[-] JUB_CheckMnemonic() return " << GetErrMsg(rv) << endl;
        JUB_FreeMemory(mnemonic);
        if(JUBR_OK != rv) {
            break;
        }
        cout << "[---------------------------- Generate Mnemonic end ----------------------------]" << endl;
        break;
    }
    case 22:
    {
        JUB_CHAR_PTR mnemonic = (JUB_CHAR_PTR)root["mnemonic"].asCString();

        cout << "[------------------------------- Import Mnemonic -------------------------------]" << endl;
        rv = JUB_CheckMnemonic(mnemonic);
        cout << "[-] JUB_CheckMnemonic() return " << GetErrMsg(rv) << endl;
        if(JUBR_OK != rv) {
            return rv;
        }

        rv = JUB_ImportMnemonic((*pDeviceID), "", mnemonic);
        cout << "[-] JUB_ImportMnemonic() return " << GetErrMsg(rv) << endl;
        if(JUBR_OK != rv) {
            return rv;
        }
        cout << "[----------------------------- Import Mnemonic end -----------------------------]" << endl;
        break;
    }
    case 23:
    case 24:
    {
        std::string coin = std::to_string(root["coin"].asInt());
        JUB_CHAR_PTR xprvOrXpub;
        if (23 == choice) {
            xprvOrXpub = (JUB_CHAR_PTR)root[coin]["xprv"].asCString();
        }
        else {
            xprvOrXpub = (JUB_CHAR_PTR)root[coin]["xpub"].asCString();
        }
        JUB_CHAR_PTR curves = (JUB_CHAR_PTR)root[coin]["curves"].asCString();
        JUB_ENUM_CURVES curve = JUB_ENUM_CURVES::SECP256K1;
        if (0 == std::string("secp256k1").compare(curves)) { ; }
        else if (0 == std::string("nist256p1").compare(curves)) { curve = JUB_ENUM_CURVES::NIST256P1; }
        else if (0 == std::string("ed25519").compare(curves)  ) { curve = JUB_ENUM_CURVES::ED25519; }
        else { rv = JUBR_ARGUMENTS_BAD; break;}

        cout << "[----------------------------- Import xprv or xpub -----------------------------]" << endl;
        rv = JUB_ImportMasterPrivateKey((*pDeviceID), curve, xprvOrXpub);
        cout << "[-] JUB_ImportMasterPrivateKey() return " << GetErrMsg(rv) << endl;
        if(JUBR_OK != rv) {
            return rv;
        }
        cout << "[--------------------------- Import xprv or xpub end ---------------------------]" << endl;
        break;
    }
    } //switch (choice) end
    cout << endl << endl;

    if (JUBR_OK != rv) {
        rv = JUB_DisconnetDeviceSWI(*pDeviceID);
        return rv;
    }

    return JUBR_OK;
}
