//
//  JUBTRXController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/10/26.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBSharedData.h"

#import "JUBTRXController.h"
#import "JUBTRXAmount.h"

@interface JUBTRXController ()

@end


@implementation JUBTRXController


- (void) viewDidLoad {
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.title = @"TRX options";
    
    self.optItem = JUB_NS_ENUM_MAIN::OPT_TRX;
}


- (NSArray*) subMenu {
    
    return @[
        BUTTON_TITLE_TRX,
        BUTTON_TITLE_TRC10,
        BUTTON_TITLE_TRC20
    ];
}


#pragma mark - 通讯库寻卡回调
- (void) CoinTRXOpt:(NSUInteger)deviceID {
    
    const char* json_file = "";
    switch (self.selectedMenuIndex) {
    case JUB_NS_ENUM_TRX_OPT::BTN_TRX:
    {
        json_file = JSON_FILE_TRX;
        break;
    }
    case JUB_NS_ENUM_TRX_OPT::BTN_TRC10:
    {
        json_file = JSON_FILE_TRC10;
        break;
    }
    case JUB_NS_ENUM_TRX_OPT::BTN_TRC20:
    {
        json_file = JSON_FILE_TRC20;
        break;
    }
    default:
        break;
    }   // switch (self.selectedMenuIndex) end
    
    NSString *filePath = [[NSBundle mainBundle] pathForResource:[NSString stringWithFormat:@"%s", json_file]
                                                         ofType:@"json"];
    Json::Value root = readJSON([filePath UTF8String]);
    
    [self TRX_test:deviceID
              root:root
            choice:(int)self.optIndex];
}


#pragma mark - TRX applet
- (void) TRX_test:(NSUInteger)deviceID
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
                [self addMsgData:[NSString stringWithFormat:@"[JUB_ClearContext() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
            }
            else {
                [self addMsgData:[NSString stringWithFormat:@"[JUB_ClearContext() OK.]"]];
            }
            [sharedData setCurrContextID:0];
        }
        
        CONTEXT_CONFIG_TRX cfg;
        cfg.mainPath = (char*)root["main_path"].asCString();
        rv = JUB_CreateContextTRX(cfg, deviceID, &contextID);
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextTRX() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
            return;
        }
        [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextTRX() OK.]"]];
        [sharedData setCurrMainPath:[NSString stringWithFormat:@"%s", cfg.mainPath]];
        [sharedData setCurrContextID:contextID];
        
        [self CoinOpt:contextID
                 root:root
               choice:choice];
    }
    catch (...) {
        error_exit("[Error format json file.]\n");
        [self addMsgData:[NSString stringWithFormat:@"[Error format json file.]"]];
    }
}


- (void) get_address_pubkey:(NSUInteger)contextID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    
    char* pubkey = nullptr;
    rv = JUB_GetMainHDNodeTRX(contextID, JUB_ENUM_PUB_FORMAT::HEX, &pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeTRX() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeTRX() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"MainXpub(%@) in hex format: %s.", [sharedData currMainPath], pubkey]];
    rv = JUB_FreeMemory(pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
    
    BIP44_Path path;
    path.change       = [sharedData currPath].change;
    path.addressIndex = [sharedData currPath].addressIndex;
    
    pubkey = nullptr;
    rv = JUB_GetHDNodeTRX(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeTRX() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeTRX() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"pubkey(%@/%d/%llu) in hex format: %s.", [sharedData currMainPath], path.change, path.addressIndex, pubkey]];
    rv = JUB_FreeMemory(pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
    
    char* address = nullptr;
    rv = JUB_GetAddressTRX(contextID, path, BOOL_FALSE, &address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressTRX() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressTRX() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"address(%@/%d/%llu): %s.", [sharedData currMainPath], path.change, path.addressIndex, address]];
    rv = JUB_FreeMemory(address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
}


- (void) show_address_test:(NSUInteger)contextID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    
    BIP44_Path path;
    path.change       = [sharedData currPath].change;
    path.addressIndex = [sharedData currPath].addressIndex;
    
    JUB_CHAR_PTR address;
    rv = JUB_GetAddressTRX(contextID, path, BOOL_TRUE, &address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressTRX() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressTRX() OK.]"]];
    [self addMsgData:[NSString stringWithFormat:@"Show address(%@/%d/%llu) is: %s.", [sharedData currMainPath], path.change, path.addressIndex, address]];
    
    rv = JUB_FreeMemory(address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
}


