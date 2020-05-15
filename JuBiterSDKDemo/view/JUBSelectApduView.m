//
//  JUBSelectApduView.m
//  JuBiterSDKDemo
//
//  Created by 张川 on 2020/5/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBSelectApduView.h"
#import "FTConstant.h"
#import "Tools.h"
#import "JUBBLEDeviceListCell.h"
#import "JUBAlertView.h"

#define ALPHANUM @"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"

@interface JUBSelectApduView()<UIPickerViewDelegate,UIPickerViewDataSource, UITextFieldDelegate>

@property (nonatomic, strong) UIPickerView *pickerView;

@property (nonatomic, strong) UIView *topBar;

@property (nonatomic, assign) NSInteger selectApduTypeIndex;

@property (nonatomic, strong) NSMutableArray *apduTypeArray;

@property (nonatomic, copy) JUBSelectApduCallBackBlock selectApduCallBackBlock;

@property (nonatomic, weak) UITextField *apduTextField;

@property (nonatomic, weak) UIViewController *superVC;

@end

@implementation JUBSelectApduView

+ (JUBSelectApduView *)showAPDUArray:(NSArray *)ApduArray AboveVC:(UIViewController *)superVC  selectApduCallBackBlock:(JUBSelectApduCallBackBlock)selectApduCallBackBlock {
    
    JUBSelectApduView *alertView = [JUBSelectApduView creatSelf];
    
    alertView.apduTypeArray = [ApduArray mutableCopy];
    
    [alertView.apduTypeArray addObject:@"SELF-DEFINED"];
    
    alertView.selectApduCallBackBlock = selectApduCallBackBlock;
    
    alertView.superVC  = superVC;
                        
    [alertView addSubviewAboveSuperView];
    
    return alertView;
    
}

+ (JUBSelectApduView *)creatSelf {
    
    JUBSelectApduView *alertView = [[JUBSelectApduView alloc] initWithFrame:CGRectMake(0, 0, KScreenWidth, KScreenHeight)];
    
    alertView.backgroundColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:0.3];
    
    [[UIApplication sharedApplication].keyWindow addSubview:alertView];
        
    return alertView;
    
}

- (UIView *)addMainView {
    
    UIView *mainView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, KScreenWidth - 50, 100)];
    
    mainView.center = CGPointMake(KScreenWidth/2, KScreenHeight*2/5);
    
    mainView.backgroundColor = [UIColor whiteColor];
    
    mainView.layer.cornerRadius = 4;
    
    mainView.layer.masksToBounds = YES;
    
    [self addSubview:mainView];
    
    return mainView;
    
}

- (void)addSubviewAboveSuperView {
    
    [self addSubview:self.pickerView];
    
    [self addSubview:self.topBar];
        
}

#pragma mark - 添加mainview上面的子视图

- (UIPickerView *)pickerView {
        
    if (_pickerView == nil) {
                
        UIPickerView *pickerView = [[UIPickerView alloc]initWithFrame:CGRectMake(0, KScreenHeight - 200 - (iphoneX ? 30 : 0), self.frame.size.width, 200)];
        
        _pickerView = pickerView;
        
        pickerView.delegate = self;
        
        pickerView.dataSource = self;
        
        pickerView.backgroundColor = [UIColor whiteColor];
        
        pickerView.layer.borderWidth = 1;
        
        pickerView.layer.borderColor = [UIColor grayColor].CGColor;
        
    } else {
        
        _pickerView.frame = CGRectMake(0, KScreenHeight - 200 - (iphoneX ? 30 : 0), self.frame.size.width, 200);
        
    }
        
    return _pickerView;
}

- (UIView *)topBar {
    
    CGFloat buttonWidth = 60;
    
    CGFloat buttonHeight = 45;
    
    if (_topBar == nil) {
                
        UIView *topBar = [[UIView alloc] initWithFrame:CGRectMake(0, KScreenHeight - buttonHeight - CGRectGetHeight(_pickerView.frame) - (iphoneX ? 30 : 0) , KScreenWidth, buttonHeight)];
        
        topBar.backgroundColor = [UIColor whiteColor];
        
        UIView *line = [[UIView alloc] initWithFrame:CGRectMake(0, 0, KScreenWidth, 1)];
        
        line.backgroundColor = [UIColor grayColor];
        
        [topBar addSubview:line];
        
        _topBar = topBar;
                
        UIButton *leftButton = [[UIButton alloc] initWithFrame:CGRectMake(0, 0, buttonWidth, buttonHeight)];
        
//        [leftButton setBackgroundColor:[UIColor colorWithRed:0x00/255.0 green:0xcc/255.0 blue:0xff/255.0 alpha:1]];
        
        [leftButton setTitleColor:[UIColor colorWithRed:0x00/255.0 green:0xcc/255.0 blue:0xff/255.0 alpha:1] forState:UIControlStateNormal];
        
        [leftButton setTitle:@"CANCEL" forState:UIControlStateNormal];
        
        [leftButton addTarget:self action:@selector(cancle) forControlEvents:UIControlEventTouchUpInside];
        
        leftButton.layer.cornerRadius = 4;
        
        leftButton.layer.masksToBounds = YES;
        
        [topBar addSubview:leftButton];
        
        UIButton *rightButton = [[UIButton alloc] initWithFrame:CGRectMake(KScreenWidth - buttonWidth, 0, buttonWidth, buttonHeight)];
        
//        [rightButton setBackgroundColor:[UIColor colorWithRed:0x00/255.0 green:0xcc/255.0 blue:0xff/255.0 alpha:1]];
        
        [rightButton setTitleColor:[UIColor colorWithRed:0x00/255.0 green:0xcc/255.0 blue:0xff/255.0 alpha:1] forState:UIControlStateNormal];
        
        [rightButton setTitle:@"OK" forState:UIControlStateNormal];
        
        [rightButton addTarget:self action:@selector(ok) forControlEvents:UIControlEventTouchUpInside];
        
        rightButton.layer.cornerRadius = 4;
        
        rightButton.layer.masksToBounds = YES;
        
        [topBar addSubview:rightButton];
        
    } else {
        
        _topBar.frame = CGRectMake(0, KScreenHeight - buttonHeight - CGRectGetHeight(_pickerView.frame) - (iphoneX ? 30 : 0) , KScreenWidth, buttonHeight);
        
    }
    
    return _topBar;
    
}

