//
//  JUBBTCController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/4/28.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBPinAlertView.h"
#import "JUBListAlert.h"
#import "JUBSharedData.h"

#import "JUBBTCController.h"

#import "JUBAlertView.h"


@interface JUBBTCController ()

@end


@implementation JUBBTCController


- (void)viewDidLoad {
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.optItem = JUB_NS_ENUM_MAIN::OPT_BTC;

    self.coinTypeArray = @[BUTTON_TITLE_BTCP2PKH,
                           BUTTON_TITLE_BTCP2WPKH,
                           BUTTON_TITLE_LTC,
                           BUTTON_TITLE_DASH,
                           BUTTON_TITLE_BCH,
                           BUTTON_TITLE_QTUM,
                           BUTTON_TITLE_QTUM_QRC20,
                           BUTTON_TITLE_USDT,
//                           BUTTON_TITLE_HCASH
    ];
    
    switch ([[JUBSharedData sharedInstance] deviceType]) {
    case JUB_NS_ENUM_DEV_TYPE::SEG_BLE:
        self.navRightButtonTitle = BUTTON_TITLE_SETUNIT;
        break;
    case JUB_NS_ENUM_DEV_TYPE::SEG_NFC:
    default:
        break;
    }   // switch ([[JUBSharedData sharedInstance] deviceType]) end
}


- (void)navRightButtonCallBack {
    
    JUBListAlert *listAlert = [JUBListAlert showCallBack:^(NSString *_Nonnull selectedItem) {
        NSLog(@"UNIT selected: %@", selectedItem);
        JUB_ENUM_BTC_UNIT_TYPE unit = JUB_ENUM_BTC_UNIT_TYPE::ns;
        if ([selectedItem isEqual:BUTTON_TITLE_UNIT_BTC]) {
            unit = JUB_ENUM_BTC_UNIT_TYPE::BTC;
        }
        else if ([selectedItem isEqual:BUTTON_TITLE_UNIT_cBTC]) {
            unit = JUB_ENUM_BTC_UNIT_TYPE::cBTC;
        }
        else if ([selectedItem isEqual:BUTTON_TITLE_UNIT_mBTC]) {
            unit = JUB_ENUM_BTC_UNIT_TYPE::mBTC;
        }
        else if ([selectedItem isEqual:BUTTON_TITLE_UNIT_uBTC]) {
            unit = JUB_ENUM_BTC_UNIT_TYPE::uBTC;
        }
        else if ([selectedItem isEqual:BUTTON_TITLE_UNIT_Satoshi]) {
            unit = JUB_ENUM_BTC_UNIT_TYPE::Satoshi;
        }
        
        [[JUBSharedData sharedInstance] setCoinUnit:unit];
    }];
    
    listAlert.title = @"Please select UNIT:";
    [listAlert addItems:@[
        BUTTON_TITLE_UNIT_BTC,
        BUTTON_TITLE_UNIT_cBTC,
        BUTTON_TITLE_UNIT_mBTC,
        BUTTON_TITLE_UNIT_uBTC,
        BUTTON_TITLE_UNIT_Satoshi
    ]];
}


