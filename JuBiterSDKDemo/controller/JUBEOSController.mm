//
//  JUBEOSController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBAlertView.h"
#import "JUBPinAlertView.h"
#import "JUBListAlert.h"
#import "JUBSharedData.h"

#import "JUBEOSController.h"


@interface JUBEOSController ()

@end


@implementation JUBEOSController


- (void)viewDidLoad {
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.optItem = JUB_NS_ENUM_MAIN::OPT_EOS;
    
    self.coinTypeArray = @[BUTTON_TITLE_EOS,
                           BUTTON_TITLE_EOSBUYRAM,
                           BUTTON_TITLE_EOSSELLRAM,
                           BUTTON_TITLE_EOSSTAKE,
                           BUTTON_TITLE_EOSUNSTAKE,
    ];
}


#pragma mark - 通讯库寻卡回调
- (void)CoinEOSOpt:(NSUInteger)deviceID {
    
    const char* json_file = "";
    switch (self.optCoinType) {
    case JUB_NS_ENUM_EOS_OPT::COIN_EOS:
    {
        json_file = JSON_FILE_EOS;
        break;
    }
    case JUB_NS_ENUM_EOS_OPT::COIN_EOS_BUYRAM:
    {
        json_file = JSON_FILE_EOS_BUYRAM;
        break;
    }
    case JUB_NS_ENUM_EOS_OPT::COIN_EOS_SELLRAM:
    {
        json_file = JSON_FILE_EOS_SELLRAM;
        break;
    }
    case JUB_NS_ENUM_EOS_OPT::COIN_EOS_STAKE:
    {
        json_file = JSON_FILE_EOS_STAKE;
        break;
    }
    case JUB_NS_ENUM_EOS_OPT::COIN_EOS_UNSTAKE:
    {
        json_file = JSON_FILE_EOS_UNSTAKE;
        break;
    }
    default:
        break;
    }   // switch (self.optCoinType) end
    
    NSString *filePath = [[NSBundle mainBundle] pathForResource:[NSString stringWithFormat:@"%s", json_file]
                                                         ofType:@"json"];
    Json::Value root = readJSON([filePath UTF8String]);
    
    [self EOS_test:deviceID
              root:root
            choice:(int)self.optIndex];
}


#pragma mark - EOS applet
- (void)EOS_test:(NSUInteger)deviceID
            root:(Json::Value)root
          choice:(int)choice {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    
    try {
        JUB_UINT16 contextID = [sharedData currContextID];
        if (0 != contextID) {
            [sharedData setCurrMainPath:nil];
            [sharedData setCurrCoinType:-1];
            rv = JUB_ClearContext(contextID);
            if (JUBR_OK != rv) {
                [self addMsgData:[NSString stringWithFormat:@"[JUB_ClearContext() return 0x%2lx.]", rv]];
            }
            else {
                [self addMsgData:[NSString stringWithFormat:@"[JUB_ClearContext() OK.]"]];
            }
            [sharedData setCurrContextID:0];
        }
        
        CONTEXT_CONFIG_EOS cfg;
        cfg.mainPath = (char*)root["main_path"].asCString();
        rv = JUB_CreateContextEOS(cfg, deviceID, &contextID);
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextEOS() return 0x%2lx.]", rv]];
            return;
        }
        [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextEOS() OK.]"]];
        [sharedData setCurrMainPath:[NSString stringWithFormat:@"%s", cfg.mainPath]];
        [sharedData setCurrContextID:contextID];
        
        switch (choice) {
        case JUB_NS_ENUM_OPT::GET_ADDRESS:
        {
            [self get_address_pubkey:contextID];
            break;
        }
        case JUB_NS_ENUM_OPT::SHOW_ADDRESS:
        {
            [self show_address_test:contextID];
            break;
        }
        case JUB_NS_ENUM_OPT::TRANSACTION:
        {
            [self transaction_test:contextID
                              root:root];
            break;
        }
        case JUB_NS_ENUM_OPT::SET_MY_ADDRESS:
        {
//            [self set_my_address_test_EOS:contextID];
            break;
        }
        default:
            break;
        }   // switch (choice) end
    }
    catch (...) {
        error_exit("[Error format json file.]\n");
        [self addMsgData:[NSString stringWithFormat:@"[Error format json file.]"]];
    }
}


