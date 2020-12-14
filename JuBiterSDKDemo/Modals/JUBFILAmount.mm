//
//  JUBFILAmount.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/11/30.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBFILAmount.h"

@implementation JUBFILAmount


+ (NSString*)title:(JUB_NS_ENUM_FIL_COIN)opt {
    
    return [NSString stringWithFormat:@"%@(%@):", [JUBFILAmount enumUnitToString:opt], [JUBFILAmount enumUnitToUnitStr:opt]];
}


+ (NSString*)formatRules {
    
    return [NSString stringWithFormat:@"Numbers or decimals (up to %lu decimal places).", decimalFIL];
}


+ (NSString*)enumUnitToString:(JUB_NS_ENUM_FIL_COIN)opt {
    
    NSString* strUnit = TITLE_FIL;
    switch (opt) {
    case JUB_NS_ENUM_FIL_COIN::BTN_FIL:
    default:
        break;
    }
    
    return strUnit;
}


+ (NSString*)enumUnitToUnitStr:(JUB_NS_ENUM_FIL_COIN)opt {
    
    NSString* strUnit = TITLE_UNIT_FIL;
    switch (opt) {
    case JUB_NS_ENUM_FIL_COIN::BTN_FIL:
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
        || 0 == [JUBRegEx regcompOpt:[[NSString stringWithFormat:@REG_FORMULA_DECIMALS, decimalFIL] UTF8String]
                               input:(char*)[amount UTF8String]
                         inputLength:(int)[amount length]]
        ) {
        return TRUE;
    }
    
    return FALSE;
}


+ (BOOL)isValid:(NSString*)amount
            opt:(JUB_NS_ENUM_FIL_COIN)opt {
    
    switch (opt) {
    default:
        return [JUBFILAmount isValid:amount];
    }
    
    return FALSE;
}


+ (NSString*)convertToProperFormat:(NSString*)amount
                               opt:(JUB_NS_ENUM_FIL_COIN)opt {
    
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
                                          decimal:decimalFIL];
        break;
    }
    
    return amt;
}


@end