- (void)cancle {
    
    NSLog(@"cancle");
    
    [self dismiss];
    
}

- (void)hidenPickerView {
    
    [UIView animateWithDuration:0.5 animations:^{
        
        self.pickerView.frame = CGRectMake(0, KScreenHeight + CGRectGetHeight(self.topBar.frame), KScreenWidth, CGRectGetHeight(self.pickerView.frame));
        
        self.topBar.frame = CGRectMake(0, KScreenHeight, KScreenWidth, CGRectGetHeight(self.topBar.frame));
        
    } completion:^(BOOL finished) {
        
        [self.pickerView removeFromSuperview];
        
        [self.topBar removeFromSuperview];
        
    }];
    
}

- (void)dismiss {
    
    [UIView animateWithDuration:0.5 animations:^{
        
        self.pickerView.frame = CGRectMake(0, KScreenHeight + CGRectGetHeight(self.topBar.frame), KScreenWidth, CGRectGetHeight(self.pickerView.frame));
        
        self.topBar.frame = CGRectMake(0, KScreenHeight, KScreenWidth, CGRectGetHeight(self.topBar.frame));
        
    } completion:^(BOOL finished) {
        
        [self removeFromSuperview];
                
    }];
    
}

- (void)ok {
    
    NSLog(@"ok");
    
    NSLog(@"self.selectApduTypeIndex = %ld", (long)self.selectApduTypeIndex);
                    
    if (self.selectApduTypeIndex == self.apduTypeArray.count - 1) {
        
        [self hidenPickerView];
        
        [self addInputApduView];
        
    } else {
        
        [self dismiss];
    self.selectApduCallBackBlock(self.apduTypeArray[self.selectApduTypeIndex]);
        
    }
    
}

- (void)addInputApduView {
    
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"Please enter APDU" message:@"" preferredStyle:UIAlertControllerStyleAlert];
        
    UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action) {
        
        [self dismiss];
        
    }];
    
    UIAlertAction *okAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        
        NSLog(@"ok");
        
        if (self.apduTextField.text.length%2 != 0) {
            
            JUBAlertView *alertView = [JUBAlertView showMsg:@"The length of APDU must be a multiple of 2"];
            
            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(2 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
                
                [alertView dismiss];
                
                [self addInputApduView];
                
            });
            
            
            
        } else {
            
            self.selectApduCallBackBlock(self.apduTextField.text);
            
            [self dismiss];
            
        }
                
    }];
        
    [alertController addAction:okAction];
    
    [alertController addAction:cancelAction];
        
    [alertController addTextFieldWithConfigurationHandler:^(UITextField *textField) {
        
        textField.placeholder = @"Please enter APDU";
        
        textField.keyboardType = UIKeyboardTypeASCIICapable;
        
        textField.delegate = self;
        
        self.apduTextField = textField;
        
    }];
    
    [self.superVC presentViewController:alertController animated:YES completion:nil];
    
}


#pragma mark ------- dateSource&&Delegate --------

////设置列数
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView {
    return 1;
}

//设置指定列包含的项数
- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component{
    return self.apduTypeArray.count;
}

//设置每个选项显示的内容
- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component{
    
    return self.apduTypeArray[row];

}

//用户进行选择
- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component{
        
    _selectApduTypeIndex = row;
    
}

#pragma mark - UITextFieldDelegate

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    NSCharacterSet *cs = [[NSCharacterSet characterSetWithCharactersInString:ALPHANUM] invertedSet];
    NSString *filtered = [[string componentsSeparatedByCharactersInSet:cs] componentsJoinedByString:@""];
    return [string isEqualToString:filtered];
}

@end
