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
@property (nonatomic,assign)JUB_ENUM_TRX_CONTRACT_TYPE transferType;

@end


@implementation JUBTRXController


- (void) viewDidLoad {
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.title = @"TRX options";
    self.buttonArray[3].disEnable = YES;
    self.transferType = JUB_ENUM_TRX_CONTRACT_TYPE::NS_ITEM_TRX_CONTRACT;
    self.optItem = JUB_NS_ENUM_MAIN::OPT_TRX;
}


- (NSArray*) subMenu {
    return @[
        BUTTON_TITLE_TRX,
        BUTTON_TITLE_TRC10,
        BUTTON_TITLE_TRCFree,
        BUTTON_TITLE_TRCUnfreeze,
        BUTTON_TITLE_TRC20,
        BUTTON_TITLE_TRC20_TRANSFER,
        BUTTON_TITLE_TRC721,
    ];
}
- (JUB_ENUM_TRX_CONTRACT_TYPE)TransferType
{
    switch(self.selectedMenuIndex) {
        case 0:
            self.transferType = JUB_ENUM_TRX_CONTRACT_TYPE::XFER_CONTRACT;
            break;
        case 1:
            self.transferType = JUB_ENUM_TRX_CONTRACT_TYPE::XFER_ASSET_CONTRACT;
            break;
        case 2:
            self.transferType = JUB_ENUM_TRX_CONTRACT_TYPE::FRZ_BLA_CONTRACT;
            break;
        case 3:
            self.transferType = JUB_ENUM_TRX_CONTRACT_TYPE::UNFRZ_BLA_CONTRACT;
            break;
        case 4:
            self.transferType = JUB_ENUM_TRX_CONTRACT_TYPE::TRIG_SMART_CONTRACT;
            break;
        case 5:
            self.transferType = (JUB_ENUM_TRX_CONTRACT_TYPE)32;
            break;
        case 6:
            self.transferType = (JUB_ENUM_TRX_CONTRACT_TYPE)33;
            break;
    default:
        break;
    }
    return self.transferType;
}


#pragma mark - 通讯库寻卡回调
- (void) CoinTRXOpt:(NSUInteger)deviceID {
    
    const char* json_file = JSON_FILE_TRX;
    
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
    
    rv = JUB_GetMainHDNodeTRX(contextID, JUB_ENUM_PUB_FORMAT::XPUB, &pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeTRX() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetMainHDNodeTRX() OK.]"]];
    
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
    
    rv = JUB_GetHDNodeTRX(contextID, JUB_ENUM_PUB_FORMAT::XPUB, path, &pubkey);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeTRX() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetHDNodeTRX() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"pubkey(%@/%d/%llu) in xpub format: %s.", [sharedData currMainPath], path.change, path.addressIndex, pubkey]];
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


- (NSString *)inputResource
{
    __block NSString * resourceStr = @"";
    __block BOOL isDone = NO;
    JUBCustomInputAlert *  resourceAlert =[JUBCustomInputAlert showCallBack:^(NSString * _Nullable content, JUBDissAlertCallBack  _Nonnull dissAlertCallBack, JUBSetErrorCallBack  _Nonnull setErrorCallBack) {
        resourceStr = content;
        dissAlertCallBack();
        isDone = YES;
    } keyboardType:UIKeyboardTypeNumberPad];
    resourceAlert.title = [JUBTRXAmount title:(JUB_NS_ENUM_TRX_OPT)self.selectedMenuIndex];
    resourceAlert.message = [JUBTRXAmount message];
    resourceAlert.textFieldPlaceholder = [JUBTRXAmount formatResourceRules];
    resourceAlert.limitLength = 1;
    
    while (!isDone) {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
                                 beforeDate:[NSDate distantFuture]];
    }

    
    return resourceStr;
}

