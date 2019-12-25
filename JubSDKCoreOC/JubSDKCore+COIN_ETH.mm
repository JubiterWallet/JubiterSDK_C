//
//  JubSDKCore+COIN_ETH.mm
//  JubSDKCore
//
//  Created by Pan Min on 2019/7/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#import "JubSDKCore+COIN_ETH.h"
#import "JUB_SDK_ETH.h"
#import "JUB_CORE.h"

//typedef struct {
//    JUB_CHAR_PTR        main_path;
//    int                    chainID;
//} CONTEXT_CONFIG_ETH;
@implementation ContextConfigETH
@synthesize mainPath;
@synthesize chainID;
@end

extern JUB_ENUM_BOOL (^inlineBool)(JUB_NS_ENUM_BOOL);
extern JUB_ENUM_PUB_FORMAT (^inlinePubFormat)(JUB_NS_ENUM_PUB_FORMAT);

@implementation JubSDKCore (COIN_ETH)

//JUB_RV JUB_CreateContextETH_soft(IN CONTEXT_CONFIG_ETH cfg,
//                                 IN JUB_CHAR_PTR masterPriInXPRV,
//                                 OUT JUB_UINT16* contextID);
- (NSUInteger)JUB_CreateContextETH_soft:(ContextConfigETH*)cfg
                        masterPriInXPRV:(NSString*)masterPriInXPRV
{
    self.lastError = JUBR_OK;
    
    CONTEXT_CONFIG_ETH cfgETH;
    cfgETH.mainPath = (JUB_CHAR_PTR)[cfg.mainPath UTF8String];
    cfgETH.chainID = (int)cfg.chainID;
    JUB_UINT16 contextID = 0;
    JUB_RV rv = JUB_CreateContextETH_soft(cfgETH,
                                          (JUB_CHAR_PTR)[masterPriInXPRV UTF8String],
                                          &contextID);
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return contextID;
    }
    
    return contextID;
}

//JUB_RV JUB_CreateContextETH(IN CONTEXT_CONFIG_ETH cfg,
//                            IN JUB_UINT16 deviceID,
//                            OUT JUB_UINT16* contextID);
- (NSUInteger)JUB_CreateContextETH:(NSUInteger)deviceID
                               cfg:(ContextConfigETH*)cfg
{
    self.lastError = JUBR_OK;
    
    CONTEXT_CONFIG_ETH cfgETH;
    cfgETH.mainPath = (JUB_CHAR_PTR)[cfg.mainPath UTF8String];
    cfgETH.chainID = (int)cfg.chainID;
    JUB_UINT16 contextID = 0;
    JUB_RV rv = JUB_CreateContextETH(cfgETH,
                                     deviceID,
                                     &contextID);
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return contextID;
    }
    
    return contextID;
}