- (void)get_address_pubkey:(NSUInteger)contextID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    
    char* pubkey = nullptr;
    rv = JUB_GetMainHDNodeEOS(contextID, JUB_ENUM_PUB_FORMAT::HEX, &pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeEOS() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeEOS() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"MainXpub(%@) in hex format: %s.", [sharedData currMainPath], pubkey]];
    rv = JUB_FreeMemory(pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
    
    BIP44_Path path;
    path.change = JUB_ENUM_BOOL(self.change);
    path.addressIndex = (JUB_UINT64)self.addressIndex;
    
    pubkey = nullptr;
    rv = JUB_GetHDNodeEOS(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeEOS() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeEOS() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"pubkey(%@/%d/%llu) in hex format: %s.", [sharedData currMainPath], path.change, path.addressIndex, pubkey]];
    rv = JUB_FreeMemory(pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
    
    char* address = nullptr;
    rv = JUB_GetAddressEOS(contextID, path, BOOL_FALSE, &address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressEOS() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressEOS() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"address(%@/%d/%llu): %s.", [sharedData currMainPath], path.change, path.addressIndex, address]];
    rv = JUB_FreeMemory(address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
}


- (void)show_address_test:(NSUInteger)contextID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    
    BIP44_Path path;
    path.change = (self.change ? JUB_ENUM_BOOL::BOOL_TRUE:JUB_ENUM_BOOL::BOOL_FALSE);
    path.addressIndex = self.addressIndex;
    
    JUB_CHAR_PTR address;
    rv = JUB_GetAddressEOS(contextID, path, BOOL_TRUE, &address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressEOS() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressEOS() OK.]"]];
    [self addMsgData:[NSString stringWithFormat:@"Show address(%@/%d/%llu) is: %s.", [sharedData currMainPath], path.change, path.addressIndex, address]];
    
    rv = JUB_FreeMemory(address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
}


- (void)transaction_test:(NSUInteger)contextID
                    root:(Json::Value)root {
    
    __block
    JUB_RV rv = JUBR_ERROR;
    
    __block
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    
    switch ([sharedData deviceType]) {
    case JUB_NS_ENUM_DEV_TYPE::SEG_NFC:
    {
        switch ([sharedData verifyMode]) {
        case JUB_NS_ENUM_VERIFY_MODE::PIN:
        {
            rv = [self verify_pin:contextID];
            if (JUBR_OK != rv) {
                return;
            }
            
            rv = [self transaction_proc:contextID
                                   root:root];
            if (JUBR_OK != rv) {
                return;
            }
            break;
        }   // case JUB_NS_ENUM_VERIFY_MODE::PIN end
        case JUB_NS_ENUM_VERIFY_MODE::VKPIN:
        case JUB_NS_ENUM_VERIFY_MODE::FGPT:
        default:
            break;
        }   // switch ([sharedData verifyMode]) end
        break;
    }   // case JUB_NS_ENUM_DEV_TYPE::SEG_NFC end
    case JUB_NS_ENUM_DEV_TYPE::SEG_BLE:
    {
        __block
        BOOL isDone = NO;
        switch ([sharedData verifyMode]) {
        case JUB_NS_ENUM_VERIFY_MODE::FGPT:
        {
            JUBListAlert *listAlert = [JUBListAlert showCallBack:^(NSString *_Nonnull selectedItem) {
                NSLog(@"Verify PIN mode selected: %@", selectedItem);
                if ([selectedItem isEqual:BUTTON_TITLE_USE_VK]) {
                    rv = [self show_virtualKeyboard:contextID];
                    if (JUBR_OK != rv) {
                        isDone = YES;
                        return;
                    }
                    
                    [JUBPinAlertView showInputPinAlert:^(NSString * _Nonnull pin) {
                        
                        if (nil == pin) {
                            [self cancel_virtualKeyboard:contextID];
                            isDone = YES;
                            rv = JUBR_USER_CANCEL;
                            return;
                        }
                        [sharedData setUserPin:pin];
                        
                        rv = [self verify_pin:contextID];
                        if (JUBR_OK != rv) {
                            isDone = YES;
                            return;
                        }
                        
                        isDone = YES;
                    }];
                }   // if ([selectedItem isEqual:BUTTON_TITLE_USE_VK]) end
                else if ([selectedItem isEqual:BUTTON_TITLE_USE_FGPT]) {
                    rv = [self verify_fgpt:contextID];
                    if (JUBR_OK != rv) {
                        isDone = YES;
                        return;
                    }
                    
                    isDone = YES;
                }   // if ([selectedItem isEqual:BUTTON_TITLE_USE_FGPT]) end
            }];
            
            listAlert.title = @"Please select Verify PIN mode:";
            [listAlert addItems:@[
                BUTTON_TITLE_USE_VK,
                BUTTON_TITLE_USE_FGPT
            ]];
            break;
        }   // case JUB_NS_ENUM_VERIFY_MODE::FGPT end
        case JUB_NS_ENUM_VERIFY_MODE::VKPIN:
        {
            rv = [self show_virtualKeyboard:contextID];
            if (JUBR_OK != rv) {
                isDone = YES;
                break;
            }
            
            [JUBPinAlertView showInputPinAlert:^(NSString * _Nonnull pin) {
                if (nil == pin) {
                    [self cancel_virtualKeyboard:contextID];
                    isDone = YES;
                    rv = JUBR_USER_CANCEL;
                    return;
                }
                [sharedData setUserPin:pin];
                
                rv = [self verify_pin:contextID];
                if (JUBR_OK != rv) {
                    isDone = YES;
                    return;
                }
                
                isDone = YES;
            }];
            
            break;
        }   // case JUB_NS_ENUM_VERIFY_MODE::VKPIN end
        case JUB_NS_ENUM_VERIFY_MODE::PIN:
        default:
            rv = JUBR_ARGUMENTS_BAD;
            isDone = YES;
            break;
        }   // switch (data.verifyMode) end
        
        while (!isDone) {
            [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
                                     beforeDate:[NSDate distantFuture]];
        }
        
        if (JUBR_OK != rv) {
            return;
        }
        
        dispatch_async(dispatch_get_global_queue(0, 0), ^{
            
            __block
            JUBAlertView *alertView;
            
            dispatch_async(dispatch_get_main_queue(), ^{
                alertView = [JUBAlertView showMsg:@"Transaction in progress..."];
            });
            
            rv = [self transaction_proc:contextID
                                   root:root];
            
            dispatch_async(dispatch_get_main_queue(), ^{
                [alertView dismiss];
            });
        });
        
        break;
    }   // case JUB_NS_ENUM_DEV_TYPE::SEG_BLE end
    default:
        break;
    }   // switch ([sharedData deviceType]) end
}


