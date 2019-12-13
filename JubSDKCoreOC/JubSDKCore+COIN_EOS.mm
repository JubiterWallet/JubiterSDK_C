//
//  JubSDKCore+COIN_EOS.mm
//  JubSDKCore
//
//  Created by Pan Min on 2019/10/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#import "JubSDKCore+COIN_EOS.h"
#import "JUB_SDK_EOS.h"
#import "JUB_core.h"

//typedef struct {
//    JUB_CHAR_PTR        main_path;
//} CONTEXT_CONFIG_EOS;
@implementation ContextConfigEOS
@synthesize mainPath;
@end

//typedef struct stTransferAction {
//    JUB_CHAR_PTR from;
//    JUB_CHAR_PTR to;
//    JUB_CHAR_PTR asset;
//    JUB_CHAR_PTR memo;
//} JUB_ACTION_TRANSFER;
@implementation XferAction
@synthesize from;
@synthesize to;
@synthesize asset;
@synthesize memo;
@end

//typedef struct stDelegateAction {
//    JUB_CHAR_PTR from;
//    JUB_CHAR_PTR receiver;
//    JUB_CHAR_PTR netQty; // stake_net_quantity/unstake_net_quantity
//    JUB_CHAR_PTR cpuQty; // stake_cpu_quantity/unstake_cpu_quantity
//    bool bStake;
//} JUB_ACTION_DELEGATE;
@implementation DeleAction
@synthesize from;
@synthesize receiver;
@synthesize netQty;
@synthesize cpuQty;
@synthesize bStake;
@end

//typedef struct stBuyRamAction {
//    JUB_CHAR_PTR payer;
//    JUB_CHAR_PTR quant;
//    JUB_CHAR_PTR receiver;
//} JUB_ACTION_BUYRAM;
@implementation BuyRamAction
@synthesize payer;
@synthesize quant;
@synthesize receiver;
@end

//typedef struct stSellRamAction {
//    JUB_CHAR_PTR account;
//    JUB_CHAR_PTR bytes;
//} JUB_ACTION_SELLRAM;
@implementation SellRamAction
@synthesize account;
@synthesize bytes;
@end

//typedef struct stActionEOS {
//    JUB_ENUM_EOS_ACTION_TYPE type;
//    JUB_CHAR_PTR currency;
//    JUB_CHAR_PTR name;
//    union {
//        JUB_ACTION_TRANSFER transfer;
//        JUB_ACTION_DELEGATE delegate;
//        JUB_ACTION_BUYRAM buyRam;
//        JUB_ACTION_SELLRAM sellRam;
//    };
//} JUB_ACTION_EOS;
@implementation ActionEOS
@synthesize type;
@synthesize currency;
@synthesize name;
@synthesize xfer;
@synthesize dele;
@synthesize buyRam;
@synthesize sellRam;
@end

extern bool (^inlinebool)(JUB_NS_ENUM_BOOL);
extern JUB_NS_ENUM_BOOL (^inlineNSbool)(bool);

// JUB_NS_EOS_ACTION_TYPE -> JUB_ENUM_EOS_ACTION_TYPE
JUB_ENUM_EOS_ACTION_TYPE (^inlineEOSActionType)(JUB_NS_EOS_ACTION_TYPE) = ^(JUB_NS_EOS_ACTION_TYPE argument) {
    JUB_ENUM_EOS_ACTION_TYPE u;
    switch (argument) {
        case NS_XFER:
            u = JUB_ENUM_EOS_ACTION_TYPE::XFER;
            break;
        case NS_DELE:
            u = JUB_ENUM_EOS_ACTION_TYPE::DELE;
            break;
        case NS_UNDELE:
            u = JUB_ENUM_EOS_ACTION_TYPE::UNDELE;
            break;
        case NS_BUYRAM:
            u = JUB_ENUM_EOS_ACTION_TYPE::BUYRAM;
            break;
        case NS_SELLRAM:
            u = JUB_ENUM_EOS_ACTION_TYPE::SELLRAM;
            break;
        default:
            u = JUB_ENUM_EOS_ACTION_TYPE::NS_ITEM_ACTION_TYPE;
            break;
    }
    
    return u;
};

