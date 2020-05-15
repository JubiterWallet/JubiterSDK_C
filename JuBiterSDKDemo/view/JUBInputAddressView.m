//
//  JUBInputAddressView.m
//  JuBiterSDKDemo
//
//  Created by 张川 on 2020/5/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBInputAddressView.h"
#import "FTConstant.h"
#import "Tools.h"

@interface JUBInputAddressView()<UIScrollViewDelegate, UITextFieldDelegate>

@property (nonatomic, copy) JUBGetAddressCallBackBlock getAddressCallBackBlock;

@property (nonatomic, assign) NSInteger change;

@property (nonatomic, weak) UILabel *addressHeaderLabel;

@property (nonatomic, weak) UITextField *addressTextField;

@end

@implementation JUBInputAddressView

+ (JUBInputAddressView *)showCallBack:(JUBGetAddressCallBackBlock)getAddressCallBackBlock {
    
    JUBInputAddressView *inputAddressView = [JUBInputAddressView creatSelf];
    
    inputAddressView.getAddressCallBackBlock = getAddressCallBackBlock;
    
    inputAddressView.change = 0;
        
    UIView *whiteMainView = [inputAddressView addMainView];
    
    [inputAddressView addSubviewAboveSuperView:whiteMainView];
    
    return inputAddressView;
    
}

+ (JUBInputAddressView *)creatSelf {
    
    JUBInputAddressView *inPutAddressView = [[JUBInputAddressView alloc] initWithFrame:CGRectMake(0, 0, KScreenWidth, KScreenHeight)];
    
    inPutAddressView.backgroundColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:0.3];
    
    [[UIApplication sharedApplication].keyWindow addSubview:inPutAddressView];
        
    return inPutAddressView;
    
}

- (UIView *)addMainView {
    
    UIView *mainView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, KScreenWidth - 50, 200)];
    
    mainView.center = CGPointMake(KScreenWidth/2, KScreenHeight/3);
    
    mainView.backgroundColor = [UIColor whiteColor];
    
    mainView.layer.cornerRadius = 4;
    
    mainView.layer.masksToBounds = YES;
    
    [self addSubview:mainView];
    
    return mainView;
    
}

- (void)addSubviewAboveSuperView:(UIView *)mainView {
    
    
    UILabel *titleLabel = [self addTitleLabelAboveSuperView:mainView];
    
    UILabel *addressHeaderLabel = [self addAddressHeaderLabelAboveSuperView:mainView frame:CGRectMake(15, CGRectGetMaxY(titleLabel.frame), CGRectGetWidth(mainView.frame)/3, 80)];
    
    self.addressHeaderLabel = addressHeaderLabel;
    
    UIScrollView *changeScrollView = [self addChangeViewAboveSuperView:mainView frame:CGRectMake(CGRectGetMaxX(addressHeaderLabel.frame), CGRectGetMidY(addressHeaderLabel.frame) - 20, 40, 40)];
    
    UILabel *line = [self addVLineAboveSuperView:mainView frame:CGRectMake(CGRectGetMaxX(changeScrollView.frame), CGRectGetMinY(addressHeaderLabel.frame), 10, CGRectGetHeight(addressHeaderLabel.frame))];
    
    [self addAddressIndexTextFieldAboveSuperView:mainView frame:CGRectMake(CGRectGetMaxX(line.frame), CGRectGetMidY(line.frame) - 15, 100, 30)];
    
    [self addCancleAndOkButtonAboveSuperView:mainView];
    
}

#pragma mark - 添加mainview上面的子视图

- (UILabel *)addTitleLabelAboveSuperView:(UIView *)mainView {
    
    UILabel *titleLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, CGRectGetWidth(mainView.frame), 50)];
    
    titleLabel.text = @"Please enter a path";
    
    titleLabel.font = [UIFont systemFontOfSize:16];
    
    titleLabel.textColor = [UIColor whiteColor];
    
    titleLabel.textAlignment = NSTextAlignmentCenter;
    
    titleLabel.backgroundColor = [[Tools defaultTools] colorWithHexString:@"#00ccff"];
    
    [mainView addSubview:titleLabel];
    
    return titleLabel;
    
}

- (UILabel *)addAddressHeaderLabelAboveSuperView:(UIView *)mainView frame:(CGRect)frame {
    
    UILabel *addressHeaderLabel = [[UILabel alloc] initWithFrame:frame];
    
    addressHeaderLabel.text = @"m/44'/0'/0'/";
    
    addressHeaderLabel.font = [UIFont systemFontOfSize:20];
    
    addressHeaderLabel.textAlignment = NSTextAlignmentRight;
    
    [mainView addSubview:addressHeaderLabel];
    
    return addressHeaderLabel;
    
}

