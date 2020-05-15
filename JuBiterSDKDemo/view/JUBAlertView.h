//
//  JUBAlertView.h
//  JuBiterSDKDemo
//
//  Created by 张川 on 2020/5/14.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface JUBAlertView : UIView

+ (JUBAlertView *)showMsg:(NSString *)msg delay:(NSInteger)time;

+ (JUBAlertView *)showMsg:(NSString *)msg;

- (void)dismiss;

@end

NS_ASSUME_NONNULL_END
