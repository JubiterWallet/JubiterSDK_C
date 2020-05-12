//
//  FTConstant.h
//  JuBiterSDKDemo
//
//  Created by ID on 2020/3/19.
//  Copyright © 2020 飞天诚信科技股份有限公司. All rights reserved.
//

#ifndef FTConstant_h
#define FTConstant_h

#define selectedTransmitTypeIndexStr @"selectedTransmitTypeIndex"

#define SINGLE_LINE_WIDTH           (1 / [UIScreen mainScreen].scale)
#define SINGLE_LINE_ADJUST_OFFSET   ((1 / [UIScreen mainScreen].scale) / 2)
#define KScreenWidth [UIScreen mainScreen].bounds.size.width
#define KScreenHeight [UIScreen mainScreen].bounds.size.height
#define KStatusBarHeight [[UIApplication sharedApplication] statusBarFrame].size.height
#define KNavigationBarHeight 44

#define singleInterface +(instancetype)defaultManager;

#define singleImplementation \
\
static id _instance = nil;\
+ (instancetype)defaultManager {\
    static dispatch_once_t onceToken;\
    dispatch_once(&onceToken, ^{\
        _instance = [[super allocWithZone:NULL] init] ;\
    }) ;\
    return _instance ;\
    }\
 \
+ (id) allocWithZone:(struct _NSZone *)zone {\
    return [[self class] defaultManager] ;\
}\
- (id)copyWithZone:(NSZone *)zone {\
    return [[self class] defaultManager] ;\
}\
- (id)mutableCopyWithZone:(NSZone *)zone {\
    return [[self class] defaultManager] ;\
}\

#define iphoneX ({\
BOOL isPhoneX = NO;\
if (@available(iOS 11.0, *)) {\
if ([[[UIApplication sharedApplication] delegate] window].safeAreaInsets.bottom) {\
isPhoneX = YES;\
}\
}\
isPhoneX;\
})

#endif /* FTConstant_h */
