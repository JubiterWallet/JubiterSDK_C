//
//  JUBCoinController.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/9.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBController.h"
#import <Foundation/Foundation.h>
#import "JubSDKCore/JUB_SDK.h"

NS_ASSUME_NONNULL_BEGIN


typedef NS_ENUM(NSInteger, JUB_NS_ENUM_OPT) {
     TRANSACTION,
       GET_ADDRESS,
      SHOW_ADDRESS,
    SET_MY_ADDRESS,
    SET_TIMEOUT
};


@interface JUBCoinController : JUBController
- (JUB_RV)verify_pin:(JUB_UINT16)contextID
                 pin:(std::string)pin;
@end


NS_ASSUME_NONNULL_END
