//
//  JUBCoinController.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/9.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "JUBErrorCode.h"

#import "JUBSubPageController.h"


NS_ASSUME_NONNULL_BEGIN


#define BUTTON_TITLE_TRANSACTION    @"TRANSACTION"
#define BUTTON_TITLE_GETADDRESS     @"   GET_ADDRESS"
#define BUTTON_TITLE_SHOWADDRESS    @"  SHOW_ADDRESS"
#define BUTTON_TITLE_SETMYADDRESS   @"SET_MY_ADDRESS"
#define BUTTON_TITLE_SETTIMEOUT     @"SET_TIMEOUT"


typedef NS_ENUM(NSInteger, JUB_NS_ENUM_OPT) {
     TRANSACTION,
       GET_ADDRESS,
      SHOW_ADDRESS,
    SET_MY_ADDRESS,
    SET_TIMEOUT
};


@interface JUBCoinController : JUBSubPageController

- (void) CoinOpt:(NSUInteger)contextID
            root:(Json::Value)root
          choice:(int)choice;

- (void)    get_address_pubkey:(NSUInteger)contextID;
- (void)   show_address_test:(NSUInteger)contextID;
- (void) set_my_address_test:(NSUInteger)contextID;
- (NSUInteger) set_unit_test:(NSUInteger)contextID;
- (NSUInteger) set_time_out_test:(NSUInteger)contextID;

- (NSString*) inputAmount;
- (NSUInteger) verify_user:(NSUInteger)contextID;
- (void) transaction_test:(NSUInteger)contextID
                   amount:(NSString*)amount
                     root:(Json::Value)root;
- (NSUInteger) tx_proc:(NSUInteger)contextID
                amount:(NSString*)amount
                  root:(Json::Value)root;

- (NSUInteger)   show_virtualKeyboard:(NSUInteger)contextID;
- (NSUInteger) cancel_virtualKeyboard:(NSUInteger)contextID;
- (NSUInteger) verify_pin:(NSUInteger)contextID;
- (NSUInteger) verify_fgpt:(NSUInteger)contextID;
@end


NS_ASSUME_NONNULL_END
