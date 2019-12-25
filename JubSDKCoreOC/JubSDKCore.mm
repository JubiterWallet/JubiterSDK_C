//
//  JubSDKCore.m
//  JubSDKCore
//
//  Created by Pan Min on 2019/6/22.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#import "JubSDKCore.h"
#import "JUB_SDK.h"
#import "JUB_CORE.h"

//typedef struct {
//    JUB_ENUM_BOOL change;
//    JUB_UINT64    addressIndex;
//} BIP44_Path;
@implementation BIP44Path
@synthesize change;
@synthesize addressIndex;
@end

//typedef struct stContextCfg {
//    JUB_CHAR_PTR            main_path;
//} CONTEXT_CONFIG;
@implementation ContextConfig
@synthesize mainPath;
@end

bool (^inlinebool)(JUB_NS_ENUM_BOOL) = ^(JUB_NS_ENUM_BOOL argument) {
    bool b;
    switch (argument) {
        case BOOL_NS_FALSE:
            b = false;
            break;
        case BOOL_NS_TRUE:
            b = true;
            break;
        case BOOL_NS_NR_ITEMS:
            b = false;
            break;
    }
    return b;
};

JUB_NS_ENUM_BOOL (^inlineNSbool)(bool) = ^(bool argument) {
    JUB_NS_ENUM_BOOL b;
    if (argument) {
        b = BOOL_NS_TRUE;
    }
    else {
        b = BOOL_NS_FALSE;
    }
    return b;
};

JUB_ENUM_BOOL (^inlineBool)(JUB_NS_ENUM_BOOL) = ^(JUB_NS_ENUM_BOOL argument) {
    JUB_ENUM_BOOL b;
    switch (argument) {
        case BOOL_NS_FALSE:
            b = BOOL_FALSE;
            break;
        case BOOL_NS_TRUE:
            b = BOOL_TRUE;
            break;
        case BOOL_NS_NR_ITEMS:
            b = BOOL_NR_ITEMS;
            break;
    }
    return b;
};

JUB_NS_ENUM_BOOL (^inlineNSBool)(JUB_ENUM_BOOL) = ^(JUB_ENUM_BOOL argument) {
    JUB_NS_ENUM_BOOL b;
    switch (argument) {
        case BOOL_FALSE:
            b = BOOL_NS_FALSE;
            break;
        case BOOL_TRUE:
            b = BOOL_NS_TRUE;
            break;
        case BOOL_NR_ITEMS:
            b = BOOL_NS_NR_ITEMS;
            break;
    }
    return b;
};

// JUB_NS_ENUM_PUB_FORMAT -> JUB_ENUM_PUB_FORMAT
JUB_ENUM_PUB_FORMAT (^inlinePubFormat)(JUB_NS_ENUM_PUB_FORMAT) = ^(JUB_NS_ENUM_PUB_FORMAT argument) {
    JUB_ENUM_PUB_FORMAT fmt;
    switch (argument) {
        case NS_HEX:
            fmt = HEX;
            break;
        case NS_XPUB:
            fmt = XPUB;
            break;
        default:
            fmt = PUB_FORMAT_NS_ITEM;
            break;
    }
    
    return fmt;
};

// JUB_NS_ENUM_MNEMONIC_STRENGTH -> JUB_ENUM_MNEMONIC_STRENGTH
JUB_ENUM_MNEMONIC_STRENGTH (^inlineMnemonicStrength)(JUB_NS_ENUM_MNEMONIC_STRENGTH) = ^(JUB_NS_ENUM_MNEMONIC_STRENGTH argument) {
    JUB_ENUM_MNEMONIC_STRENGTH strength;
    switch (argument) {
        case JUB_NS_ENUM_MNEMONIC_STRENGTH128:
            strength = JUB_ENUM_MNEMONIC_STRENGTH::STRENGTH128;
            break;
        case JUB_NS_ENUM_MNEMONIC_STRENGTH192:
            strength = JUB_ENUM_MNEMONIC_STRENGTH::STRENGTH192;
            break;
        case JUB_NS_ENUM_MNEMONIC_STRENGTH256:
            strength = JUB_ENUM_MNEMONIC_STRENGTH::STRENGTH256;
            break;
        case JUB_NS_ENUM_MNEMONIC_STRENGTH_NR_ITEMS:
        default:
            strength = JUB_ENUM_MNEMONIC_STRENGTH::STRENGTH128;
            break;
    }
    return strength;
};

