//
//  JUBETHController.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/4/28.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBCoinController.h"

NS_ASSUME_NONNULL_BEGIN


#define JSON_FILE_ETH       "testETH"
#define JSON_FILE_ECH       "testECH"


#define BUTTON_TITLE_ETH            @"ETH"
#define BUTTON_TITLE_ETH_ERC20      @"ETH_ERC20"
#define BUTTON_TITLE_ETC            @"ETC"


typedef NS_ENUM(NSInteger, JUB_NS_ENUM_ETH_COIN) {
    BTN_ETH,
    BTN_ETH_ERC20,
    BTN_ECH
};


@interface JUBETHController : JUBCoinController

@end

NS_ASSUME_NONNULL_END
