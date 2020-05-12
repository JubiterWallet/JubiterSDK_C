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


typedef NS_ENUM(NSInteger, JUB_NS_ENUM_ETH_COINTYPE) {
    COIN_ETH,
    COIN_ETH_ERC20,
    COIN_ECH
};


@interface JUBETHController : JUBCoinController

@end

NS_ASSUME_NONNULL_END
