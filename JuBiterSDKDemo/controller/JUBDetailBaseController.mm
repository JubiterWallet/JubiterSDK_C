//
//  JUBDetailBaseController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/9.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBSharedData.h"

#import "JUBDetailBaseController.h"
#import "JUBScanDeviceInfo.h"
#import "JUBNotification.h"

#pragma mark - NFC 通讯库寻卡回调
inline void NFCScanFuncCallBack(unsigned int errorCode,
                                const char* uuid,
                                unsigned int devType) {
    
//    JUBSharedData *data = [JUBSharedData sharedInstance];
//    JUBDetailBaseController *selfClass = (JUBDetailBaseController*)data.selfClass;
    
    if (JUBR_OK != errorCode) {
        [cSelf addMsgData:[NSString stringWithFormat:@"[NFCScanFuncCallBackDev() return 0x%iu.]", errorCode]];
        
        return;
    }
    [cSelf addMsgData:[NSString stringWithFormat:@"[NFCScanFuncCallBackDev() OK.]"]];
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_UINT16 deviceID = 0;
    rv = JUB_connectNFCDevice((JUB_BYTE_PTR)uuid,
                              &deviceID);
    if (JUBR_OK != rv) {
        [cSelf addMsgData:[NSString stringWithFormat:@"[JUB_connectNFCDevice() return 0x%lu.]", rv]];
        return;
    }
    [cSelf addMsgData:[NSString stringWithFormat:@"[JUB_connectNFCDevice() OK.]"]];
    
    rv = JUB_isDeviceNFCConnect(deviceID);
    if(JUBR_OK != rv) {
        [cSelf addMsgData:[NSString stringWithFormat:@"[JUB_isDeviceNFCConnect() return 0x%lu.]", rv]];
        return;
    }
    [cSelf addMsgData:[NSString stringWithFormat:@"[JUB_isDeviceNFCConnect() OK.]"]];
    
    [cSelf MenuOption:deviceID];
    
    rv = JUB_disconnectNFCDevice(deviceID);
    if (JUBR_OK != rv) {
        [cSelf addMsgData:[NSString stringWithFormat:@"[JUB_disconnectNFCDevice() return 0x%lu.]", rv]];
        return;
    }
    [cSelf addMsgData:[NSString stringWithFormat:@"[JUB_disconnectNFCDevice() OK.]"]];
}


#pragma mark - BLE 通讯库寻卡回调
int BLEReadFuncCallBack(JUB_ULONG devHandle, JUB_BYTE_PTR data, JUB_UINT32 dataLen) {
    
    return -1;
}


void BLEScanFuncCallBack(JUB_BYTE_PTR devName, JUB_BYTE_PTR uuid, JUB_UINT32 type) {
    
    NSLog(@"Scan: [%s:%s:%ul]", devName, uuid, type);
    
    JUBScanDeviceInfo* deviceInfo = [[JUBScanDeviceInfo alloc] init];
    deviceInfo.name = [NSString stringWithCString:(char*)devName
                                         encoding:NSUTF8StringEncoding];
    deviceInfo.uuid = [NSString stringWithCString:(char*)uuid
                                         encoding:NSUTF8StringEncoding];
    deviceInfo.type = NSUInteger(type);
    
    [[NSNotificationCenter defaultCenter] postNotificationName:NOTIF_DEVICE_SCAN
                                                        object:nil
                                                      userInfo:@{SCAN_DEVICE_INFO_UUID:[NSString stringWithCString:(char*)uuid
                                                                                            encoding:NSUTF8StringEncoding],
                                                                 SCAN_DEVICE_INFO_DEVICE:deviceInfo}];
}


void BLEDiscFuncCallBack(JUB_BYTE_PTR uuid) {
    
    NSLog(@"Disc: [%s]", uuid);
    
    [[NSNotificationCenter defaultCenter] postNotificationName:NOTIF_DEVICE_DISC
                                                        object:nil
                                                      userInfo:@{SCAN_DEVICE_INFO_UUID:[NSString stringWithCString:(char*)uuid
                                                                                            encoding:NSUTF8StringEncoding]}];
}


