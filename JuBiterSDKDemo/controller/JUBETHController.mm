//
//  JUBETHController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/4/28.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBSharedData.h"

#import "JUBETHController.h"
#import "JUBETHAmount.h"

@interface JUBETHController ()

@end


@implementation JUBETHController


- (void) viewDidLoad {
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.title = @"ETH options";
    
    self.optItem = JUB_NS_ENUM_MAIN::OPT_ETH;
}


- (NSArray*) subMenu {
    
    return @[
        BUTTON_TITLE_ETH,
        BUTTON_TITLE_ETH_ERC20,
        BUTTON_TITLE_ETH_ERC721,
        BUTTON_TITLE_ETH_UNISWAP,
        BUTTON_TITLE_ETH_BYTESTR,
//        BUTTON_TITLE_ETC
    ];
}


#pragma mark - 通讯库寻卡回调
- (void) CoinETHOpt:(NSUInteger)deviceID {
    
    const char* json_file = "";
    switch (self.selectedMenuIndex) {
    case JUB_NS_ENUM_ETH_COIN::BTN_ETH:
    case JUB_NS_ENUM_ETH_COIN::BTN_ETH_ERC20:
    case JUB_NS_ENUM_ETH_COIN::BTN_ETH_ERC721:
    case JUB_NS_ENUM_ETH_COIN::BTN_ETH_UNISWAP:
    case JUB_NS_ENUM_ETH_COIN::BTN_ETH_BYTESTR:
    {
        json_file = JSON_FILE_ETH;
        break;
    }
    case JUB_NS_ENUM_ETH_COIN::BTN_ETC:
    {
        json_file = JSON_FILE_ETC;
        break;
    }
    default:
        break;
    }   // switch (self.selectedMenuIndex) end
    
    NSString *filePath = [[NSBundle mainBundle] pathForResource:[NSString stringWithFormat:@"%s", json_file]
                                                         ofType:@"json"];
    Json::Value root = readJSON([filePath UTF8String]);
    
    [self ETH_test:deviceID
              root:root
            choice:(int)self.optIndex];
}


#pragma mark - ETH applet
- (void) EnterAmount {
    
    __block
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    
    switch(self.selectedMenuIndex) {
    case JUB_NS_ENUM_ETH_COIN::BTN_ETH_BYTESTR:
        [sharedData setAmount:@""];
        break;
    default:
        [super EnterAmount];
        break;
    }
}


- (void) ETH_test:(NSUInteger)deviceID
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
//            [sharedData setCurrCoinType:-1];
            rv = JUB_ClearContext(contextID);
            if (JUBR_OK != rv) {
                [self addMsgData:[NSString stringWithFormat:@"[JUB_ClearContext() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
            }
            else {
                [self addMsgData:[NSString stringWithFormat:@"[JUB_ClearContext() OK.]"]];
            }
            [sharedData setCurrContextID:0];
        }
        
        CONTEXT_CONFIG_ETH cfg;
        cfg.mainPath = (char*)root["main_path"].asCString();
        cfg.chainID = root["chainID"].asInt();
        rv = JUB_CreateContextETH(cfg, deviceID, &contextID);
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextETH() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
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
    rv = JUB_GetMainHDNodeETH(contextID, JUB_ENUM_PUB_FORMAT::HEX, &pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeETH() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeETH() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"MainXpub(%@) in hex format: %s.", [sharedData currMainPath], pubkey]];
    rv = JUB_FreeMemory(pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
    
    pubkey = nullptr;
    rv = JUB_GetMainHDNodeETH(contextID, JUB_ENUM_PUB_FORMAT::XPUB, &pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeETH() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeETH() OK.]"]];
    
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
    rv = JUB_GetHDNodeETH(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeETH() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeETH() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"pubkey(%@/%d/%llu) in hex format: %s.", [sharedData currMainPath], path.change, path.addressIndex, pubkey]];
    rv = JUB_FreeMemory(pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
    
    pubkey = nullptr;
    rv = JUB_GetHDNodeETH(contextID, JUB_ENUM_PUB_FORMAT::XPUB, path, &pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeETH() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeETH() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"pubkey(%@/%d/%llu) in xpub format: %s.", [sharedData currMainPath], path.change, path.addressIndex, pubkey]];
    rv = JUB_FreeMemory(pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
    
    char* address = nullptr;
    rv = JUB_GetAddressETH(contextID, path, BOOL_FALSE, &address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressETH() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressETH() OK.]"]];
    
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
    rv = JUB_GetAddressETH(contextID, path, BOOL_TRUE, &address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressETH() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressETH() OK.]"]];
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
    rv = JUB_SetMyAddressETH(contextID, path, &address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SetMyAddressETH() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_SetMyAddressETH() OK.]"]];
    
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
                 || [JUBETHAmount isValid:content]
                 ) {
            //隐藏弹框
            amount = content;
            isDone = YES;
            dissAlertCallBack();
        }
        else {
            setErrorCallBack([JUBETHAmount formatRules]);
            isDone = NO;
        }
    } keyboardType:UIKeyboardTypeDecimalPad];
    customInputAlert.title = [JUBETHAmount title:(JUB_NS_ENUM_ETH_COIN)self.selectedMenuIndex];
    customInputAlert.message = [JUBETHAmount message];
    customInputAlert.textFieldPlaceholder = [JUBETHAmount formatRules];
    customInputAlert.limitLength = [JUBETHAmount limitLength];
    
    while (!isDone) {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
                                 beforeDate:[NSDate distantFuture]];
    }
    
    // Convert to the smallest unit
    return [JUBETHAmount convertToProperFormat:amount
                                           opt:(JUB_NS_ENUM_ETH_COIN)self.selectedMenuIndex];
}


