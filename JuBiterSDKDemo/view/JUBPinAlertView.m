//
//  JUBPinAlertView.m
//  JuBiterSDKDemo
//
//  Created by zhangchuan on 2020/6/30.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#define inputPinTextFieldTag 100
#define oldPinTextFieldTag 200
#define n1wPinTextFieldTag 300

#import "JUBPinAlertView.h"


@interface JUBPinAlertView()<UITextFieldDelegate>
@property (nonatomic, weak) UIAlertAction *okAction;
@property (nonatomic, weak) UITextField *inputPinTextField;
@property (nonatomic, weak) UITextField *oldPinTextField;
@property (nonatomic, weak) UITextField *n1wPinTextField;
@end


@implementation JUBPinAlertView


#pragma mark - 输入pin码
+ (void)showInputPinAlert:(JUBInputPinCallBack)inputPinCallBack {
    
    [[[JUBPinAlertView alloc] init] showInputPinAlert:inputPinCallBack
                                 fingerprintsCallBack:nil];
}


+ (void)showInputPinAlert:(JUBInputPinCallBack)inputPinCallBack
     fingerprintsCallBack:(JUBFingerprintsCallBack)fingerprintsCallBack {
    
    [[[JUBPinAlertView alloc] init] showInputPinAlert:inputPinCallBack
                                 fingerprintsCallBack:fingerprintsCallBack];
}


- (void)showInputPinAlert:(JUBInputPinCallBack)inputPinCallBack
     fingerprintsCallBack:(JUBFingerprintsCallBack)fingerprintsCallBack {
    
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"Please enter PIN"
                                                                             message:@""
                                                                      preferredStyle:UIAlertControllerStyleAlert];
    
    UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:@"Cancel"
                                                           style:UIAlertActionStyleCancel
                                                         handler:^(UIAlertAction * _Nonnull action) {
        dispatch_async(dispatch_get_global_queue(0, 0), ^{
            inputPinCallBack(NULL);
        });
    }];
    
    UIAlertAction *okAction = [UIAlertAction actionWithTitle:@"OK"
                                                       style:UIAlertActionStyleDefault
                                                     handler:^(UIAlertAction * _Nonnull action) {
        // Solve the problem that the log cannot be updated in time because the operation is stuck in the main thread
//        inputPinCallBack(self.inputPinTextField.text);
        NSString *inputPin = self.inputPinTextField.text;
        dispatch_async(dispatch_get_global_queue(0, 0), ^{
            inputPinCallBack(inputPin);
        });
    }];
    
    self.okAction = okAction;
    
    okAction.enabled = NO;
    
    [alertController addAction:okAction];
    
    [alertController addAction:cancelAction];
    
    if (fingerprintsCallBack) {
        
        UIAlertAction *fingerprintsAction = [UIAlertAction actionWithTitle:@"Switch to fingerprint"
                                                                     style:UIAlertActionStyleDefault
                                                                   handler:^(UIAlertAction * _Nonnull action) {
            fingerprintsCallBack();
        }];
        
        [alertController addAction:fingerprintsAction];
    }
    
    [alertController addTextFieldWithConfigurationHandler:^(UITextField *textField) {
        
        textField.placeholder = @"Please enter PIN";
        
        textField.keyboardType = UIKeyboardTypeNumbersAndPunctuation;
        
        [textField addTarget:self
                      action:@selector(changedInputPinTextField:)
            forControlEvents:UIControlEventEditingChanged];
        
        self.inputPinTextField = textField;
        
        self.inputPinTextField.delegate = self;
    }];
    
    [[self getCurrentVC] presentViewController:alertController
                                      animated:YES
                                    completion:nil];
}


#pragma mark - 修改pin码
+ (void)showChangePinAlert:(JUBChangePinCallBack)changePinCallBack {
    
    [[[JUBPinAlertView alloc] init] showChangePinAlert:changePinCallBack];
}


- (void)showChangePinAlert:(JUBChangePinCallBack)changePinCallBack {
    
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"Modify PIN"
                                                                             message:@""
                                                                      preferredStyle:UIAlertControllerStyleAlert];
    
    UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:@"Cancel"
                                                           style:UIAlertActionStyleCancel
                                                         handler:^(UIAlertAction * _Nonnull action) {
    }];
    
    UIAlertAction *okAction = [UIAlertAction actionWithTitle:@"OK"
                                                       style:UIAlertActionStyleDefault
                                                     handler:^(UIAlertAction * _Nonnull action) {
        changePinCallBack(self.oldPinTextField.text, self.n1wPinTextField.text);
    }];
    
    self.okAction = okAction;
    
    [alertController addAction:okAction];
    
    [alertController addAction:cancelAction];
    
    [alertController addTextFieldWithConfigurationHandler:^(UITextField *textField) {
        
        textField.placeholder = @"Please enter old PIN";
        
        textField.keyboardType = UIKeyboardTypeNumbersAndPunctuation;
        
        [textField addTarget:self
                      action:@selector(changedOldPinTextField:)
            forControlEvents:UIControlEventEditingChanged];
        
        self.oldPinTextField = textField;
    }];
    
    [alertController addTextFieldWithConfigurationHandler:^(UITextField *textField) {
        
        textField.placeholder = @"Please enter new PIN";
        
        textField.keyboardType = UIKeyboardTypeNumbersAndPunctuation;
        
        [textField addTarget:self
                      action:@selector(changedN1wPinTextField:)
            forControlEvents:UIControlEventEditingChanged];
        
        self.n1wPinTextField = textField;
    }];
    
    [[self getCurrentVC] presentViewController:alertController
                                      animated:YES
                                    completion:nil];
}


#pragma mark - textField代理
- (void)changedInputPinTextField:(UITextField *)textField {
    
    NSLog(@"textField12 = %@", textField.text);
    
    if (textField.text.length > 0) {
        self.okAction.enabled = YES;
    }
    else {
        self.okAction.enabled = NO;
    }
}


- (void)changedOldPinTextField:(UITextField *)textField {
    
    if (self.n1wPinTextField.text.length > 0
//        &&         textField.text.length > 0
        ) {
        self.okAction.enabled = YES;
    }
    else {
        self.okAction.enabled = NO;
    }
}


- (void)changedN1wPinTextField:(UITextField *)textField {

    if (textField.text.length > 0
//        && self.oldPinTextField.text.length > 0
        ) {
        self.okAction.enabled = YES;
    }
    else {
        self.okAction.enabled = NO;
    }
}


#pragma mark - 获取当前控制器
- (UIViewController *)getCurrentVC {
    
    UIViewController *result = nil;
    
    UIWindow *window = [[UIApplication sharedApplication] keyWindow];
    if (window.windowLevel != UIWindowLevelNormal) {
        NSArray *windows = [[UIApplication sharedApplication] windows];
        for (UIWindow *temp in windows) {
            if (temp.windowLevel == UIWindowLevelNormal) {
                window = temp;
                break;
            }
        }
    }
    
    //取当前展示的控制器
    result = window.rootViewController;
    while (result.presentedViewController) {
        result = result.presentedViewController;
    }
    
    //如果为UITabBarController：取选中控制器
    if ([result isKindOfClass:[UITabBarController class]]) {
        result = [(UITabBarController *)result selectedViewController];
    }
    
    //如果为UINavigationController：取可视控制器
    if ([result isKindOfClass:[UINavigationController class]]) {
        result = [(UINavigationController *)result visibleViewController];
    }
    
    return result;
}

@end
