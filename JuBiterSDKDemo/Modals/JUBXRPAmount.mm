//
//  JUBXRPAmount.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/9/6.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBXRPAmount.h"

@implementation JUBXRPAmount


+ (NSString*)title:(JUB_NS_ENUM_XRP_COIN)opt {
    
    return [NSString stringWithFormat:@"%@(%@):", [JUBXRPAmount enumUnitToString:opt], [JUBXRPAmount enumUnitToUnitStr:opt]];
}


+ (NSString*)formatRules {
    
    return [NSString stringWithFormat:@"Numbers or decimals (up to %lu decimal places).", decimalXRP];
}


+ (NSString*)enumUnitToString:(JUB_NS_ENUM_XRP_COIN)opt {
    
    NSString* strUnit = TITLE_XRP;
    switch (opt) {
    case JUB_NS_ENUM_XRP_COIN::BTN_XRP:
    default:
        break;
    }
    
    return strUnit;
}


+ (NSString*)enumUnitToUnitStr:(JUB_NS_ENUM_XRP_COIN)opt {
    
    NSString* strUnit = TITLE_UNIT_XRP;
    switch (opt) {
    case JUB_NS_ENUM_XRP_COIN::BTN_XRP:
    default:
        break;
    }
    
    return strUnit;
}


+ (BOOL)isValid:(NSString*)amount {
    
    if (   0 == [JUBRegEx regcompOpt:REG_NON_ZERO_START
                               input:(char*)[amount UTF8String]
                         inputLength:(int)[amount length]]
        || 0 == [JUBRegEx regcompOpt:REG_NUMBER
                               input:(char*)[amount UTF8String]
                         inputLength:(int)[amount length]]
        || 0 == [JUBRegEx regcompOpt:[[NSString stringWithFormat:@REG_FORMULA_DECIMALS, decimalXRP] UTF8String]
                               input:(char*)[amount UTF8String]
                         inputLength:(int)[amount length]]
        ) {
        return TRUE;
    }
    
    return FALSE;
}


+ (BOOL)isValid:(NSString*)amount
            opt:(JUB_NS_ENUM_XRP_COIN)opt {
    
    switch (opt) {
    default:
        return [JUBXRPAmount isValid:amount];
    }
    
    return FALSE;
}


+ (NSString*)convertToProperFormat:(NSString*)amount
                               opt:(JUB_NS_ENUM_XRP_COIN)opt {
    
    NSString *amt = amount;
    if (   nil == amount
        || [amount isEqual:@""]
        ) {
        return amt;
    }
    
    switch (opt) {
    default:
        amt = [JUBAmount convertToTheSmallestUnit:amount
                                            point:@"."
                                          decimal:decimalXRP];
        break;
    }
    
    return amt;
}


@end
