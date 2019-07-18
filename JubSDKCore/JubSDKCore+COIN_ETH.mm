//
//  JubSDKCore+COIN_ETH.mm
//  JubSDKCore
//
//  Created by Pan Min on 2019/7/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#import "JubSDKCore+COIN_ETH.h"
#import "JUB_SDK_ETH.h"

//typedef struct {
//    JUB_CHAR_PTR        main_path;
//    int                    chainID;
//} CONTEXT_CONFIG_ETH;
@implementation ContextConfigETH
@synthesize mainPath;
@synthesize chainID;
@end

@implementation JubSDKCore (COIN_ETH)

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
//                         IN BIP32_Path path,
//                         IN JUB_ENUM_BOOL bShow,
//                         OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_GetAddressETH:(NSUInteger)contextID
                          path:(BIP32Path*)path
                         bShow:(JUB_NS_ENUM_BOOL)bShow
{
    self.lastError = JUBR_OK;
    
    BIP32_Path bip32Path;
    switch (path.change) {
        case BOOL_NS_FALSE:
            bip32Path.change = BOOL_FALSE;
            break;
        case BOOL_NS_TRUE:
            bip32Path.change = BOOL_TRUE;
            break;
        case BOOL_NS_NR_ITEMS:
            bip32Path.change = BOOL_NR_ITEMS;
            break;
    }
    bip32Path.addressIndex = path.addressIndex;
    JUB_ENUM_BOOL isShow;
    switch (bShow) {
        case BOOL_NS_FALSE:
            isShow = BOOL_FALSE;
            break;
        case BOOL_NS_TRUE:
            isShow = BOOL_TRUE;
            break;
        case BOOL_NS_NR_ITEMS:
            isShow = BOOL_NR_ITEMS;
            break;
    }
    JUB_CHAR_PTR address;
    JUB_RV rv = JUB_GetAddressETH(contextID,
                                  bip32Path,
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
//                        IN JUB_ETH_PUB_FORMAT format,
//                        IN BIP32_Path path,
//                        OUT JUB_CHAR_PTR_PTR pubkey);
- (NSString*)JUB_GetHDNodeETH:(NSUInteger)contextID
                       format:(JUB_NS_ETH_PUB_FORMAT)format
                         path:(BIP32Path*)path
{
    self.lastError = JUBR_OK;
    
    JUB_ETH_PUB_FORMAT fmt;
    switch (format) {
        case NS_HEX:
            fmt = HEX;
            break;
        case NS_XPUB:
            fmt = XPUB;
            break;
        default:
            self.lastError = JUBR_ARGUMENTS_BAD;
            return @"";
    }
    BIP32_Path bip32Path;
    switch (path.change) {
        case BOOL_NS_FALSE:
            bip32Path.change = BOOL_FALSE;
            break;
        case BOOL_NS_TRUE:
            bip32Path.change = BOOL_TRUE;
            break;
        case BOOL_NS_NR_ITEMS:
            bip32Path.change = BOOL_NR_ITEMS;
            break;
    }
    bip32Path.addressIndex = path.addressIndex;
    JUB_CHAR_PTR pubkey = nullptr;
    JUB_RV rv = JUB_GetHDNodeETH(contextID,
                                 fmt,
                                 bip32Path,
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
//                            IN JUB_ETH_PUB_FORMAT format,
//                            OUT JUB_CHAR_PTR_PTR xpub);
- (NSString*)JUB_GetMainHDNodeETH:(NSUInteger)contextID
                           format:(JUB_NS_ETH_PUB_FORMAT)format
{
    self.lastError = JUBR_OK;
    
    JUB_ETH_PUB_FORMAT fmt;
    switch (format) {
        case NS_HEX:
            fmt = HEX;
            break;
        case NS_XPUB:
            fmt = XPUB;
            break;
        default:
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
//                           IN BIP32_Path path,
//                           OUT JUB_CHAR_PTR_PTR address);
- (NSString*)JUB_SetMyAddressETH:(NSUInteger)contextID
                            path:(BIP32Path*)path
{
    self.lastError = JUBR_OK;
    
    BIP32_Path bip32Path;
    switch (path.change) {
        case BOOL_NS_FALSE:
            bip32Path.change = BOOL_FALSE;
            break;
        case BOOL_NS_TRUE:
            bip32Path.change = BOOL_TRUE;
            break;
        case BOOL_NS_NR_ITEMS:
            bip32Path.change = BOOL_NR_ITEMS;
            break;
    }
    bip32Path.addressIndex = path.addressIndex;
    JUB_CHAR_PTR address;
    JUB_RV rv = JUB_SetMyAddressETH(contextID,
                                    bip32Path,
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
//                              IN BIP32_Path path,
//                              IN JUB_UINT32 nonce,
//                              IN JUB_UINT32 gasLimit,
//                              IN JUB_CHAR_PTR gasPriceInWei,
//                              IN JUB_CHAR_PTR to,
//                              IN JUB_CHAR_PTR valueInWei,
//                              IN JUB_CHAR_PTR input,
//                              OUT JUB_CHAR_PTR_PTR raw);
- (NSString*)JUB_SignTransactionETH:(NSUInteger)contextID
                               path:(BIP32Path*)path
                              nonce:(NSUInteger)nonce
                           gasLimit:(NSUInteger)gasLimit
                      gasPriceInWei:(NSString*)gasPriceInWei
                                 to:(NSString*)to
                         valueInWei:(NSString*)valueInWei
                              input:(NSString*)input
{
    self.lastError = JUBR_OK;
    
    BIP32_Path bip32Path;
    switch (path.change) {
        case BOOL_NS_FALSE:
            bip32Path.change = BOOL_FALSE;
            break;
        case BOOL_NS_TRUE:
            bip32Path.change = BOOL_TRUE;
            break;
        case BOOL_NS_NR_ITEMS:
            bip32Path.change = BOOL_NR_ITEMS;
            break;
    }
    bip32Path.addressIndex = path.addressIndex;
    JUB_CHAR_PTR raw = nullptr;
    JUB_RV rv = JUB_SignTransactionETH(contextID,
                                       bip32Path,
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
//                            IN JUB_CHAR_PTR token_to, IN JUB_CHAR_PTR token_value,
//                            OUT JUB_CHAR_PTR_PTR abi);
- (NSString*)JUB_BuildERC20AbiETH:(NSUInteger)contextID
                          tokenTo:(NSString*)tokenTo
                       tokenValue:(NSString*)tokenValue
{
    self.lastError = JUBR_OK;
    
    JUB_CHAR_PTR abi = nullptr;
    JUB_RV rv = JUB_BuildERC20AbiETH(contextID,
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
