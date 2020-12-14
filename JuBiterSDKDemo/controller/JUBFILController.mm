//
//  JUBFILController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/11/30.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBSharedData.h"

#import "JUBFILController.h"
#import "JUBFILAmount.h"

@interface JUBFILController ()

@end


@implementation JUBFILController


- (void) viewDidLoad {
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.title = @"FIL options";
    
    self.optItem = JUB_NS_ENUM_MAIN::OPT_FIL;
}


- (NSArray*) subMenu {
    
    return @[
        BUTTON_TITLE_FIL,
    ];
}


#pragma mark - 通讯库寻卡回调
- (void) CoinFILOpt:(NSUInteger)deviceID {
    
    const char* json_file = "";
    switch (self.selectedMenuIndex) {
    case JUB_NS_ENUM_FIL_COIN::BTN_FIL:
    {
        json_file = JSON_FILE_FIL;
        break;
    }
    default:
        break;
    }   // switch (self.selectedMenuIndex) end
    
    NSString *filePath = [[NSBundle mainBundle] pathForResource:[NSString stringWithFormat:@"%s", json_file]
                                                         ofType:@"json"];
    Json::Value root = readJSON([filePath UTF8String]);
    
    [self FIL_test:deviceID
              root:root
            choice:(int)self.optIndex];
}


#pragma mark - FIL applet
- (void) EnterAmount {
    
    __block
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    
    switch(self.selectedMenuIndex) {
    default:
        [super EnterAmount];
        break;
    }
}


- (void) FIL_test:(NSUInteger)deviceID
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
        
        CONTEXT_CONFIG_FIL cfg;
        cfg.mainPath = (char*)root["main_path"].asCString();
        rv = JUB_CreateContextFIL(cfg, deviceID, &contextID);
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextFIL() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
            return;
        }
        [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextETH() OK.]"]];
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
    rv = JUB_GetMainHDNodeFIL(contextID, JUB_ENUM_PUB_FORMAT::HEX, &pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeFIL() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeFIL() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"MainXpub(%@) in hex format: %s.", [sharedData currMainPath], pubkey]];
    rv = JUB_FreeMemory(pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
    
    pubkey = nullptr;
    rv = JUB_GetMainHDNodeFIL(contextID, JUB_ENUM_PUB_FORMAT::XPUB, &pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeFIL() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeFIL() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"MainXpub(%@) in xpub format: %s.", [sharedData currMainPath], pubkey]];
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
    rv = JUB_GetHDNodeFIL(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeFIL() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeFIL() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"pubkey(%@/%d/%llu) in hex format: %s.", [sharedData currMainPath], path.change, path.addressIndex, pubkey]];
    rv = JUB_FreeMemory(pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
    
    pubkey = nullptr;
    rv = JUB_GetHDNodeFIL(contextID, JUB_ENUM_PUB_FORMAT::XPUB, path, &pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeFIL() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeFIL() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"pubkey(%@/%d/%llu) in xpub format: %s.", [sharedData currMainPath], path.change, path.addressIndex, pubkey]];
    rv = JUB_FreeMemory(pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
    
    char* address = nullptr;
    rv = JUB_GetAddressFIL(contextID, path, BOOL_FALSE, &address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressFIL() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressFIL() OK.]"]];
    
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
    rv = JUB_GetAddressFIL(contextID, path, BOOL_TRUE, &address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressFIL() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressFIL() OK.]"]];
    [self addMsgData:[NSString stringWithFormat:@"Show address(%@/%d/%llu) is: %s.", [sharedData currMainPath], path.change, path.addressIndex, address]];
    
    rv = JUB_FreeMemory(address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return %@ 0x%2lx.]", [JUBErrorCode GetErrMsg:rv], rv]];
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
    rv = JUB_SetMyAddressFIL(contextID, path, &address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SetMyAddressFIL() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_SetMyAddressFIL() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"set my address is: %s.", address]];
    rv = JUB_FreeMemory(address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
    
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
                 || [JUBFILAmount isValid:content]
                 ) {
            //隐藏弹框
            amount = content;
            isDone = YES;
            dissAlertCallBack();
        }
        else {
            setErrorCallBack([JUBFILAmount formatRules]);
            isDone = NO;
        }
    } keyboardType:UIKeyboardTypeDecimalPad];
    customInputAlert.title = [JUBFILAmount title:(JUB_NS_ENUM_FIL_COIN)self.selectedMenuIndex];
    customInputAlert.message = [JUBFILAmount message];
    customInputAlert.textFieldPlaceholder = [JUBFILAmount formatRules];
    customInputAlert.limitLength = [JUBFILAmount limitLength];
    
    while (!isDone) {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
                                 beforeDate:[NSDate distantFuture]];
    }
    
    // Convert to the smallest unit
    return [JUBFILAmount convertToProperFormat:amount
                                           opt:(JUB_NS_ENUM_FIL_COIN)self.selectedMenuIndex];
}


