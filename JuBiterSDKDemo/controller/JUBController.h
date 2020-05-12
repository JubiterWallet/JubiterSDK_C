//
//  JUBController.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/9.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBCoinTestDetailBaseController.h"
#import <Foundation/Foundation.h>
#import "JubSDKCore/JUB_SDK.h"
#include "JUB_SDK_main.h"


NS_ASSUME_NONNULL_BEGIN


typedef NS_ENUM(NSInteger, JUB_NS_ENUM_MAIN) {
    OPT_DEVICE,
    OPT_BTC,
    OPT_ETH,
    OPT_EOS,
    OPT_XRP
};


@interface JUBController : JUBCoinTestDetailBaseController

@property (   strong, nonatomic, readwrite) JUBController *selfClass;
@property (nonatomic, nonatomic, readwrite) NSInteger optItem;

@property (nonatomic, nonatomic, readwrite) long optCoinType;
@property (nonatomic, nonatomic, readwrite) long optIndex;
@property (   strong, nonatomic, readwrite) NSString* userPIN;
@property (   strong, nonatomic, readwrite) NSString* userPath;

- (void)beginNFCSession;

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


static JUBController *g_selfClass;
static NSInteger g_optItem;


#pragma mark - 通讯库寻卡回调

static void NFCScanFuncCallBack(unsigned int errorCode,
                                const char* uuid,
                                unsigned int devType);

NS_ASSUME_NONNULL_END
