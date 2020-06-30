//
//  JUBDetailBaseController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/9.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBDetailBaseController.h"


#pragma mark - NFC 通讯库寻卡回调
inline void NFCScanFuncCallBack(unsigned int errorCode,
                                const char* uuid,
                                unsigned int devType) {
    
    [g_selfClass addMsgData:[NSString stringWithFormat:@"[NFCScanFuncCallBackDev() return 0x%iu.]", errorCode]];
    if (JUBR_OK != errorCode) {
        [g_selfClass addMsgData:[NSString stringWithFormat:@"[NFCScanFuncCallBackDev() return 0x%iu.]", errorCode]];
        
        return;
    }
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_UINT16 deviceID = 0;
    rv = JUB_connectNFCDevice((JUB_BYTE_PTR)uuid,
                              &deviceID);
    [g_selfClass addMsgData:[NSString stringWithFormat:@"[JUB_connectNFCDevice() return 0x%lu.]", rv]];
    if (JUBR_OK != rv) {
        [g_selfClass addMsgData:[NSString stringWithFormat:@"[JUB_connectNFCDevice() ERROR.]"]];
        
        return;
    }
    [g_selfClass addMsgData:[NSString stringWithFormat:@"[JUB_connectNFCDevice() OK.]"]];
    
    rv = JUB_isDeviceNFCConnect(deviceID);
    [g_selfClass addMsgData:[NSString stringWithFormat:@"[JUB_isDeviceNFCConnect() return 0x%lu.]", rv]];
    if(JUBR_OK != rv) {
        [g_selfClass addMsgData:[NSString stringWithFormat:@"[Device not connected.]"]];
        
        return;
    }
    [g_selfClass addMsgData:[NSString stringWithFormat:@"[Device connected]"]];
    
    switch (g_optItem) {
    case JUB_NS_ENUM_MAIN::OPT_DEVICE:
        [g_selfClass DeviceOpt:deviceID];
        break;
    case JUB_NS_ENUM_MAIN::OPT_BTC:
        [g_selfClass CoinBTCOpt:deviceID];
        break;
    case JUB_NS_ENUM_MAIN::OPT_ETH:
        [g_selfClass CoinETHOpt:deviceID];
        break;
    case JUB_NS_ENUM_MAIN::OPT_EOS:
        [g_selfClass CoinEOSOpt:deviceID];
        break;
    case JUB_NS_ENUM_MAIN::OPT_XRP:
        [g_selfClass CoinXRPOpt:deviceID];
        break;
    default:
        break;
    }
    
    rv = JUB_disconnectNFCDevice(deviceID);
    [g_selfClass addMsgData:[NSString stringWithFormat:@"[JUB_disconnectNFCDevice() return 0x%lu.]", rv]];
}


#pragma mark - BLE 通讯库寻卡回调
inline int BLEReadFuncCallBack(JUB_ULONG devHandle, JUB_BYTE_PTR data, JUB_UINT32 dataLen) {
    return -1;
}


inline void BLEScanFuncCallBack(JUB_BYTE_PTR devName, JUB_BYTE_PTR uuid, JUB_UINT32 type) {
    
}


inline void BLEDiscFuncCallBack(JUB_BYTE_PTR uuid) {
    
}


@interface JUBDetailBaseController ()

@end


@implementation JUBDetailBaseController
@synthesize selfClass;
@synthesize optItem;


- (void)beginNFCSession {
    
    g_selfClass = self.selfClass;
    g_optItem = self.optItem;
    g_filePath = "42584E46433230303532353030303031_apk";
//    g_filePath = "42584E46433230303532353030303032_apk";
    NSString *filePath = [[NSBundle mainBundle] pathForResource:[NSString stringWithFormat:@"%s", g_filePath.c_str()]
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
        [selfClass addMsgData:[NSString stringWithFormat:@"[JUB_initNFCDevice() ERROR.]"]];
        
        return;
    }
    [selfClass addMsgData:[NSString stringWithFormat:@"[JUB_initNFCDevice() OK.]"]];
}


- (void)beginBLESession {
    
    g_selfClass = self.selfClass;
    g_optItem   = self.optItem;
    
    //通讯库调用
    DEVICE_INIT_PARAM param;
    param.callBack     = BLEReadFuncCallBack;
    param.scanCallBack = BLEScanFuncCallBack;
    param.discCallBack = BLEDiscFuncCallBack;
    JUB_RV rv = JUB_initDevice(param);
    if (JUBR_OK != rv) {
        [selfClass addMsgData:[NSString stringWithFormat:@"[JUB_initDevice() ERROR.]"]];
        
        return;
    }
    [selfClass addMsgData:[NSString stringWithFormat:@"[JUB_initDevice() OK.]"]];
    
    rv = JUB_enumDevices();
    if (JUBR_OK != rv) {
        [selfClass addMsgData:[NSString stringWithFormat:@"[JUB_enumDevices()ERROR.]"]];
        
        return;
    }
    [selfClass addMsgData:[NSString stringWithFormat:@"[JUB_enumDevices() 成功.]"]];
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