@interface JUBDetailBaseController ()

@end


@implementation JUBDetailBaseController
//@synthesize selfClass;
@synthesize optItem;

- (void)viewDidLoad {
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    cSelf = self;
}


- (void)viewDidDisappear:(BOOL)animated {
    
    [super viewDidDisappear:animated];
    cSelf = nil;
}


- (void)beginNFCSession {
    
    JUBSharedData *data = [JUBSharedData sharedInstance];
//    [data setSelfClass:self.selfClass];
    data.optItem = self.optItem;
    
    std::string fileName = "42584E46433230303532353030303031_apk";
//    std::string fileName = "42584E46433230303532353030303032_apk";
    NSString *filePath = [[NSBundle mainBundle] pathForResource:[NSString stringWithFormat:@"%s", fileName.c_str()]
                                                         ofType:@"settings"];
    Json::Value root = readJSON([filePath UTF8String]);
    
    //通讯库调用
    NFC_DEVICE_INIT_PARAM param;
    param.scanCallBack = NFCScanFuncCallBack;
    param.crt = (char*)root["SCP11c"]["OCE"][1][0].asCString();
    param.sk  = (char*)root["SCP11c"]["OCE"][1][2].asCString();
    param.hostID = (char*)root["SCP11c"]["HostID"].asCString();
    param.keyLength = root["SCP11c"]["KeyLength"].asUInt();
    JUB_RV rv = JUB_initNFCDevice(param);
    if (JUBR_OK != rv) {
        [cSelf addMsgData:[NSString stringWithFormat:@"[JUB_initNFCDevice() ERROR.]"]];
        
        return;
    }
    [cSelf addMsgData:[NSString stringWithFormat:@"[JUB_initNFCDevice() OK.]"]];
}


- (void)beginBLESession {
    
    JUBSharedData *data = [JUBSharedData sharedInstance];
//    [data setSelfClass:self.selfClass];
    data.optItem = self.optItem;
    
    [self MenuOption:[[data currDeviceID] intValue]];
}


#pragma mark - 操作菜单 通讯库寻卡回调
- (void)MenuOption:(JUB_UINT16)deviceID {
    
    JUBSharedData *data = [JUBSharedData sharedInstance];
//    JUBDetailBaseController *selfClass = (JUBDetailBaseController*)data.selfClass;
    
    switch (data.optItem) {
    case JUB_NS_ENUM_MAIN::OPT_DEVICE:
    {
        [self DeviceOpt:deviceID];
        break;
    }
    case JUB_NS_ENUM_MAIN::OPT_BTC:
    {
        [self CoinBTCOpt:deviceID];
        break;
    }
    case JUB_NS_ENUM_MAIN::OPT_ETH:
    {
        [self CoinETHOpt:deviceID];
        break;
    }
    case JUB_NS_ENUM_MAIN::OPT_EOS:
    {
        [self CoinEOSOpt:deviceID];
        break;
    }
    case JUB_NS_ENUM_MAIN::OPT_XRP:
    {
        [self CoinXRPOpt:deviceID];
        break;
    }
    default:
        break;
    }   // switch (data.optItem) end
}


#pragma mark - Device 通讯库寻卡回调
- (void)DeviceOpt:(JUB_UINT16)deviceID {
    
}


#pragma mark - BTC 通讯库寻卡回调
- (void)CoinBTCOpt:(JUB_UINT16)deviceID {
    
}


#pragma mark - ETH 通讯库寻卡回调
- (void)CoinETHOpt:(JUB_UINT16)deviceID {
    
}


#pragma mark - EOS 通讯库寻卡回调
- (void)CoinEOSOpt:(JUB_UINT16)deviceID {
    
}


#pragma mark - XRP 通讯库寻卡回调
- (void)CoinXRPOpt:(JUB_UINT16)deviceID {
    
}


@end
