//
//  JUBRegEx.m
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/9/3.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBRegEx.h"


@implementation JUBRegEx

+ (int)regcompOpt:(const char *)pPattern
            input:(const char *)pInput
      inputLength:(int)iInputLen
{
    int ret = 0;
//    size_t ret1 = 0;
//    char ebuf[128] = {0,};
    
    size_t iPatternLen = strlen(pPattern);
    size_t iTempPatternLen = iPatternLen+2;
    char *pTempPattern = (char *)malloc(iTempPatternLen+1);
    bzero(pTempPattern, iTempPatternLen+1);
    memcpy(pTempPattern, pPattern, iPatternLen);
    
    // Compile regular expression
    regex_t regex;
    // REG_BASIC, REG_EXTENDED, REG_ICASE, REG_NOSUB, REG_NEWLINE
    int cflags = REG_BASIC|REG_EXTENDED|REG_ICASE|REG_NOSUB|REG_NEWLINE;
    if (   (REG_EXTENDED_BEGIN == *pPattern)
        && (REG_EXTENDED_END   == *(pPattern+iPatternLen-1))
        ) {
    }
    else {
        *pTempPattern = REG_EXTENDED_BEGIN;
        memcpy(pTempPattern+1, pPattern, iPatternLen);
        *(pTempPattern+1+iPatternLen) = REG_EXTENDED_END;
    }
    ret = regcomp(&regex, pTempPattern, cflags);
    if (0 != ret) {
//        ret1 = REG_NOMATCH;
//        ret1 = regerror(ret, &regex, ebuf, (size_t)sizeof(ebuf));
        regfree(&regex);
        return ret;
    }
    
    // Execute regular expression
    // REG_NOTBOL, REG_NOTEOL
    regmatch_t pmatch[20] = {0x00,};
    ret = regexec(&regex, pInput, iInputLen, pmatch, 0);
    // REG_NOMATCH
    if (0 != ret) {
//        ret1 = regerror(ret, &regex, ebuf, (size_t)sizeof(ebuf));
        regfree(&regex);
        return ret;
    }
    
    // Free compiled regular expression if you want to use the regex_t again
    regfree(&regex);
    
    if (pTempPattern) {
        free(pTempPattern); pTempPattern = NULL;
    }
    
    return ret;
}



@end
