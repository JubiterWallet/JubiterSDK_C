//
//  AppDelegate.m
//  JuBiterDemo
//
//  Created by 飞天诚信科技股份有限公司 on 2019/10/21.
//  Copyright © 2019 飞天诚信科技股份有限公司. All rights reserved.
//

#import "AppDelegate.h"
#import "JUBHomePageController.h"

@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    
    self.window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    
    self.window.backgroundColor = [UIColor whiteColor];
    
    JUBHomePageController *mainVC = [[JUBHomePageController alloc] init];
    
    UINavigationController *nav = [[UINavigationController alloc] initWithRootViewController:mainVC];
    
    nav.navigationBar.translucent = NO;
    
    self.window.rootViewController = nav;
    
    [self.window makeKeyAndVisible];
    
    return YES;
}

@end
