//
//  JUBTRXAmount.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/10/26.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBTRXAmount.h"

@implementation JUBTRXAmount


+ (NSString*)title:(JUB_NS_ENUM_TRX_OPT)opt {
    
    return [NSString stringWithFormat:@"%@(%@):", [JUBTRXAmount enumUnitToString:opt], [JUBTRXAmount enumUnitToUnitStr:opt]];
}


+ (NSString*)formatRules {
    
    return [NSString stringWithFormat:@"Numbers or decimals (up to %lu decimal places).", decimalTRX];
}


+ (NSString*)enumUnitToString:(JUB_NS_ENUM_TRX_OPT)opt {
    
    NSString* strUnit = TITLE_TRX;
    switch (opt) {
    case JUB_NS_ENUM_TRX_OPT::BTN_TRX:
    default:
        break;
    }
    
    return strUnit;
}


+ (NSString*)enumUnitToUnitStr:(JUB_NS_ENUM_TRX_OPT)opt {
    
    NSString* strUnit = TITLE_UNIT_TRX;
    switch (opt) {
    case JUB_NS_ENUM_TRX_OPT::BTN_TRX:
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
        || 0 == [JUBRegEx regcompOpt:[[NSString stringWithFormat:@REG_FORMULA_DECIMALS, decimalTRX] UTF8String]
                               input:(char*)[amount UTF8String]
                         inputLength:(int)[amount length]]
        ) {
        return TRUE;
    }
    
    return FALSE;
}


+ (BOOL)isValid:(NSString*)amount
            opt:(JUB_NS_ENUM_TRX_OPT)opt {
    
    switch (opt) {
    default:
        return [JUBTRXAmount isValid:amount];
    }
    
    return FALSE;
}


+ (NSString*)convertToProperFormat:(NSString*)amount
                               opt:(JUB_NS_ENUM_TRX_OPT)opt {
    
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
                                          decimal:decimalTRX];
        break;
    }
    
    return amt;
}


@end
