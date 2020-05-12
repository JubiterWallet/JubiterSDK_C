//
//  main.m
//  JuBiterDemo
//
//  Created by 飞天诚信科技股份有限公司 on 2019/10/21.
//  Copyright © 2019 飞天诚信科技股份有限公司. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"

int main(int argc, char * argv[]) {
    NSString * appDelegateClassName;
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
        appDelegateClassName = NSStringFromClass([AppDelegate class]);
    }
    return UIApplicationMain(argc, argv, nil, appDelegateClassName);
}
