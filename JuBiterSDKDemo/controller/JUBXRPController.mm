//
//  JUBXRPController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBAlertView.h"
#import "JUBPinAlertView.h"
#import "JUBListAlert.h"
#import "JUBSharedData.h"

#import "JUBXRPController.h"


@interface JUBXRPController ()

@end


@implementation JUBXRPController


- (void)viewDidLoad {
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.optItem = JUB_NS_ENUM_MAIN::OPT_XRP;
    
    self.coinTypeArray = @[BUTTON_TITLE_XRP
    ];
}


#pragma mark - 通讯库寻卡回调
- (void)CoinXRPOpt:(NSUInteger)deviceID {
    
    const char* json_file = JSON_FILE_XRP;
    
    NSString *filePath = [[NSBundle mainBundle] pathForResource:[NSString stringWithFormat:@"%s", json_file]
                                                         ofType:@"json"];
    Json::Value root = readJSON([filePath UTF8String]);
    
    [self XRP_test:deviceID
              root:root
            choice:(int)self.optIndex];
}


#pragma mark - XRP applet
- (void)XRP_test:(NSUInteger)deviceID
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
        
        CONTEXT_CONFIG_XRP cfg;
        cfg.mainPath = (char*)root["main_path"].asCString();
        rv = JUB_CreateContextXRP(cfg, deviceID, &contextID);
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextXRP() return 0x%2lx.]", rv]];
            return;
        }
        [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextXRP() OK.]"]];
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
//            [self set_my_address_test_XRP:contextID];
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
    
    JUB_CHAR_PTR mainXpub;
    rv = JUB_GetMainHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::HEX, &mainXpub);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeXRP() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeXRP() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"Main xpub(%@): %s.", [sharedData currMainPath], mainXpub]];
    rv = JUB_FreeMemory(mainXpub);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
    
    BIP44_Path path;
    path.change = (self.change ? JUB_ENUM_BOOL::BOOL_TRUE:JUB_ENUM_BOOL::BOOL_FALSE);
    path.addressIndex = self.addressIndex;
    
    JUB_CHAR_PTR xpub;
    rv = JUB_GetHDNodeXRP(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &xpub);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeXRP() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeXRP() OK.]"]];
    [self addMsgData:[NSString stringWithFormat:@"input xpub(%@/%u/%llu): %s.", [sharedData currMainPath], path.change, path.addressIndex, xpub]];
    rv = JUB_FreeMemory(xpub);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
    
    JUB_CHAR_PTR address;
    rv = JUB_GetAddressXRP(contextID, path, BOOL_FALSE, &address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressXRP() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressXRP() OK.]"]];
    [self addMsgData:[NSString stringWithFormat:@"input address(%@/%d/%llu): %s.", [sharedData currMainPath], path.change, path.addressIndex, address]];
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
    rv = JUB_GetAddressXRP(contextID, path, BOOL_TRUE, &address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressXRP() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressXRP() OK.]"]];
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
    }   // switch (xrp.type) end
    
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
    }   // switch (xrp.pymt.type) end
    char* raw = nullptr;
    rv = JUB_SignTransactionXRP(contextID,
                                path,
                                xrp,
                                &raw);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionXRP() return 0x%2lx.]", rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionXRP() OK.]"]];
    
    if (raw) {
        size_t txLen = strlen(raw)/2;
        [self addMsgData:[NSString stringWithFormat:@"tx raw[%lu]: %s.", txLen, raw]];
        
        rv = JUB_FreeMemory(raw);
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return 0x%2lx.]", rv]];
            return rv;
        }
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
    }
    
    return rv;
}


@end
