//
//  JUBBTCAmount.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/9/4.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBBTCAmount.h"
#import <JubSDKCore/JUB_SDK_BTC.h>


@implementation JUBBTCAmount


+ (NSString*)title:(JUB_ENUM_BTC_UNIT_TYPE)coinUnit {
    
    return [JUBAmount title:[JUBBTCAmount enumUnitToString:coinUnit]];
}


+ (NSString*)enumUnitToString:(JUB_ENUM_BTC_UNIT_TYPE)unit {
    
    NSString* strUnit = TITLE_UNIT_mBTC;
    switch (unit) {
    case JUB_ENUM_BTC_UNIT_TYPE::BTC:
        strUnit = TITLE_UNIT_BTC;
        break;
    case JUB_ENUM_BTC_UNIT_TYPE::cBTC:
        strUnit = TITLE_UNIT_cBTC;
        break;
    case JUB_ENUM_BTC_UNIT_TYPE::uBTC:
        strUnit = TITLE_UNIT_uBTC;
        break;
    case JUB_ENUM_BTC_UNIT_TYPE::Satoshi:
        strUnit = TITLE_UNIT_Satoshi;
        break;
    case JUB_ENUM_BTC_UNIT_TYPE::mBTC:
    default:
        break;
    }
    
    return strUnit;
}


+ (JUB_ENUM_BTC_UNIT_TYPE)stringToEnumUnit:(NSString*)unitString {
    
    JUB_ENUM_BTC_UNIT_TYPE unit = JUB_ENUM_BTC_UNIT_TYPE::ns;
    
    if ([unitString isEqual:TITLE_UNIT_BTC]) {
        unit = JUB_ENUM_BTC_UNIT_TYPE::BTC;
    }
    else if ([unitString isEqual:TITLE_UNIT_cBTC]) {
        unit = JUB_ENUM_BTC_UNIT_TYPE::cBTC;
    }
    else if ([unitString isEqual:TITLE_UNIT_mBTC]) {
        unit = JUB_ENUM_BTC_UNIT_TYPE::mBTC;
    }
    else if ([unitString isEqual:TITLE_UNIT_uBTC]) {
        unit = JUB_ENUM_BTC_UNIT_TYPE::uBTC;
    }
    else if ([unitString isEqual:TITLE_UNIT_Satoshi]) {
        unit = JUB_ENUM_BTC_UNIT_TYPE::Satoshi;
    }
    
    return unit;
}


+ (NSUInteger)enumUnitToDecimal:(JUB_ENUM_BTC_UNIT_TYPE)unit {
    
    NSUInteger decimal = 0;
    
    switch (unit) {
    case JUB_ENUM_BTC_UNIT_TYPE::BTC:
        decimal = 8;
        break;
    case JUB_ENUM_BTC_UNIT_TYPE::cBTC:
        decimal = 6;
        break;
    case JUB_ENUM_BTC_UNIT_TYPE::mBTC:
        decimal = 5;
        break;
    case JUB_ENUM_BTC_UNIT_TYPE::uBTC:
        decimal = 2;
        break;
    case JUB_ENUM_BTC_UNIT_TYPE::Satoshi:
        decimal = 8;
        break;
    default:
        break;
    }
    
    return decimal;
}


+ (NSString*)convertToProperFormat:(NSString*)amount
                               opt:(JUB_NS_ENUM_BTC_COIN)opt {
    
    NSString *amt = amount;
    if (   nil == amount
        || [amount isEqual:@""]
        ) {
        return amt;
    }
    
    switch (opt) {
    case JUB_NS_ENUM_BTC_COIN::BTN_QTUM_QRC20:
        amt = [JUBAmount convertToTheSmallestUnit:amount
                                            point:@"."
                                          decimal:decimalQRC20];
        break;
    case JUB_NS_ENUM_BTC_COIN::BTN_USDT:
        amt = [JUBAmount convertToTheSmallestUnit:amount
                                            point:@"."
                                          decimal:decimalUSDT];
        break;
    default:
        break;
    }
    
    return amt;
}


@end
