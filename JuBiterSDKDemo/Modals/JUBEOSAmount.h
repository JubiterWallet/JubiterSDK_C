//
//  JUBEOSAmount.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/9/4.
//  Copyright © 2020 JuBiter. All rights reserved.
//


#import <Foundation/Foundation.h>
#import "JUBAmount.h"


typedef NS_ENUM(NSInteger, JUB_NS_ENUM_EOS_OPT) {
    BTN_EOS,
    BTN_EOS_TOKEN,
    BTN_EOS_BUYRAM,
    BTN_EOS_SELLRAM,
    BTN_EOS_STAKE,
    BTN_EOS_UNSTAKE
};

#define TITLE_EOS       @"Transfer"
#define TITLE_EOS_TOKEN @"Transfer token"
#define TITLE_SELLRAM   @"Sell RAM"
#define TITLE_BUYRAM    @"Buy RAM"
#define TITLE_STAKE     @"Stake"
#define TITLE_UNSTAKE   @"Unstake"


#define TITLE_UNIT_EOS       @"EOS"
#define TITLE_UNIT_EOS_TOKEN @"token"
#define TITLE_UNIT_BYTES     @"bytes"


static NSUInteger decimalEOS = 4;
@interface JUBEOSAmount : JUBAmount

+ (NSString*)title:(JUB_NS_ENUM_EOS_OPT)opt;
+ (NSString*)formatRules:(JUB_NS_ENUM_EOS_OPT)opt;

+ (NSString*)enumUnitToString:(JUB_NS_ENUM_EOS_OPT)opt;
+ (NSString*)enumUnitToUnitStr:(JUB_NS_ENUM_EOS_OPT)opt;

+ (BOOL)isBytesValid:(NSString*)bytes;
+ (BOOL)isValid:(NSString*)amount;
+ (BOOL)isValid:(NSString*)amount
            opt:(JUB_NS_ENUM_EOS_OPT)opt;

+ (NSString*)convertToProperFormat:(NSString*)amount
                               opt:(JUB_NS_ENUM_EOS_OPT)opt;

+ (NSString*)replaceAmount:(NSString*)amount
                     asset:(NSString*)asset;

@end
