//
//  Tools.m
//  JuBiterSDKDemo
//
//  Created by Jermy on 2017/7/8.
//  Copyright © 2020年 FEITIAN. All rights reserved.
//

#import "Tools.h"
#import "FTConstant.h"

@interface Tools()

@property (nonatomic, weak) UITextField *pinTextField;

@end

@implementation Tools

static Tools *_instance;

+ (instancetype)defaultTools
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{

        _instance = [[self alloc] init];
    });

    return _instance;
}

- (UIColor *)colorWithHexString: (NSString *)color
{
    NSString *cString = [[color stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] uppercaseString];
    
    // String should be 6 or 8 characters
    if ([cString length] < 6) {
        return [UIColor clearColor];
    }
    // 判断前缀
    if ([cString hasPrefix:@"0X"])
        cString = [cString substringFromIndex:2];
    if ([cString hasPrefix:@"#"])
        cString = [cString substringFromIndex:1];
    if ([cString length] != 6)
        return [UIColor clearColor];
    // 从六位数值中找到RGB对应的位数并转换
    NSRange range;
    range.location = 0;
    range.length = 2;
    //R、G、B
    NSString *rString = [cString substringWithRange:range];
    range.location = 2;
    NSString *gString = [cString substringWithRange:range];
    range.location = 4;
    NSString *bString = [cString substringWithRange:range];
    // Scan values
    unsigned int r, g, b;
    [[NSScanner scannerWithString:rString] scanHexInt:&r];
    [[NSScanner scannerWithString:gString] scanHexInt:&g];
    [[NSScanner scannerWithString:bString] scanHexInt:&b];
    
    return [UIColor colorWithRed:((float) r / 255.0f) green:((float) g / 255.0f) blue:((float) b / 255.0f) alpha:1.0f];
}

+ (void)showToast:(NSString *)tip time:(NSInteger)delay {
    //防止多次添加
    if ([[UIApplication sharedApplication].keyWindow viewWithTag:1990]) {
        return;
    }
    
    UIView *backgroundView =  [[UIView alloc] initWithFrame:CGRectMake(0, 0, 100, 100)];
    
    backgroundView.backgroundColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:0.7];
    
    backgroundView.tag = 1990;
    
    [[UIApplication sharedApplication].keyWindow addSubview:backgroundView];
    
    UILabel *toastLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, KScreenWidth/3, 20)];
    
    toastLabel.text = tip;
    
    toastLabel.font = [UIFont systemFontOfSize:13];
    
    toastLabel.textColor = [UIColor whiteColor];
    
    [backgroundView addSubview:toastLabel];
    
    toastLabel.textAlignment = NSTextAlignmentCenter;
    
    toastLabel.numberOfLines = 0;
    
    [toastLabel sizeToFit];
    
    if (toastLabel.numberOfLines == 1) {
        backgroundView.frame = CGRectMake(0, 0, CGRectGetWidth(toastLabel.frame) + 40, CGRectGetHeight(toastLabel.frame) + 20);
    } else {
        backgroundView.frame = CGRectMake(0, 0, CGRectGetWidth(toastLabel.frame) + 40, CGRectGetHeight(toastLabel.frame) + 30);
    }
    
    backgroundView.center = CGPointMake(KScreenWidth/2, KScreenHeight/2);
    backgroundView.layer.cornerRadius = 4;
    backgroundView.layer.masksToBounds = YES;
    
    toastLabel.center = CGPointMake(CGRectGetWidth(backgroundView.frame)/2, CGRectGetHeight(backgroundView.frame)/2);;
    
    dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delay * NSEC_PER_SEC);
    dispatch_after(popTime, dispatch_get_main_queue(), ^() {
        [backgroundView removeFromSuperview];
    });
}

- (void)showPinAlertAboveVC:(UIViewController *)superVC getPinCallBackBlock:(JUBGetPinCallBackBlock)getPinCallBackBlock {
    
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"请输入Pin码" message:@"" preferredStyle:UIAlertControllerStyleAlert];
        
    UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action) {
        NSLog(@"点击了Cancel");
    }];
    
    UIAlertAction *okAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        
        NSLog(@"点击了OK");
        
        getPinCallBackBlock(self.pinTextField.text);
        
    }];
        
    [alertController addAction:okAction];
    
    [alertController addAction:cancelAction];
        
    [alertController addTextFieldWithConfigurationHandler:^(UITextField *textField) {
        
        textField.placeholder = @"请输入pin码";
        
        textField.keyboardType = UIKeyboardTypeNumbersAndPunctuation; 
        
        self.pinTextField = textField;
        
    }];
    
    [superVC presentViewController:alertController animated:YES completion:nil];
    
}

+ (UIViewController *)getCurrentVC {
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
