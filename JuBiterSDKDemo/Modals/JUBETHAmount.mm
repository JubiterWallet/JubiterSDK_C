//
//  JUBETHAmount.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/9/6.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBETHAmount.h"

@implementation JUBETHAmount


+ (NSString*)title:(JUB_NS_ENUM_ETH_COIN)opt {
    
    return [NSString stringWithFormat:@"%@(%@):", [JUBETHAmount enumUnitToString:opt], [JUBETHAmount enumUnitToUnitStr:opt]];
}


+ (NSString*)formatRules {
    
    return [NSString stringWithFormat:@"Numbers or decimals (up to %lu decimal places).", decimalETH];
}


+ (NSString*)enumUnitToString:(JUB_NS_ENUM_ETH_COIN)opt {
    
    NSString* strUnit = TITLE_ETH;
    switch (opt) {
    case JUB_NS_ENUM_ETH_COIN::BTN_ETH_ERC20:
        strUnit = TITLE_ETH_TOKEN;
        break;
    case JUB_NS_ENUM_ETH_COIN::BTN_ETH:
    default:
        break;
    }
    
    return strUnit;
}


+ (NSString*)enumUnitToUnitStr:(JUB_NS_ENUM_ETH_COIN)opt {
    
    NSString* strUnit = TITLE_UNIT_ETH;
    switch (opt) {
    case JUB_NS_ENUM_ETH_COIN::BTN_ETH:
    case JUB_NS_ENUM_ETH_COIN::BTN_ETH_ERC20:
    case JUB_NS_ENUM_ETH_COIN::BTN_ETC:
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
        || 0 == [JUBRegEx regcompOpt:[[NSString stringWithFormat:@REG_FORMULA_DECIMALS, decimalETH] UTF8String]
                               input:(char*)[amount UTF8String]
                         inputLength:(int)[amount length]]
        ) {
        return TRUE;
    }
    
    return FALSE;
}


+ (BOOL)isValid:(NSString*)amount
            opt:(JUB_NS_ENUM_ETH_COIN)opt {
    
    switch (opt) {
    default:
        return [JUBETHAmount isValid:amount];
    }
    
    return FALSE;
}


+ (NSString*)convertToProperFormat:(NSString*)amount
                               opt:(JUB_NS_ENUM_ETH_COIN)opt {
    
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
                                          decimal:decimalETH];
        break;
    }
    
    return amt;
}


@end
