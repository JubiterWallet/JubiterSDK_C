//
//  JUBEOSAmount.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/9/4.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBEOSAmount.h"

@implementation JUBEOSAmount


+ (NSString*)title:(JUB_NS_ENUM_EOS_OPT)opt {
    
    return [NSString stringWithFormat:@"%@(%@):", [JUBEOSAmount enumUnitToString:opt], [JUBEOSAmount enumUnitToUnitStr:opt]];
}


+ (NSString*)formatRules:(JUB_NS_ENUM_EOS_OPT)opt {
    
    NSString* strRules = [NSString stringWithFormat:@"Numbers or decimals (up to %lu decimal places).", decimalEOS];
    switch (opt) {
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_SELLRAM:
        strRules = @"Numeric characters only, less than 2^63.";
        break;
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_BUYRAM:
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_STAKE:
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_UNSTAKE:
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_TOKEN:
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS:
    default:
        break;
    }
    
    return strRules;
}


+ (NSString*)enumUnitToString:(JUB_NS_ENUM_EOS_OPT)opt {
    
    NSString* strUnit = TITLE_EOS;
    switch (opt) {
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_SELLRAM:
        strUnit = TITLE_SELLRAM;
        break;
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_BUYRAM:
        strUnit = TITLE_BUYRAM;
        break;
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_STAKE:
        strUnit = TITLE_STAKE;
        break;
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_UNSTAKE:
        strUnit = TITLE_UNSTAKE;
        break;
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_TOKEN:
        strUnit = TITLE_EOS_TOKEN;
        break;
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS:
    default:
        break;
    }
    
    return strUnit;
}


+ (NSString*)enumUnitToUnitStr:(JUB_NS_ENUM_EOS_OPT)opt {
    
    NSString* strUnit = TITLE_UNIT_EOS;
    switch (opt) {
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_SELLRAM:
        strUnit = TITLE_UNIT_BYTES;
        break;
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS:
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_TOKEN:
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_BUYRAM:
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_STAKE:
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_UNSTAKE:
    default:
        break;
    }
    
    return strUnit;
}


+ (BOOL)isBytesValid:(NSString*)bytes {
    
    if (0 == [JUBRegEx regcompOpt:"^(0|[1-9][0-9]*)$"
                            input:(char*)[bytes UTF8String]
                      inputLength:(int)[bytes length]]
        ) {
        return TRUE;
    }
    
    return FALSE;
}


+ (BOOL)isValid:(NSString*)amount {
    
    if (   0 == [JUBRegEx regcompOpt:REG_NON_ZERO_START
                               input:(char*)[amount UTF8String]
                         inputLength:(int)[amount length]]
        || 0 == [JUBRegEx regcompOpt:REG_NUMBER
                                input:(char*)[amount UTF8String]
                          inputLength:(int)[amount length]]
        || 0 == [JUBRegEx regcompOpt:[[NSString stringWithFormat:@REG_FORMULA_DECIMALS, decimalEOS] UTF8String]
                               input:(char*)[amount UTF8String]
                         inputLength:(int)[amount length]]
        ) {
        return TRUE;
    }
    
    return FALSE;
}


+ (BOOL)isValid:(NSString*)amount
            opt:(JUB_NS_ENUM_EOS_OPT)opt {
    
    switch (opt) {
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_SELLRAM:
        return [JUBEOSAmount isBytesValid:amount];
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS:
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_BUYRAM:
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_STAKE:
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_UNSTAKE:
    default:
        return [JUBEOSAmount isValid:amount];
    }
    
    return FALSE;
}


+ (NSString*)convertToProperFormat:(NSString*)amount
                               opt:(JUB_NS_ENUM_EOS_OPT)opt {
    
    NSString *amt = amount;
    if (   nil == amount
        || [amount isEqual:@""]
        ) {
        return amt;
    }
    
    switch (opt) {
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_SELLRAM:
        break;
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_TOKEN:
        amt = [JUBEOSAmount convertToProperFormat:amount
                                            point:@"."
                                          decimal:0];
        break;
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS:
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_BUYRAM:
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_STAKE:
    case JUB_NS_ENUM_EOS_OPT::BTN_EOS_UNSTAKE:
    default:
        amt = [JUBEOSAmount convertToProperFormat:amount
                                            point:@"."
                                          decimal:decimalEOS];
        break;
    }
    
    return amt;
}


+ (NSString*)replaceAmount:(NSString*)amount
                     asset:(NSString*)asset {
    
    NSRange range = [asset rangeOfString:@" "];
    if (NSNotFound == range.location) {
        return amount;
    }
    
    NSRange unitRange = NSMakeRange(range.location,
                                    [asset length] - range.location);
    
    return [amount stringByAppendingString:[asset substringWithRange:unitRange]];
}


@end
