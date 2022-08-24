//
//  JUB_SDK_test_sol.cpp
//  JubSDKTest
//
//  Created by panmin on 2022/7/20.
//  Copyright © 2022 JuBiter. All rights reserved.
//

#include "../../include/JUB_SDK_SOL.h"

#include "JUB_SDK_COMM.h"
#include "JUB_SDK_main.h"
#include "JUB_SDK_test.h"
#include "JUB_SDK_test_dev.hpp"
#include "JUB_SDK_test_sol.hpp"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <limits>
#include <string>

void SOL_test(JUB_UINT16 deviceID, JUB_CHAR_CPTR json_file) {

    JUB_RV rv = JUBR_ERROR;

    Json::Value root = readJSON(json_file);
    if (root.empty()) {
        return;
    }

    JUB_UINT16 contextID = 0;

    CONTEXT_CONFIG_SOL cfg;
    cfg.mainPath = (JUB_CHAR_PTR)root["main_path"].asCString();
    rv           = JUB_CreateContextSOL(cfg, deviceID, &contextID);
    cout << "[-] JUB_CreateContextSOL() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << endl;

    while (true) {
        cout << "---------------------------------------------------" << endl;
        cout << "|************** Jubiter Wallet SOL  **************|" << endl;
        cout << "|    0.                 show_address_pubkey_test. |" << endl;
        cout << "|                                                 |" << endl;
        cout << "|    2.                         transaction_test. |" << endl;
        cout << "|    3.                   token_transaction_test. |" << endl;
        cout << "|    4.    create_token_account_transaction_test. |" << endl;
        cout << "|                                                 |" << endl;
        cout << "|                                                 |" << endl;
        cout << "|                                                 |" << endl;
        cout << "|    9. return.                                   |" << endl;
        cout << "---------------------------------------------------" << endl;
        cout << "* Please enter your choice:" << endl;

        int choice = 0;
        cin >> choice;

        switch (choice) {
        case 0: {
            get_address_pubkey_SOL(contextID, cfg.mainPath);
        } break;
        case 2: {
            transaction_test_SOL(contextID, root);
        } break;
        case 3: {
            transaction_test_SOL_token_transfer(contextID, root);
        } break;
        case 4: {
            transaction_test_SOL_associated_token_create(contextID, root);
        } break;
        case 9: {
            JUB_ClearContext(contextID);
            main_test();
        }
        default: {
            continue;
        }
        } // switch (choice) end
    }     // while (true) end
}

void get_address_pubkey_SOL(JUB_UINT16 contextID, std::string mainPath) {

    JUB_RV rv = JUBR_ERROR;

    cout << "[----------------------------------- HD Node -----------------------------------]" << endl;
    JUB_CHAR_PTR pubkey = nullptr;
    rv                  = JUB_GetMainHDNodeSOL(contextID, JUB_ENUM_PUB_FORMAT::HEX, &pubkey);
    cout << "[-] JUB_GetMainHDNodeSOL() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "MainXpub in  HEX format:  " << pubkey << endl;
    JUB_FreeMemory(pubkey);
    cout << endl;

    pubkey = nullptr;
    rv     = JUB_GetMainHDNodeSOL(contextID, JUB_ENUM_PUB_FORMAT::XPUB, &pubkey);
    cout << "[-] JUB_GetMainHDNodeSOL() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "MainXpub in XPUB format:  " << pubkey << endl;
    JUB_FreeMemory(pubkey);
    cout << endl;

    std::string path = mainPath;
    std::string line;
    // auto pathValue = [](std::string line) {
    //     uint32_t v = 0;
    //     if (*line.rbegin() == '\'') {
    //         v = 0x80000000;
    //     }
    //     v |= std::stoi(line);
    //     return v;
    // };

    cout << "please input change level (non-zero means 1):" << endl;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    getline(cin, line, '\n');
    path += "/";
    path += line;
    cout << "please input index " << endl;
    getline(cin, line, '\n');
    path += "/";
    path += line;

    pubkey = nullptr;

    rv = JUB_GetHDNodeSOL(contextID, JUB_ENUM_PUB_FORMAT::HEX, (JUB_CHAR_PTR)path.c_str(), &pubkey);
    cout << "[-] JUB_GetHDNodeSOL() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "  pubkey in  HEX format :  " << pubkey << endl;
    JUB_FreeMemory(pubkey);
    cout << endl;

    pubkey = nullptr;

    rv = JUB_GetHDNodeSOL(contextID, JUB_ENUM_PUB_FORMAT::XPUB, (JUB_CHAR_PTR)path.c_str(), &pubkey);
    cout << "[-] JUB_GetHDNodeSOL() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "  pubkey in XPUB format :  " << pubkey << endl;
    JUB_FreeMemory(pubkey);
    cout << "[--------------------------------- HD Node end ---------------------------------]" << endl;
    cout << endl << endl;

    cout << "[----------------------------------- Address -----------------------------------]" << endl;
    JUB_CHAR_PTR address = nullptr;

    rv = JUB_GetAddressSOL(contextID, (JUB_CHAR_PTR)path.c_str(), BOOL_FALSE, &address);
    cout << "[-] JUB_GetAddressSOL() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "         address: " << address << endl;
    JUB_FreeMemory(address);
    address = nullptr;

    rv = JUB_GetAddressSOL(contextID, (JUB_CHAR_PTR)path.c_str(), BOOL_TRUE, &address);
    cout << "[-] JUB_GetAddressSOL() return " << GetErrMsg(rv) << endl;
    if (JUBR_OK != rv) {
        return;
    }
    cout << "    show address: " << address << endl;
    JUB_FreeMemory(address);
    cout << "[--------------------------------- Address end ---------------------------------]" << endl;
    cout << endl << endl;
}

