//
//  JUBXRPController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBXRPController.h"


@interface JUBXRPController ()

@end


@implementation JUBXRPController


- (void)viewDidLoad {
    
    self.selfClass = self;
    self.optItem = JUB_NS_ENUM_MAIN::OPT_XRP;
    
    self.coinTypeArray = @[BUTTON_TITLE_XRP
    ];
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}


#pragma mark - 通讯库寻卡回调
- (void)CoinXRPOpt:(JUB_UINT16)deviceID {
    
    const char* json_file = JSON_FILE_XRP;
    
    NSString *filePath = [[NSBundle mainBundle] pathForResource:[NSString stringWithFormat:@"%s", json_file]
                                                         ofType:@"json"];
    Json::Value root = readJSON([filePath UTF8String]);
    
    [self XRP_test:deviceID
              root:root
            choice:(int)self.optIndex];
}


#pragma mark - XRP applet
- (void)XRP_test:(JUB_UINT16)deviceID
            root:(Json::Value)root
          choice:(int)choice {
    
    JUB_RV rv = JUBR_ERROR;
    
    try {
        JUB_UINT16 contextID = 0;

        CONTEXT_CONFIG_XRP cfg;
        cfg.mainPath = (char*)root["main_path"].asCString();
        rv = JUB_CreateContextXRP(cfg, deviceID, &contextID);
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextXRP() return 0x%2lx.]", rv]];
            return;
        }
        [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextXRP() OK.]"]];
        
        switch (choice) {
        case JUB_NS_ENUM_OPT::GET_ADDRESS:
            [self get_address_pubkey_XRP:contextID];
            break;
        case JUB_NS_ENUM_OPT::SHOW_ADDRESS:
            break;
        case JUB_NS_ENUM_OPT::TRANSACTION:
        {
            [self transaction_test_XRP:contextID
                                  root:root];
            break;
        }
        case JUB_NS_ENUM_OPT::SET_MY_ADDRESS:
//            [self set_my_address_test_XRP:contextID];
            break;
        default:
            break;
        }
    }
    catch (...) {
        error_exit("[Error format json file.]\n");
        [self addMsgData:[NSString stringWithFormat:@"[Error format json file.]"]];
    }
}


- (void)get_address_pubkey_XRP:(JUB_UINT16)contextID {
    
    JUB_RV rv = JUBR_ERROR;
    
    int change = 0;
    JUB_UINT64 index = 0;
//    cout << "please input change level (non-zero means 1):" << endl;
//    cin >> change;
//    cout << "please input index " << endl;
//    cin >> index;
    
    BIP44_Path path;
    path.change = JUB_ENUM_BOOL(change);
    path.addressIndex = index;
    
    char* pubkey = nullptr;
    rv = JUB_GetMainHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::HEX, &pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeXRP() return 0x%2lx.]", rv]];
        return;
    }
    
    [self addMsgData:[NSString stringWithFormat:@"MainXpub in hex format: %s.", pubkey]];
    JUB_FreeMemory(pubkey);
    
    pubkey = nullptr;
    rv = JUB_GetHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeXRP() return 0x%2lx.]", rv]];
        return;
    }
    
    [self addMsgData:[NSString stringWithFormat:@"pubkey in hex format: %s.", pubkey]];
    JUB_FreeMemory(pubkey);
    
    char* address = nullptr;
    rv = JUB_GetAddressXRP(contextID, path, BOOL_TRUE, &address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressXRP() return 0x%2lx.]", rv]];
        return;
    }
    
    [self addMsgData:[NSString stringWithFormat:@"address: %s.", address]];
    JUB_FreeMemory(address);
}


- (void)transaction_test_XRP:(JUB_UINT16)contextID
                        root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
    rv = [self verify_pin:contextID
                      pin:[self.userPIN UTF8String]];
    if (JUBR_OK != rv) {
        return;
    }
    
    rv = [self transaction_proc_XRP:contextID
                               root:root];
    if (JUBR_OK != rv) {
        return;
    }
}


