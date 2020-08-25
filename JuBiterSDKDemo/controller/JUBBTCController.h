//
//  JUBBTCController.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/4/28.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBCoinController.h"
#import "JUBAmount.h"

NS_ASSUME_NONNULL_BEGIN


#define JSON_FILE_BTC_44     "testBTC44"
#define JSON_FILE_BTC_49     "testBTC49"
#define JSON_FILE_LTC        "testLTC"
#define JSON_FILE_DASH       "testDASH"
#define JSON_FILE_BCH        "testBCH"
#define JSON_FILE_QTUM       "testQTUM"
#define JSON_FILE_QTUM_QRC20 "testQTUM_qrc20"
#define JSON_FILE_BTC_USDT   "testUSDT"
#define JSON_FILE_HCASH      "testHCash"


#define BUTTON_TITLE_SETUNIT        @"Set Unit"
#define BUTTON_TITLE_BTCP2PKH       @"BTC_P2PKH"
#define BUTTON_TITLE_BTCP2WPKH      @"BTC_P2WPKH"
#define BUTTON_TITLE_LTC            @"LTC"
#define BUTTON_TITLE_DASH           @"DASH"
#define BUTTON_TITLE_BCH            @"BCH"
#define BUTTON_TITLE_QTUM           @"QTUM"
#define BUTTON_TITLE_QTUM_QRC20     @"QTUM_QRC20"
#define BUTTON_TITLE_USDT           @"USDT"
#define BUTTON_TITLE_HCASH          @"HCASH"


@interface JUBBTCController : JUBCoinController

@end

NS_ASSUME_NONNULL_END