#pragma mark - 通讯库寻卡回调
- (void)CoinBTCOpt:(NSUInteger)deviceID {
    
    const char* json_file = "";
    JUB_ENUM_COINTYPE_BTC coinType = JUB_ENUM_COINTYPE_BTC::COINBTC;
    switch ((JUB_NS_ENUM_BTC_COIN)self.optCoinType) {
    case JUB_NS_ENUM_BTC_COIN::BTN_BTC_P2PKH:
    {
        json_file = JSON_FILE_BTC_44;
        coinType = COINBTC;
        break;
    }
    case JUB_NS_ENUM_BTC_COIN::BTN_BTC_P2WPKH:
    {
        json_file = JSON_FILE_BTC_49;
        coinType = COINBTC;
        break;
    }
    case JUB_NS_ENUM_BTC_COIN::BTN_LTC:
    {
        json_file = JSON_FILE_LTC;
        coinType = COINLTC;
        break;
    }
    case JUB_NS_ENUM_BTC_COIN::BTN_DASH:
    {
        json_file = JSON_FILE_DASH;
        coinType = COINDASH;
        break;
    }
    case JUB_NS_ENUM_BTC_COIN::BTN_BCH:
    {
        json_file = JSON_FILE_BCH;
        coinType = COINBCH;
        break;
    }
    case JUB_NS_ENUM_BTC_COIN::BTN_QTUM:
    {
        json_file = JSON_FILE_QTUM;
        coinType = COINQTUM;
        break;
    }
    case JUB_NS_ENUM_BTC_COIN::BTN_QTUM_QRC20:
    {
        json_file = JSON_FILE_QTUM_QRC20;
        coinType = COINQTUM;
        break;
    }
    case JUB_NS_ENUM_BTC_COIN::BTN_USDT:
    {
        json_file = JSON_FILE_BTC_USDT;
        coinType = COINUSDT;
        break;
    }
    default:
//        json_file = JSON_FILE_HCASH;
        break;
    }   // switch ((JUB_NS_ENUM_BTC_COINTYPE)self.optCoinType) end
    
    NSString *filePath = [[NSBundle mainBundle] pathForResource:[NSString stringWithFormat:@"%s", json_file]
                                                         ofType:@"json"];
    Json::Value root = readJSON([filePath UTF8String]);
    
    switch (self.optCoinType) {
//    case JUB_NS_ENUM_BTC_COIN::BTN_HCASH:
//    {
//        HC_test(deviceID, root, (int)self.optIndex);
//        break;
//    }
    default:
    {
        [self BTC_test:deviceID
                  root:root
                choice:(int)self.optIndex
              coinType:coinType];
        break;
    }
    }   // switch (self.optCoinType) end
}


#pragma mark - BTC applet
- (void)BTC_test:(NSUInteger)deviceID
            root:(Json::Value)root
          choice:(int)choice
        coinType:(JUB_ENUM_COINTYPE_BTC)coinType {
    
    JUB_RV rv = JUBR_ERROR;
    
    try {
        JUB_UINT16 contextID = [[JUBSharedData sharedInstance] currContextID];
        if (0 == contextID) {
            
            CONTEXT_CONFIG_BTC cfg;
            cfg.mainPath = (char*)root["main_path"].asCString();
            cfg.coinType = coinType;
            
            if (COINBCH == coinType) {
                cfg.transType = p2pkh;
            }
            else {
                if (root["p2sh-segwit"].asBool()) {
                    cfg.transType = p2sh_p2wpkh;
                }
                else {
                    cfg.transType = p2pkh;
                }
            }
            
            rv = JUB_CreateContextBTC(cfg, deviceID, &contextID);
            if (JUBR_OK != rv) {
                [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextBTC() return 0x%2lx.]", rv]];
                return;
            }
            [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextBTC() OK.]"]];
        }
        
        switch ([[JUBSharedData sharedInstance] deviceType]) {
        case JUB_NS_ENUM_DEV_TYPE::SEG_BLE:
        {
            dispatch_async(dispatch_get_global_queue(0, 0), ^ {
                [self BTCSingleStepOpt:contextID
                                  root:root
                                choice:choice];
            });
            
            [self BTCCombStepOpt:contextID
                            root:root
                          choice:choice];
            break;
        }
        case JUB_NS_ENUM_DEV_TYPE::SEG_NFC:
        {
            [self BTCSingleStepOpt:contextID
                              root:root
                            choice:choice];
            [self BTCCombStepOpt:contextID
                            root:root
                          choice:choice];
        }
        default:
            break;
        }   // switch ([[JUBSharedData sharedInstance] deviceType]) end
    }
    catch (...) {
        error_exit("[Error format json file.]\n");
        [self addMsgData:[NSString stringWithFormat:@"[Error format json file.]"]];
    }
}