- (NSUInteger) set_my_address_proc:(NSUInteger)contextID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return rv;
    }
    
    BIP44_Path path;
    path.change       = [sharedData currPath].change;
    path.addressIndex = [sharedData currPath].addressIndex;
    
    JUB_CHAR_PTR address = nullptr;
    rv = JUB_SetMyAddressTRX(contextID, path, &address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SetMyAddressTRX() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_SetMyAddressTRX() OK.]"]];
    [self addMsgData:[NSString stringWithFormat:@"Set my address(%@/%u/%llu) is: %s.", [sharedData currMainPath], path.change, path.addressIndex, address]];
    
    return rv;
}


- (NSString*) inputAmount {
    
    __block
    NSString *amount;
    
    __block
    BOOL isDone = NO;
    JUBCustomInputAlert *customInputAlert = [JUBCustomInputAlert showCallBack:^(
        NSString * _Nonnull content,
        JUBDissAlertCallBack _Nonnull dissAlertCallBack,
        JUBSetErrorCallBack  _Nonnull setErrorCallBack
    ) {
        NSLog(@"content = %@", content);
        if (nil == content) {
            isDone = YES;
            dissAlertCallBack();
        }
        else if (        [content isEqual:@""]
                 || [JUBTRXAmount isValid:content
                                      opt:(JUB_NS_ENUM_TRX_OPT)self.selectedMenuIndex]
                 ) {
            //隐藏弹框
            amount = content;
            isDone = YES;
            dissAlertCallBack();
        }
        else {
            setErrorCallBack([JUBTRXAmount formatRules]);
            isDone = NO;
        }
    } keyboardType:UIKeyboardTypeDecimalPad];
    customInputAlert.title = [JUBTRXAmount title:(JUB_NS_ENUM_TRX_OPT)self.selectedMenuIndex];
    customInputAlert.message = [JUBTRXAmount message];
    customInputAlert.textFieldPlaceholder = [JUBTRXAmount formatRules];
    customInputAlert.limitLength = [JUBTRXAmount limitLength];
    
    while (!isDone) {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
                                 beforeDate:[NSDate distantFuture]];
    }
    
    return [JUBTRXAmount convertToProperFormat:amount
                                           opt:(JUB_NS_ENUM_TRX_OPT)self.selectedMenuIndex];
}


- (NSUInteger) tx_proc:(NSUInteger)contextID
                amount:(NSString*)amount
                  root:(Json::Value)root {
    
    NSUInteger rv = JUBR_ERROR;
    
    switch(self.selectedMenuIndex) {
    default:
        rv = [self transaction_proc:contextID
                             amount:amount
                               root:root];
        break;
    }   // switch(self.selectedMenuIndex) end
    
    return rv;
}


