//
//  JUBWarningAlert.m
//  JuBiterSDKDemo
//
//  Created by zhangchuan on 2020/8/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBWarningAlert.h"
#import "Tools.h"

@implementation JUBWarningAlert

+ (void)warningAlert:(NSString *)message warningCallBack:(JUBWarningCallBack)warningCallBack {
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"Attention" message:message preferredStyle:UIAlertControllerStyleAlert];
    
    UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action) {
    }];
    
    UIAlertAction *okAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        
        warningCallBack();
    }];
    
    [alertController addAction:okAction];
    
    [alertController addAction:cancelAction];
    
    [[Tools getCurrentVC] presentViewController:alertController animated:YES completion:nil];
}

@end
