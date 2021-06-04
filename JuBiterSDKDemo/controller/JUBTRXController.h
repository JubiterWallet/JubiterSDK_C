//
//  JUBTRXController.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/10/26.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBCoinController.h"


NS_ASSUME_NONNULL_BEGIN


#define JSON_FILE_TRX           "testTRX"

#define BUTTON_TITLE_TRX        @"TRX"                  //1
#define BUTTON_TITLE_TRC10      @"TRC10"                //2
#define BUTTON_TITLE_TRCFree    @"TRC-Free"             //11
#define BUTTON_TITLE_TRCUnfreeze    @"TRC-Unfreeze"     //12
#define BUTTON_TITLE_TRC20      @"TRC20"                //31
#define BUTTON_TITLE_TRC20_TRANSFER @"TRC20(transfer)"  //32
#define BUTTON_TITLE_TRC721     @"TRC721"               //33

@interface JUBTRXController : JUBCoinController

@end


NS_ASSUME_NONNULL_END
