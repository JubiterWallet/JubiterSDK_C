//
//  JUBETHController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/4/28.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBETHController.h"


@interface JUBETHController ()

@end


@implementation JUBETHController


- (void)viewDidLoad {
    
    self.selfClass = self;
    self.optItem = JUB_NS_ENUM_MAIN::OPT_ETH;
    
    self.coinTypeArray = @[BUTTON_TITLE_ETH,
                           BUTTON_TITLE_ETH_ERC20,
//                           BUTTON_TITLE_ETC
    ];
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}


#pragma mark - 通讯库寻卡回调
- (void)CoinETHOpt:(JUB_UINT16)deviceID {
    
    const char* json_file = "";
    switch (self.optCoinType) {
    case JUB_NS_ENUM_ETH_COINTYPE::COIN_ETH:
        json_file = JSON_FILE_ETH;
        break;
    case JUB_NS_ENUM_ETH_COINTYPE::COIN_ETH_ERC20:
        json_file = JSON_FILE_ETH;
        break;
    case JUB_NS_ENUM_ETH_COINTYPE::COIN_ECH:
        json_file = JSON_FILE_ECH;
        break;
    default:
        break;
    }
    
    NSString *filePath = [[NSBundle mainBundle] pathForResource:[NSString stringWithFormat:@"%s", json_file]
                                                         ofType:@"json"];
    Json::Value root = readJSON([filePath UTF8String]);
    
    [self ETH_test:deviceID
              root:root
            choice:(int)self.optIndex];
}


#pragma mark - ETH applet
- (void)ETH_test:(JUB_UINT16)deviceID
            root:(Json::Value)root
          choice:(int)choice {
    
    JUB_RV rv = JUBR_ERROR;
    
    try {
        JUB_UINT16 contextID = 0;
        
        CONTEXT_CONFIG_ETH cfg;
        cfg.mainPath = (char*)root["main_path"].asCString();
        cfg.chainID = root["chainID"].asInt();
        rv = JUB_CreateContextETH(cfg, deviceID, &contextID);
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextETH() return 0x%2lx.]", rv]];
            return;
        }
        [self addMsgData:[NSString stringWithFormat:@"[JUB_CreateContextETH() OK.]"]];
        
        switch (choice) {
        case JUB_NS_ENUM_OPT::GET_ADDRESS:
            [self get_address_pubkey_ETH:contextID];
            break;
        case JUB_NS_ENUM_OPT::SHOW_ADDRESS:
            break;
        case JUB_NS_ENUM_OPT::TRANSACTION:
        {
            switch(self.optCoinType) {
            case JUB_NS_ENUM_ETH_COINTYPE::COIN_ETH_ERC20:
                [self transaction_test_ERC20_ETH:contextID
                                            root:root];
                break;
            default:
                [self transaction_test_ETH:contextID
                                      root:root];
                break;
            }
            break;
        }
        case JUB_NS_ENUM_OPT::SET_MY_ADDRESS:
//            [self set_my_address_test_ETH:contextID];
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


- (void)get_address_pubkey_ETH:(JUB_UINT16)contextID {
    
    char* pubkey = nullptr;
    JUB_RV rv = JUB_GetMainHDNodeETH(contextID, JUB_ENUM_PUB_FORMAT::HEX, &pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeETH() return 0x%2lx.]", rv]];
        return;
    }
    
    [self addMsgData:[NSString stringWithFormat:@"MainXpub in hex format: %s.", pubkey]];
    JUB_FreeMemory(pubkey);
    
    pubkey = nullptr;
    rv = JUB_GetMainHDNodeETH(contextID, JUB_ENUM_PUB_FORMAT::XPUB, &pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeETH() return 0x%2lx.]", rv]];
        return;
    }
    
    [self addMsgData:[NSString stringWithFormat:@"MainXpub in xpub format: %s.", pubkey]];
    JUB_FreeMemory(pubkey);
    
    BIP44_Path path;
    path.change = JUB_ENUM_BOOL(self.change);
    path.addressIndex = (JUB_UINT64)self.addressIndex;
    
    pubkey = nullptr;
    rv = JUB_GetHDNodeETH(contextID, JUB_ENUM_PUB_FORMAT::HEX, path, &pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeETH() return 0x%2lx.]", rv]];
        return;
    }
    
    [self addMsgData:[NSString stringWithFormat:@"pubkey(%d/%llu) in hex format: %s.", path.change, path.addressIndex, pubkey]];
    JUB_FreeMemory(pubkey);
    
    pubkey = nullptr;
    rv = JUB_GetHDNodeETH(contextID, JUB_ENUM_PUB_FORMAT::XPUB, path, &pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeETH() return 0x%2lx.]", rv]];
        return;
    }
    
    [self addMsgData:[NSString stringWithFormat:@"pubkey(%d/%llu) in xpub format: %s.", path.change, path.addressIndex, pubkey]];
    JUB_FreeMemory(pubkey);
    
    char* address = nullptr;
    rv = JUB_GetAddressETH(contextID, path, BOOL_TRUE, &address);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAddressETH() return 0x%2lx.]", rv]];
        return;
    }
    
    [self addMsgData:[NSString stringWithFormat:@"address(%d/%llu): %s.", path.change, path.addressIndex, address]];
    JUB_FreeMemory(address);
}


