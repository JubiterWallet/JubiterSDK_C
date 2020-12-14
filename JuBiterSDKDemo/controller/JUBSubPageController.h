//
//  JUBSubPageController.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/9.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "JubSDKCore/JUB_SDK.h"
#include "JUB_SDK_main.h"

#import "JUBHomePageController.h"


NS_ASSUME_NONNULL_BEGIN

@class JUBSubPageController;
static JUBSubPageController *cSelf;

int  BLEReadFuncCallBack(JUB_ULONG devHandle,  JUB_BYTE_PTR data, JUB_UINT32 dataLen);
void BLEScanFuncCallBack(JUB_BYTE_PTR devName, JUB_BYTE_PTR uuid, JUB_UINT32 type);
void BLEDiscFuncCallBack(JUB_BYTE_PTR uuid);


@interface JUBSubPageController : JUBDetailBaseController

//@property (weak, nonatomic, readwrite) JUBSubPageController *selfClass;
@property (nonatomic, nonatomic, readwrite) NSInteger optItem;

@property (nonatomic, nonatomic, readwrite) long optIndex;

- (void)beginNFCSession;
- (void)beginBLESession;


#pragma mark - 操作菜单 通讯库寻卡回调
- (void)MenuOption:(NSUInteger)deviceID;


#pragma mark - Device 通讯库寻卡回调
- (void)DeviceOpt:(NSUInteger)deviceID;


#pragma mark - BTC 通讯库寻卡回调
- (void)CoinBTCOpt:(NSUInteger)deviceID;


#pragma mark - ETH 通讯库寻卡回调
- (void)CoinETHOpt:(NSUInteger)deviceID;


#pragma mark - EOS 通讯库寻卡回调
- (void)CoinEOSOpt:(NSUInteger)deviceID;


#pragma mark - XRP 通讯库寻卡回调
- (void)CoinXRPOpt:(NSUInteger)deviceID;


#pragma mark - TRX 通讯库寻卡回调
- (void)CoinTRXOpt:(NSUInteger)deviceID;


#pragma mark - FIL 通讯库寻卡回调
- (void)CoinFILOpt:(NSUInteger)deviceID;


@end


NS_ASSUME_NONNULL_END
