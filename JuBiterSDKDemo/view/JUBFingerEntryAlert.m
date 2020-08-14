//
//  JUBFingerEntryAlert.m
//  JuBiterSDKDemo
//
//  Created by zhangchuan on 2020/8/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBFingerEntryAlert.h"
#import "FTConstant.h"
#import "Tools.h"
#import "JUBBLEDeviceListCell.h"
#import "JUBWarningAlert.h"

@interface JUBFingerEntryAlert()

@property (nonatomic, weak) UILabel *titleLabel;

@property (nonatomic, weak) UIImageView *fingerImageView;

@property (nonatomic, copy) NSString *msg;

@property (nonatomic, weak) NSTimer *timer;

@end

@implementation JUBFingerEntryAlert

+ (JUBFingerEntryAlert *)show {
    
    JUBFingerEntryAlert *alertView = [JUBFingerEntryAlert creatSelf];
    
    UIView *whiteMainView = [alertView addMainView];
    
    [alertView addSubviewAboveSuperView:whiteMainView];
    
    return alertView;
}


+ (JUBFingerEntryAlert *)creatSelf {
    
    JUBFingerEntryAlert *alertView = [[JUBFingerEntryAlert alloc] initWithFrame:CGRectMake(0, 0, KScreenWidth, KScreenHeight)];
    
    alertView.backgroundColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:0.3];
    
    [[UIApplication sharedApplication].keyWindow addSubview:alertView];
    
    return alertView;
}


- (UIView *)addMainView {
    
    UIView *mainView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, KScreenWidth - 50, 200)];
    
    mainView.center = CGPointMake(KScreenWidth/2, KScreenHeight*2/5);
    
    mainView.backgroundColor = [UIColor whiteColor];
    
    mainView.layer.cornerRadius = 4;
    
    mainView.layer.masksToBounds = YES;
    
    [self addSubview:mainView];
    
    return mainView;
}


- (void)addSubviewAboveSuperView:(UIView *)mainView {
    
    [self addButtonAboveSuperView:mainView];
    
    self.fingerImageView = [self addFingerImageViewAboveSuperView:mainView];
    
    self.titleLabel = [self addTitleLabelAboveSuperView:mainView];
}


#pragma mark - 添加mainview上面的子视图
- (UIButton *)addButtonAboveSuperView:(UIView *)mainView {
    
    UIButton *closeButton = [[UIButton alloc] initWithFrame:CGRectMake(CGRectGetWidth(mainView.frame) - 45, 10, 35, 35)];
    
    [closeButton addTarget:self action:@selector(close) forControlEvents:UIControlEventTouchUpInside];
    
    [mainView addSubview:closeButton];
    
    UIImageView *imageView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 15, 15)];
    
    imageView.image = [UIImage imageNamed:@"close"];
    
    [closeButton addSubview:imageView];
    
    imageView.center = CGPointMake(CGRectGetWidth(closeButton.frame)/2.0, CGRectGetHeight(closeButton.frame)/2.0);
    
    return closeButton;
}


- (void)close {
    
    [JUBWarningAlert warningAlert:@"Are you sure you want to CANCEL the fingerprint enroll ?" warningCallBack:^{
        [self dismiss];
    }];
}


- (UIImageView *)addFingerImageViewAboveSuperView:(UIView *)mainView {
    
    UIImageView *fingerImageView = [[UIImageView alloc] initWithFrame:CGRectMake(CGRectGetWidth(mainView.frame)/2 - 40, 30, 80, 84)];
    
    fingerImageView.image = [UIImage imageNamed:@"finger-0"];
    
    [mainView addSubview:fingerImageView];
    
    return fingerImageView;
}


- (UILabel *)addTitleLabelAboveSuperView:(UIView *)mainView {
    
    UILabel *titleLabel = [[UILabel alloc] initWithFrame:CGRectMake(20, CGRectGetMaxY(self.fingerImageView.frame) + 20, CGRectGetWidth(mainView.frame) - 2 * 20, 20)];
    
    titleLabel.text = @"Please enroll the fingerprint";
    
    titleLabel.font = [UIFont systemFontOfSize:16];
    
    titleLabel.textAlignment = NSTextAlignmentCenter;
    
    [mainView addSubview:titleLabel];
    
    return titleLabel;
}


- (void)dismiss {
    
    [self.timer invalidate];
    
    [self removeFromSuperview];
}


#pragma mark - getter setter方法
- (void)setFingerNumber:(NSInteger)fingerNumber {
    
    NSAssert(fingerNumber <= 10, @"fingerNumber的数值最大为10");
    
    NSAssert(self.fingerNumberSum > 0, @"需要先设置fingerNumberSum的值");
    
    _fingerNumber = fingerNumber;
    
    if (self.fingerNumberSum > 0) {
        
        NSString *fingerImageName = [NSString stringWithFormat:@"finger-%ld", (NSInteger)((float)fingerNumber/self.fingerNumberSum * 10) - 1];
        
        NSLog(@"fingerImageName = %@", fingerImageName);
        
        self.fingerImageView.image = [UIImage imageNamed:fingerImageName];
        
        NSInteger remainingTime = self.fingerNumberSum - (long)fingerNumber;
        
        if (remainingTime > 0) {
            
            self.titleLabel.text = [NSString stringWithFormat:@"%ld more time(s) is required", remainingTime];
        }
        else {
            self.titleLabel.text = @"Fingerprint entry completed";
        }
    }
}


- (void)setFingerNumberSum:(NSInteger)fingerNumberSum {
    
    NSAssert(fingerNumberSum <= 10, @"fingerNumberSum的数值最大为10");
    
    _fingerNumberSum = fingerNumberSum;
}


- (void)dealloc
{
    NSLog(@"JUBFingerEntryAlert-dealloc");
}

@end
