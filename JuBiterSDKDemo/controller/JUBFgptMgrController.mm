//
//  JUBFgptMgrController.m
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/8/13.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBAlertView.h"
#import "JUBPinAlertView.h"
//#import "JUBListAlert.h"
#import "JUBFingerEntryAlert.h"
#import "JUBSharedData.h"


#import "JUBFgptMgrController.h"


@interface JUBFgptMgrController ()

@end

@implementation JUBFgptMgrController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    NSUInteger deviceID = [[JUBSharedData sharedInstance] currDeviceID];
    JUB_ENUM_BOOL b = JUB_IsBootLoader(deviceID);
    if (b) {
        [self addMsgData:[NSString stringWithFormat:@"[Is in main SE.]"]];
    }
    else {
        [self addMsgData:[NSString stringWithFormat:@"[Isn't in main SE.]"]];
    }
    
    //这个fingerArray可以任意时候向他赋值,界面会自动更新
    if (JUBR_OK != [self enum_fgpt_test:deviceID]) {
        [self setFingerArray:nil];
    }
    
    self.navRightButtonTitle = @"Verify";
}


#pragma mark - 业务
- (NSUInteger)verify_fgptForIntl_test:(NSUInteger)deviceID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_ULONG retry = 0;
    rv = JUB_VerifyFgptForIntl(deviceID, &retry);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_VerifyFgptForIntl() return 0x%2lx.]", rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_VerifyFgptForIntl() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"Fingerprint retry: %lu.", retry]];
    
    return rv;
}


//- (NSUInteger)identity_verify:(NSUInteger)deviceID
//                         mode:(JUB_ENUM_IDENTITY_VERIFY_MODE)mode {
//
//    JUB_RV rv = JUBR_ERROR;
//
//    JUB_ULONG retry = 0;
//    rv = JUB_IdentityVerify(deviceID, mode, &retry);
//    if (   JUBR_OK               != rv
//        && JUBR_IMPL_NOT_SUPPORT != rv
//        ) {
//        [self addMsgData:[NSString stringWithFormat:@"[JUB_IdentityVerify(%lu) return 0x%2lx.]", retry, rv]];
//        return rv;
//    }
//    [self addMsgData:[NSString stringWithFormat:@"[JUB_IdentityVerify() OK.]"]];
//
//    return rv;
//}
//
//
- (NSUInteger)identity_verifyPIN:(NSUInteger)deviceID
                            mode:(JUB_ENUM_IDENTITY_VERIFY_MODE)mode
                             pin:(NSString*)pin {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_ULONG retry = 0;
    rv = JUB_IdentityVerifyPIN(deviceID, mode, [pin UTF8String], &retry);
    if (   JUBR_OK               != rv
        && JUBR_IMPL_NOT_SUPPORT != rv
        ) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_IdentityVerifyPIN(%lu) return 0x%2lx.]", retry, rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_IdentityVerifyPIN() OK.]"]];
    
    return rv;
}


- (NSUInteger)identity_showNineGrids:(NSUInteger)deviceID {
    
    JUB_RV rv = JUBR_ERROR;
    
    rv = JUB_IdentityShowNineGrids(deviceID);
    if (   JUBR_OK               != rv
        && JUBR_IMPL_NOT_SUPPORT != rv
        ) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_IdentityShowNineGrids() return 0x%2lx.]", rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_IdentityShowNineGrids() OK.]"]];
    
    return rv;
}


- (NSUInteger)identity_cancelNineGrids:(NSUInteger)deviceID {
    
    JUB_RV rv = JUBR_ERROR;
    
    rv = JUB_IdentityCancelNineGrids(deviceID);
    if (   JUBR_OK               != rv
        && JUBR_IMPL_NOT_SUPPORT != rv
        ) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_IdentityCancelNineGrids() return 0x%2lx.]", rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_IdentityCancelNineGrids() OK.]"]];
    
    return rv;
}


