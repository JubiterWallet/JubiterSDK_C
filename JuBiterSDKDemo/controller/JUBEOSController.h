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
#define JSON_FILE_EOS_TOKEN     "testEOS_token"
#define JSON_FILE_EOS_BUYRAM    "testEOS_buyram"
#define JSON_FILE_EOS_SELLRAM   "testEOS_sellram"
#define JSON_FILE_EOS_STAKE     "testEOS_stake"
#define JSON_FILE_EOS_UNSTAKE   "testEOS_unstake"


#define BUTTON_TITLE_EOS            @"EOS"
#define BUTTON_TITLE_EOSTOKEN       @"EOS_TOKEN"
#define BUTTON_TITLE_EOSBUYRAM      @"EOS_BUY_RAM"
#define BUTTON_TITLE_EOSSELLRAM     @"EOS_SELL_RAM"
#define BUTTON_TITLE_EOSSTAKE       @"EOS_STAKE"
#define BUTTON_TITLE_EOSUNSTAKE     @"EOS_UNSTAKE"


@interface JUBEOSController : JUBCoinController

@end


NS_ASSUME_NONNULL_END