- (void)transaction_test_ETH:(JUB_UINT16)contextID
                        root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
    rv = [self verify_pin:contextID
                      pin:[self.userPIN UTF8String]];
    if (JUBR_OK != rv) {
        return;
    }
    
    rv = [self transaction_proc_ETH:contextID
                               root:root];
    if (JUBR_OK != rv) {
        return;
    }
}


- (JUB_RV)transaction_proc_ETH:(JUB_UINT16)contextID
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
    char* to = (char*)root["ETH"]["to"].asCString();
    char* data = (char*)root["ETH"]["data"].asCString();
    
    char* raw = nullptr;
    rv = JUB_SignTransactionETH(contextID, path, nonce, gasLimit, gasPriceInWei, to, valueInWei, data, &raw);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionETH() return 0x%2lx.]", rv]];
        return rv;
    }
    
    if (raw) {
        size_t txLen = strlen(raw)/2;
        [self addMsgData:[NSString stringWithFormat:@"tx raw[%lu]: %s.", txLen, raw]];
        
        JUB_FreeMemory(raw);
    }
    
    return rv;
}

//ERC-20 Test
- (void)transaction_test_ERC20_ETH:(JUB_UINT16)contextID
                              root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
    rv = [self verify_pin:contextID
                      pin:[self.userPIN UTF8String]];
    if (JUBR_OK != rv) {
        return;
    }
    
    rv = [self transaction_proc_ERC20_ETH:contextID
                                     root:root];
    if (JUBR_OK != rv) {
        return;
    }
}


- (JUB_RV)transaction_proc_ERC20_ETH:(JUB_UINT16)contextID
                                root:(Json::Value)root {
    
    JUB_RV rv = JUBR_ERROR;
    
    char* tokenName = (char*)root["ERC20"]["tokenName"].asCString();
    JUB_UINT16 unitDP = root["ERC20"]["dp"].asUInt();
    char* contractAddress = (char*)root["ERC20"]["contract_address"].asCString();
    char* to = (char*)root["ERC20"]["contract_address"].asCString();
    char* token_to = (char*)root["ERC20"]["token_to"].asCString();
    char* token_value = (char*)root["ERC20"]["token_value"].asCString();
    
    char* abi = nullptr;
    rv = JUB_BuildERC20AbiETH(contextID,
                              tokenName, unitDP, contractAddress,
                              token_to, token_value, &abi);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_BuildERC20AbiETH() return 0x%2lx.]", rv]];
        return rv;
    }
    if (abi) {
        size_t abiLen = strlen(abi)/2;
        [self addMsgData:[NSString stringWithFormat:@"erc20 raw[%lu]: %s.", abiLen, abi]];
        
    }
    
    BIP44_Path path;
    path.change = (JUB_ENUM_BOOL)root["ERC20"]["bip32_path"]["change"].asBool();
    path.addressIndex = root["ERC20"]["bip32_path"]["addressIndex"].asUInt();
    uint32_t nonce = root["ERC20"]["nonce"].asUInt();//.asDouble();
    uint32_t gasLimit = root["ERC20"]["gasLimit"].asUInt();//.asDouble();
    char* gasPriceInWei = (char*)root["ERC20"]["gasPriceInWei"].asCString();
    char* valueInWei = nullptr; //"" and "0" ara also OK
    char* raw = nullptr;
    rv = JUB_SignTransactionETH(contextID, path, nonce, gasLimit, gasPriceInWei, to, valueInWei, abi, &raw);
    JUB_FreeMemory(abi);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_SignTransactionETH() return 0x%2lx.]", rv]];
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
