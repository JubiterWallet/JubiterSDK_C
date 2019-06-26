//
//  ViewController.h
//  JubSDKDemo
//
//  Created by Pan Min on 2019/6/22.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Notification.h"
#import "JubSDKCore.h"

@interface ViewController : UIViewController
@property (nonatomic) JubSDKCore* _sdk;
@property (nonatomic) NSMutableDictionary* _deviceDict;


- (void)updateUI;

- (IBAction)enumAction:(UIButton *)sender;

- (IBAction)stopEnumAction:(UIButton *)sender;

- (void)deviceArrivalObserver:notification;
@end
