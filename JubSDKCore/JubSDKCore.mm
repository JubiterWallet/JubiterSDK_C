//
//  JubSDKCore.m
//  JubSDKCore
//
//  Created by Pan Min on 2019/6/22.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#import "JubSDKCore.h"
#import "JUB_SDK.h"

//typedef struct {
//    JUB_ENUM_BOOL change;
//    JUB_UINT64    addressIndex;
//} BIP32_Path;
@implementation BIP32Path
@synthesize change;
@synthesize addressIndex;
@end

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

@end
