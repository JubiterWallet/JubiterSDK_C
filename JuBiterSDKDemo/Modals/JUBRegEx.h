//
//  JUBRegEx.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/9/3.
//  Copyright © 2020 JuBiter. All rights reserved.
//


#import <Foundation/Foundation.h>
#import <regex.h>


#define REG_EXTENDED_BEGIN  '^'
#define REG_EXTENDED_END    '$'

#define REG_NON_ZERO_START  "^(0)$"
#define REG_NUMBER          "^([0-9]{1,})$"
#define REG_DECIMALS        "^([0-9]{1,}[.][0-9]*)$"

#define REG_FORMULA_DECIMALS    "^([0-9]{1,}[.][0-9]{1,%lu})$"


@interface JUBRegEx : NSObject

+ (int)regcompOpt:(const char *)pPattern
            input:(const char *)pInput
      inputLength:(int)iInputLen;

@end
