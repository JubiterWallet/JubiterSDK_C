//
//  JUBAmount.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/9/3.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBAmount.h"

@implementation JUBAmount


+ (NSString*)title:(NSString*)unit {
    
    if (   nil == unit
        || [unit isEqualToString:@""]
        ) {
        return @"Transfer amount:";
    }
    
    return [NSString stringWithFormat:@"Transfer amount(%@):", unit];
}


+ (NSString*)formatRules {
    
    return @"Numeric characters only";
}


+ (NSString*)message {
    
    return @"Press OK directly for the default values.";
}


+ (NSUInteger)limitLength {
    
    return 100;
}


+ (NSString*)errorMsg {
    
    return @"The transaction amount format is invalid.";
}


+ (BOOL)isValid:(NSString*)amount {
    
    if (   0 == [JUBRegEx regcompOpt:REG_NON_ZERO_START
                               input:(char*)[amount UTF8String]
                         inputLength:(int)[amount length]]
        || 0 == [JUBRegEx regcompOpt:REG_NUMBER
                               input:(char*)[amount UTF8String]
                         inputLength:(int)[amount length]]
        || 0 == [JUBRegEx regcompOpt:REG_DECIMALS
                               input:(char*)[amount UTF8String]
                         inputLength:(int)[amount length]]
        ) {
        
        return TRUE;
    }
    
    return FALSE;
}


+ (NSString*)convertToTheSmallestUnit:(NSString*)amount
                                point:(NSString*)point
                              decimal:(NSUInteger)decimal {
    
    NSString *toSmallestUnit = amount;
    if (   nil == amount
        || [amount isEqual:@""]
        ) {
        return toSmallestUnit;
    }
    
    if (0 == decimal) {
        
        return toSmallestUnit;
    }
    
    NSRange range = [amount rangeOfString:point];
    if (NSNotFound == range.location) {
        toSmallestUnit = [toSmallestUnit stringByAppendingString:point];
        range = [toSmallestUnit rangeOfString:point];
    }
    
    NSString* withoutPoint = [toSmallestUnit stringByReplacingOccurrencesOfString:point
                                                                       withString:@""];
    NSString * smallestUnit = [withoutPoint stringByReplacingOccurrencesOfString:@"^0+"
                                                                      withString:@""
                                                                         options:NSRegularExpressionSearch
                                                                           range:NSMakeRange(0, [withoutPoint length])];
    
    NSUInteger nDecimal = [toSmallestUnit length] - range.length - range.location;
    NSUInteger nZero = decimal - nDecimal;
    for (NSUInteger i=0; i<nZero; ++i) {
        smallestUnit = [NSString stringWithFormat:@"%@0", smallestUnit];
    }
    
    return smallestUnit;
}


+ (NSString*)convertToProperFormat:(NSString*)amount
                             point:(NSString*)point
                           decimal:(NSUInteger)decimal {
    
    NSString *proper = amount;
    
    if (0 == decimal) {
        
        return proper;
    }
    
    NSRange range = [amount rangeOfString:point];
    if (NSNotFound == range.location) {
        
        proper = [proper stringByAppendingString:point];
        range = [proper rangeOfString:point];
    }
    
    NSUInteger nDecimal = [proper length] - range.length - range.location;
    NSUInteger nZero = decimal - nDecimal;
    for (NSUInteger i=0; i<nZero; ++i) {
        
        proper = [NSString stringWithFormat:@"%@0", proper];
    }
    
    return proper;
}


@end
