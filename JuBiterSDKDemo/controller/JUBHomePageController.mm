//
//  JUBHomePageController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/13.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBErrorCode.h"
#import "JUBSharedData.h"

#import "JUBScanDeviceInfo.h"
#import "JUBDevicePool.h"
#import "JUBNotification.h"

#import "JUBHomePageController.h"
#import "JUBSendApduController.h"
#import "JUBFgptMgrController.h"
#import "JUBDeviceController.h"
#import "JUBBTCController.h"
#import "JUBETHController.h"
#import "JUBEOSController.h"
#import "JUBXRPController.h"
#import "JUBTRXController.h"
#import "JUBFILController.h"


@interface JUBHomePageController ()
@property (nonatomic, weak) UIButton *disConnectBLEButton;
@property (nonatomic, weak) NSTimer* scanDeviceTimer;
@property (nonatomic, strong) NSMutableDictionary* scanDeviceDict;
@property (nonatomic, strong) NSMutableDictionary* connDeviceDict;
@property (nonatomic, weak) JUBListAlert* BLEDeviceScanListView;
@end


@implementation JUBHomePageController
@synthesize disConnectBLEButton;
@synthesize scanDeviceTimer;
@synthesize scanDeviceDict;
@synthesize connDeviceDict;


- (void) viewDidLoad {
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.title = @"JuBiter SDK Demo";
    self.leftNAVButtonTitle  = @"Scan BLE";
    self.rightNAVButtonTitle = @"View BLE";
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(deviceArrivalObserver:)
                                                 name:NSNotificationName(NOTIF_DEVICE_ARRVL)
                                               object:[JUBDevicePool shareDevicePool]];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(deviceScanObserver:)
                                                 name:NSNotificationName(NOTIF_DEVICE_SCAN)
                                               object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(deviceDiscObserver:)
                                                 name:NSNotificationName(NOTIF_DEVICE_DISC)
                                               object:nil];
    
    [self initData];
    
    [self initUI];
}


- (void) initData {
    
    JUB_NS_ENUM_DEV_TYPE defaultDeviceType = JUB_NS_ENUM_DEV_TYPE::SEG_NFC;
    
    NSString *indexStr = [[NSUserDefaults standardUserDefaults] objectForKey:selectedTransmitTypeIndexStr];
    if (!indexStr) {
        [[NSUserDefaults standardUserDefaults] setObject:[NSString stringWithFormat:@"%ld", (long)JUB_NS_ENUM_DEV_TYPE::SEG_NFC]
                                                  forKey:selectedTransmitTypeIndexStr];
    }
    else {
        defaultDeviceType = (JUB_NS_ENUM_DEV_TYPE)[indexStr integerValue];
    }
    
    self->scanDeviceDict = [[NSMutableDictionary alloc] init];
    self->connDeviceDict = [[NSMutableDictionary alloc] init];
    
    [[JUBSharedData sharedInstance] setDeviceType:defaultDeviceType];
}


#pragma mark - 初始化UI
- (void) initUI {
    
    if (JUB_NS_ENUM_DEV_TYPE::SEG_BLE == [[JUBSharedData sharedInstance] deviceType]) {
        self.showBLEButton = YES;
    }
}


- (void) updateBLEDeviceList {
    
    [self.BLEDeviceScanListView removeAllItems];
    
    for (id device in self->scanDeviceDict) {
        JUBScanDeviceInfo* deviceInfo = [self->scanDeviceDict objectForKey:device];
        [self.BLEDeviceScanListView addItem:[deviceInfo getName]];
    }
}


#pragma mark - 页面内部按钮回调方法
- (void) segmentAction:(UISegmentedControl *)seg {
    
    NSInteger index = [seg selectedSegmentIndex];
    
    [[NSUserDefaults standardUserDefaults] setObject:[NSString stringWithFormat:@"%ld", (long)index]
                                              forKey:selectedTransmitTypeIndexStr];
    
    [[NSUserDefaults standardUserDefaults] synchronize];
    
    [self selectTransmitTypeIndex:index];
    
    NSLog(@"segmentAction index = %ld", (long)index);
    
    [[JUBSharedData sharedInstance] setDeviceType:(JUB_NS_ENUM_DEV_TYPE)index];
}


