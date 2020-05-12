//
//  JUBController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/9.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBController.h"


@interface JUBController ()

@end


@implementation JUBController
@synthesize selfClass;
@synthesize optItem;

//测试类型的按钮点击回调
- (void)selectedTestActionTypeIndex:(NSInteger)index {
    
    NSLog(@"JUBBTCController--coinType = %ld, selectedTestActionType %ld", (long)self.selectCoinTypeIndex, (long)index);
}


- (void)beginNFCSession {
    
    g_selfClass = self.selfClass;
    g_optItem = self.optItem;
    
    //通讯库调用
    NFC_DEVICE_INIT_PARAM param;
    param.scanCallBack = NFCScanFuncCallBack;
    JUB_RV rv = JUB_initNFCDevice(param);
    if (JUBR_OK != rv) {
        [selfClass addMsgData:[NSString stringWithFormat:@"[JUB_initNFCDevice() 失败.]"]];
        
        return;
    }
    
    [selfClass addMsgData:[NSString stringWithFormat:@"[JUB_initNFCDevice() 成功.]"]];
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


#pragma mark - 通讯库寻卡回调
void NFCScanFuncCallBack(unsigned int errorCode,
                         const char* uuid,
                         unsigned int devType) {
    
    JUB_RV rv = JUBR_ERROR;
    
    [g_selfClass addMsgData:[NSString stringWithFormat:@"[NFCScanFuncCallBackDev() return 0x%iu.]", errorCode]];
    if (JUBR_OK != errorCode) {
        [g_selfClass addMsgData:[NSString stringWithFormat:@"[NFCScanFuncCallBackDev() return 0x%iu.]", errorCode]];
        
        return;
    }
    
    JUB_UINT16 deviceID = 0;
    rv = JUB_connectNFCDevice((JUB_BYTE_PTR)uuid,
                              &deviceID);
    [g_selfClass addMsgData:[NSString stringWithFormat:@"[JUB_connectNFCDevice() return 0x%lu.]", rv]];
    if (JUBR_OK != rv) {
        [g_selfClass addMsgData:[NSString stringWithFormat:@"[JUB_connectNFCDevice() 错误]"]];
        
        return;
    }
    
    rv = JUB_isDeviceNFCConnect(deviceID);
    [g_selfClass addMsgData:[NSString stringWithFormat:@"[JUB_isDeviceNFCConnect() return 0x%lu.]", rv]];
    if(JUBR_OK != rv) {
        [g_selfClass addMsgData:[NSString stringWithFormat:@"[设备未连接]"]];
        
        return;
    }
    
    [g_selfClass addMsgData:[NSString stringWithFormat:@"[设备已连接]"]];
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
