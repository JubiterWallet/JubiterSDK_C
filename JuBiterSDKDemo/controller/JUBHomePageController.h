//
//  JUBHomePageController.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/13.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBSharedData.h"


NS_ASSUME_NONNULL_BEGIN


#define FLAG_CURR           @"(*)"


#define BUTTON_TITLE_NFC    @"NFC"
#define BUTTON_TITLE_BLE    @"BLE"


#define BUTTON_TITLE_SENDAPDU   @"SEND APDU"
#define BUTTON_TITLE_FGPT       @"FINGERPRINT MANAGEMENT"
#define BUTTON_TITLE_DEVICE     @"DEVICE"
#define BUTTON_TITLE_BTC        @"BTC"
#define BUTTON_TITLE_ETH        @"ETH"
#define BUTTON_TITLE_UNISWAP    @"ETH-UNISWAP"
#define BUTTON_TITLE_EOS        @"EOS"
#define BUTTON_TITLE_XRP        @"XRP"
#define BUTTON_TITLE_TRX        @"TRX"
#define BUTTON_TITLE_FIL        @"FIL"


typedef NS_ENUM(NSInteger, JUB_NS_ENUM_MAIN) {
    OPT_SENDAPDU,
    OPT_FGPT,
    OPT_DEVICE,
    OPT_BTC,
    OPT_ETH,
    OPT_ETH_UNISWAP,
    OPT_EOS,
    OPT_XRP,
    OPT_TRX,
    OPT_FIL
};


@interface JUBHomePageController : JUBListBaseController
@property (nonatomic, assign) BOOL showDisConnectBLEButton;
@end


NS_ASSUME_NONNULL_END