// JUB_NS_ENUM_MNEMONIC_STRENGTH <- JUB_ENUM_MNEMONIC_STRENGTH
JUB_NS_ENUM_MNEMONIC_STRENGTH (^inlineNSMnemonicStrength)(JUB_ENUM_MNEMONIC_STRENGTH) = ^(JUB_ENUM_MNEMONIC_STRENGTH argument) {
    JUB_NS_ENUM_MNEMONIC_STRENGTH strength;
    switch (argument) {
        case JUB_ENUM_MNEMONIC_STRENGTH::STRENGTH128:
            strength = JUB_NS_ENUM_MNEMONIC_STRENGTH128;
            break;
        case JUB_ENUM_MNEMONIC_STRENGTH::STRENGTH192:
            strength = JUB_NS_ENUM_MNEMONIC_STRENGTH192;
            break;
        case JUB_ENUM_MNEMONIC_STRENGTH::STRENGTH256:
            strength = JUB_NS_ENUM_MNEMONIC_STRENGTH256;
            break;
        default:
            strength = JUB_NS_ENUM_MNEMONIC_STRENGTH_NR_ITEMS;
            break;
    }
    return strength;
};

// JUB_NS_ENUM_CURVES -> JUB_ENUM_CURVES
JUB_ENUM_CURVES (^inlineCurves)(JUB_NS_ENUM_CURVES) = ^(JUB_NS_ENUM_CURVES argument) {
    JUB_ENUM_CURVES curves;
    switch (argument) {
        case JUB_NS_ENUM_SECP256K1:
        case JUB_NS_ENUM_NR_ITEMS:
            curves = JUB_ENUM_CURVES::secp256k1;
            break;
        case JUB_NS_ENUM_ED25519:
            curves = JUB_ENUM_CURVES::ed25519;
            break;
        case JUB_NS_ENUM_NIST256P1:
            curves = JUB_ENUM_CURVES::nist256p1;
            break;
        default:
            break;
    }
    return curves;
};

// JUB_NS_ENUM_CURVES <- JUB_ENUM_CURVES
JUB_NS_ENUM_CURVES (^inlineNSCurves)(JUB_ENUM_CURVES) = ^(JUB_ENUM_CURVES argument) {
    JUB_NS_ENUM_CURVES curves;
    switch (argument) {
        case JUB_ENUM_CURVES::secp256k1:
            curves = JUB_NS_ENUM_SECP256K1;
            break;
        case JUB_ENUM_CURVES::ed25519:
            curves = JUB_NS_ENUM_ED25519;
            break;
        case JUB_ENUM_CURVES::nist256p1:
            curves = JUB_NS_ENUM_NIST256P1;
            break;
        default:
            curves = JUB_NS_ENUM_NR_ITEMS;
            break;
    }
    return curves;
};

@implementation JubSDKCore
- (NSUInteger)JUB_LastError
{
    return _lastError;
}

//JUB_RV JUB_ClearContext(IN JUB_UINT16 contextID);
- (void)JUB_ClearContext:(NSUInteger)contextID
{
    _lastError = JUB_ClearContext(contextID);
}

//JUB_RV JUB_ShowVirtualPwd(IN JUB_UINT16 contextID);
- (void)JUB_ShowVirtualPwd:(NSUInteger)contextID
{
    _lastError = JUB_ShowVirtualPwd(contextID);
}

//JUB_RV JUB_CancelVirtualPwd(IN JUB_UINT16 contextID);
- (void)JUB_CancelVirtualPwd:(NSUInteger)contextID
{
    _lastError = JUB_CancelVirtualPwd(contextID);
}