- (NSUInteger) tx_proc:(NSUInteger)contextID
                amount:(NSString*)amount
                  root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
    switch(self.selectedMenuIndex) {
    case JUB_NS_ENUM_ETH_COIN::BTN_ETH_ERC20:
        rv = [self transactionERC20_proc:contextID
                                  amount:amount
                                    root:root];
        break;
    case JUB_NS_ENUM_ETH_COIN::BTN_ETH_ERC721:
        rv = [self transactionERC721_proc:contextID
                                     root:root];
        break;
    case JUB_NS_ENUM_ETH_COIN::BTN_ETH_UNISWAP:
        rv = [self transactionUNISWAP_proc:contextID amount:amount root:root];
        break;
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
    path.change = (JUB_ENUM_BOOL)root["ETH"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["ETH"]["bip32_path"]["addressIndex"].asUInt();
    
    //ETH Test
    uint32_t nonce = root["ETH"]["nonce"].asUInt();//.asDouble();
    uint32_t gasLimit = root["ETH"]["gasLimit"].asUInt();//.asDouble();
    char* gasPriceInWei = (char*)root["ETH"]["gasPriceInWei"].asCString();
    char* valueInWei = (char*)root["ETH"]["valueInWei"].asCString();
    if (NSComparisonResult::NSOrderedSame != [amount compare:@""]) {
        valueInWei = (char*)[amount UTF8String];
    }
    char* to = (char*)root["ETH"]["to"].asCString();
    char* data = (char*)root["ETH"]["data"].asCString();
    
    char* raw = nullptr;
    rv = JUB_SignTransactionETH(contextID,
                                path,
                                nonce, gasLimit, gasPriceInWei,
                                to, valueInWei, data,
                                &raw);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionETH() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionETH() OK.]"]];
    
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


//ERC-20 Test
- (NSUInteger) transactionERC20_proc:(NSUInteger)contextID
                              amount:(NSString*)amount
                                root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
    char* tokenName = (char*)root["ERC20"]["tokenName"].asCString();
    JUB_UINT16 unitDP = root["ERC20"]["dp"].asUInt();
    char* contractAddress = (char*)root["ERC20"]["contract_address"].asCString();
    char* to = (char*)root["ERC20"]["contract_address"].asCString();
    char* token_to = (char*)root["ERC20"]["token_to"].asCString();
    char* token_value = (char*)root["ERC20"]["token_value"].asCString();
    if (NSComparisonResult::NSOrderedSame != [amount compare:@""]) {
        token_value = (char*)[amount UTF8String];
    }
    
    char* abi = nullptr;
    
    rv = JUB_SetERC20TokenETH(contextID,
                              tokenName, unitDP, contractAddress);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SetERC20TokenETH() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_SetERC20TokenETH() OK.]"]];
    
    rv = JUB_BuildERC20TransferAbiETH(contextID,
                                      token_to, token_value, &abi);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_BuildERC20TransferAbiETH() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_BuildERC20TransferAbiETH() OK.]"]];
    
    if (abi) {
        size_t abiLen = strlen(abi)/2;
        [self addMsgData:[NSString stringWithFormat:@"ERC-20 abi[%lu]: %s.", abiLen, abi]];
    }
    
    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["ERC20"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["ERC20"]["bip32_path"]["addressIndex"].asUInt();
    uint32_t nonce = root["ERC20"]["nonce"].asUInt();//.asDouble();
    uint32_t gasLimit = root["ERC20"]["gasLimit"].asUInt();//.asDouble();
    char* gasPriceInWei = (char*)root["ERC20"]["gasPriceInWei"].asCString();
    char* valueInWei = nullptr; //"" and "0" ara also OK
    char* raw = nullptr;
    rv = JUB_SignTransactionETH(contextID,
                                path,
                                nonce, gasLimit, gasPriceInWei,
                                to, valueInWei, abi,
                                &raw);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionETH() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionETH() OK.]"]];
    
    rv = JUB_FreeMemory(abi);
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


