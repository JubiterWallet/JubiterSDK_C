//
//  JUBEOSController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBEOSController.h"


@interface JUBEOSController ()

@end


@implementation JUBEOSController


- (void)viewDidLoad {
    
    self.selfClass = self;
    self.optItem = JUB_NS_ENUM_MAIN::OPT_EOS;
    
    self.coinTypeArray = @[@"EOS",
                           @"EOS_BUY_RAM",
                           @"EOS_SELL_RAM",
                           @"EOS_STAKE",
                           @"EOS_UNSTAKE",
    ];
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}


#pragma mark - 通讯库寻卡回调
- (void)CoinEOSOpt:(JUB_UINT16)deviceID {
    
    const char* json_file = "";
    switch (self.optCoinType) {
    case JUB_NS_ENUM_EOS_OPT::COIN_EOS:
        json_file = JSON_FILE_EOS;
        break;
    case JUB_NS_ENUM_EOS_OPT::COIN_EOS_BUYRAM:
        json_file = JSON_FILE_EOS_BUYRAM;
        break;
    case JUB_NS_ENUM_EOS_OPT::COIN_EOS_SELLRAM:
        json_file = JSON_FILE_EOS_SELLRAM;
        break;
    case JUB_NS_ENUM_EOS_OPT::COIN_EOS_STAKE:
        json_file = JSON_FILE_EOS_STAKE;
        break;
    case JUB_NS_ENUM_EOS_OPT::COIN_EOS_UNSTAKE:
        json_file = JSON_FILE_EOS_UNSTAKE;
        break;
    default:
        break;
    }
    
    NSString *filePath = [[NSBundle mainBundle] pathForResource:[NSString stringWithFormat:@"%s", json_file]
                                                         ofType:@"json"];
    Json::Value root = readJSON([filePath UTF8String]);

    [self EOS_test:deviceID
              root:root
            choice:(int)self.optIndex];
}


#pragma mark - EOS applet
- (void)EOS_test:(JUB_UINT16)deviceID
            root:(Json::Value)root
          choice:(int)choice {
    
    JUB_RV rv = JUBR_ERROR;

    try {
        JUB_UINT16 contextID = 0;

        CONTEXT_CONFIG_EOS cfg;
        cfg.mainPath = (char*)root["main_path"].asCString();
        rv = JUB_CreateContextEOS(cfg, deviceID, &contextID);
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextEOS() return 0x%2lx.]", rv]];
            return;
        }
        
        switch (choice) {
        case JUB_NS_ENUM_OPT::GET_ADDRESS:
            [self get_address_pubkey_EOS:contextID];
            break;
        case JUB_NS_ENUM_OPT::SHOW_ADDRESS:
            break;
        case JUB_NS_ENUM_OPT::TRANSACTION:
        {
            [self transaction_test_EOS:contextID
                                  root:root];
            break;
        }
        case JUB_NS_ENUM_OPT::SET_MY_ADDRESS:
//            [self set_my_address_test_EOS:contextID];
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


- (void)get_address_pubkey_EOS:(JUB_UINT16)contextID {
    
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
    
    char* address = nullptr;
    rv = JUB_GetAddressEOS(contextID, path, BOOL_TRUE, &address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressEOS() return 0x%2lx.]", rv]];
        return;
    }
    
    [self addMsgData:[NSString stringWithFormat:@"address: %s.", address]];
    JUB_FreeMemory(address);
}


- (void)transaction_test_EOS:(JUB_UINT16)contextID
                        root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
    rv = [self verify_pin:contextID
                      pin:[self.userPIN UTF8String]];
    if (JUBR_OK != rv) {
        return;
    }
    
    rv = [self transaction_proc_EOS:contextID
                               root:root];
    if (JUBR_OK != rv) {
        return;
    }
}