- (void)BTCSingleStepOpt:(NSUInteger)contextID
                    root:(Json::Value)root
                  choice:(int)choice {
    
    switch (choice) {
    case JUB_NS_ENUM_OPT::GET_ADDRESS:
    {
        [self get_address_test:contextID
                          root:root];
        break;
    }
    case JUB_NS_ENUM_OPT::SHOW_ADDRESS:
    {
        [self show_address_test:contextID];
        break;
    }
    case JUB_NS_ENUM_OPT::SET_TIMEOUT:
    {
        break;
    }
    case JUB_NS_ENUM_OPT::SET_MY_ADDRESS:
    case JUB_NS_ENUM_OPT::TRANSACTION:
    default:
        break;
    }   // switch (choice) end
}


- (void)BTCCombStepOpt:(NSUInteger)contextID
                  root:(Json::Value)root
                choice:(int)choice {
    
    switch (choice) {
    case JUB_NS_ENUM_OPT::SET_MY_ADDRESS:
    {
        [self set_my_address_test_BTC:contextID];
        break;
    }
    case JUB_NS_ENUM_OPT::TRANSACTION:
    {

        [self transaction_test:contextID
                          root:root];
        break;
    }
    case JUB_NS_ENUM_OPT::GET_ADDRESS:
    case JUB_NS_ENUM_OPT::SHOW_ADDRESS:
    case JUB_NS_ENUM_OPT::SET_TIMEOUT:
    default:
        break;
    }   // switch (choice) end
}


- (void)get_address_test:(NSUInteger)contextID
                    root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_CHAR_PTR mainXpub;
    rv = JUB_GetMainHDNodeBTC(contextID, &mainXpub);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeBTC() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeBTC() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"Main xpub : %s.", mainXpub]];
    
    JUB_FreeMemory(mainXpub);
    
    JUB_CHAR_PTR xpub;
    
    BIP44_Path path;
    path.change = (self.change ? JUB_ENUM_BOOL::BOOL_TRUE:JUB_ENUM_BOOL::BOOL_FALSE);
    path.addressIndex = self.addressIndex;
    
    rv = JUB_GetHDNodeBTC(contextID, path, &xpub);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeBTC() return 0x%2lx.]", rv]];
        return;
    }
    else {
        [self addMsgData:[NSString stringWithFormat:@"input xpub(%d/%llu): %s.", path.change, path.addressIndex, xpub]];
        
        JUB_FreeMemory(xpub);
    }
    
    JUB_CHAR_PTR address;
    rv = JUB_GetAddressBTC(contextID, path, BOOL_FALSE, &address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressBTC() return 0x%2lx.]", rv]];
    }
    else {
        [self addMsgData:[NSString stringWithFormat:@"input address(%d/%llu): %s.", path.change, path.addressIndex, address]];
        
        JUB_FreeMemory(address);
    }
}


- (void)show_address_test:(NSUInteger)contextID {
    
    JUB_RV rv = JUBR_ERROR;
    
    BIP44_Path path;
    path.change = (self.change ? JUB_ENUM_BOOL::BOOL_TRUE:JUB_ENUM_BOOL::BOOL_FALSE);
    path.addressIndex = self.addressIndex;
    
    JUB_CHAR_PTR address;
    rv = JUB_GetAddressBTC(contextID, path, BOOL_TRUE, &address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressBTC() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"show address is: %s.", address]];
    
    JUB_FreeMemory(address);
}


