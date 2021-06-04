//
//  JUBETHAmount.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/9/6.
//  Copyright © 2020 JuBiter. All rights reserved.
//


#import <Foundation/Foundation.h>
#import "JUBAmount.h"


typedef NS_ENUM(NSInteger, JUB_NS_ENUM_ETH_COIN) {
    BTN_ETH,
    BTN_ETH_ERC20,
    BTN_ETH_ERC721,
    BTN_ETH_UNISWAP,
    BTN_ETH_BYTESTR,
    BTN_ETC,
};


#define TITLE_ETH       @"Transfer"
#define TITLE_ETH_TOKEN @"Transfer token"


#define TITLE_UNIT_ETH       @"ETH"
#define TITLE_UNIT_ETH_TOKEN @"token"


static NSUInteger decimalETH = 18;
@interface JUBETHAmount : JUBAmount

+ (NSString*)title:(JUB_NS_ENUM_ETH_COIN)opt;
+ (NSString*)formatRules;

+ (NSString*)enumUnitToString:(JUB_NS_ENUM_ETH_COIN)opt;
+ (NSString*)enumUnitToUnitStr:(JUB_NS_ENUM_ETH_COIN)opt;

+ (BOOL)isValid:(NSString*)amount;
+ (BOOL)isValid:(NSString*)amount
            opt:(JUB_NS_ENUM_ETH_COIN)opt;

+ (NSString*)convertToProperFormat:(NSString*)amount
                               opt:(JUB_NS_ENUM_ETH_COIN)opt;


@end
