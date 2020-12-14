//
//  JUBFILAmount.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/11/30.
//  Copyright © 2020 JuBiter. All rights reserved.
//


#import <Foundation/Foundation.h>
#import "JUBAmount.h"


typedef NS_ENUM(NSInteger, JUB_NS_ENUM_FIL_COIN) {
    BTN_FIL
};


#define TITLE_FIL       @"Transfer"
//#define TITLE_FIL_TOKEN @"Transfer token"


#define TITLE_UNIT_FIL       @"FIL"
#define TITLE_UNIT_nanoFIL   @"nanoFIL"
//#define TITLE_UNIT_FIL_TOKEN @"token"


static NSUInteger decimalFIL = 9;
@interface JUBFILAmount : JUBAmount

+ (NSString*)title:(JUB_NS_ENUM_FIL_COIN)opt;
+ (NSString*)formatRules;

+ (NSString*)enumUnitToString:(JUB_NS_ENUM_FIL_COIN)opt;
+ (NSString*)enumUnitToUnitStr:(JUB_NS_ENUM_FIL_COIN)opt;

+ (BOOL)isValid:(NSString*)amount;
+ (BOOL)isValid:(NSString*)amount
            opt:(JUB_NS_ENUM_FIL_COIN)opt;

+ (NSString*)convertToProperFormat:(NSString*)amount
                               opt:(JUB_NS_ENUM_FIL_COIN)opt;


@end
