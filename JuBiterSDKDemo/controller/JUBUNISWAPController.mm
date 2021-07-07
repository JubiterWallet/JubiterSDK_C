//
//  JUBETHController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/4/28.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBSharedData.h"

#import "JUBUNISWAPController.h"
#import "JUBETHAmount.h"

@interface JUBUNISWAPController ()
{
    NSArray * transferTypeArr;
}

@end


@implementation JUBUNISWAPController


- (void) viewDidLoad {
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.title = @"ETH-Uniswap options";
    
    self.optItem = JUB_NS_ENUM_MAIN::OPT_ETH_UNISWAP;
    transferTypeArr = @[@"approve",@"swapExactTokensForETH",@"swapExactETHForTokens",@"swapExactTokensForTokens",@"withdraw",@"deposit",@"addLiquidityETH",@"addLiquidity",@"removeLiquidity",@"removeLiquidityETHSupportingFeeOnTransferTokens"];
}


- (NSArray*) subMenu {
    return @[
        BUTTON_TITLE_ETH_APPROVE,
        BUTTON_TITLE_ETH_ETHSWAPTOKENS,
        BUTTON_TITLE_ETH_TOKENSSWAPETH,
        BUTTON_TITLE_ETH_TOKENSSWAPTOKENS,
        BUTTON_TITLE_ETH_WITHDRAW,
        BUTTON_TITLE_ETH_DEPOSIT,
        BUTTON_TITLE_ETC_ADDLIQUIDITYETH,
        BUTTON_TITLE_ETC_ADDLIQUIDITY,
        BUTTON_TITLE_ETH_REMOVELIQUIDITY,
        BUTTON_TITLE_ETH_REMOVELIQUIDITYTOKENS
    ];
}


#pragma mark - 通讯库寻卡回调
- (void) CoinETHUNISWAPOpt:(NSUInteger)deviceID {
    
    const char* json_file = "";
    json_file = JSON_FILE_UNISWAP;

    NSString *filePath = [[NSBundle mainBundle] pathForResource:[NSString stringWithFormat:@"%s", json_file]
                                                         ofType:@"json"];
    Json::Value root = readJSON([filePath UTF8String]);
    
    [self ETH_UNISWAP_test:deviceID
              root:root
            choice:(int)self.optIndex];
}

- (void) ETH_UNISWAP_test:(NSUInteger)deviceID
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


- (NSUInteger) tx_proc:(NSUInteger)contextID
                amount:(NSString*)amount
                  root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
    switch(self.selectedMenuIndex) {
    default:
            rv = [self transactionUNISWAP_proc:contextID amount:amount root:root];

        break;
    }   // switch(self.selectedMenuIndex) end
    
    return rv;
}


//ETH-UNISWAP Test
- (NSUInteger) transactionUNISWAP_proc:(NSUInteger)contextID
                              amount:(NSString*)amount
                                root:(Json::Value)root {
    JUB_RV rv = JUBR_ERROR;
    JUB_CHAR_PTR uniswip = (JUB_CHAR_PTR)((NSString *)transferTypeArr[self.selectedMenuIndex]).UTF8String;

    JUB_UINT16 count = root[uniswip]["tokens"].size();
    ERC20_TOKEN_INFO* tokens = new ERC20_TOKEN_INFO[count];
    memset(tokens, 0, count);
    
    for (JUB_UINT16 i=0; i<count; ++i) {
        tokens[i].contractAddress = (JUB_CHAR_PTR)root[uniswip]["tokens"][i]["contract_address"].asCString();
        tokens[i].unitDP          = root[uniswip]["tokens"][i]["dp"].asUInt();
        tokens[i].tokenName       = (JUB_CHAR_PTR)root[uniswip]["tokens"][i]["tokenName"].asCString();
    }
    
    if (1 == count) {
        rv = JUB_SetERC20TokenETH(contextID,
                                  tokens[0].tokenName, tokens[0].unitDP, tokens[0].contractAddress);
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_SetERC20TokenETH() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
            return rv;
        }
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SetERC20TokenETH() OK.]"]];

    }
    else {
        rv = JUB_SetERC20TokensETH(contextID,
                                   tokens, count);
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_SetERC20TokensETH() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
            return rv;
        }
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SetERC20TokensETH() OK.]"]];
    }
    if (JUBR_OK != rv) {
        delete [] tokens; tokens = nullptr;
        return rv;
    }
    delete [] tokens; tokens = nullptr;

    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["bip32_path"]["change"].asBool();
    path.addressIndex = root["bip32_path"]["addressIndex"].asUInt();

    uint32_t nonce    = root[uniswip]["nonce"].asUInt();//.asDouble();
    uint32_t gasLimit = root[uniswip]["gasLimit"].asUInt();//.asDouble();
    JUB_CHAR_PTR gasPriceInWei = (JUB_CHAR_PTR)root[uniswip]["gasPriceInWei"].asCString();
    JUB_CHAR_PTR valueInWei = (JUB_CHAR_PTR)root[uniswip]["valueInWei"].asCString();; //"" and "0" ara also OK
    JUB_CHAR_PTR to   = (JUB_CHAR_PTR)root[uniswip]["to"].asCString();
    JUB_CHAR_PTR abi  = (JUB_CHAR_PTR)root[uniswip]["data"].asCString();

    JUB_CHAR_PTR raw = nullptr;
    rv = JUB_SignTransactionETH(contextID, path,
                                nonce, gasLimit, gasPriceInWei,
                                to, valueInWei, abi,
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



@end