- (NSUInteger) transaction_proc:(NSUInteger)contextID
                         amount:(NSString*)amount
                           root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
    if (!root["TRX"]["contracts"].isObject()) {
        return JUBR_ARGUMENTS_BAD;
    }
    
    JUB_TX_TRX tx;
    tx.ref_block_bytes = (char*)root["TRX"]["pack"]["ref_block_bytes"].asCString();
    tx.ref_block_num = 0;
    tx.ref_block_hash = (char*)root["TRX"]["pack"]["ref_block_hash"].asCString();
    tx.data = nullptr;
    tx.expiration = (char*)root["TRX"]["pack"]["expiration"].asCString();
    tx.timestamp = (char*)root["TRX"]["pack"]["timestamp"].asCString();
    tx.fee_limit = (char*)"0";
    
    JUB_CONTRACT_TRX contrTRX;
    contrTRX.type = (JUB_ENUM_TRX_CONTRACT_TYPE)root["TRX"]["contracts"]["type"].asUInt();
    
    std::string strType = std::to_string((unsigned int)contrTRX.type);
    char* sType = new char[strType.length()+1];
    memset(sType, 0x00, strType.length()+1);
    std::copy(strType.begin(), strType.end(), sType);
    
    std::string strOwnerAddress = root["TRX"]["contracts"]["owner_address"].asCString();
    
    switch ((JUB_ENUM_TRX_CONTRACT_TYPE)contrTRX.type) {
    case JUB_ENUM_TRX_CONTRACT_TYPE::XFER_CONTRACT:
        contrTRX.transfer.owner_address = (JUB_CHAR_PTR)strOwnerAddress.c_str();
        contrTRX.transfer.to_address = (JUB_CHAR_PTR)root["TRX"]["contracts"][sType]["to_address"].asCString();
        contrTRX.transfer.amount = root["TRX"]["contracts"][sType]["amount"].asUInt();
        if (NSComparisonResult::NSOrderedSame != [amount compare:@""]) {
            contrTRX.transfer.amount = [amount integerValue];
        }
        break;
    case JUB_ENUM_TRX_CONTRACT_TYPE::XFER_ASSET_CONTRACT:
        contrTRX.transferAsset.owner_address = (JUB_CHAR_PTR)strOwnerAddress.c_str();
        contrTRX.transferAsset.asset_name = (JUB_CHAR_PTR)root["TRX"]["contracts"][sType]["asset_name"].asCString();
        contrTRX.transferAsset.to_address = (JUB_CHAR_PTR)root["TRX"]["contracts"][sType]["to_address"].asCString();
        contrTRX.transferAsset.amount = root["TRX"]["contracts"][sType]["amount"].asUInt();
        if (NSComparisonResult::NSOrderedSame != [amount compare:@""]) {
            contrTRX.transferAsset.amount = [amount integerValue];
        }
        break;
    case JUB_ENUM_TRX_CONTRACT_TYPE::TRIG_SMART_CONTRACT:
        tx.fee_limit = (char*)root["TRX"]["contracts"][sType]["fee_limit"].asCString();
        contrTRX.triggerSmart.owner_address = (JUB_CHAR_PTR)strOwnerAddress.c_str();
        contrTRX.triggerSmart.contract_address = (char*)root["TRX"]["contracts"][sType]["contract_address"].asCString();
        contrTRX.triggerSmart.data = (char*)root["TRX"]["contracts"][sType]["data"].asCString();
        contrTRX.triggerSmart.call_value = root["TRX"]["contracts"][sType]["call_value"].asUInt64();
        if (NSComparisonResult::NSOrderedSame != [amount compare:@""]) {
            contrTRX.triggerSmart.call_value = [amount integerValue];
        }
        contrTRX.triggerSmart.call_token_value = root["TRX"]["contracts"][sType]["call_token_value"].asUInt64();
        contrTRX.triggerSmart.token_id = root["TRX"]["contracts"][sType]["token_id"].asUInt64();
        break;
    case JUB_ENUM_TRX_CONTRACT_TYPE::CREATE_SMART_CONTRACT:
    case JUB_ENUM_TRX_CONTRACT_TYPE::NS_ITEM_TRX_CONTRACT:
    default:
        return JUBR_ARGUMENTS_BAD;
    }
    
    tx.contracts = &contrTRX;
    tx.contractCount = 1;
    
    JUB_CHAR_PTR packContractInPb = nullptr;
    rv = JUB_PackContractTRX(contextID, tx, &packContractInPb);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_PackContractTRX() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_PackContractTRX() OK.]"]];
    
    if (packContractInPb) {
        [self addMsgData:[NSString stringWithFormat:@"pack contract in protobuf(%lu): %s.", strlen(packContractInPb)/2, packContractInPb]];
    }
    
    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["TRX"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["TRX"]["bip32_path"]["addressIndex"].asUInt();
    
    JUB_CHAR_PTR rawInJSON = nullptr;
    rv = JUB_SignTransactionTRX(contextID,
                                path,
                                packContractInPb,
                                &rawInJSON);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionTRX() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionTRX() OK.]"]];
    
    rv = JUB_FreeMemory(packContractInPb);
    if (rawInJSON) {
        [self addMsgData:[NSString stringWithFormat:@"tx raw in JSON: %s.", rawInJSON]];
        
        rv = JUB_FreeMemory(rawInJSON);
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
            return rv;
        }
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
    }
    
    return JUBR_OK;
}


@end