- (UIScrollView *)addChangeViewAboveSuperView:(UIView *)mainView frame:(CGRect)frame{
    
    UIScrollView *changeScrollView = [[UIScrollView alloc] initWithFrame:frame];
    
    changeScrollView.showsVerticalScrollIndicator = FALSE;
    
    changeScrollView.contentSize = CGSizeMake(CGRectGetWidth(changeScrollView.frame), CGRectGetWidth(changeScrollView.frame) * 2);
    
    changeScrollView.pagingEnabled = YES;
    
    changeScrollView.bounces = NO;
    
    [mainView addSubview:changeScrollView];
    
    UILabel *label0 = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, CGRectGetWidth(changeScrollView.frame), CGRectGetHeight(changeScrollView.frame))];
    
    label0.text = @"0";
    
    label0.font = [UIFont systemFontOfSize:20];
    
    label0.textAlignment = NSTextAlignmentCenter;
        
    label0.textColor = [[Tools defaultTools] colorWithHexString:@"#00ccff"];
    
    [changeScrollView addSubview:label0];
    
    UILabel *label1 = [[UILabel alloc] initWithFrame:CGRectMake(0, CGRectGetMaxY(label0.frame), CGRectGetWidth(changeScrollView.frame), CGRectGetHeight(changeScrollView.frame))];
    
    label1.text = @"1";
    
    label1.font = [UIFont systemFontOfSize:20];
    
    label1.textAlignment = NSTextAlignmentCenter;
    
    label1.textColor = [[Tools defaultTools] colorWithHexString:@"#00ccff"];
        
    [changeScrollView addSubview:label1];
    
    changeScrollView.delegate = self;
    
    return changeScrollView;
    
}

- (UILabel *)addVLineAboveSuperView:(UIView *)mainView frame:(CGRect)frame {
    
    UILabel *line = [[UILabel alloc] initWithFrame:frame];
    
    line.text = @"/";
    
    line.font = [UIFont systemFontOfSize:20];
    
    line.textAlignment = NSTextAlignmentCenter;
            
    [mainView addSubview:line];
    
    return line;
    
}

- (UITextField *)addAddressIndexTextFieldAboveSuperView:(UIView *)mainView frame:(CGRect)frame {
    
    UITextField *textField = [[UITextField alloc] initWithFrame:frame];
    
    self.addressTextField = textField;
    
    textField.text = @"0";
        
    textField.keyboardType = UIKeyboardTypeNumberPad;
    
    textField.layer.borderColor = [UIColor lightGrayColor].CGColor;
    
    textField.layer.borderWidth = 1;
        
    [textField becomeFirstResponder];
    
    [mainView addSubview:textField];
        
    return textField;
    
}

- (void)addCancleAndOkButtonAboveSuperView:(UIView *)mainView {
    
    CGFloat mainViewWidth = CGRectGetWidth(mainView.frame);
    
    CGFloat buttonWidth = 100;
    
    CGFloat buttonSpace = (mainViewWidth - 2 * buttonWidth)/3;

    CGFloat buttonHeight = 40;
    
    CGFloat buttonY = CGRectGetHeight(mainView.frame) - 20 - buttonHeight;
    
    UIButton *leftButton = [[UIButton alloc] initWithFrame:CGRectMake(mainViewWidth/2 - buttonSpace/2 - buttonWidth, buttonY, buttonWidth, buttonHeight)];
    
    [leftButton setTitle:@"CANCEL" forState:UIControlStateNormal];
    
    [leftButton setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    
    [leftButton setBackgroundColor:[[Tools defaultTools] colorWithHexString:@"#00ccff"]];
    
    leftButton.layer.cornerRadius = 4;
    
    leftButton.layer.masksToBounds = YES;
    
    [leftButton addTarget:self action:@selector(cancle) forControlEvents:UIControlEventTouchUpInside];
    
    [mainView addSubview:leftButton];
    
    UIButton *rightButton = [[UIButton alloc] initWithFrame:CGRectMake(mainViewWidth/2 + buttonSpace/2, buttonY, buttonWidth, buttonHeight)];
    
    [rightButton setTitle:@"COMPLETE" forState:UIControlStateNormal];
    
    [rightButton setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    
    [rightButton setBackgroundColor:[[Tools defaultTools] colorWithHexString:@"#00ccff"]];
        
    rightButton.layer.cornerRadius = 4;
        
    rightButton.layer.masksToBounds = YES;
    
    [rightButton addTarget:self action:@selector(ok) forControlEvents:UIControlEventTouchUpInside];
    
    [mainView addSubview:rightButton];
    
}

#pragma mark - action
- (void)cancle {
    
    [self removeFromSuperview];
    
}

- (void)ok {
    
    self.getAddressCallBackBlock(self.change, [self.addressTextField.text integerValue]);
    
    [self removeFromSuperview];
    
}

#pragma mark - UIScrollViewDelegate

- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView {
    
    NSLog(@"scrollView.contentOffset.x = %f", scrollView.contentOffset.y);
    
    if (scrollView.contentOffset.y == 0) {
        
        self.change = 0;
        
    }
    
    if (scrollView.contentOffset.y == 40) {
        
        self.change = 1;
        
    }
    
}

- (void)setAddressHeader:(NSString *)addressHeader {
    
    _addressHeader = addressHeader;
    
    self.addressHeaderLabel.text = addressHeader;
    
}

@end