- (JUBScanDeviceInfo*) searchScanDeviceInfo:deviceName {
    
    for (id device in self->scanDeviceDict) {
        JUBScanDeviceInfo* deviceInfo = [self->scanDeviceDict objectForKey:device];
        if ([deviceInfo getName] == deviceName) {
            return deviceInfo;
        }
    }
    
    return nil;
}


- (NSNumber*) searchConnDeviceInfo:deviceName {
    
    for (id deviceID in self->connDeviceDict) {
        JUBScanDeviceInfo* deviceInfo = [self->connDeviceDict objectForKey:deviceID];
        if (NSComparisonResult::NSOrderedSame == [[deviceInfo getName] compare:deviceName]) {
            return deviceID;
        }
    }
    
    return nil;
}


- (void) selectTransmitTypeIndex:(NSInteger)index {
    
    switch (index) {
    case JUB_NS_ENUM_DEV_TYPE::SEG_BLE:
    {
        self.showBLEButton = YES;
        break;
    }
    case JUB_NS_ENUM_DEV_TYPE::SEG_NFC:
    {
        self.showBLEButton = NO;
        
        if (JUBR_OK != JUB_stopEnumDevices()) {

            [self addMsgData:[NSString stringWithFormat:@"[JUB_stopEnumDevices() ERROR.]"]];
            return;
        }
        [self addMsgData:[NSString stringWithFormat:@"[JUB_stopEnumDevices() OK.]"]];
        
        NSArray *buttonTitleArray = @[
            BUTTON_TITLE_FGPT
        ];
        BOOL disEnable = NO;
        [self updateButtons:buttonTitleArray
                  disEnable:disEnable];
        
//        [scanDeviceTimer invalidate];
//        scanDeviceTimer = nil;
        break;
    }
    default:
        break;
    }   // switch (index) end
}


#pragma mark - 蓝牙通讯库寻卡回调
- (void) deviceArrivalObserver:notification {
    
    dispatch_async(dispatch_get_main_queue(), ^{
        //通讯库调用
        DEVICE_INIT_PARAM param;
        param.callBack     = BLEReadFuncCallBack;
        param.scanCallBack = BLEScanFuncCallBack;
        param.discCallBack = BLEDiscFuncCallBack;
        JUB_RV rv = JUB_initDevice(param);
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_initDevice() return %@ (0x%lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
            return;
        }
        
        [self addMsgData:[NSString stringWithFormat:@"[JUB_initDevice() OK.]"]];
    });
}


- (void) deviceScanObserver:(NSNotification *) notification {
    
    if (nil == [scanDeviceDict objectForKey:[[notification userInfo] objectForKey:SCAN_DEVICE_INFO_UUID]]) {
        
        [scanDeviceDict setObject:[[notification userInfo] objectForKey:SCAN_DEVICE_INFO_DEVICE]
                           forKey:[[notification userInfo] objectForKey:SCAN_DEVICE_INFO_UUID]];
    }
    
    [self updateBLEDeviceList];
}


- (void) deviceDiscObserver:(NSNotification *) notification {
    
//    if (nil == [deviceDict objectForKey:[[notification userInfo] objectForKey:SCAN_DEVICE_INFO_UUID]]) {
//
//        [deviceDict setObject:[[notification userInfo] objectForKey:SCAN_DEVICE_INFO_DEVICE]
//                       forKey:[[notification userInfo] objectForKey:SCAN_DEVICE_INFO_UUID]];
//    }
//
    [self updateBLEDeviceList];
}


#pragma mark - 懒加载
- (NSInteger) selectedTransmitTypeIndex {
    
    NSString *indexStr = [[NSUserDefaults standardUserDefaults] objectForKey:selectedTransmitTypeIndexStr];
    
    return [indexStr integerValue];
}


- (NSArray *) getTransmitTypeArray {
    
    return @[
        BUTTON_TITLE_NFC,
        BUTTON_TITLE_BLE
    ];
}


//设置首页列表内容
- (NSArray<JUBButtonModel *> *) buttonArray {
    
    return [self updateButtons:nil
                     disEnable:NO];
}


