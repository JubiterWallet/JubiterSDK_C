//
//  JUBAmount.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/9/3.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBTimeOut.h"
#import "JUBRegEx.h"

@implementation JUBTimeOut

+ (NSString*)title {
    return @"Set time-out";
}

+ (NSString*)formatRules {
    return @"Must be a number and less than 600.";
}

+ (NSUInteger)limitLength {
    return 3;
}

+ (NSString*)errorMsg {
    return @"Input format error.";
}

+ (BOOL)isValid:(NSString*)timeout {
    //"^[0-9]*$"
    if (   0 == [JUBRegEx regcompOpt:REG_NON_ZERO_START
                               input:(char*)[timeout UTF8String]
                         inputLength:(int)[timeout length]]
        || 0 == [JUBRegEx regcompOpt:REG_NUMBER
                               input:(char*)[timeout UTF8String]
                         inputLength:(int)[timeout length]]
        ) {
        return TRUE;
    }
    
    return FALSE;
}


@end
