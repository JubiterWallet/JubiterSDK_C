//
//  JUBTRXAmount.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/10/26.
//  Copyright © 2020 JuBiter. All rights reserved.
//


#import <Foundation/Foundation.h>
#import "JUBAmount.h"


typedef NS_ENUM(NSInteger, JUB_NS_ENUM_TRX_OPT) {
    BTN_TRX,
    BTN_TRC10,
    BTN_TRC20_TRANSFER,
    BTN_TRC20
};


#define TITLE_TRX       @"Transfer"


#define TITLE_UNIT_TRX       @"TRX"


static NSUInteger decimalTRX = 6;
@interface JUBTRXAmount : JUBAmount

+ (NSString*)title:(JUB_NS_ENUM_TRX_OPT)opt;
+ (NSString*)formatRules;

+ (NSString*)enumUnitToString:(JUB_NS_ENUM_TRX_OPT)opt;
+ (NSString*)enumUnitToUnitStr:(JUB_NS_ENUM_TRX_OPT)opt;

+ (BOOL)isValid:(NSString*)amount;
+ (BOOL)isValid:(NSString*)amount
            opt:(JUB_NS_ENUM_TRX_OPT)opt;

+ (NSString*)convertToProperFormat:(NSString*)amount
                               opt:(JUB_NS_ENUM_TRX_OPT)opt;

@end