- (NSMutableArray *) updateButtons:(NSArray*)updateArray
                         disEnable:(BOOL)disEnable {
    
    NSArray *buttonTitleArray = @[
        BUTTON_TITLE_SENDAPDU,
        BUTTON_TITLE_FGPT,
        BUTTON_TITLE_DEVICE,
        BUTTON_TITLE_BTC,
        BUTTON_TITLE_ETH,
        BUTTON_TITLE_EOS,
        BUTTON_TITLE_XRP,
        BUTTON_TITLE_TRX,
        BUTTON_TITLE_FIL,
    ];
    
    NSMutableArray *buttonModelArray = [NSMutableArray array];
    for (NSString *title in buttonTitleArray) {
        JUBButtonModel *model = [[JUBButtonModel alloc] init];
        model.title = title;
        model.isNeedMainThread = YES;
        
        if (updateArray) {
            for (NSString *updateTitle in updateArray) {
                if (title == updateTitle) {
                    model.disEnable = disEnable;
                    break;
                }
            }
        }
        
        [buttonModelArray addObject:model];
    }
    
    if (updateArray) {
        self.buttonArray = buttonModelArray;
    }
    
    return buttonModelArray;
}


- (void) buttonConfiguration {
    
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    
    if (-1 == [sharedData currDeviceID]) {
        return;
    }
    
    NSUInteger rv = JUBR_ERROR;
    
    JUB_ENUM_COMMODE comMode  = JUB_ENUM_COMMODE::COMMODE_NS_ITEM;
    JUB_ENUM_DEVICE deviceClass = JUB_ENUM_DEVICE::DEVICE_NS_ITEM;
    rv = JUB_GetDeviceType([sharedData currDeviceID], &comMode, &deviceClass);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetDeviceType() return %@ (0x%lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
    }
    else {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetDeviceType(%d:%d) OK.]", comMode, deviceClass]];
        
        [sharedData setComMode:comMode];
        [sharedData setDeviceClass:deviceClass];
        
        NSArray *buttonTitleArray = @[
            BUTTON_TITLE_FGPT
        ];
        BOOL disEnable = NO;
        if (JUB_ENUM_DEVICE::BIO != deviceClass) {
            disEnable = YES;
        }
        [self updateButtons:buttonTitleArray
                  disEnable:disEnable];
    }
}


//首页按钮点击响应事件
- (void) doActionAccordingOption:(NSInteger)optType {
    
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    [sharedData setCurrCoinType:optType];
    
    if (JUB_NS_ENUM_DEV_TYPE::SEG_BLE == [sharedData deviceType]
        &&                          0 == [sharedData currDeviceID]
        ) {
        NSString *connDevAlertString = @"Please connect the device first...";
        JUBAlertView *alertView = [JUBAlertView showMsg:connDevAlertString];
        
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW,
                                     (int64_t)(1 * NSEC_PER_SEC)),
                       dispatch_get_main_queue(),
                       ^ {
            [alertView dismiss];
            [self addMsgData:[NSString stringWithFormat:connDevAlertString]];
        });
        return;
    }
    
    JUBInputOrderBaseController *sendApduVc = nil;
    JUBFingerManagerBaseController *fgptVc = nil;
    JUBSubPageController *vc = nil;
    switch (optType) {
    case JUB_NS_ENUM_MAIN::OPT_SENDAPDU:
    {
        sendApduVc = [[JUBSendApduController alloc] init];
        break;
    }
    case JUB_NS_ENUM_MAIN::OPT_FGPT:
    {
        fgptVc = [[JUBFgptMgrController alloc] init];
        break;
    }
    case JUB_NS_ENUM_MAIN::OPT_DEVICE:
    {
        vc = [[JUBDeviceController alloc] init];
        break;
    }
    case JUB_NS_ENUM_MAIN::OPT_BTC:
    {
        vc = [[JUBBTCController alloc] init];
        break;
    }
    case JUB_NS_ENUM_MAIN::OPT_ETH:
    {
        vc = [[JUBETHController alloc] init];
        break;
    }
    case JUB_NS_ENUM_MAIN::OPT_EOS:
    {
        vc = [[JUBEOSController alloc] init];
        break;
    }
    case JUB_NS_ENUM_MAIN::OPT_XRP:
    {
        vc = [[JUBXRPController alloc] init];
        break;
    }
    case JUB_NS_ENUM_MAIN::OPT_TRX:
    {
        vc = [[JUBTRXController alloc] init];
        break;
    }
    case JUB_NS_ENUM_MAIN::OPT_FIL:
    {
        vc = [[JUBFILController alloc] init];
        break;
    }
    default:
        return;
    }   // switch (optType) end
    
    if (vc) {
        [self.navigationController pushViewController:vc
                                             animated:YES];
    }
    else if (sendApduVc) {
        [self.navigationController pushViewController:sendApduVc
                                             animated:YES];
    }
    else if (fgptVc) {
        [self.navigationController pushViewController:fgptVc
                                             animated:YES];
    }
}