//JUB_RV JUB_VerifyPIN(IN JUB_UINT16 contextID,
//                     IN JUB_CHAR_PTR pinMix,
//                     OUT JUB_ULONG_PTR retry);
- (NSUInteger)JUB_VerifyPIN:(NSUInteger)contextID
                     pinMix:(NSString*)pinMix
{
    _lastError = JUBR_OK;
    
    JUB_ULONG retry = 0;
    JUB_RV rv = JUB_VerifyPIN(contextID,
                              (JUB_CHAR_PTR)[pinMix UTF8String],
                              &retry);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return retry;
    }
    
    return retry;
}

//JUB_CHAR_PTR JUB_GetVersion(void);
- (NSString*)JUB_GetVersion
{
    _lastError = JUBR_OK;
    
    return [NSString stringWithCString:JUB_GetVersion()
                              encoding:NSUTF8StringEncoding];
}

//JUB_RV JUB_GenerateMnemonic(IN JUB_ENUM_MNEMONIC_STRENGTH strength,
//                            OUT JUB_CHAR_PTR_PTR mnemonic);
- (NSString*)JUB_GenerateMnemonic:(JUB_NS_ENUM_MNEMONIC_STRENGTH)strength
{
    _lastError = JUBR_OK;
    
    JUB_ENUM_MNEMONIC_STRENGTH mnemonicStr = inlineMnemonicStrength(strength);
    JUB_CHAR_PTR mnemonic = nullptr;
    JUB_RV rv = JUB_GenerateMnemonic(mnemonicStr,
                                     &mnemonic);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return @"";
    }
    
    NSString* strMnemonic = [NSString stringWithCString:mnemonic
                                               encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(mnemonic);
    
    return strMnemonic;
}

//JUB_RV JUB_CheckMnemonic(IN JUB_CHAR_CPTR mnemonic);
- (void)JUB_CheckMnemonic:(NSString*)mnemonic
{
    _lastError = JUBR_OK;
    
    JUB_RV rv = JUB_CheckMnemonic((JUB_CHAR_PTR)[mnemonic UTF8String]);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return;
    }
}

//JUB_RV JUB_GenerateSeed(IN JUB_CHAR_CPTR mnemonic, IN JUB_CHAR_CPTR passphrase,
//                        OUT JUB_BYTE seed[64],
//                        void (*progress_callback)(JUB_UINT32 current, JUB_UINT32 total));
- (NSString*)JUB_GenerateSeed:(NSString*)mnemonic
                   passphrase:(NSString*)passphrase
                     callback:(ProgressCallback)callback
{
    _lastError = JUBR_OK;
    
    JUB_BYTE seed[64] = {0x00,};
    JUB_RV rv = JUB_GenerateSeed((JUB_CHAR_CPTR)[mnemonic UTF8String],
                                 (JUB_CHAR_CPTR)[passphrase UTF8String],
                                 seed,
                                 callback);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return @"";
    }
    
    NSString* strSeed = [NSString stringWithFormat:@"%s", seed];
    
    return strSeed;
}

//JUB_RV JUB_SeedToMasterPrivateKey(IN JUB_BYTE_PTR seed, IN JUB_UINT16 seed_len,
//                                  IN JUB_ENUM_CURVES curve,
//                                  OUT JUB_CHAR_PTR_PTR prikeyInXPRV);
- (NSString*)JUB_SeedToMasterPrivateKey:(NSString*)seed
                                seedLen:(NSInteger)seedLen
                                  curve:(JUB_NS_ENUM_CURVES)curve
{
    _lastError = JUBR_OK;
    
    JUB_ENUM_CURVES c = inlineCurves(curve);
    JUB_CHAR_PTR prikeyInXPRV = nullptr;
    JUB_RV rv = JUB_SeedToMasterPrivateKey((JUB_BYTE_PTR)[seed UTF8String],
                                           seedLen,
                                           c,
                                           &prikeyInXPRV);
    if (JUBR_OK != rv) {
        _lastError = rv;
        return @"";
    }
    
    NSString* strPrikeyInXPRV = [NSString stringWithCString:prikeyInXPRV
                                                   encoding:NSUTF8StringEncoding];
    JUB_FreeMemory(prikeyInXPRV);
    
    return strPrikeyInXPRV;
}

@end
