//
//  JUBSendApduController.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/9/1.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBSharedData.h"


NS_ASSUME_NONNULL_BEGIN


typedef NS_ENUM(NSInteger, JUB_NS_ENUM_APDU_OPT) {
    APDU_SEND,
    APDU_SAVE,
};


@interface JUBSendApduController : JUBInputOrderBaseController
@end

NS_ASSUME_NONNULL_END