- (void)set_my_address_test_BTC:(NSUInteger)contextID {
    
    __block JUB_RV rv = [self show_virtualKeyboard:contextID];
    if (JUBR_OK != rv) {
        return;
    }
    
    [JUBPinAlertView showInputPinAlert:^(NSString * _Nonnull pin) {
        JUBSharedData *sharedData = [JUBSharedData sharedInstance];
        [sharedData setUserPin:pin];
        
        rv = [self verify_pin:contextID];
        if (JUBR_OK != rv) {
            return;
        }
        
        rv = [self set_my_address_proc:contextID];
        if (JUBR_OK != rv) {
            return;
        }
    }];
}


- (NSUInteger)set_my_address_proc:(NSUInteger)contextID {
    
    JUB_RV rv = JUBR_ERROR;
    
    BIP44_Path path;
    path.change = (self.change ? JUB_ENUM_BOOL::BOOL_TRUE:JUB_ENUM_BOOL::BOOL_FALSE);
    path.addressIndex = self.addressIndex;
    
    JUB_CHAR_PTR address = nullptr;
    rv = JUB_SetMyAddressBTC(contextID, path, &address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SetMyAddressBTC() return 0x%2lx.]", rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_SetMyAddressBTC() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"set my address is: %s.", address]];
    JUB_FreeMemory(address);
    
    return rv;
}


- (NSUInteger)set_unit_test:(NSUInteger)contextID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_ENUM_BTC_UNIT_TYPE unit = [[JUBSharedData sharedInstance] coinUnit];
    if (JUB_ENUM_BTC_UNIT_TYPE::ns == unit) {
        return JUBR_OK;
    }
    
    rv = JUB_SetUnitBTC(contextID, unit);
    if (   JUBR_OK               != rv
        && JUBR_IMPL_NOT_SUPPORT != rv
        ) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SetUnitBTC() return 0x%2lx.]", rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_SetUnitBTC() OK.]"]];
    
    return rv;
}