// JUB_NS_EOS_ACTION_TYPE <- JUB_ENUM_EOS_ACTION_TYPE
JUB_NS_EOS_ACTION_TYPE (^inlineNSEOSActionType)(JUB_ENUM_EOS_ACTION_TYPE) = ^(JUB_ENUM_EOS_ACTION_TYPE argument) {
    JUB_NS_EOS_ACTION_TYPE u;
    switch (argument) {
        case JUB_ENUM_EOS_ACTION_TYPE::XFER:
            u = NS_XFER;
            break;
        case JUB_ENUM_EOS_ACTION_TYPE::DELE:
            u = NS_DELE;
            break;
        case JUB_ENUM_EOS_ACTION_TYPE::UNDELE:
            u = NS_UNDELE;
            break;
        case JUB_ENUM_EOS_ACTION_TYPE::BUYRAM:
            u = NS_BUYRAM;
            break;
        case JUB_ENUM_EOS_ACTION_TYPE::SELLRAM:
            u = NS_SELLRAM;
            break;
        default:
            u = NS_ITEM_ACTION_TYPE_NS;
            break;
    }
    
    return u;
};

// XferAction* -> JUB_ACTION_TRANSFER
JUB_ACTION_TRANSFER (^inlineXferAction)(XferAction*) = ^(XferAction* xferAction) {
    JUB_ACTION_TRANSFER xfer;
    
    xfer.from = (JUB_CHAR_PTR)[xferAction.from UTF8String];
    xfer.to = (JUB_CHAR_PTR)[xferAction.to UTF8String];
    xfer.asset = (JUB_CHAR_PTR)[xferAction.asset UTF8String];
    xfer.memo = (JUB_CHAR_PTR)[xferAction.memo UTF8String];
    
    return xfer;
};

