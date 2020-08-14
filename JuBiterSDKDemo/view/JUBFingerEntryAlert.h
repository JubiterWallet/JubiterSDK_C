//
//  JUBFingerEntryAlert.h
//  JuBiterSDKDemo
//
//  Created by zhangchuan on 2020/8/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface JUBFingerEntryAlert : UIView

@property (nonatomic, assign) NSInteger fingerNumberSum;

@property (nonatomic, assign) NSInteger fingerNumber;

+ (JUBFingerEntryAlert *)show;

- (void)dismiss;

@end

NS_ASSUME_NONNULL_END