- (void)transaction_test:(NSUInteger)contextID
                    root:(Json::Value)root {
    
    __block JUB_RV rv = JUBR_ERROR;
    
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
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
            
            rv = [self tx_proc:contextID
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
        __block BOOL isDone = NO;
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
                        JUBSharedData *sharedData = [JUBSharedData sharedInstance];
                        [sharedData setUserPin:pin];
                        
                        rv = [self verify_pin:contextID];
                        if (JUBR_OK != rv) {
                            isDone = YES;
                            return;
                        }
                        
                        rv = [self set_unit_test:contextID];
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
                    
                    rv = [self set_unit_test:contextID];
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
                [sharedData setUserPin:pin];
                
                rv = [self verify_pin:contextID];
                if (JUBR_OK != rv) {
                    isDone = YES;
                    return;
                }
                
                rv = [self set_unit_test:contextID];
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
            
            __block JUBAlertView *alertView;
            
            dispatch_async(dispatch_get_main_queue(), ^{
                alertView = [JUBAlertView showMsg:@"Transaction in progress..."];
            });
            
            rv = [self tx_proc:contextID
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


- (NSUInteger)tx_proc:(NSUInteger)contextID
                 root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
    switch((JUB_NS_ENUM_BTC_COIN)self.optCoinType) {
    case JUB_NS_ENUM_BTC_COIN::BTN_BTC_P2PKH:
    case JUB_NS_ENUM_BTC_COIN::BTN_BTC_P2WPKH:
    case JUB_NS_ENUM_BTC_COIN::BTN_LTC:
    case JUB_NS_ENUM_BTC_COIN::BTN_DASH:
    case JUB_NS_ENUM_BTC_COIN::BTN_QTUM:
        rv = [self transaction_proc:contextID
                               root:root];
        break;
    case JUB_NS_ENUM_BTC_COIN::BTN_QTUM_QRC20:
        rv = [self transactionQTUM_proc:contextID
                                   root:root];
        break;
    case JUB_NS_ENUM_BTC_COIN::BTN_USDT:
        rv = [self transactionUSDT_proc:contextID
                                   root:root];
        break;
//    case JUB_NS_ENUM_BTC_COIN::BTN_HCASH:
//        rv = [self transactionHC_proc:contextID
//                                 root:root];
//        break;
    default:
        break;
    }
    
    return rv;
}


- (NSUInteger)transaction_proc:(NSUInteger)contextID
                          root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_UINT32 version = root["ver"].asInt();
    
    std::vector<INPUT_BTC> inputs;
    std::vector<OUTPUT_BTC> outputs;
    int inputNumber = root["inputs"].size();
    
    for (int i = 0; i < inputNumber; i++) {
        INPUT_BTC input;
        input.type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
        input.preHash = (char*)root["inputs"][i]["preHash"].asCString();
        input.preIndex = root["inputs"][i]["preIndex"].asInt();
        input.path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
        input.path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();
        input.amount = root["inputs"][i]["amount"].asUInt64();
        input.nSequence = 0xffffffff;
        inputs.push_back(input);
    }
    
    int outputNumber = root["outputs"].size();
    
    for (int i = 0; i < outputNumber; i++) {
        OUTPUT_BTC output;
        output.type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
        output.stdOutput.address = (char*)root["outputs"][i]["address"].asCString();
        output.stdOutput.amount = root["outputs"][i]["amount"].asUInt64();
        output.stdOutput.changeAddress = (JUB_ENUM_BOOL)root["outputs"][i]["change_address"].asBool();
        if (output.stdOutput.changeAddress) {
            output.stdOutput.path.change = (JUB_ENUM_BOOL)root["outputs"][i]["bip32_path"]["change"].asBool();
            output.stdOutput.path.addressIndex = root["outputs"][i]["bip32_path"]["addressIndex"].asInt();
        }
        outputs.push_back(output);
    }
    
    char* raw = nullptr;
    rv = JUB_SignTransactionBTC(contextID, version, &inputs[0], (JUB_UINT16)inputs.size(), &outputs[0], (JUB_UINT16)outputs.size(), 0, &raw);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionBTC() return 0x%2lx.]", rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionBTC() OK.]"]];
    
    if (JUBR_USER_CANCEL == rv) {
        [self addMsgData:[NSString stringWithFormat:@"[User cancel the transaction !]"]];
        return rv;
    }
    if (   JUBR_OK != rv
        || nullptr == raw
        ) {
        [self addMsgData:[NSString stringWithFormat:@"[Error sign tx.]"]];
        return rv;
    }
    if (raw) {
        size_t txLen = strlen(raw)/2;
        [self addMsgData:[NSString stringWithFormat:@"tx raw[%lu]: %s.", txLen, raw]];
        
        JUB_FreeMemory(raw);
    }
    
    return rv;
}


