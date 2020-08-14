//
//  Tools.h
//  JuBiterSDKDemo
//
//  Created by Jermy on 2017/7/8.
//  Copyright © 2020年 FEITIAN. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "JUBButtonModel.h"

typedef void (^JUBGetPinCallBackBlock)(NSString *pin);

@interface Tools : NSObject

+(instancetype)defaultTools;

+ (void)showToast:(NSString *)tip time:(NSInteger)delay;

- (UIColor *)colorWithHexString: (NSString *)color;

+ (UIViewController *)getCurrentVC;

@end
