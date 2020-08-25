//
//  JUBAmount.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/9/3.
//  Copyright © 2020 JuBiter. All rights reserved.
//


#import <Foundation/Foundation.h>
#import "JUBRegEx.h"

@interface JUBAmount : NSObject

+ (NSString*)title:(NSString*)unit;
+ (NSString*)formatRules;
+ (NSString*)message;
+ (NSUInteger)limitLength;

+ (NSString*)errorMsg;
+ (BOOL)isValid:(NSString*)amount;

+ (NSString*)convertToTheSmallestUnit:(NSString*)amount
                                point:(NSString*)point
                              decimal:(NSUInteger)decimal;

+ (NSString*)convertToProperFormat:(NSString*)amount
                             point:(NSString*)point
                           decimal:(NSUInteger)decimal;

@end