- (NSString *)inputDuration
{
    __block NSString * durationStr = @"";
    __block BOOL isDone = NO;

    JUBCustomInputAlert * durationAlert = [JUBCustomInputAlert showCallBack:^(NSString * _Nullable content, JUBDissAlertCallBack  _Nonnull dissAlertCallBack, JUBSetErrorCallBack  _Nonnull setErrorCallBack) {
        durationStr = content;
        dissAlertCallBack();
        isDone = YES;
    } keyboardType:UIKeyboardTypeNumberPad];
    durationAlert.title = [JUBTRXAmount title:(JUB_NS_ENUM_TRX_OPT)self.selectedMenuIndex];
    durationAlert.message = [JUBTRXAmount message];
    durationAlert.textFieldPlaceholder = [JUBTRXAmount formatDurationRules];
    durationAlert.limitLength = 2;
    
    
    while (!isDone) {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
                                 beforeDate:[NSDate distantFuture]];
    }
    return durationStr;
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
    
    JUB_CHAR_PTR strOwnerAddress = (JUB_CHAR_PTR)root["TRX"]["contracts"]["owner_address"].asCString();
    
    JUB_CONTRACT_TRX contrTRX;
    contrTRX.type = (JUB_ENUM_TRX_CONTRACT_TYPE)[self TransferType];
    
    if (JUB_ENUM_TRX_CONTRACT_TYPE::XFER_ASSET_CONTRACT == contrTRX.type) {
        string assetName = (char*)root["TRX"]["TRC10"]["assetName"].asCString();
        string assetID = (char*)root["TRX"]["TRC10"]["assetID"].asCString();
        JUB_UINT16 unitDP = root["TRX"]["TRC10"]["dp"].asUInt64();
        rv = JUB_SetTRC10Asset(contextID,
                               assetName.c_str(),
                               unitDP,
                               assetID.c_str());
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_setTRC10Asset() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
            return rv;
        }
    }
    
    int trc = 20;
    if ((JUB_ENUM_TRX_CONTRACT_TYPE)32 == contrTRX.type) {
        contrTRX.type = JUB_ENUM_TRX_CONTRACT_TYPE::TRIG_SMART_CONTRACT;
    }
    else if ((JUB_ENUM_TRX_CONTRACT_TYPE)33 == contrTRX.type) {
        contrTRX.type = JUB_ENUM_TRX_CONTRACT_TYPE::TRIG_SMART_CONTRACT;
        trc = 721;
    }
    else {
        trc = 0;
    }
    
    std::string strType = std::to_string((unsigned int)contrTRX.type);
    char* sType = new char[strType.length()+1];
    memset(sType, 0x00, strType.length()+1);
    std::copy(strType.begin(), strType.end(), sType);
    
    JUB_CHAR_PTR trcAbi = nullptr;
    std::string contractAddress = "";
    switch (trc) {
    case 20:
    {
        contractAddress   = (JUB_CHAR_PTR)root["TRX"]["TRC20"]["contract_address"].asCString();
        string tokenName  = (JUB_CHAR_PTR)root["TRX"]["TRC20"]["tokenName"].asCString();
        JUB_UINT16 unitDP = root["TRX"]["TRC20"]["dp"].asUInt64();
        string tokenTo    = (JUB_CHAR_PTR)root["TRX"]["TRC20"]["token_to"].asCString();
        string tokenValue = (JUB_CHAR_PTR)root["TRX"]["TRC20"]["token_value"].asCString();
        
        rv = JUB_BuildTRC20Abi(contextID,
                               tokenName.c_str(),
                               unitDP,
                               contractAddress.c_str(),
                               tokenTo.c_str(), tokenValue.c_str(),
                               &trcAbi);
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_BuildTRC20Abi() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
            return rv;
        }
        [self addMsgData:[NSString stringWithFormat:@"[JUB_BuildTRC20Abi() OK.]"]];
        if (trcAbi) {
            [self addMsgData:[NSString stringWithFormat:@"TRC-20  Abi: %s.", trcAbi]];
        }
        break;
    }
    case 721:
    {
        contractAddress   = (JUB_CHAR_PTR)root["TRX"]["TRC721"]["contract_address"].asCString();
        string tokenName  = (JUB_CHAR_PTR)root["TRX"]["TRC721"]["tokenName"].asCString();
        string tokenFrom  = (JUB_CHAR_PTR)root["TRX"]["TRC721"]["token_from"].asCString();
        string tokenTo    = (JUB_CHAR_PTR)root["TRX"]["TRC721"]["token_to"].asCString();
        string tokenID    = (JUB_CHAR_PTR)root["TRX"]["TRC721"]["tokenID"].asCString();
        
        rv = JUB_BuildTRC721Abi(contextID,
                               tokenName.c_str(),
                               contractAddress.c_str(),
                               tokenFrom.c_str(), tokenTo.c_str(), tokenID.c_str(),
                               &trcAbi);
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_BuildTRC721Abi() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
            return rv;
        }
        [self addMsgData:[NSString stringWithFormat:@"[JUB_BuildTRC721Abi() OK.]"]];
        if (trcAbi) {
            [self addMsgData:[NSString stringWithFormat:@"TRC-721 Abi: %s.", trcAbi]];
        }
        break;
    }
    default:
        break;
    }
    
    JUB_TX_TRX tx;
    tx.ref_block_bytes = (char*)root["TRX"]["pack"]["ref_block_bytes"].asCString();
    tx.ref_block_num = 0;
    tx.ref_block_hash = (char*)root["TRX"]["pack"]["ref_block_hash"].asCString();
    tx.data = nullptr;
    tx.expiration = (char*)root["TRX"]["pack"]["expiration"].asCString();
    tx.timestamp = (char*)root["TRX"]["pack"]["timestamp"].asCString();
    tx.fee_limit = (char*)"0";
    
    switch ((JUB_ENUM_TRX_CONTRACT_TYPE)contrTRX.type) {
    case JUB_ENUM_TRX_CONTRACT_TYPE::XFER_CONTRACT:
    {
        contrTRX.transfer.owner_address = strOwnerAddress;
        contrTRX.transfer.to_address = (JUB_CHAR_PTR)root["TRX"]["contracts"][sType]["to_address"].asCString();
        contrTRX.transfer.amount = root["TRX"]["contracts"][sType]["amount"].asUInt64();
        if (NSComparisonResult::NSOrderedSame != [amount compare:@""]) {
            contrTRX.transfer.amount = [amount integerValue];
        }
        break;
    }
    case JUB_ENUM_TRX_CONTRACT_TYPE::XFER_ASSET_CONTRACT:
    {
        contrTRX.transferAsset.owner_address = strOwnerAddress;
        contrTRX.transferAsset.asset_name = (JUB_CHAR_PTR)root["TRX"]["contracts"][sType]["asset_name"].asCString();
        contrTRX.transferAsset.to_address = (JUB_CHAR_PTR)root["TRX"]["contracts"][sType]["to_address"].asCString();
        contrTRX.transferAsset.amount = root["TRX"]["contracts"][sType]["amount"].asUInt64();
        if (NSComparisonResult::NSOrderedSame != [amount compare:@""]) {
            contrTRX.transferAsset.amount = [amount integerValue];
        }
        break;
    }
    case JUB_ENUM_TRX_CONTRACT_TYPE::FRZ_BLA_CONTRACT:
    {
        contrTRX.freezeBalance.owner_address = strOwnerAddress;
        contrTRX.freezeBalance.frozen_balance = root["TRX"]["contracts"][sType]["frozen_balance"].asUInt64();
        if (NSComparisonResult::NSOrderedSame != [amount compare:@""]) {
            contrTRX.freezeBalance.frozen_balance = amount.intValue;
        }
        contrTRX.freezeBalance.frozen_duration = root["TRX"]["contracts"][sType]["frozen_duration"].asUInt64();
        NSString * fDuration = [self inputDuration];
        if (fDuration.length) {
            contrTRX.freezeBalance.frozen_duration = fDuration.intValue;
        }
        
        contrTRX.freezeBalance.resource = (JUB_ENUM_RESOURCE_CODE)root["TRX"]["contracts"][sType]["resource"].asUInt64();
        NSString * res = [self inputResource];
        if (res.length && ([res isEqualToString:@"0"] || [res isEqualToString:@"1"])) {
            contrTRX.freezeBalance.resource = (JUB_ENUM_RESOURCE_CODE)res.intValue;
        }
        
        contrTRX.freezeBalance.receiver_address = (char*)root["TRX"]["contracts"][sType]["receiver_address"].asCString();
        break;
    }
    case JUB_ENUM_TRX_CONTRACT_TYPE::UNFRZ_BLA_CONTRACT:
    {
        contrTRX.unfreezeBalance.owner_address = strOwnerAddress;
        contrTRX.unfreezeBalance.resource = (JUB_ENUM_RESOURCE_CODE)root["TRX"]["contracts"][sType]["resource"].asUInt64();
        NSString * res = [self inputResource];
        if (res.length && ([res isEqualToString:@"0"] || [res isEqualToString:@"1"])) {
            contrTRX.unfreezeBalance.resource = (JUB_ENUM_RESOURCE_CODE)res.intValue;
        }
        contrTRX.unfreezeBalance.receiver_address = (char*)root["TRX"]["contracts"][sType]["receiver_address"].asCString();
        break;
    }
    case JUB_ENUM_TRX_CONTRACT_TYPE::TRIG_SMART_CONTRACT:
    {
        tx.fee_limit = (char*)root["TRX"]["contracts"][sType]["fee_limit"].asCString();
        contrTRX.triggerSmart.owner_address = strOwnerAddress;
        switch (trc) {
        case 20:
        case 721:
            contrTRX.triggerSmart.contract_address = (char*)contractAddress.c_str();
            contrTRX.triggerSmart.data = trcAbi;
            break;
        default:
            contrTRX.triggerSmart.contract_address = (char*)root["TRX"]["contracts"][sType]["contract_address"].asCString();
            contrTRX.triggerSmart.data = (char*)root["TRX"]["contracts"][sType]["data"].asCString();
            break;
        }
        contrTRX.triggerSmart.call_value = root["TRX"]["contracts"][sType]["call_value"].asUInt64();
        contrTRX.triggerSmart.call_token_value = root["TRX"]["contracts"][sType]["call_token_value"].asUInt64();
        if (NSComparisonResult::NSOrderedSame != [amount compare:@""]) {
            contrTRX.triggerSmart.call_token_value = [amount integerValue];
        }
        contrTRX.triggerSmart.token_id = root["TRX"]["contracts"][sType]["token_id"].asUInt64();
        break;
    }
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
