//
//  JUBWarningAlert.h
//  JuBiterSDKDemo
//
//  Created by zhangchuan on 2020/8/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//  警告类弹框

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

typedef void (^JUBWarningCallBack)(void);

@interface JUBWarningAlert : UIView

+ (void)warningAlert:(NSString *)message warningCallBack:(JUBWarningCallBack)warningCallBack;

@end

NS_ASSUME_NONNULL_END