//- (NSUInteger)identity_verify_viaNineGrids:(NSUInteger)deviceID {
//
//    __block
//    JUB_RV rv = JUBR_ERROR;
//
//    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
//    if (nil == sharedData) {
//        return;
//    }
//
//    rv = [self identity_showNineGrids:deviceID];
//    if (JUBR_OK != rv) {
////        isDone = YES;
//        return rv;
//    }
//
//    __block
//    BOOL isDone = NO;
//    [JUBPinAlertView showInputPinAlert:^(NSString * _Nonnull pin) {
//        if (nil == pin) {
//            [self identity_cancelNineGrids:deviceID];
//            isDone = YES;
//            rv = JUBR_USER_CANCEL;
//            return;
//        }
//        [sharedData setUserPin:pin];
//
//        rv = [self identity_verifyPIN:deviceID
//                                 mode:JUB_ENUM_IDENTITY_VERIFY_MODE::VIA_9GRIDS
//                                  pin:pin];
//        if (JUBR_OK != rv) {
//            isDone = YES;
//            return;
//        }
//
//        isDone = YES;
//    }];
//
//    while (!isDone) {
//        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
//                                 beforeDate:[NSDate distantFuture]];
//    }
//
//    return rv;
//}
//
//
//- (NSUInteger)identity_verify_viaAPDU:(NSUInteger)deviceID {
//
//    __block
//    JUB_RV rv = JUBR_ERROR;
//
//    __block
//    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
//    if (nil == sharedData) {
//        return rv;
//    }
//
//    __block
//    BOOL isDone = NO;
//    [JUBPinAlertView showInputPinAlert:^(NSString * _Nonnull pin) {
//        if (nil == pin) {
//            [sharedData setUserPin:nil];
//            rv = JUBR_USER_CANCEL;
//            return;
//        }
//        [sharedData setUserPin:pin];
//
//        rv = [self identity_verifyPIN:deviceID
//                                 mode:JUB_ENUM_IDENTITY_VERIFY_MODE::VIA_APDU
//                                  pin:pin];
//        if (JUBR_OK != rv) {
//            isDone = YES;
//            return;
//        }
//
//        isDone = YES;
//    }];
//
//    while (!isDone) {
//        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
//                                 beforeDate:[NSDate distantFuture]];
//    }
//
//    return rv;
//}
//
//
- (NSUInteger)identity_verify_test:(NSUInteger)deviceID {
    
    __block
    NSUInteger rv = JUBR_ERROR;
    
    __block
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return rv;
    }
    
    __block
    BOOL isDone = NO;
    
//    JUBListAlert *listAlert = [JUBListAlert showCallBack:^(NSString *_Nonnull selectedItem) {
//        if (   [selectedItem isEqual:BUTTON_TITLE_VIA_DEV]
//            || [selectedItem isEqual:BUTTON_TITLE_VIA_FGPT]
//            ) {
//            dispatch_async(dispatch_get_global_queue(0, 0), ^{
//
//                __block
//                JUBAlertView *alertView;
//
//                if ([selectedItem isEqual:BUTTON_TITLE_VIA_DEV]) {
//
//                    dispatch_async(dispatch_get_main_queue(), ^{
//                        alertView = [JUBAlertView showMsg:@"Identity verification via device in progress..."];
//                    });
//
//                    rv = [self identity_verify:deviceID
//                                          mode:JUB_ENUM_IDENTITY_VERIFY_MODE::VIA_DEVICE];
//
//                    dispatch_async(dispatch_get_main_queue(), ^{
//                        [alertView dismiss];
//                    });
//                }   // if ([selectedItem isEqual:BUTTON_TITLE_VIA_DEV]) end
//                else if ([selectedItem isEqual:BUTTON_TITLE_VIA_FGPT]) {
//
//                    dispatch_async(dispatch_get_main_queue(), ^{
//                        alertView = [JUBAlertView showMsg:@"Identity verification via fgpt in progress..."];
//                    });
//
//                    rv = [self identity_verify:deviceID
//                                          mode:JUB_ENUM_IDENTITY_VERIFY_MODE::VIA_FPGT];
//
//                    dispatch_async(dispatch_get_main_queue(), ^{
//                        [alertView dismiss];
//                    });
//                }   // if ([selectedItem isEqual:BUTTON_TITLE_VIA_FGPT]) end
//            }); // dispatch_async(dispatch_get_global_queue(0, 0), ^ end
//        }
//        else if ([selectedItem isEqual:BUTTON_TITLE_VIA_9GRIDS]) {
//            rv = [self identity_verify_viaNineGrids:deviceID];
//
            rv = [self identity_showNineGrids:deviceID];
            if (JUBR_OK != rv) {
                return rv;
            }
            
            [JUBPinAlertView showInputPinAlert:^(NSString * _Nonnull pin) {
                
                if (nil == pin) {
                    [self identity_cancelNineGrids:deviceID];
                    rv = JUBR_USER_CANCEL;
                    isDone = YES;
                    return;
                }
                [sharedData setUserPin:pin];
                
                rv = [self identity_verifyPIN:deviceID
                                         mode:JUB_ENUM_IDENTITY_VERIFY_MODE::VIA_9GRIDS
                                          pin:pin];
                if (JUBR_OK != rv) {
                    isDone = YES;
                    return;
                }
            }]; // [JUBPinAlertView showInputPinAlert:^(NSString * _Nonnull pin) end
