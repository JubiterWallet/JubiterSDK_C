//
//  JUBErrorCode.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/9/7.
//  Copyright © 2020 JuBiter. All rights reserved.
//


#import <Foundation/Foundation.h>
#import <JubSDKCore/JUB_SDK.h>

@interface JUBErrorCode : NSObject

+ (NSString*) GetErrMsg:(NSUInteger)rv;

@end