//JUB_RV JUB_GetAddressETH(IN JUB_UINT16 contextID,
//                         IN BIP44_Path path,
//                         IN JUB_ENUM_BOOL bShow,
//                         OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_GetAddressETH:(NSUInteger)contextID
                          path:(BIP44Path*)path
                         bShow:(JUB_NS_ENUM_BOOL)bShow
{
    self.lastError = JUBR_OK;
    
    BIP44_Path bip44Path;
    bip44Path.change = inlineBool(path.change);
    bip44Path.addressIndex = path.addressIndex;
    JUB_ENUM_BOOL isShow = inlineBool(bShow);
    JUB_CHAR_PTR address;
    JUB_RV rv = JUB_GetAddressETH(contextID,
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

//JUB_RV JUB_GetHDNodeETH(IN JUB_UINT16 contextID,
//                        IN JUB_ENUM_PUB_FORMAT format,
//                        IN BIP44_Path path,
//                        OUT JUB_CHAR_PTR_PTR pubkey);
- (NSString*)JUB_GetHDNodeETH:(NSUInteger)contextID
                       format:(JUB_NS_ENUM_PUB_FORMAT)format
                         path:(BIP44Path*)path
{
    self.lastError = JUBR_OK;
    
    JUB_ENUM_PUB_FORMAT fmt = inlinePubFormat(format);
    if (PUB_FORMAT_NS_ITEM == fmt) {
        self.lastError = JUBR_ARGUMENTS_BAD;
        return @"";
    }
    BIP44_Path bip44Path;
    bip44Path.change = inlineBool(path.change);
    bip44Path.addressIndex = path.addressIndex;
    JUB_CHAR_PTR pubkey = nullptr;
    JUB_RV rv = JUB_GetHDNodeETH(contextID,
                                 fmt,
                                 bip44Path,
                                 &pubkey);
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return @"";
    }
    
    NSString* strPubkey = [NSString stringWithCString:pubkey
                                             encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(pubkey);
    
    return strPubkey;
}

//JUB_RV JUB_GetMainHDNodeETH(IN JUB_UINT16 contextID,
//                            IN JUB_ENUM_PUB_FORMAT format,
//                            OUT JUB_CHAR_PTR_PTR xpub);
- (NSString*)JUB_GetMainHDNodeETH:(NSUInteger)contextID
                           format:(JUB_NS_ENUM_PUB_FORMAT)format
{
    self.lastError = JUBR_OK;
    
    JUB_ENUM_PUB_FORMAT fmt = inlinePubFormat(format);
    if (PUB_FORMAT_NS_ITEM == fmt) {
        self.lastError = JUBR_ARGUMENTS_BAD;
        return @"";
    }
    JUB_CHAR_PTR xpub = nullptr;
    JUB_RV rv = JUB_GetMainHDNodeETH(contextID,
                                     fmt,
                                     &xpub);
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return @"";
    }
    
    NSString* strXpub = [NSString stringWithCString:xpub
                                           encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(xpub);
    
    return strXpub;
}

//JUB_RV JUB_SetMyAddressETH(IN JUB_UINT16 contextID,
//                           IN BIP44_Path path,
//                           OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_SetMyAddressETH:(NSUInteger)contextID
                            path:(BIP44Path*)path
{
    self.lastError = JUBR_OK;
    
    BIP44_Path bip44Path;
    bip44Path.change = inlineBool(path.change);
    bip44Path.addressIndex = path.addressIndex;
    JUB_CHAR_PTR address;
    JUB_RV rv = JUB_SetMyAddressETH(contextID,
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

//JUB_RV JUB_SignTransactionETH(IN JUB_UINT16 contextID,
//                              IN BIP44_Path path,
//                              IN JUB_UINT32 nonce,
//                              IN JUB_UINT32 gasLimit,
//                              IN JUB_CHAR_PTR gasPriceInWei,
//                              IN JUB_CHAR_PTR to,
//                              IN JUB_CHAR_PTR valueInWei,
//                              IN JUB_CHAR_PTR input,
//                              OUT JUB_CHAR_PTR_PTR raw);
- (NSString*)JUB_SignTransactionETH:(NSUInteger)contextID
                               path:(BIP44Path*)path
                              nonce:(NSUInteger)nonce
                           gasLimit:(NSUInteger)gasLimit
                      gasPriceInWei:(NSString*)gasPriceInWei
                                 to:(NSString*)to
                         valueInWei:(NSString*)valueInWei
                              input:(NSString*)input
{
    self.lastError = JUBR_OK;
    
    BIP44_Path bip44Path;
    bip44Path.change = inlineBool(path.change);
    bip44Path.addressIndex = path.addressIndex;
    JUB_CHAR_PTR raw = nullptr;
    JUB_RV rv = JUB_SignTransactionETH(contextID,
                                       bip44Path,
                                       (JUB_UINT32)nonce,
                                       (JUB_UINT32)gasLimit,
                                       (JUB_CHAR_PTR)[gasPriceInWei UTF8String],
                                       (JUB_CHAR_PTR)[to UTF8String],
                                       (JUB_CHAR_PTR)[valueInWei UTF8String],
                                       (JUB_CHAR_PTR)[input UTF8String],
                                       &raw);
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return @"";
    }
    
    NSString* strRaw = [NSString stringWithCString:raw
                                          encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(raw);
    
    return strRaw;
}

//JUB_RV JUB_BuildERC20AbiETH(IN JUB_UINT16 contextID,
//                            IN JUB_CHAR_PTR tokenName,
//                            IN JUB_UINT16 unitDP,
//                            IN JUB_CHAR_PTR contractAddress,
//                            IN JUB_CHAR_PTR tokenTo, IN JUB_CHAR_PTR tokenValue,
//                            OUT JUB_CHAR_PTR_PTR abi);
- (NSString*)JUB_BuildERC20AbiETH:(NSUInteger)contextID
                        tokenName:(NSString*)tokenName
                           unitDP:(NSUInteger)unitDP
                  contractAddress:(NSString*)contractAddress
                          tokenTo:(NSString*)tokenTo
                       tokenValue:(NSString*)tokenValue
{
    self.lastError = JUBR_OK;
    
    JUB_CHAR_PTR abi = nullptr;
    JUB_RV rv = JUB_BuildERC20AbiETH(contextID,
                                     (JUB_CHAR_PTR)[tokenName UTF8String],
                                     unitDP,
                                     (JUB_CHAR_PTR)[contractAddress UTF8String],
                                     (JUB_CHAR_PTR)[tokenTo UTF8String],
                                     (JUB_CHAR_PTR)[tokenValue UTF8String],
                                     &abi);
    if (JUBR_OK != rv) {
        self.lastError = rv;
        return @"";
    }
    
    NSString* strAbi = [NSString stringWithCString:abi
                                          encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(abi);
    
    return strAbi;
}

@end
