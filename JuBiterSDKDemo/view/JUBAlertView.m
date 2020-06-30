//
//  JUBAlertView.m
//  JuBiterSDKDemo
//
//  Created by 张川 on 2020/5/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBAlertView.h"
#import "FTConstant.h"
#import "Tools.h"
#import "JUBBLEDeviceListCell.h"


@interface JUBAlertView()
@property (nonatomic, weak) UILabel *titleLabel;
@property (nonatomic, copy) NSString *msg;
@end


@implementation JUBAlertView


+ (JUBAlertView *)showMsg:(NSString *)msg {
    
    JUBAlertView *alertView = [JUBAlertView creatSelf];
    
    alertView.msg = msg;
                
    UIView *whiteMainView = [alertView addMainView];
    
    [alertView addSubviewAboveSuperView:whiteMainView];
    
    return alertView;
    
}


+ (JUBAlertView *)showMsg:(NSString *)msg
                    delay:(NSInteger)time {
    
    JUBAlertView *alertView = [self showMsg:msg];
    
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW,
                                 (int64_t)(time * NSEC_PER_SEC)),
                   dispatch_get_main_queue(), ^{
        
        [alertView dismiss];
    });
    
    return alertView;
    
}


+ (JUBAlertView *)creatSelf {
    
    JUBAlertView *alertView = [[JUBAlertView alloc] initWithFrame:CGRectMake(0,
                                                                             0,
                                                                             KScreenWidth,
                                                                             KScreenHeight)];
    
    alertView.backgroundColor = [UIColor colorWithRed:0
                                                green:0
                                                 blue:0
                                                alpha:0.3];
    
    [[UIApplication sharedApplication].keyWindow addSubview:alertView];
        
    return alertView;
    
}

- (UIView *)addMainView {
    
    UIView *mainView = [[UIView alloc] initWithFrame:CGRectMake(0,
                                                                0,
                                                                KScreenWidth - 50,
                                                                100)];
    
    mainView.center = CGPointMake(KScreenWidth/2, KScreenHeight*2/5);
    
    mainView.backgroundColor = [UIColor whiteColor];
    
    mainView.layer.cornerRadius = 4;
    
    mainView.layer.masksToBounds = YES;
    
    [self addSubview:mainView];
    
    return mainView;
    
}


- (void)addSubviewAboveSuperView:(UIView *)mainView {
    
    self.titleLabel = [self addTitleLabelAboveSuperView:mainView];
}


#pragma mark - 添加mainview上面的子视图

- (UILabel *)addTitleLabelAboveSuperView:(UIView *)mainView {
    
//    UILabel *titleLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, CGRectGetWidth(mainView.frame), 50)];
    
    UILabel *titleLabel = [[UILabel alloc] initWithFrame:CGRectMake(20,
                                                                    0,
                                                                    CGRectGetWidth(mainView.frame) - 2 * 20,
                                                                    CGRectGetHeight(mainView.frame))];
    
    titleLabel.text = self.msg;
    
    titleLabel.font = [UIFont systemFontOfSize:16];
    
    titleLabel.textAlignment = NSTextAlignmentCenter;
    
    titleLabel.numberOfLines = 0;
    
    [mainView addSubview:titleLabel];
    
    return titleLabel;
}


- (void)dismiss {
    
    [self removeFromSuperview];
}

@end
