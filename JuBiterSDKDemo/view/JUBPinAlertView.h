//
//  JUBPinAlertView.h
//  JuBiterSDKDemo
//
//  Created by zhangchuan on 2020/6/30.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

typedef void     (^JUBInputPinCallBack)(NSString *pin);
typedef void (^JUBFingerprintsCallBack)(void);
typedef void    (^JUBChangePinCallBack)(NSString *oldPin, NSString *newPin);


@interface JUBPinAlertView : NSObject

/// 弹出输入pin码弹框
/// @param inputPinCallBack pin码回调block
+ (void)showInputPinAlert:(JUBInputPinCallBack)inputPinCallBack;

/// 弹出输入pin码弹框
/// @param inputPinCallBack pin码回调block
+ (void)showInputPinAlert:(JUBInputPinCallBack)inputPinCallBack fingerprintsCallBack:(JUBFingerprintsCallBack)fingerprintsCallBack;

/// 弹出修改pin码弹框
/// @param changePinCallBack 新、旧pin码回调block
+ (void)showChangePinAlert:(JUBChangePinCallBack)changePinCallBack;

@end

NS_ASSUME_NONNULL_END