//        }   // if ([selectedItem isEqual:BUTTON_TITLE_VIA_9GRIDS]) end
//        else if ([selectedItem isEqual:BUTTON_TITLE_VIA_APDU]) {
////            rv = [self identity_verify_viaAPDU:deviceID];
//
//            [JUBPinAlertView showInputPinAlert:^(NSString * _Nonnull pin) {
//
//                if (nil == pin) {
//                    rv = JUBR_USER_CANCEL;
//                    return;
//                }
//                [sharedData setUserPin:pin];
//
//                rv = [self identity_verifyPIN:deviceID
//                                         mode:JUB_ENUM_IDENTITY_VERIFY_MODE::VIA_APDU
//                                          pin:pin];
//                if (JUBR_OK != rv) {
//                    return;
//                }
//            }]; // [JUBPinAlertView showInputPinAlert:^(NSString * _Nonnull pin) end
//
//        }   // if ([selectedItem isEqual:BUTTON_TITLE_VIA_APDU]) end
//
//    }]; // JUBListAlert *listAlert end
//
//    listAlert.title = @"Please select mode:";
//    [listAlert addItems:@[
//        BUTTON_TITLE_VIA_DEV,
//        BUTTON_TITLE_VIA_9GRIDS,
//        BUTTON_TITLE_VIA_APDU,
//        BUTTON_TITLE_VIA_FGPT
//    ]];
    
    while (!isDone) {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
                                 beforeDate:[NSDate distantFuture]];
    }
    
    return rv;
}


- (NSUInteger)enum_fgpt_test:(NSUInteger)deviceID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_CHAR_PTR fgptList = nil;
    rv = JUB_EnumFingerprint(deviceID, &fgptList);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_EnumFingerprint() return 0x%2lx.]", rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_EnumFingerprint() OK.]"]];
    
    NSString* fingerprintList = [NSString stringWithUTF8String:std::string(fgptList).c_str()];
    rv = JUB_FreeMemory(fgptList);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return 0x%2lx.]", rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"FingerprintIDs are: %@.", fingerprintList]];
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [self setFingerArray:[fingerprintList componentsSeparatedByString:@" "]];
    });
    
    return rv;
}


- (FgptEnrollResult)enroll_fgpt_test:(NSUInteger)deviceID
                           fgptIndex:(NSUInteger)fgptIndex
                               times:(NSUInteger)times
                              fgptID:(NSUInteger)fgptID {
    
    JUB_RV rv = JUBR_ERROR;
    
    NSUInteger fgptNextIndex = fgptIndex;
    JUB_ULONG remainingTimes = times;
    NSUInteger assignedID = fgptID;
    rv = JUB_EnrollFingerprint(deviceID,
                               (JUB_BYTE_PTR)(&fgptNextIndex), &remainingTimes,
                               (JUB_BYTE_PTR)(&assignedID));
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_EnrollFingerprint() return 0x%2lx.]", rv]];
        return FgptEnrollResult{FgptEnrollInfo{assignedID, fgptNextIndex, remainingTimes}, rv};
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_EnrollFingerprint() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"FgptNextIndex(%lu), times(%lu), FgptID(%lu).", fgptNextIndex, remainingTimes, assignedID]];
    
    return FgptEnrollResult{FgptEnrollInfo{assignedID, fgptNextIndex, remainingTimes}, rv};
}


- (NSUInteger)erase_fgpt_test:(NSUInteger)deviceID {
    
    JUB_RV rv = JUBR_ERROR;
    
    rv = JUB_EraseFingerprint(deviceID);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_EraseFingerprint() return 0x%2lx.]", rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_EraseFingerprint() OK.]"]];
    
    return rv;
}


- (NSUInteger)delete_fgpt_test:(NSUInteger)deviceID
                        fgptID:(NSUInteger)fgptID {
    
    JUB_RV rv = JUBR_ERROR;
    
    rv = JUB_DeleteFingerprint(deviceID, fgptID);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_DeleteFingerprint() return 0x%2lx.]", rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_DeleteFingerprint() OK.]"]];
    
    return rv;
}


#pragma mark - 回调子类方法
//导航栏右边按钮点击回调，重写该方法可以接管点击按钮的响应
- (void)navRightButtonCallBack {
    
//    if (JUBR_OK != [self verify_fgptForIntl_test:[[JUBSharedData sharedInstance] currDeviceID]]) {
//        return;
//    }
    [self identity_verify_test:[[JUBSharedData sharedInstance] currDeviceID]];
}


