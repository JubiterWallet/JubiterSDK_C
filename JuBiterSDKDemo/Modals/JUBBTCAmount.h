//
//  JUBBTCAmount.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/9/3.
//  Copyright © 2020 JuBiter. All rights reserved.
//


#import <Foundation/Foundation.h>
#import <JubSDKCore/JUB_SDK_BTC.h>

#import "JUBAmount.h"

typedef NS_ENUM(NSInteger, JUB_NS_ENUM_BTC_COIN) {
    BTN_BTC_P2PKH,
    BTN_BTC_P2WPKH,
    BTN_LTC,
    BTN_DASH,
    BTN_BCH,
    BTN_QTUM,
    BTN_QTUM_QRC20,
    BTN_USDT,
    BTN_HCASH
};


#define TITLE_UNIT_BTC       @"BTC"
#define TITLE_UNIT_cBTC      @"cBTC"
#define TITLE_UNIT_mBTC      @"mBTC"
#define TITLE_UNIT_uBTC      @"uBTC"
#define TITLE_UNIT_Satoshi   @"Satoshi"


static NSUInteger decimalUSDT = 8;
static NSUInteger decimalQRC20 = 8;
@interface JUBBTCAmount : JUBAmount

+ (NSString*)title:(JUB_ENUM_BTC_UNIT_TYPE)coinUnit;

+ (NSString*)enumUnitToString:(JUB_ENUM_BTC_UNIT_TYPE)unit;
+ (JUB_ENUM_BTC_UNIT_TYPE)stringToEnumUnit:(NSString*)unit;

+ (NSUInteger)enumUnitToDecimal:(JUB_ENUM_BTC_UNIT_TYPE)unit;

+ (NSString*)convertToProperFormat:(NSString*)amount
                               opt:(JUB_NS_ENUM_BTC_COIN)opt;

@end
