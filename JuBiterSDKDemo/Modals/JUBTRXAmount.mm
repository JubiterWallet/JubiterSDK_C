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

+(NSString*)formatResourceRules
{
    return @"Please enter 0 or 1(resource)";
}
+(NSString*)formatDurationRules
{
    return @"Minimum 3 daysDuration()";
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
                strUnit = @"TRX";
                break;
        case JUB_NS_ENUM_TRX_OPT::BTN_TRC10:
                strUnit = @"TRC10";
                break;
        case JUB_NS_ENUM_TRX_OPT::BTN_TRCFree:
                strUnit = @"TRCFree";
                break;
        case JUB_NS_ENUM_TRX_OPT::BTN_TRCUnfreeze:
                strUnit = @"TRCUnfreeze";
                break;
        case JUB_NS_ENUM_TRX_OPT::BTN_TRC20:
                strUnit = @"TRC20";
                break;
        case JUB_NS_ENUM_TRX_OPT::BTN_TRC20_transfer:
                strUnit = @"TRC20_transfer";
                break;
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

+(NSString *)HexToStr: (Byte *)ibuffer alllength: (int) ilength
{
    NSString *strresult = [[NSString alloc]init];
    
    for(int i = 0; i< ilength; i++)
    {
        int ivalue = ibuffer[i];
        NSString *stringint = [NSString stringWithFormat:@"%02X", ivalue];
        strresult = [[NSString alloc]initWithFormat:@"%@%@", strresult, stringint];
    }
    return strresult;
}

+ (NSString *)HexToStr: (NSData *)data
{
    Byte *ibuffer = (Byte *)[data bytes];
    return [self HexToStr:ibuffer alllength:(int)[data length]];
}


+ (Byte *)StrToHex: (NSString *)hexstring reBuffer:(Byte *)ibuffer
{
    if ([hexstring length]%2)
        return Nil;
    
    Byte *result = ibuffer; //alloca([hexstring length]/2);
    
    for (int i = 0; i< [hexstring length]; i += 2)
    {
        NSString *str1 = [hexstring substringWithRange:NSMakeRange(i, 2)];
        unsigned long ivalue = strtoul([str1 UTF8String], 0, 16);
        result[i/2] = (ivalue & 0xff);
    }
    return result;
}


+ (NSData *)StrToHex: (NSString *)hexstring
{
    NSData *retdata = nil;
    Byte *result = (Byte *)alloca([hexstring length]/2);//new Byte[[hexstring length]/2];
    
    result = [self StrToHex:hexstring reBuffer:result];
    retdata = [[NSData alloc]initWithBytes:result length:[hexstring length]/2];
    
    return retdata;
}
@end