//指纹录入
- (void)fingerPrintEntry {
    
//    JUBFingerEntryAlert *fingerEntryAlert = [JUBFingerEntryAlert show];
//
//    [NSTimer scheduledTimerWithTimeInterval:2 repeats:YES block:^(NSTimer * _Nonnull timer) {
//
//        //在你使用的时候直接使用这部分内容就可以，定时器可以去掉了
//        if (fingerEntryAlert.fingerNumber == 5) {
//
//            [timer invalidate];
//
//            [fingerEntryAlert dismiss];
//
//            return;
//
//        }
//
//        //这个fingerNumber可以任意时候向他赋值,界面会自动更新
//        fingerEntryAlert.fingerNumber = fingerEntryAlert.fingerNumber + 1;
//        NSLog(@"fingerEntryAlert.fingerNumber = %ld.", fingerEntryAlert.fingerNumber);
//    }];
    
    NSUInteger deviceID = [[JUBSharedData sharedInstance] currDeviceID];
    
    JUBFingerEntryAlert *fingerEntryAlert = [JUBFingerEntryAlert show];
    
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        
        NSUInteger fgptIndex = 0;
        NSUInteger times = 0;
        NSUInteger fgptID = 0;
        __block
        FgptEnrollResult result = [self enroll_fgpt_test:deviceID
                                               fgptIndex:fgptIndex
                                                   times:times
                                                  fgptID:fgptID];
        NSLog(@"....enrollInfo.nextIndex = %ld.", result.enrollInfo.nextIndex);
        if (JUBR_OK != result.rv) {
            dispatch_async(dispatch_get_main_queue(), ^{
                [fingerEntryAlert dismiss];
            });
            return;
        }
        
        dispatch_async(dispatch_get_main_queue(), ^{
            [fingerEntryAlert setFingerNumberSum:result.enrollInfo.times];
            [fingerEntryAlert setFingerNumber:result.enrollInfo.nextIndex];
        });
        
        for (JUB_BYTE i=result.enrollInfo.nextIndex; i<result.enrollInfo.times; ++i) {
            
            //这个fingerNumber可以任意时候向他赋值,界面会自动更新
            dispatch_async(dispatch_get_main_queue(), ^{
                [fingerEntryAlert setFingerNumberSum:result.enrollInfo.times];
                [fingerEntryAlert setFingerNumber:result.enrollInfo.nextIndex];
                NSLog(@"fingerEntryAlert.fingerNumber = %ld.", fingerEntryAlert.fingerNumber);
            });
            
            times = result.enrollInfo.times;
            fgptIndex = result.enrollInfo.nextIndex;
            fgptID    = result.enrollInfo.modalityID;
            result = [self enroll_fgpt_test:deviceID
                                  fgptIndex:fgptIndex
                                      times:times
                                     fgptID:fgptID];
            if (JUBR_OK != result.rv) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [fingerEntryAlert dismiss];
                });
                return;
            }
        }
        
        //这个fingerNumber可以任意时候向他赋值,界面会自动更新
        dispatch_async(dispatch_get_main_queue(), ^{
            [fingerEntryAlert setFingerNumberSum:result.enrollInfo.times];
            [fingerEntryAlert setFingerNumber:result.enrollInfo.nextIndex];
            NSLog(@"fingerEntryAlert.fingerNumber = %ld.", fingerEntryAlert.fingerNumber);
        });
        
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            [fingerEntryAlert dismiss];
        });
        
        [self enum_fgpt_test:deviceID];
    });
}


//清空指纹
- (void)clearFingerPrint {
    
    //向设备发送清空指纹的指令
    //
    if (JUBR_OK != [self erase_fgpt_test:[[JUBSharedData sharedInstance] currDeviceID]]) {
        return;
    }
    
    //这里应该先完成与设备的通信，设备清空成功，再刷新界面，如果通信失败，则不刷新界面
    self.fingerArray = nil;
}


//删除指纹
- (void)selectedFinger:(NSInteger)selectedFingerIndex {
    
    NSLog(@"selectedFingerIndex = %ld", (long)selectedFingerIndex);
    
    //向设备发送清空指纹的指令
    //
    if (JUBR_OK != [self delete_fgpt_test:[[JUBSharedData sharedInstance] currDeviceID]
                                   fgptID:(JUB_BYTE)selectedFingerIndex]) {
        return;
    }
    
    //这里应该先完成与设备的通信，设备清空成功，再刷新界面，如果通信失败，则不刷新界面
    NSMutableArray *fingerArray = [self.fingerArray mutableCopy];
    
    [fingerArray removeObjectAtIndex:selectedFingerIndex];
    
    self.fingerArray = fingerArray;
}

@end