// XferAction* <- JUB_ACTION_TRANSFER
XferAction* (^inlineNSXferAction)(JUB_ACTION_TRANSFER&) = ^(JUB_ACTION_TRANSFER& xferAction) {
    XferAction* xfer = [[XferAction alloc] init];
    
    xfer.from = [[NSString alloc] init];
    xfer.from = [[NSString stringWithCString:xferAction.from
                                    encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(xferAction.from);
    
    xfer.to = [[NSString alloc] init];
    xfer.to = [[NSString stringWithCString:xferAction.to
                                  encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(xferAction.to);
    
    xfer.asset = [[NSString alloc] init];
    xfer.asset = [[NSString stringWithCString:xferAction.asset
                                     encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(xferAction.asset);
    
    xfer.memo = [[NSString alloc] init];
    xfer.memo = [[NSString stringWithCString:xferAction.memo
                                    encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(xferAction.memo);
    
    return xfer;
};

// DeleAction* -> JUB_ACTION_DELEGATE
JUB_ACTION_DELEGATE (^inlineDeleAction)(DeleAction*) = ^(DeleAction* deleAction) {
    JUB_ACTION_DELEGATE dele;
    
    dele.from = (JUB_CHAR_PTR)[deleAction.from UTF8String];
    dele.receiver = (JUB_CHAR_PTR)[deleAction.receiver UTF8String];
    dele.netQty = (JUB_CHAR_PTR)[deleAction.netQty UTF8String];
    dele.cpuQty = (JUB_CHAR_PTR)[deleAction.cpuQty UTF8String];
    
    dele.bStake = inlinebool(deleAction.bStake);
    
    return dele;
};

// DeleAction* <- JUB_ACTION_DELEGATE
DeleAction* (^inlineNSDeleAction)(JUB_ACTION_DELEGATE&) = ^(JUB_ACTION_DELEGATE& deleAction) {
    DeleAction* dele = [[DeleAction alloc] init];
    
    dele.from = [[NSString alloc] init];
    dele.from = [[NSString stringWithCString:deleAction.from
                                    encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(deleAction.from);
    
    dele.receiver = [[NSString alloc] init];
    dele.receiver = [[NSString stringWithCString:deleAction.receiver
                                  encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(deleAction.receiver);
    
    dele.netQty = [[NSString alloc] init];
    dele.netQty = [[NSString stringWithCString:deleAction.netQty
                                      encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(deleAction.netQty);
    
    dele.cpuQty = [[NSString alloc] init];
    dele.cpuQty = [[NSString stringWithCString:deleAction.cpuQty
                                      encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(deleAction.cpuQty);
    
    dele.bStake = inlineNSbool(deleAction.bStake);
    
    return dele;
};

// BuyRamAction* -> JUB_ACTION_BUYRAM
JUB_ACTION_BUYRAM (^inlineBuyRamAction)(BuyRamAction*) = ^(BuyRamAction* buyRamAction) {
    JUB_ACTION_BUYRAM buyRam;
    
    buyRam.payer = (JUB_CHAR_PTR)[buyRamAction.payer UTF8String];
    buyRam.quant = (JUB_CHAR_PTR)[buyRamAction.quant UTF8String];
    buyRam.receiver = (JUB_CHAR_PTR)[buyRamAction.receiver UTF8String];
    
    return buyRam;
};

// BuyRamAction* <- JUB_ACTION_BUYRAM
BuyRamAction* (^inlineNSBuyRamAction)(JUB_ACTION_BUYRAM&) = ^(JUB_ACTION_BUYRAM& buyRamAction) {
    BuyRamAction* buyRam = [[BuyRamAction alloc] init];
    
    buyRam.payer = [[NSString alloc] init];
    buyRam.payer = [[NSString stringWithCString:buyRamAction.payer
                                       encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(buyRamAction.payer);
    
    buyRam.quant = [[NSString alloc] init];
    buyRam.quant = [[NSString stringWithCString:buyRamAction.quant
                                       encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(buyRamAction.quant);
    
    buyRam.receiver = [[NSString alloc] init];
    buyRam.receiver = [[NSString stringWithCString:buyRamAction.receiver
                                          encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(buyRamAction.receiver);
    
    return buyRam;
};

// SellRamAction* -> JUB_ACTION_BUYRAM
JUB_ACTION_SELLRAM (^inlineSellRamAction)(SellRamAction*) = ^(SellRamAction* sellRamAction) {
    JUB_ACTION_SELLRAM sellRam;
    
    sellRam.account = (JUB_CHAR_PTR)[sellRamAction.account UTF8String];
    sellRam.bytes   = (JUB_CHAR_PTR)[sellRamAction.bytes UTF8String];
    
    return sellRam;
};

// SellRamAction* <- JUB_ACTION_BUYRAM
SellRamAction* (^inlineNSSellRamAction)(JUB_ACTION_SELLRAM&) = ^(JUB_ACTION_SELLRAM& sellRamAction) {
    SellRamAction* sellRam = [[SellRamAction alloc] init];
    
    sellRam.account = [[NSString alloc] init];
    sellRam.account = [[NSString stringWithCString:sellRamAction.account
                                          encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(sellRamAction.account);
    
    sellRam.bytes = [[NSString alloc] init];
    sellRam.bytes = [[NSString stringWithCString:sellRamAction.bytes
                                        encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(sellRamAction.bytes);
    
    return sellRam;
};

// ActionEOS* -> JUB_ACTION_EOS
JUB_ACTION_EOS (^inlineActionEOS)(ActionEOS*) = ^(ActionEOS* actionEOS) {
    JUB_ACTION_EOS action;
    
    action.type = inlineEOSActionType(actionEOS.type);
    action.currency = (JUB_CHAR_PTR)[actionEOS.currency UTF8String];
    action.name = (JUB_CHAR_PTR)[actionEOS.name UTF8String];
    
    switch (actionEOS.type) {
        case JUB_NS_EOS_ACTION_TYPE::NS_XFER:
//            inlineXferAction(actionEOS.xfer);
            action.transfer.from = (JUB_CHAR_PTR)[actionEOS.xfer.from UTF8String];
            action.transfer.to = (JUB_CHAR_PTR)[actionEOS.xfer.to UTF8String];
            action.transfer.asset = (JUB_CHAR_PTR)[actionEOS.xfer.asset UTF8String];
            action.transfer.memo = (JUB_CHAR_PTR)[actionEOS.xfer.memo UTF8String];
            break;
        case JUB_NS_EOS_ACTION_TYPE::NS_DELE:
        case JUB_NS_EOS_ACTION_TYPE::NS_UNDELE:
//            inlineDeleAction(actionEOS.dele);
            action.delegate.from = (JUB_CHAR_PTR)[actionEOS.dele.from UTF8String];
            action.delegate.receiver = (JUB_CHAR_PTR)[actionEOS.dele.receiver UTF8String];
            action.delegate.netQty = (JUB_CHAR_PTR)[actionEOS.dele.netQty UTF8String];
            action.delegate.cpuQty = (JUB_CHAR_PTR)[actionEOS.dele.cpuQty UTF8String];
            if (JUB_NS_EOS_ACTION_TYPE::NS_DELE == actionEOS.type) {
                action.delegate.bStake = true;
            }
            else {
                action.delegate.bStake = false;
            }
            break;
        case JUB_NS_EOS_ACTION_TYPE::NS_BUYRAM:
//            inlineBuyRamAction(actionEOS.buyRam);
            action.buyRam.payer = (JUB_CHAR_PTR)[actionEOS.buyRam.payer UTF8String];
            action.buyRam.quant = (JUB_CHAR_PTR)[actionEOS.buyRam.quant UTF8String];
            action.buyRam.receiver = (JUB_CHAR_PTR)[actionEOS.buyRam.receiver UTF8String];
            break;
        case JUB_NS_EOS_ACTION_TYPE::NS_SELLRAM:
//            inlineSellRamAction(actionEOS.sellRam);
            action.sellRam.account = (JUB_CHAR_PTR)[actionEOS.sellRam.account UTF8String];
            action.sellRam.bytes   = (JUB_CHAR_PTR)[actionEOS.sellRam.bytes UTF8String];
            break;
        case JUB_NS_EOS_ACTION_TYPE::NS_ITEM_ACTION_TYPE_NS:
        default:
            break;
    }
    
    return action;
};

// ActionEOS* <- JUB_ACTION_EOS
ActionEOS* (^inlineNSActionEOS)(JUB_ACTION_EOS) = ^(JUB_ACTION_EOS actionEOS) {
    ActionEOS* action = [[ActionEOS alloc] init];
    
    action.type = inlineNSEOSActionType(actionEOS.type);
    
    action.currency = [[NSString alloc] init];
    action.currency = [[NSString stringWithCString:actionEOS.currency
                                          encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(actionEOS.currency);
    
    action.name = [[NSString alloc] init];
    action.name = [[NSString stringWithCString:actionEOS.name
                                      encoding:NSUTF8StringEncoding] copy];
    JUB_FreeMemory(actionEOS.name);
    
    switch (actionEOS.type) {
        case JUB_ENUM_EOS_ACTION_TYPE::XFER:
            inlineNSXferAction(actionEOS.transfer);
            break;
        case JUB_ENUM_EOS_ACTION_TYPE::DELE:
        case JUB_ENUM_EOS_ACTION_TYPE::UNDELE:
            inlineNSDeleAction(actionEOS.delegate);
            break;
        case JUB_ENUM_EOS_ACTION_TYPE::BUYRAM:
            inlineNSBuyRamAction(actionEOS.buyRam);
            break;
        case JUB_ENUM_EOS_ACTION_TYPE::SELLRAM:
            inlineNSSellRamAction(actionEOS.sellRam);
            break;
        case JUB_ENUM_EOS_ACTION_TYPE::NS_ITEM_ACTION_TYPE:
        default:
            break;
    }
    
    return action;
};

extern JUB_ENUM_BOOL (^inlineBool)(JUB_NS_ENUM_BOOL);

@implementation JubSDKCore (COIN_EOS)

//JUB_RV JUB_CreateContextEOS_soft(IN CONTEXT_CONFIG_EOS cfg,
//                                 IN JUB_CHAR_PTR masterPriInXPRV,
//                                 OUT JUB_UINT16* contextID);
- (NSUInteger)JUB_CreateContextEOS_soft:(ContextConfigEOS*)cfg
                        masterPriInXPRV:(NSString*)masterPriInXPRV
{
    self.lastError = JUBR_OK;
    
    CONTEXT_CONFIG_EOS cfgEOS;
    cfgEOS.mainPath = (JUB_CHAR_PTR)[cfg.mainPath UTF8String];
    JUB_UINT16 contextID = 0;
    JUB_RV rv = JUB_CreateContextEOS_soft(cfgEOS,
                                          (JUB_CHAR_PTR)[masterPriInXPRV UTF8String],
                                          &contextID);
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return contextID;
    }
    
    return contextID;
}

//JUB_RV JUB_CreateContextEOS(IN CONTEXT_CONFIG_EOS cfg,
//                            IN JUB_UINT16 deviceID,
//                            OUT JUB_UINT16* contextID);
- (NSUInteger)JUB_CreateContextEOS:(NSUInteger)deviceID
                               cfg:(ContextConfigEOS*)cfg
{
    self.lastError = JUBR_OK;
    
    CONTEXT_CONFIG_EOS cfgEOS;
    cfgEOS.mainPath = (JUB_CHAR_PTR)[cfg.mainPath UTF8String];
    JUB_UINT16 contextID = 0;
    JUB_RV rv = JUB_CreateContextEOS(cfgEOS,
                                     deviceID,
                                     &contextID);
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return contextID;
    }
    
    return contextID;
}

//JUB_RV JUB_GetAddressEOS(IN JUB_UINT16 contextID,
//                         IN BIP44_Path path,
//                         IN JUB_ENUM_BOOL bShow,
//                         OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_GetAddressEOS:(NSUInteger)contextID
                          path:(BIP44Path*)path
                         bShow:(JUB_NS_ENUM_BOOL)bShow
{
    self.lastError = JUBR_OK;
    
    BIP44_Path bip44Path;
    bip44Path.change = inlineBool(path.change);
    bip44Path.addressIndex = path.addressIndex;
    JUB_ENUM_BOOL isShow = inlineBool(bShow);
    JUB_CHAR_PTR address;
    JUB_RV rv = JUB_GetAddressEOS(contextID,
                                  bip44Path,
                                  isShow,
                                  &address);
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return @"";
    }
    
    NSString* strAddress = [NSString stringWithCString:address
                                              encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(address);
    
    return strAddress;
}

//JUB_RV JUB_SetMyAddressEOS(IN JUB_UINT16 contextID,
//                           IN BIP44_Path path,
//                           OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_SetMyAddressEOS:(NSUInteger)contextID
                            path:(BIP44Path*)path
{
    self.lastError = JUBR_OK;
    
    BIP44_Path bip44Path;
    bip44Path.change = inlineBool(path.change);
    bip44Path.addressIndex = path.addressIndex;
    JUB_CHAR_PTR address;
    JUB_RV rv = JUB_SetMyAddressEOS(contextID,
                                    bip44Path,
                                    &address);
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return @"";
    }
    
    NSString* strAddress = [NSString stringWithCString:address
                                              encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(address);
    
    return strAddress;
}

//JUB_RV JUB_SignTransactionEOS(IN JUB_UINT16 contextID,
//                              IN BIP44_Path path,
//                              IN JUB_CHAR_PTR chainID,
//                              IN JUB_CHAR_PTR expiration,
//                              IN JUB_CHAR_PTR referenceBlockId,
//                              IN JUB_CHAR_PTR referenceBlockTime,
//                              IN JUB_CHAR_PTR actionsInJSON,
//                              OUT JUB_CHAR_PTR_PTR rawInJSON);
- (NSString*)JUB_SignTransactionEOS:(NSUInteger)contextID
                               path:(BIP44Path*)path
                            chainID:(NSString*)chainID
                         expiration:(NSString*)expiration
                   referenceBlockId:(NSString*)referenceBlockId
                 referenceBlockTime:(NSString*)referenceBlockTime
                             action:(NSString*)actionsInJSON
{
    self.lastError = JUBR_OK;
    
    BIP44_Path bip44Path;
    bip44Path.change = inlineBool(path.change);
    bip44Path.addressIndex = path.addressIndex;
    JUB_CHAR_PTR rawInJSON = nullptr;
    JUB_RV rv = JUB_SignTransactionEOS(contextID,
                                       bip44Path,
                                       (JUB_CHAR_PTR)[chainID UTF8String],
                                       (JUB_CHAR_PTR)[expiration UTF8String],
                                       (JUB_CHAR_PTR)[referenceBlockId UTF8String],
                                       (JUB_CHAR_PTR)[referenceBlockTime UTF8String],
                                       (JUB_CHAR_PTR)[actionsInJSON UTF8String],
                                       &rawInJSON);
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return @"";
    }
    
    NSString* strRawInJSON = [NSString stringWithCString:rawInJSON
                                                encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(rawInJSON);
    
    return strRawInJSON;
}

//JUB_RV JUB_BuildActionEOS(IN JUB_UINT16 contextID,
//                          IN JUB_ACTION_EOS_PTR actions,
//                          IN JUB_UINT16 actionCount,
//                          OUT JUB_CHAR_PTR_PTR actionsInJSON);
- (NSString*)JUB_BuildActionEOS:(NSUInteger)contextID
                         action:(NSMutableArray*)actionArray
{
    self.lastError = JUBR_OK;
    
    NSUInteger actionCnt = [actionArray count];
    JUB_ACTION_EOS_PTR actions = new JUB_ACTION_EOS[actionCnt+1];
    memset(actions, 0x00, actionCnt+1);
    
    for (NSUInteger i = 0; i < actionCnt; i++) {
        actions[i] = inlineActionEOS(actionArray[i]);
    }
    JUB_CHAR_PTR actionsInJSON;
    JUB_RV rv = JUB_BuildActionEOS(contextID,
                                   actions,
                                   actionCnt,
                                   &actionsInJSON);
    free(actions); actions = NULL;
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return nil;
    }
    
    NSString* strActionInJSON = [NSString stringWithCString:actionsInJSON
                                                   encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(actionsInJSON);
    
    return strActionInJSON;
}

@end
