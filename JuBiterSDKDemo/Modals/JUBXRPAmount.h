//
//  JUBXRPAmount.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/9/6.
//  Copyright © 2020 JuBiter. All rights reserved.
//


#import <Foundation/Foundation.h>
#import "JUBAmount.h"


typedef NS_ENUM(NSInteger, JUB_NS_ENUM_XRP_COIN) {
    BTN_XRP
};

#define TITLE_XRP       @"Transfer"


#define TITLE_UNIT_XRP       @"XRP"


static NSUInteger decimalXRP = 6;
@interface JUBXRPAmount : JUBAmount

+ (NSString*)title:(JUB_NS_ENUM_XRP_COIN)opt;
+ (NSString*)formatRules;

+ (NSString*)enumUnitToString:(JUB_NS_ENUM_XRP_COIN)opt;
+ (NSString*)enumUnitToUnitStr:(JUB_NS_ENUM_XRP_COIN)opt;

+ (BOOL)isValid:(NSString*)amount;
+ (BOOL)isValid:(NSString*)amount
            opt:(JUB_NS_ENUM_XRP_COIN)opt;

+ (NSString*)convertToProperFormat:(NSString*)amount
                               opt:(JUB_NS_ENUM_XRP_COIN)opt;

@end
