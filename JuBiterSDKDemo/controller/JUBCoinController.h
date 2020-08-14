//
//  JUBCoinController.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/9.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBDetailBaseController.h"
#import <Foundation/Foundation.h>
#import "JUBInputAddressView.h"

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


@interface JUBCoinController : JUBDetailBaseController
@property (   strong, nonatomic, readwrite) JUBInputAddressView *inputAddrView;

@property (nonatomic, nonatomic, readwrite) long change;
@property (nonatomic, nonatomic, readwrite) long addressIndex;


- (NSUInteger)show_virtualKeyboard:(NSUInteger)contextID;
- (NSUInteger)verify_pin:(NSUInteger)contextID;
- (NSUInteger)verify_fgpt:(NSUInteger)contextID;
@end


NS_ASSUME_NONNULL_END
