//
//  JUBHomeController.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/13.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBCoinTestListBaseController.h"

NS_ASSUME_NONNULL_BEGIN


#define BUTTON_TITLE_NFC    @"NFC"
#define BUTTON_TITLE_BLE    @"蓝牙"

typedef NS_ENUM(NSInteger, JUB_NS_ENUM_DEV_TYPE) {
    NFC,
    BLE
};


#define BUTTON_TITLE_DEVICE     @"DEVICE"
#define BUTTON_TITLE_BTC        @"BTC"
#define BUTTON_TITLE_ETH        @"ETH"
#define BUTTON_TITLE_EOS        @"EOS"
#define BUTTON_TITLE_XRP        @"XRP"

typedef NS_ENUM(NSInteger, JUB_NS_ENUM_MAIN) {
    OPT_DEVICE,
    OPT_BTC,
    OPT_ETH,
    OPT_EOS,
    OPT_XRP
};


@interface JUBHomeController : JUBCoinTestListBaseController
@property (nonatomic, assign) BOOL showDisConnectBLEButton;
@end


NS_ASSUME_NONNULL_END