//ERC-721 Test
- (NSUInteger) transactionERC721_proc:(NSUInteger)contextID
                                 root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
//    rv = JUB_GetAddressETH(contextID, path, JUB_ENUM_BOOL::BOOL_FALSE, &token_from);
//    cout << "[-] JUB_GetAddressETH() return " << GetErrMsg(rv) << endl;
//    if (JUBR_OK != rv) {
//        return rv;
//    }
    
    JUB_CHAR_PTR tokenName = (JUB_CHAR_PTR)root["ERC721"]["tokenName"].asCString();
    JUB_CHAR_PTR contractAddress = (JUB_CHAR_PTR)root["ERC721"]["contract_address"].asCString();
    JUB_CHAR_PTR token_from = (JUB_CHAR_PTR)root["ERC721"]["token_from"].asCString();
    JUB_CHAR_PTR to = (JUB_CHAR_PTR)root["ERC721"]["contract_address"].asCString();
    JUB_CHAR_PTR token_to = (JUB_CHAR_PTR)root["ERC721"]["token_to"].asCString();
    JUB_CHAR_PTR tokenID = (JUB_CHAR_PTR)root["ERC721"]["tokenID"].asCString();
    
    JUB_CHAR_PTR abi = nullptr;
    
    rv = JUB_SetERC721TokenETH(contextID,
                               tokenName, contractAddress);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SetERC721TokenETH() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_SetERC721TokenETH() OK.]"]];


    rv = JUB_BuildERC721TransferAbiETH(contextID,
                                       token_from, token_to, tokenID,
                                       &abi);
    
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_BuildERC721TransferAbiETH() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_BuildERC721TransferAbiETH() OK.]"]];
    
    if (abi) {
        size_t abiLen = strlen(abi)/2;
        [self addMsgData:[NSString stringWithFormat:@"ERC-721 abi[%lu]: %s.", abiLen, abi]];
    }
    
    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["ERC721"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["ERC721"]["bip32_path"]["addressIndex"].asUInt();
    
    uint32_t nonce = root["ERC721"]["nonce"].asUInt();//.asDouble();
    uint32_t gasLimit = root["ERC721"]["gasLimit"].asUInt();//.asDouble();
    JUB_CHAR_PTR gasPriceInWei = (JUB_CHAR_PTR)root["ERC721"]["gasPriceInWei"].asCString();
    JUB_CHAR_PTR valueInWei = nullptr; //"" and "0" ara also OK
    JUB_CHAR_PTR raw = nullptr;
    rv = JUB_SignTransactionETH(contextID, path, nonce, gasLimit, gasPriceInWei, to, valueInWei, abi, &raw);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionETH() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionETH() OK.]"]];
    JUB_FreeMemory(abi);
    
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


//ETH-UNISWAP Test
- (NSUInteger) transactionUNISWAP_proc:(NSUInteger)contextID
                              amount:(NSString*)amount
                                root:(Json::Value)root {
    JUB_RV rv = JUBR_ERROR;

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["contract"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["contract"]["bip32_path"]["addressIndex"].asUInt();
    uint32_t nonce = root["contract"]["nonce"].asUInt();//.asDouble();
    uint32_t gasLimit = root["contract"]["gasLimit"].asUInt();//.asDouble();
    char* gasPriceInWei = (char*)root["contract"]["gasPriceInWei"].asCString();
    char* valueInWei = nullptr; //"" and "0" ara also OK
    if (NSComparisonResult::NSOrderedSame != [amount compare:@""]) {
        valueInWei = (char*)[amount UTF8String];
    }
    char* to = (char*)root["contract"]["to"].asCString();
    char* abi = (char*)root["contract"]["data"].asCString();
    
    char* raw = nullptr;
    rv = JUB_SignContractETH(contextID, path, nonce, gasLimit, gasPriceInWei, to, valueInWei, abi, &raw);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SignContractETH() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_SignContractETH() OK.]"]];
    
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


- (NSUInteger) message_proc:(NSUInteger)contextID
                       root:(Json::Value)root {
    
    return [self bytestring_proc:contextID
                            root:root];
}


- (NSUInteger) bytestring_proc:(NSUInteger)contextID
                          root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["Bytestring"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["Bytestring"]["bip32_path"]["addressIndex"].asUInt();
    
    //ETH Test
    char* data = (char*)root["Bytestring"]["data"].asCString();
    
    char* raw = nullptr;
    rv = JUB_SignBytestringETH(contextID,
                               path,
                               data,
                               &raw);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SignBytestringETH() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_SignBytestringETH() OK.]"]];
    
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


@end