- (NSUInteger)transactionQTUM_proc:(NSUInteger)contextID
                              root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_UINT32 version = root["ver"].asInt();
    
    std::vector<INPUT_BTC> inputs;
    std::vector<OUTPUT_BTC> outputs;
    int inputNumber = root["inputs"].size();
    
    for (int i = 0; i < inputNumber; i++) {
        INPUT_BTC input;
        input.type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
        input.preHash = (char*)root["inputs"][i]["preHash"].asCString();
        input.preIndex = root["inputs"][i]["preIndex"].asInt();
        input.path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
        input.path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();
        input.amount = root["inputs"][i]["amount"].asUInt64();
        inputs.push_back(input);
    }
    
    int outputNumber = root["outputs"].size();
    
    for (int i = 0; i < outputNumber; i++) {
        OUTPUT_BTC output;
        output.type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
        output.stdOutput.address = (char*)root["outputs"][i]["address"].asCString();
        output.stdOutput.amount = root["outputs"][i]["amount"].asUInt64();
        output.stdOutput.changeAddress = (JUB_ENUM_BOOL)root["outputs"][i]["change_address"].asBool();
        if (output.stdOutput.changeAddress) {
            output.stdOutput.path.change = (JUB_ENUM_BOOL)root["outputs"][i]["bip32_path"]["change"].asBool();
            output.stdOutput.path.addressIndex = root["outputs"][i]["bip32_path"]["addressIndex"].asInt();
        }
        outputs.push_back(output);
    }
    
    OUTPUT_BTC QRC20_output;
    JUB_CHAR_CPTR contractAddress = (char*)root["QRC20_contractAddr"].asCString();
    JUB_UINT8 decimal = root["QRC20_decimal"].asUInt64();
    JUB_CHAR_CPTR symbol = (char*)root["QRC20_symbol"].asCString();
    JUB_UINT64 gasLimit = root["gasLimit"].asUInt64();
    JUB_UINT64 gasPrice = root["gasPrice"].asUInt64();
    JUB_CHAR_CPTR to = (char*)root["QRC20_to"].asCString();
    JUB_CHAR_CPTR value = (char*)root["QRC20_amount"].asCString();
    rv = JUB_BuildQRC20Outputs(contextID,
                               contractAddress, decimal, symbol,
                               gasLimit, gasPrice,
                               to, value,
                               &QRC20_output);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_BuildQRC20Outputs() return 0x%2lx.]", rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_BuildQRC20Outputs() OK.]"]];
    outputs.emplace_back(QRC20_output);
    
    char* raw = nullptr;
    rv = JUB_SignTransactionBTC(contextID, version, &inputs[0], (JUB_UINT16)inputs.size(), &outputs[0], (JUB_UINT16)outputs.size(), 0, &raw);
    [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionBTC() return 0x%2lx.]", rv]];
    
    if (JUBR_USER_CANCEL == rv) {
        [self addMsgData:[NSString stringWithFormat:@"[User cancel the transaction !]"]];
        return rv;
    }
    if (   JUBR_OK != rv
        || nullptr == raw
        ) {
        [self addMsgData:[NSString stringWithFormat:@"[Error sign tx.]"]];
        return rv;
    }
    if (raw) {
        size_t txLen = strlen(raw)/2;
        [self addMsgData:[NSString stringWithFormat:@"tx raw[%lu]: %s.", txLen, raw]];
        
        JUB_FreeMemory(raw);
    }
    
    return rv;
}


- (NSUInteger)transactionUSDT_proc:(NSUInteger)contextID
                              root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_UINT32 version = root["ver"].asInt();
    
    std::vector<INPUT_BTC> inputs;
    std::vector<OUTPUT_BTC> outputs;
    int inputNumber = root["inputs"].size();
    
    for (int i = 0; i < inputNumber; i++) {
        INPUT_BTC input;
        input.type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
        input.preHash = (char*)root["inputs"][i]["preHash"].asCString();
        input.preIndex = root["inputs"][i]["preIndex"].asInt();
        input.path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
        input.path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();
        input.amount = root["inputs"][i]["amount"].asUInt64();
        input.nSequence = 0xffffffff;
        inputs.push_back(input);
    }
    
    int outputNumber = root["outputs"].size();
    
    for (int i = 0; i < outputNumber; i++) {
        OUTPUT_BTC output;
        output.type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
        output.stdOutput.address = (char*)root["outputs"][i]["address"].asCString();
        output.stdOutput.amount = root["outputs"][i]["amount"].asUInt64();
        output.stdOutput.changeAddress = (JUB_ENUM_BOOL)root["outputs"][i]["change_address"].asBool();
        if (output.stdOutput.changeAddress) {
            output.stdOutput.path.change = (JUB_ENUM_BOOL)root["outputs"][i]["bip32_path"]["change"].asBool();
            output.stdOutput.path.addressIndex = root["outputs"][i]["bip32_path"]["addressIndex"].asInt();
        }
        outputs.push_back(output);
    }
    
    OUTPUT_BTC USDT_outputs[2] = {};
    rv = JUB_BuildUSDTOutputs(contextID, (char*)root["USDT_to"].asCString(), root["USDT_amount"].asUInt64(), USDT_outputs);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_BuildUSDTOutputs() return 0x%2lx.]", rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_BuildUSDTOutputs() OK.]"]];
    
    outputs.emplace_back(USDT_outputs[0]);
    outputs.emplace_back(USDT_outputs[1]);
    
    char* raw = nullptr;
    rv = JUB_SignTransactionBTC(contextID, version, &inputs[0], (JUB_UINT16)inputs.size(), &outputs[0], (JUB_UINT16)outputs.size(), 0, &raw);
    [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionBTC() return 0x%2lx.]", rv]];

    if (JUBR_USER_CANCEL == rv) {
        [self addMsgData:[NSString stringWithFormat:@"[User cancel the transaction !]"]];
        return rv;
    }
    if (   JUBR_OK != rv
        || nullptr == raw
        ) {
        [self addMsgData:[NSString stringWithFormat:@"[Error sign tx.]"]];
        return rv;
    }
    if (raw) {
        size_t txLen = strlen(raw)/2;
        [self addMsgData:[NSString stringWithFormat:@"tx raw[%lu]: %s.", txLen, raw]];
        
        JUB_FreeMemory(raw);
    }
    
    return rv;
}


