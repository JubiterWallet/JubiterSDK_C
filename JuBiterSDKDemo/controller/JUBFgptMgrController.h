//
//  JUBFgptMgrController.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/8/13.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JubSDKCore/JUB_SDK.h"
#include "JUB_SDK_main.h"


NS_ASSUME_NONNULL_BEGIN

#define BUTTON_TITLE_FGPT_ENROLL    @"ENROLL FGPT"
#define BUTTON_TITLE_FGPT_ENUM      @"ENUM FGPT"
#define BUTTON_TITLE_FGPT_ERASE     @"ERASE ALL FGPT"
#define BUTTON_TITLE_FGPT_DELETE    @"DELETE FGPT"

typedef NS_ENUM(NSInteger, JUB_NS_ENUM_FGPT_OPT) {
    MGR_ENROLL,
    MGR_ENUM,
    MGR_ERASE,
    MGR_DELETE,
};


#define BUTTON_TITLE_VIA_DEV    @"via device"
#define BUTTON_TITLE_VIA_9GRIDS @"via 9 Grids"
#define BUTTON_TITLE_VIA_APDU   @"via APDU"
//#define BUTTON_TITLE_VIA_FGPT   @"via fingerprint"


typedef struct FgptEnrollInfo {
   NSUInteger modalityID;   // Modality ID.
   NSUInteger nextIndex;    // The number of times of current fingerprint modality.
   NSUInteger times;        // Total number of times that need to enroll for current fingerprint modality.
} stFgptEnrollInfo;


typedef struct FgptEnrollResult {
    FgptEnrollInfo enrollInfo;
    NSUInteger rv;
} stFgptEnrollResult;


@interface JUBFgptMgrController : JUBFingerManagerBaseController

@end

NS_ASSUME_NONNULL_END