void transaction_test_SOL(JUB_UINT16 contextID, const Json::Value &root) {

    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    rv = transaction_proc_SOL(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}

JUB_RV transaction_proc_SOL(JUB_UINT16 contextID, const Json::Value &root) {
    JUB_RV rv = JUBR_ERROR;
    try {
        auto path       = (JUB_CHAR_CPTR)root["SOL"]["bip32_path"].asCString();
        auto recentHash = (JUB_CHAR_CPTR)root["SOL"]["recent_hash"].asCString();
        auto dest       = (JUB_CHAR_CPTR)root["SOL"]["dest"].asCString();
        auto amount     = root["SOL"]["amount"].asUInt64();
        auto raw        = (JUB_CHAR_PTR) nullptr;

        rv = JUB_SignTransactionSOL(contextID, path, recentHash, dest, amount, &raw);
        cout << "[-] JUB_SignTransactionSOL() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            return rv;
        }
        std::cout << "    raw[" << strlen(raw) / 2 << "]: " << raw << std::endl;
        JUB_FreeMemory(raw);
    } catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return rv;
}

void transaction_test_SOL_token_transfer(JUB_UINT16 contextID, const Json::Value &root) {
    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    rv = transaction_proc_SOL_token_transfer(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}

JUB_RV transaction_proc_SOL_token_transfer(JUB_UINT16 contextID, const Json::Value &root) {
    JUB_RV rv = JUBR_ERROR;
    try {
        auto &tr        = root["TOKEN-TRANSFER"];
        auto path       = (JUB_CHAR_CPTR)tr["bip32_path"].asCString();
        auto name       = (JUB_CHAR_CPTR)tr["token_name"].asCString();
        auto mint       = (JUB_CHAR_CPTR)tr["token_mint"].asCString();
        auto recentHash = (JUB_CHAR_CPTR)tr["recent_hash"].asCString();
        auto source     = (JUB_CHAR_CPTR)tr["source"].asCString();
        auto dest       = (JUB_CHAR_CPTR)tr["dest"].asCString();
        auto amount     = tr["amount"].asUInt64();
        auto decimal    = tr["decimal"].asUInt64();
        auto raw        = (JUB_CHAR_PTR) nullptr;

        rv = JUB_SetTokenSOL(contextID, name, decimal, mint);
        cout << "[-] JUB_SetTokenSOL() return " << GetErrMsg(rv) << endl;
        if (rv != JUBR_OK) {
            return rv;
        }

        rv = JUB_SignTransactionTokenSOL(contextID, path, recentHash, mint, source, dest, amount, decimal, &raw);
        cout << "[-] JUB_SignTransactionTokenSOL() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            return rv;
        }
        std::cout << "    raw[" << strlen(raw) / 2 << "]: " << raw << std::endl;
        JUB_FreeMemory(raw);
    } catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return rv;
}

void transaction_test_SOL_associated_token_create(JUB_UINT16 contextID, const Json::Value &root) {
    JUB_RV rv = verify_pin(contextID);
    if (JUBR_OK != rv) {
        return;
    }

    rv = transaction_proc_SOL_associated_token_create(contextID, root);
    if (JUBR_OK != rv) {
        return;
    }
}

JUB_RV transaction_proc_SOL_associated_token_create(JUB_UINT16 contextID, const Json::Value &root) {
    JUB_RV rv = JUBR_ERROR;
    try {
        auto &tr        = root["ASSOCIATED-TOKEN-CREATE"];
        auto path       = (JUB_CHAR_CPTR)tr["bip32_path"].asCString();
        auto name       = (JUB_CHAR_CPTR)tr["token_name"].asCString();
        auto mint       = (JUB_CHAR_CPTR)tr["token_mint"].asCString();
        auto recentHash = (JUB_CHAR_CPTR)tr["recent_hash"].asCString();
        auto owner      = (JUB_CHAR_CPTR)tr["owner"].asCString();
        auto decimal    = tr["decimal"].asUInt64();
        auto raw        = (JUB_CHAR_PTR) nullptr;

        rv = JUB_SetTokenSOL(contextID, name, decimal, mint);
        cout << "[-] JUB_SetTokenSOL() return " << GetErrMsg(rv) << endl;
        if (rv != JUBR_OK) {
            return rv;
        }

        rv = JUB_SignTransactionAssociatedTokenCreateSOL(contextID, path, recentHash, owner, mint, &raw);

        cout << "[-] JUB_SignTransactionAssociatedTokenCreateSOL() return " << GetErrMsg(rv) << endl;
        if (JUBR_OK != rv) {
            return rv;
        }
        std::cout << "    raw[" << strlen(raw) / 2 << "]: " << raw << std::endl;
        JUB_FreeMemory(raw);
    } catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return rv;
}