- (void) leftNAVButtonClicked {
    
    NSLog(@"JUBHomePageController::scanBLEButtonClicked");
    
    __block
    JUB_RV rv = JUBR_ERROR;
    
    __block
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    
    rv = JUB_enumDevices();
    if (JUBR_OK != rv) {
        
        [self addMsgData:[NSString stringWithFormat:@"[JUB_enumDevices() return %@ (0x%lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_enumDevices() OK.]"]];
    
    self.BLEDeviceScanListView = [JUBListAlert showCallBack:^(NSString *_Nonnull deviceName) {
        
        NSLog(@"deviceName = %@", deviceName);
        JUBScanDeviceInfo *deviceInfo = [self searchScanDeviceInfo:deviceName];
        if (!deviceInfo) {
            return;
        }
        
//        JUBAlertView *alertView = [JUBAlertView showMsg:@"Connecting BLE device..."];
//        //蓝牙连接成功之后将alertView隐藏掉，延时可以去掉，需要隐藏的时候直接隐藏即可
//        dispatch_after(dispatch_time(DISPATCH_TIME_NOW,
//                                     (int64_t)(5 * NSEC_PER_SEC)),
//                       dispatch_get_main_queue(),
//                       ^ {
//            [alertView dismiss];
//        });
            
        JUBAlertView *alertView = [JUBAlertView showMsg:@"Connecting BLE device..."];
        {
            JUB_UINT16 deviceID = 0;
            rv = JUB_connectDevice((JUB_BYTE_PTR)[deviceInfo.name UTF8String],
                                   (JUB_BYTE_PTR)[deviceInfo.uuid UTF8String],
                                   (JUB_UINT32)deviceInfo.type,
                                   &deviceID, 30000);
            if (JUBR_OK != rv) {
                [self addMsgData:[NSString stringWithFormat:@"[JUB_connectDevice() return %@ (0x%lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
            }
            else {
                [self addMsgData:[NSString stringWithFormat:@"[JUB_connectDevice(%@ - %d) OK.]", deviceInfo.name, deviceID]];
                
                [self->connDeviceDict setObject:deviceInfo
                                         forKey:[NSNumber numberWithUnsignedInteger:deviceID]];
                
                [sharedData setCurrDeviceID:deviceID];
                [sharedData setCurrContextID:0];
                [self buttonConfiguration];
            }
            [alertView dismiss];
        }
    }];
    
    self.BLEDeviceScanListView.title = @"Please select the BLE device:";
//
//    scanDeviceTimer = [NSTimer scheduledTimerWithTimeInterval:10
//                                                      repeats:YES
//                                                        block:^(NSTimer * _Nonnull timer) {
//        rv = JUB_enumDevices();
//        if (JUBR_OK != rv) {
//
//            [self addMsgData:[NSString stringWithFormat:@"[JUB_enumDevices() ERROR.]"]];
//            return;
//        }
//        [self addMsgData:[NSString stringWithFormat:@"[JUB_enumDevices() OK.]"]];
//    }];
}


//断开连接按钮回调
- (void) rightNAVButtonClicked {
    
    NSLog(@"disConnectBLEButtonClicked");
    
    __block
    JUB_RV rv = JUBR_ERROR;
    
    __block
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    
    __block
    NSNumber* optDeviceID = nil;
    __block
    NSUInteger currDeviceID = -1;
    JUBListAlert *listAlert = [JUBListAlert showCallBack:^(NSString *_Nonnull selectedItem) {
        
        if (!selectedItem) {
            NSLog(@"disConnectBLEButtonClicked::JUBListAlert canceled");
            return;
        }
        
        NSLog(@"selectedItem = %@", selectedItem);
        NSString* deviceName = [selectedItem stringByReplacingOccurrencesOfString:FLAG_CURR
                                                                       withString:@""];
        optDeviceID = [self searchConnDeviceInfo:deviceName];
        if (!optDeviceID) {
            return;
        }
        
        currDeviceID = [optDeviceID intValue];
        rv = JUB_isDeviceConnect(currDeviceID);
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_isDeviceConnect() return %@ (0x%lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
            [self->connDeviceDict removeObjectForKey:optDeviceID];
            
            currDeviceID = -1;
            if (0 < [self->connDeviceDict count]) {
                currDeviceID = [[[self->connDeviceDict allKeys] objectAtIndex:0] intValue];
            }
        }
        else {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_isDeviceConnect(%@ - %ld) OK.]", selectedItem, currDeviceID]];
        }
        
        [sharedData setCurrDeviceID:currDeviceID];
        [sharedData setCurrContextID:0];
        [self buttonConfiguration];
    }];
    
    NSMutableArray *deviceNames = [[NSMutableArray alloc] init];
    for (id deviceID in self->connDeviceDict) {
        JUBScanDeviceInfo* deviceInfo = [self->connDeviceDict objectForKey:deviceID];
        NSString *suffix = @"";
        if ([deviceID intValue] == [sharedData currDeviceID]) {
            suffix = FLAG_CURR;
        }
        NSString *flagName = [NSString stringWithFormat:@"%@%@", [deviceInfo getName], suffix];
        [deviceNames addObject:flagName];
    }
    
    listAlert.title = @"Please select the BLE device:";
    [listAlert addItems:deviceNames];
    [listAlert setTextAlignment:NSTextAlignment::NSTextAlignmentLeft];
    
    [listAlert setDeleteCellCallBackBlock:^(NSString * _Nullable deletedItem) {
        
        if (!deletedItem) {
            NSLog(@"disConnectBLEButtonClicked::JUBListAlert canceled");
            return;
        }
        
        NSLog(@"deletedItem = %@", deletedItem);
        NSString* deviceName = [deletedItem stringByReplacingOccurrencesOfString:FLAG_CURR
                                                                      withString:@""];
        optDeviceID = [self searchConnDeviceInfo:deviceName];
        if (!optDeviceID) {
            return;
        }
        
//        JUBAlertView *alertView = [JUBAlertView showMsg:@"Disconnecting BLE device..."];
//
//        //蓝牙连接成功之后将alertView隐藏掉，延时可以去掉，需要隐藏的时候直接隐藏即可
//        dispatch_after(dispatch_time(DISPATCH_TIME_NOW,
//                                     (int64_t)(3 * NSEC_PER_SEC)),
//                       dispatch_get_main_queue(),
//                       ^ {
//            [alertView dismiss];
//        });
        
        JUBAlertView *alertView = [JUBAlertView showMsg:@"Disconnecting BLE device..."];
        {
            currDeviceID = [optDeviceID intValue];
            rv = JUB_disconnectDevice(currDeviceID);
            if (JUBR_OK != rv) {
                [self addMsgData:[NSString stringWithFormat:@"[JUB_disconnectDevice() return %@ (0x%lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
            }
            else {
                [self addMsgData:[NSString stringWithFormat:@"[JUB_disconnectDevice(%@ - %ld) OK.]", deletedItem, currDeviceID]];
                [self->connDeviceDict removeObjectForKey:optDeviceID];
                
                NSUInteger currDeviceID = -1;
                if (0 < [self->connDeviceDict count]) {
                    currDeviceID = [[[self->connDeviceDict allKeys] objectAtIndex:0] intValue];
                }
                [sharedData setCurrDeviceID:currDeviceID];
                [sharedData setCurrContextID:0];
                [self buttonConfiguration];
            }
            [alertView dismiss];
        }
    }];
}


@end