- (NSUInteger)transaction_proc:(NSUInteger)contextID
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
        {
            action.transfer.from  = (char*)(*it)[sType]["from"].asCString();
            action.transfer.to    = (char*)(*it)[sType]["to"].asCString();
            action.transfer.asset = (char*)(*it)[sType]["asset"].asCString();
            action.transfer.memo  = (char*)(*it)[sType]["memo"].asCString();
            break;
        }
        case JUB_ENUM_EOS_ACTION_TYPE::DELE:
        {
            action.delegate.from     = (char*)(*it)[sType]["from"].asCString();
            action.delegate.receiver = (char*)(*it)[sType]["receiver"].asCString();
            action.delegate.netQty   = (char*)(*it)[sType]["stake_net_quantity"].asCString();
            action.delegate.cpuQty   = (char*)(*it)[sType]["stake_cpu_quantity"].asCString();
            action.delegate.bStake = true;
            break;
        }
        case JUB_ENUM_EOS_ACTION_TYPE::UNDELE:
        {
            action.delegate.from     = (char*)(*it)[sType]["from"].asCString();
            action.delegate.receiver = (char*)(*it)[sType]["receiver"].asCString();
            action.delegate.netQty   = (char*)(*it)[sType]["unstake_net_quantity"].asCString();
            action.delegate.cpuQty   = (char*)(*it)[sType]["unstake_cpu_quantity"].asCString();
            action.delegate.bStake = false;
            break;
        }
        case JUB_ENUM_EOS_ACTION_TYPE::BUYRAM:
        {
            action.buyRam.payer    = (char*)(*it)[sType]["payer"].asCString();
            action.buyRam.quant    = (char*)(*it)[sType]["quant"].asCString();
            action.buyRam.receiver = (char*)(*it)[sType]["receiver"].asCString();
            break;
        }
        case JUB_ENUM_EOS_ACTION_TYPE::SELLRAM:
        {
            action.sellRam.account = (char*)(*it)[sType]["account"].asCString();
            action.sellRam.bytes   = (char*)(*it)[sType]["bytes"].asCString();
            break;
        }
        case JUB_ENUM_EOS_ACTION_TYPE::NS_ITEM_EOS_ACTION_TYPE:
        default:
            return JUBR_ARGUMENTS_BAD;
        }   // switch (action.type) end
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
    [self addMsgData:[NSString stringWithFormat:@"[JUB_BuildActionEOS() OK.]"]];
    
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
    rv = JUB_FreeMemory(actionsInJSON);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return 0x%2lx.]", rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
    
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionEOS() return 0x%2lx.]", rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionEOS() OK.]"]];
    
    if (raw) {
        [self addMsgData:[NSString stringWithFormat:@"tx raw in JSON: %s.", raw]];
        
        rv = JUB_FreeMemory(raw);
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return 0x%2lx.]", rv]];
            return rv;
        }
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
    }
    
    return JUBR_OK;
}


@end
