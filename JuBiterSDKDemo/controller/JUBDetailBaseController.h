//
//  JUBDetailBaseController.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/9.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "JubSDKCore/JUB_SDK.h"
#include "JUB_SDK_main.h"

#import "JUBCoinTestDetailBaseController.h"
#import "JUBHomeController.h"


NS_ASSUME_NONNULL_BEGIN

@class JUBDetailBaseController;
static JUBDetailBaseController *cSelf;

int  BLEReadFuncCallBack(JUB_ULONG devHandle,  JUB_BYTE_PTR data, JUB_UINT32 dataLen);
void BLEScanFuncCallBack(JUB_BYTE_PTR devName, JUB_BYTE_PTR uuid, JUB_UINT32 type);
void BLEDiscFuncCallBack(JUB_BYTE_PTR uuid);


@interface JUBDetailBaseController : JUBCoinTestDetailBaseController

//@property (weak, nonatomic, readwrite) JUBDetailBaseController *selfClass;
@property (nonatomic, nonatomic, readwrite) NSInteger optItem;

@property (nonatomic, nonatomic, readwrite) long optCoinType;
@property (nonatomic, nonatomic, readwrite) long optIndex;

- (void)beginNFCSession;
- (void)beginBLESession;


#pragma mark - 操作菜单 通讯库寻卡回调
- (void)MenuOption:(JUB_UINT16)deviceID;


#pragma mark - Device 通讯库寻卡回调
- (void)DeviceOpt:(JUB_UINT16)deviceID;


#pragma mark - BTC 通讯库寻卡回调
- (void)CoinBTCOpt:(JUB_UINT16)deviceID;


#pragma mark - ETH 通讯库寻卡回调
- (void)CoinETHOpt:(JUB_UINT16)deviceID;


#pragma mark - EOS 通讯库寻卡回调
- (void)CoinEOSOpt:(JUB_UINT16)deviceID;


#pragma mark - XRP 通讯库寻卡回调
- (void)CoinXRPOpt:(JUB_UINT16)deviceID;


@end


NS_ASSUME_NONNULL_END