- (JUB_RV)transaction_proc_EOS:(JUB_UINT16)contextID
                          root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["EOS"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["EOS"]["bip32_path"]["addressIndex"].asUInt();
    
    if (!root["EOS"]["actions"].isArray()) {
        return JUBR_ARGUMENTS_BAD;
    }
    
    std::vector<JUB_ACTION_EOS> actions;
    //EOS Test
    for (Json::Value::iterator it = root["EOS"]["actions"].begin(); it != root["EOS"]["actions"].end(); ++it) {
        JUB_ACTION_EOS action;
        
        action.type = (JUB_ENUM_EOS_ACTION_TYPE)(*it)["type"].asUInt();
        action.currency = (char*)(*it)["currency"].asCString();
        action.name     = (char*)(*it)["name"].asCString();
        
        const char* sType = std::to_string((unsigned int)action.type).c_str();
        switch (action.type) {
        case JUB_ENUM_EOS_ACTION_TYPE::XFER:
            action.transfer.from  = (char*)(*it)[sType]["from"].asCString();
            action.transfer.to    = (char*)(*it)[sType]["to"].asCString();
            action.transfer.asset = (char*)(*it)[sType]["asset"].asCString();
            action.transfer.memo  = (char*)(*it)[sType]["memo"].asCString();
            break;
        case JUB_ENUM_EOS_ACTION_TYPE::DELE:
            action.delegate.from     = (char*)(*it)[sType]["from"].asCString();
            action.delegate.receiver = (char*)(*it)[sType]["receiver"].asCString();
            action.delegate.netQty   = (char*)(*it)[sType]["stake_net_quantity"].asCString();
            action.delegate.cpuQty   = (char*)(*it)[sType]["stake_cpu_quantity"].asCString();
            action.delegate.bStake = true;
            break;
        case JUB_ENUM_EOS_ACTION_TYPE::UNDELE:
            action.delegate.from     = (char*)(*it)[sType]["from"].asCString();
            action.delegate.receiver = (char*)(*it)[sType]["receiver"].asCString();
            action.delegate.netQty   = (char*)(*it)[sType]["unstake_net_quantity"].asCString();
            action.delegate.cpuQty   = (char*)(*it)[sType]["unstake_cpu_quantity"].asCString();
            action.delegate.bStake = false;
            break;
        case JUB_ENUM_EOS_ACTION_TYPE::BUYRAM:
            action.buyRam.payer    = (char*)(*it)[sType]["payer"].asCString();
            action.buyRam.quant    = (char*)(*it)[sType]["quant"].asCString();
            action.buyRam.receiver = (char*)(*it)[sType]["receiver"].asCString();
            break;
        case JUB_ENUM_EOS_ACTION_TYPE::SELLRAM:
            action.sellRam.account = (char*)(*it)[sType]["account"].asCString();
            action.sellRam.bytes   = (char*)(*it)[sType]["bytes"].asCString();
            break;
        case JUB_ENUM_EOS_ACTION_TYPE::NS_ITEM_EOS_ACTION_TYPE:
        default:
            return JUBR_ARGUMENTS_BAD;
        }
        actions.push_back(action);
    }
    size_t actionCnt = actions.size();
    JUB_ACTION_EOS_PTR pActions = new JUB_ACTION_EOS[actionCnt*sizeof(JUB_ACTION_EOS)+1];
    memset(pActions, 0x00, actionCnt*sizeof(JUB_ACTION_EOS)+1);
    int i=0;
    for (const JUB_ACTION_EOS& action:actions) {
        pActions[i] = action;
        ++i;
    }
    
    JUB_CHAR_PTR actionsInJSON = nullptr;
    rv = JUB_BuildActionEOS(contextID,
                            pActions, actionCnt,
                            &actionsInJSON);
    delete [] pActions; pActions = nullptr;
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_BuildActionEOS() return 0x%2lx.]", rv]];
        return rv;
    }
    if (actionsInJSON) {
        [self addMsgData:[NSString stringWithFormat:@"action in JSON: %s.", actionsInJSON]];
    }
    
    char* chainID    = (char*)root["EOS"]["chainID"].asCString();
    char* expiration = (char*)root["EOS"]["expiration"].asCString();
    char* referenceBlockId = (char*)root["EOS"]["referenceBlockId"].asCString();
    
    char* refBlockT = (char*)root["EOS"]["referenceBlockTime"].asCString();
    int yy, month, dd, hh, mm, ss;
    sscanf(refBlockT, "%d-%d-%d %d:%d:%d",
           &yy, &month, &dd,
           &hh, &mm, &ss);
    tm refblocktime;
    refblocktime.tm_year = yy - 1900;
    refblocktime.tm_mon = month - 1;
    refblocktime.tm_mday = dd;
    refblocktime.tm_hour = hh;
    refblocktime.tm_min = mm;
    refblocktime.tm_sec = ss;
    refblocktime.tm_isdst = -1;
    time_t tRefblocktime = mktime(&refblocktime);
    
    time_t localTime;
    tRefblocktime += localtime(&localTime)->tm_gmtoff;
    char* referenceBlockTime = (char*)std::to_string(tRefblocktime).c_str();
    
    JUB_CHAR_PTR raw = nullptr;
    rv = JUB_SignTransactionEOS(contextID,
                                path,
                                chainID,
                                expiration,
                                referenceBlockId,
                                referenceBlockTime,
                                actionsInJSON,
                                &raw);
    JUB_FreeMemory(actionsInJSON);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionEOS() return 0x%2lx.]", rv]];
        return rv;
    }
    if (raw) {
        [self addMsgData:[NSString stringWithFormat:@"tx raw in JSON: %s.", raw]];
        
        JUB_FreeMemory(raw);
    }
    
    return JUBR_OK;
}


@end