- (NSUInteger) tx_proc:(NSUInteger)contextID
                amount:(NSString*)amount
                  root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
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
    
    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["FIL"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["FIL"]["bip32_path"]["addressIndex"].asUInt();
    
    //FIL Test
    uint32_t nonce = root["FIL"]["nonce"].asUInt();//.asDouble();
    uint32_t gasLimit = root["FIL"]["gasLimit"].asUInt();//.asDouble();
    char* gasPriceInAtto = (char*)root["FIL"]["gasPriceInAtto"].asCString();
    char* valueInAtto = (char*)root["FIL"]["valueInAtto"].asCString();
    if (NSComparisonResult::NSOrderedSame != [amount compare:@""]) {
        valueInAtto = (char*)[amount UTF8String];
    }
    char* to = (char*)root["FIL"]["to"].asCString();
    char* data = (char*)root["FIL"]["data"].asCString();
    
    char* raw = nullptr;
    rv = JUB_SignTransactionFIL(contextID,
                                path,
                                nonce, gasLimit, gasPriceInAtto,
                                to, valueInAtto, data,
                                &raw);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionFIL() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionFIL() OK.]"]];
    
    if (raw) {
        size_t txLen = strlen(raw)/2;
        [self addMsgData:[NSString stringWithFormat:@"tx raw[%lu]: %s.", txLen, raw]];
        
        rv = JUB_FreeMemory(raw);
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
            return rv;
        }
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
    }
    
    return rv;
}


////ERC-20 Test
//- (NSUInteger) transactionERC20_proc:(NSUInteger)contextID
//                              amount:(NSString*)amount
//                                root:(Json::Value)root {
//
//    JUB_RV rv = JUBR_ERROR;
//
//    char* tokenName = (char*)root["ERC20"]["tokenName"].asCString();
//    JUB_UINT16 unitDP = root["ERC20"]["dp"].asUInt();
//    char* contractAddress = (char*)root["ERC20"]["contract_address"].asCString();
//    char* to = (char*)root["ERC20"]["contract_address"].asCString();
//    char* token_to = (char*)root["ERC20"]["token_to"].asCString();
//    char* token_value = (char*)root["ERC20"]["token_value"].asCString();
//    if (NSComparisonResult::NSOrderedSame != [amount compare:@""]) {
//        token_value = (char*)[amount UTF8String];
//    }
//
//    char* abi = nullptr;
//    rv = JUB_BuildERC20AbiETH(contextID,
//                              tokenName, unitDP, contractAddress,
//                              token_to, token_value, &abi);
//    if (JUBR_OK != rv) {
//        [self addMsgData:[NSString stringWithFormat:@"[JUB_BuildERC20AbiETH() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
//        return rv;
//    }
//    [self addMsgData:[NSString stringWithFormat:@"[JUB_BuildERC20AbiETH() OK.]"]];
//
//    if (abi) {
//        size_t abiLen = strlen(abi)/2;
//        [self addMsgData:[NSString stringWithFormat:@"erc20 raw[%lu]: %s.", abiLen, abi]];
//    }
//
//    BIP44_Path path;
//    path.change = (JUB_ENUM_BOOL)root["ERC20"]["bip32_path"]["change"].asBool();
//    path.addressIndex = root["ERC20"]["bip32_path"]["addressIndex"].asUInt();
//    uint32_t nonce = root["ERC20"]["nonce"].asUInt();//.asDouble();
//    uint32_t gasLimit = root["ERC20"]["gasLimit"].asUInt();//.asDouble();
//    char* gasPriceInWei = (char*)root["ERC20"]["gasPriceInWei"].asCString();
//    char* valueInWei = nullptr; //"" and "0" ara also OK
//    char* raw = nullptr;
//    rv = JUB_SignTransactionETH(contextID,
//                                path,
//                                nonce, gasLimit, gasPriceInWei,
//                                to, valueInWei, abi,
//                                &raw);
//    if (JUBR_OK != rv) {
//        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
//        return rv;
//    }
//    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
//    if (JUBR_OK != rv) {
//        [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionETH() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
//        return rv;
//    }
//    [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionETH() OK.]"]];
//
//    rv = JUB_FreeMemory(abi);
//    if (raw) {
//        size_t txLen = strlen(raw)/2;
//        [self addMsgData:[NSString stringWithFormat:@"tx raw[%lu]: %s.", txLen, raw]];
//
//        rv = JUB_FreeMemory(raw);
//        if (JUBR_OK != rv) {
//            [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
//            return rv;
//        }
//        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
//    }
//
//    return rv;
//}


@end
