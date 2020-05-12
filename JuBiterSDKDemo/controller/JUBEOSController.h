//
//  JUBEOSController.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBCoinController.h"

NS_ASSUME_NONNULL_BEGIN


#define JSON_FILE_EOS           "testEOS"
#define JSON_FILE_EOS_BUYRAM    "testEOS_buyram"
#define JSON_FILE_EOS_SELLRAM   "testEOS_sellram"
#define JSON_FILE_EOS_STAKE     "testEOS_stake"
#define JSON_FILE_EOS_UNSTAKE   "testEOS_unstake"


typedef NS_ENUM(NSInteger, JUB_NS_ENUM_EOS_OPT) {
    COIN_EOS,
    COIN_EOS_BUYRAM,
    COIN_EOS_SELLRAM,
    COIN_EOS_STAKE,
    COIN_EOS_UNSTAKE
};


@interface JUBEOSController : JUBCoinController

@end


NS_ASSUME_NONNULL_END