- (JUB_RV)transaction_proc_XRP:(JUB_UINT16)contextID
                          root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
        
    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["XRP"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["XRP"]["bip32_path"]["addressIndex"].asUInt();

    //typedef struct stPaymentXRP {
    //    JUB_ENUM_XRP_PYMT_TYPE type;
    //    JUB_CHAR_PTR invoiceID; // [Optional]
    //    JUB_CHAR_PTR sendMax;   // [Optional]
    //    JUB_CHAR_PTR deliverMin;// [Optional]
    //    union {
    //        JUB_PYMT_DXRP dxrp;
    //        JUB_PYMT_DXRP fx;
    //    };
    //} JUB_PYMT_XRP;
    //typedef struct stTxXRP {
    //    JUB_CHAR_PTR account;
    //    JUB_ENUM_XRP_TX_TYPE type;
    //    JUB_CHAR_PTR fee;
    //    JUB_CHAR_PTR sequence;
    //    JUB_CHAR_PTR accountTxnID;// [Optional]
    //    JUB_CHAR_PTR flags;
    //    JUB_CHAR_PTR lastLedgerSequence;
    //    JUB_XRP_MEMO memo;        // [Optional]
    //    JUB_CHAR_PTR sourceTag;   // [Optional]
    //    union {
    //        JUB_PYMT_XRP pymt;
    //    };
    //} JUB_TX_XRP;
    JUB_TX_XRP xrp;
    xrp.type = (JUB_ENUM_XRP_TX_TYPE)root["XRP"]["type"].asUInt();
    xrp.memo.type   = (char*)root["XRP"]["memo"]["type"].asCString();
    xrp.memo.data   = (char*)root["XRP"]["memo"]["data"].asCString();
    xrp.memo.format = (char*)root["XRP"]["memo"]["format"].asCString();
    switch (xrp.type) {
        case JUB_ENUM_XRP_TX_TYPE::PYMT:
        {
            xrp.account  = (char*)root["XRP"]["account"].asCString();
            xrp.fee      = (char*)root["XRP"]["fee"].asCString();
            xrp.flags    = (char*)root["XRP"]["flags"].asCString();
            xrp.sequence = (char*)root["XRP"]["sequence"].asCString();
            xrp.lastLedgerSequence = (char*)root["XRP"]["lastLedgerSequence"].asCString();
            break;
        }
        default:
            return JUBR_ARGUMENTS_BAD;
    }

    //typedef struct stDxrpPymt {
    //    JUB_CHAR_PTR destination;
    //    JUB_CHAR_PTR amount;
    //    JUB_CHAR_PTR destinationTag;
    //} JUB_PYMT_DXRP;
    const char* sType = std::to_string((unsigned int)xrp.type).c_str();
    xrp.pymt.type = (JUB_ENUM_XRP_PYMT_TYPE)root["XRP"][sType]["type"].asUInt();
    switch (xrp.pymt.type) {
        case JUB_ENUM_XRP_PYMT_TYPE::DXRP:
        {
            xrp.pymt.destination    = (char*)root["XRP"][sType]["destination"].asCString();
            xrp.pymt.amount.value   = (char*)root["XRP"][sType]["amount"]["value"].asCString();
            xrp.pymt.destinationTag = (char*)root["XRP"][sType]["destinationTag"].asCString();
            break;
        }
        default:
            return JUBR_ARGUMENTS_BAD;
    }
    char* raw = nullptr;
    rv = JUB_SignTransactionXRP(contextID,
                                path,
                                xrp,
                                &raw);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionXRP() return 0x%2lx.]", rv]];
        return rv;
    }
    
    if (raw) {
        size_t txLen = strlen(raw)/2;
        [self addMsgData:[NSString stringWithFormat:@"tx raw[%lu]: %s.", txLen, raw]];
        
        JUB_FreeMemory(raw);
    }
    
    return rv;
}


@end
