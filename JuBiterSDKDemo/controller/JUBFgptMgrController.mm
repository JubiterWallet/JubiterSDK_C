//
//  JUBFgptMgrController.m
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/8/13.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBSharedData.h"
#import "JUBFingerEntryAlert.h"


#import "JUBFgptMgrController.h"


@interface JUBFgptMgrController ()

@end

@implementation JUBFgptMgrController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    //这个fingerArray可以任意时候向他赋值,界面会自动更新
    NSUInteger deviceID = [[JUBSharedData sharedInstance] currDeviceID];
    if (JUBR_OK != [self enum_fgpt_test:deviceID]) {
        [self setFingerArray:nil];
    }
    
    self.navRightButtonTitle = @"Verify";
    
    if (JUB_IsBootLoader(deviceID)) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_IsBootLoader() return TRUE.]"]];
    }
    else {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_IsBootLoader() return FALSE.]"]];
    }
}


#pragma mark - 业务
- (NSUInteger)verify_fgpt_test:(NSUInteger)deviceID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_ULONG retry = 0;
    rv = JUB_VerifyFgpt(deviceID, &retry);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_VerifyFgpt() return 0x%2lx.]", rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_VerifyFgpt() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"Fingerprint retry: %lu.", retry]];
    
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
    JUB_FreeMemory(fgptList);
    
    [self addMsgData:[NSString stringWithFormat:@"FingerprintIDs are: %@.", fingerprintList]];
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [self setFingerArray:[fingerprintList componentsSeparatedByString:@" "]];
    });
        
    return rv;
}


- (FgptEnrollInfo)enroll_fgpt_test:(NSUInteger)deviceID
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
        return FgptEnrollInfo{assignedID, fgptNextIndex, remainingTimes, rv};
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_EnrollFingerprint() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"FgptNextIndex(%lu), times(%lu), FgptID(%lu).", fgptNextIndex, remainingTimes, assignedID]];
    
    return FgptEnrollInfo{assignedID, fgptNextIndex, remainingTimes, rv};
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
    
    if (JUBR_OK != [self verify_fgpt_test:[[JUBSharedData sharedInstance] currDeviceID]]) {
        return;
    }
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
        FgptEnrollInfo enrollInfo = [self enroll_fgpt_test:deviceID
                                                 fgptIndex:fgptIndex
                                                     times:times
                                                    fgptID:fgptID];
        NSLog(@"....enrollInfo.nextIndex = %ld.", enrollInfo.nextIndex);
        if (JUBR_OK != enrollInfo.rv) {
            dispatch_async(dispatch_get_main_queue(), ^{
                [fingerEntryAlert dismiss];
            });
            return;
        }
        
        dispatch_async(dispatch_get_main_queue(), ^{
            [fingerEntryAlert setFingerNumberSum:enrollInfo.times];
            [fingerEntryAlert setFingerNumber:enrollInfo.nextIndex];
        });
        
        for (JUB_BYTE i=enrollInfo.nextIndex; i<enrollInfo.times; ++i) {
            
            //这个fingerNumber可以任意时候向他赋值,界面会自动更新
            dispatch_async(dispatch_get_main_queue(), ^{
                [fingerEntryAlert setFingerNumberSum:enrollInfo.times];
                [fingerEntryAlert setFingerNumber:enrollInfo.nextIndex];
                NSLog(@"fingerEntryAlert.fingerNumber = %ld.", fingerEntryAlert.fingerNumber);
            });
            
            times     = enrollInfo.times;
            fgptIndex = enrollInfo.nextIndex;
            fgptID    = enrollInfo.modalityID;
            enrollInfo = [self enroll_fgpt_test:deviceID
                                      fgptIndex:fgptIndex
                                          times:times
                                         fgptID:fgptID];
            if (JUBR_OK != enrollInfo.rv) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [fingerEntryAlert dismiss];
                });
                return;
            }
        }
        
        //这个fingerNumber可以任意时候向他赋值,界面会自动更新
        dispatch_async(dispatch_get_main_queue(), ^{
            [fingerEntryAlert setFingerNumberSum:enrollInfo.times];
            [fingerEntryAlert setFingerNumber:enrollInfo.nextIndex];
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