//#pragma mark - Hcash applet
//void HC_test(NSUInteger deviceID, Json::Value root, int choice) {
//
//    JUB_RV rv = JUBR_ERROR;
//
//    try {
//        JUB_UINT16 contextID = 0;
//        CONTEXT_CONFIG_HC cfg;
//        cfg.mainPath = (char*)root["main_path"].asCString();
//
//        rv = JUB_CreateContextHC(cfg, deviceID, &contextID);
//        if (JUBR_OK != rv) {
//            [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextHC() return 0x%2lx.]", rv]];
//            return;
//        }
//        [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextHC() OK.]"]];
//
//        switch (choice) {
//        case JUB_NS_ENUM_OPT::GET_ADDRESS:
//        {
//            get_address_test_HC(contextID, root);
//            break;
//        }
//        case JUB_NS_ENUM_OPT::SHOW_ADDRESS:
//        {
////            show_address_test_HC(contextID);
//            break;
//        }
//        case JUB_NS_ENUM_OPT::TRANSACTION:
//        {
//            transactionHC_test(contextID, root);
//            break;
//        }
//        case JUB_NS_ENUM_OPT::SET_MY_ADDRESS:
//        {
////            [self set_my_address_test_BTC:contextID];
//            break;
//        }
//        case JUB_NS_ENUM_OPT::SET_TIMEOUT:
//        {
//            break;
//        }
//        default:
//            break;
//        }   // switch (choice) end
//    }
//    catch (...) {
//        error_exit("[Error format json file.]\n");
//        [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextHC() OK.]"]];
//    }
//}
//
//
//void get_address_test_HC(NSUInteger contextID, Json::Value root) {
//
//    JUB_CHAR_PTR mainXpub;
//    JUB_RV rv = JUB_GetMainHDNodeHC(contextID, &mainXpub);
//    if (JUBR_OK != rv) {
//        cout << "[JUB_GetMainHDNodeHC() return " << GetErrMsg(rv) << ".]" << endl;
//        return;
//    }
//    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeHC() OK.]"]];
//
//    cout << "Main xpub : " << mainXpub << endl;
//    JUB_FreeMemory(mainXpub);
//
//    int inputNumber = root["inputs"].size();
//    for (int i = 0; i < inputNumber; i++) {
//        JUB_CHAR_PTR xpub;
//
//        BIP44_Path path;
//        path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
//        path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();
//
//        JUB_RV rv = JUB_GetHDNodeHC(contextID, path, &xpub);
//        if (JUBR_OK != rv) {
//            cout << "[JUB_GetHDNodeHC() return " << GetErrMsg(rv) << ".]" << endl;
//            break;
//        }
//        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeHC() OK.]"]];
//
//        cout << "input " << i << " xpub : " << xpub << endl;
//        [self addMsgData:[NSString stringWithFormat:@"input %d xpub : %s.", i, xpub]];
//        JUB_FreeMemory(xpub);
//
//        JUB_CHAR_PTR address;
//        rv = JUB_GetAddressHC(contextID, path, BOOL_FALSE, &address);
//        if (JUBR_OK != rv) {
//            cout << "[JUB_GetAddressHC() return " << GetErrMsg(rv) << ".]" << endl;
//            break;
//        }
//        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressHC() OK.]"]];
//        cout << "input " << i << " address : " << address << endl;
//        [self addMsgData:[NSString stringWithFormat:@"input %d address : %s.", i, address]];
//        JUB_FreeMemory(address);
//    }
//    if (JUBR_OK != rv) {
//        return;
//    }
//}
//
//
//void show_address_test_HC(NSUInteger contextID) {
//
//    BIP44_Path path;
//    path.change = (self.change ? JUB_ENUM_BOOL::BOOL_TRUE:JUB_ENUM_BOOL::BOOL_FALSE);
//    path.addressIndex = self.addressIndex;
//
//    JUB_CHAR_PTR address;
//    JUB_RV rv = JUB_GetAddressHC(contextID, path, BOOL_TRUE, &address);
//    if (JUBR_OK != rv) {
//        cout << "JUB_GetAddressHC() return " << GetErrMsg(rv) << endl;
//        return;
//    }
//    cout << "show address is : " << address << endl;
//
//    JUB_FreeMemory(address);
//}
//
//
//NSUInteger transactionHC_proc(NSUInteger contextID, Json::Value root) {
//
//    JUB_RV rv = JUBR_ERROR;
//
//    std::vector<INPUT_HC> inputs;
//    std::vector<OUTPUT_HC> outputs;
//    int inputNumber = root["inputs"].size();
//
//    for (int i = 0; i < inputNumber; i++) {
//        INPUT_HC input;
//        input.path.change = (JUB_ENUM_BOOL)root["inputs"][i]["bip32_path"]["change"].asBool();
//        input.path.addressIndex = root["inputs"][i]["bip32_path"]["addressIndex"].asInt();
//        input.amount = root["inputs"][i]["amount"].asUInt64();
//        inputs.push_back(input);
//    }
//
//    int outputNumber = root["outputs"].size();
//
//    for (int i = 0; i < outputNumber; i++) {
//        OUTPUT_HC output;
//        output.changeAddress = (JUB_ENUM_BOOL)root["outputs"][i]["change_address"].asBool();
//        if (output.changeAddress) {
//            output.path.change = (JUB_ENUM_BOOL)root["outputs"][i]["bip32_path"]["change"].asBool();
//            output.path.addressIndex = root["outputs"][i]["bip32_path"]["addressIndex"].asInt();
//        }
//        outputs.push_back(output);
//    }
//
//    char* unsignedRaw = (char*)root["unsigned_tx"].asCString();
//    //NSString* unsignedTx = [NSString stringWithUTF8String:(char*)root["unsigned_tx"].asCString()];
//
//    char* raw = nullptr;
//    rv = JUB_SignTransactionHC(contextID, &inputs[0], (JUB_UINT16)inputs.size(), &outputs[0], (JUB_UINT16)outputs.size(), unsignedRaw, &raw);
//    cout << "JUB_SignTransactionHC() return " << GetErrMsg(rv) << endl;
//
//    if (JUBR_USER_CANCEL == rv) {
//        cout << "User cancel the transaction !" << endl;
//        return rv;
//    }
//    if (   JUBR_OK != rv
//        || nullptr == raw
//        ) {
//        cout << "error sign tx" << endl;
//        return rv;
//    }
//    if (raw) {
//        cout << raw;
//        JUB_FreeMemory(raw);
//    }
//
//    return rv;
//}
//
//
@end
