//
//  JUBAmount.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/9/3.
//  Copyright © 2020 JuBiter. All rights reserved.
//


#import <Foundation/Foundation.h>


@interface JUBTimeOut : NSObject


+ (NSString*)title;
+ (NSString*)formatRules;
+ (NSUInteger)limitLength;

+ (NSString*)errorMsg;
+ (BOOL)isValid:(NSString*)timeout;


@end
