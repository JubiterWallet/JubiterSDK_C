//
//  JUBSubPageController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/9.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBErrorCode.h"
#import "JUBSharedData.h"

#import "JUBScanDeviceInfo.h"
#import "JUBNotification.h"

#import "JUBSubPageController.h"


#pragma mark - NFC 通讯库寻卡回调
inline void NFCScanFuncCallBack(unsigned int errorCode,
                                const char* uuid,
                                unsigned int devType) {
    
//    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
//    if (nil == sharedData) {
//        return;
//    }
//
//    JUBSubPageController *selfClass = (JUBSubPageController*)data.selfClass;
    
    if (JUBR_OK != errorCode) {
        [cSelf addMsgData:[NSString stringWithFormat:@"[NFCScanFuncCallBackDev() return %@ (0x%iu).]", [JUBErrorCode GetErrMsg:errorCode], errorCode]];
        return;
    }
    [cSelf addMsgData:[NSString stringWithFormat:@"[NFCScanFuncCallBackDev() OK.]"]];
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_UINT16 deviceID = 0;
    rv = JUB_connectNFCDevice((JUB_BYTE_PTR)uuid,
                              &deviceID);
    if (JUBR_OK != rv) {
        [cSelf addMsgData:[NSString stringWithFormat:@"[JUB_connectNFCDevice() return %@ (0x%lu).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [cSelf addMsgData:[NSString stringWithFormat:@"[JUB_connectNFCDevice() OK.]"]];
    
    rv = JUB_isDeviceNFCConnect(deviceID);
    if(JUBR_OK != rv) {
        [cSelf addMsgData:[NSString stringWithFormat:@"[JUB_isDeviceNFCConnect() return %@ (0x%lu).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [cSelf addMsgData:[NSString stringWithFormat:@"[JUB_isDeviceNFCConnect() OK.]"]];
    
    [cSelf MenuOption:deviceID];
    
    rv = JUB_disconnectNFCDevice(deviceID);
    if (JUBR_OK != rv) {
        [cSelf addMsgData:[NSString stringWithFormat:@"[JUB_disconnectNFCDevice() return %@ (0x%lu).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [cSelf addMsgData:[NSString stringWithFormat:@"[JUB_disconnectNFCDevice() OK.]"]];
}


#pragma mark - BLE 通讯库寻卡回调
int BLEReadFuncCallBack(JUB_ULONG devHandle, JUB_BYTE_PTR data, JUB_UINT32 dataLen) {
    
    return -1;
}


void BLEScanFuncCallBack(JUB_BYTE_PTR devName, JUB_BYTE_PTR uuid, JUB_UINT32 type) {
    
    NSLog(@"Scan: [%s:%s:0x%u]", devName, uuid, type);
    
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


@interface JUBSubPageController ()

@end


@implementation JUBSubPageController
//@synthesize selfClass;
@synthesize optItem;

- (void) viewDidLoad {
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    cSelf = self;
    
    self.title = @"JuBiter SDK Demo";
}


- (void) viewDidDisappear:(BOOL)animated {
    
    [super viewDidDisappear:animated];
    cSelf = nil;
}


- (void) beginNFCSession {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    
    //通讯库调用
    NFC_DEVICE_INIT_PARAM param;
    param.scanCallBack = NFCScanFuncCallBack;
    rv = JUB_initNFCDevice(param);
    if (JUBR_OK != rv) {
        [cSelf addMsgData:[NSString stringWithFormat:@"[JUB_initNFCDevice() ERROR.]"]];
        return;
    }
    [cSelf addMsgData:[NSString stringWithFormat:@"[JUB_initNFCDevice() OK.]"]];
    
//    [data setSelfClass:self.selfClass];
    [sharedData setOptItem:self.optItem];
    
    if ([sharedData deviceCert]) {
        JUB_CHAR_PTR sn = nullptr;
        JUB_CHAR_PTR subjectID = nullptr;
        rv = JUB_ParseDeviceCert((char*)[[sharedData deviceCert] UTF8String], &sn, &subjectID);
        if (JUBR_OK != rv) {
            [cSelf addMsgData:[NSString stringWithFormat:@"[JUB_ParseDeviceCert() ERROR.]"]];
            return;
        }
        [cSelf addMsgData:[NSString stringWithFormat:@"[JUB_ParseDeviceCert() OK.]"]];
        
        NSString *subID = [NSString stringWithFormat:@"%s", subjectID];
        
        if (sn) {
            JUB_FreeMemory(sn);
        }
        if (subjectID) {
            JUB_FreeMemory(subjectID);
        }
        
//        std::string fileName = "42584E46433230303532353030303031_apk";
        std::string fileName = "42584E46433230303532353030303031_oce";
//        std::string fileName = "42584E46433230303532353030303032_apk";
        NSString *filePath = [[NSBundle mainBundle] pathForResource:[NSString stringWithFormat:@"%s", fileName.c_str()]
                                                             ofType:@"settings"];
        Json::Value root = readJSON([filePath UTF8String]);
        
        NFC_DEVICE_SET_PARAM param;
        param.crt = (char*)root["SCP11c"]["OCE"][1][0].asCString();
        param.sk  = (char*)root["SCP11c"]["OCE"][1][2].asCString();
        param.hostID = (char*)root["SCP11c"]["HostID"].asCString();
        param.keyLength = root["SCP11c"]["KeyLength"].asUInt();
        param.cardGroupID = (char*)[subID UTF8String];
        rv = JUB_setNFCDeviceParam(param);
        if (JUBR_OK != rv) {
            [cSelf addMsgData:[NSString stringWithFormat:@"[JUB_setNFCDeviceParam() ERROR.]"]];
            return;
        }
        [cSelf addMsgData:[NSString stringWithFormat:@"[JUB_setNFCDeviceParam() OK.]"]];
    }
}


- (void) beginBLESession {
    
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    
//    [data setSelfClass:self.selfClass];
    [sharedData setOptItem:self.optItem];
    
    [self MenuOption:[sharedData currDeviceID]];
}


#pragma mark - 操作菜单 通讯库寻卡回调
- (void) MenuOption:(NSUInteger)deviceID {
    
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    
//    JUBSubPageController *selfClass = (JUBSubPageController*)data.selfClass;
    
    switch ([sharedData optItem]) {
    case JUB_NS_ENUM_MAIN::OPT_DEVICE:
    {
        JUBAlertView *alertView = [JUBAlertView showMsg:@"Processing..."];
        {
            [self DeviceOpt:deviceID];
            [alertView dismiss];
        }
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
    case JUB_NS_ENUM_MAIN::OPT_ETH_UNISWAP:
    {
        [self CoinETHUNISWAPOpt:deviceID];
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
    case JUB_NS_ENUM_MAIN::OPT_TRX:
    {
        [self CoinTRXOpt:deviceID];
        break;
    }
    case JUB_NS_ENUM_MAIN::OPT_FIL:
    {
        [self CoinFILOpt:deviceID];
        break;
    }
    default:
        break;
    }   // switch (data.optItem) end
}


#pragma mark - Device 通讯库寻卡回调
- (void) DeviceOpt:(NSUInteger)deviceID {
    
}


#pragma mark - BTC 通讯库寻卡回调
- (void) CoinBTCOpt:(NSUInteger)deviceID {
    
}


#pragma mark - ETH 通讯库寻卡回调
- (void) CoinETHOpt:(NSUInteger)deviceID {
    
}
#pragma mark - ETH-UNISWAP 通讯库寻卡回调
- (void) CoinETHUNISWAPOpt:(NSUInteger)deviceID {
    
}


#pragma mark - EOS 通讯库寻卡回调
- (void) CoinEOSOpt:(NSUInteger)deviceID {
    
}


#pragma mark - XRP 通讯库寻卡回调
- (void) CoinXRPOpt:(NSUInteger)deviceID {
    
}


#pragma mark - TRX 通讯库寻卡回调
- (void) CoinTRXOpt:(NSUInteger)deviceID {
    
}


#pragma mark - FIL 通讯库寻卡回调
- (void) CoinFILOpt:(NSUInteger)deviceID {
    
}

@end
