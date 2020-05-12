//
//  JUBDeviceController.h
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/6.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBController.h"
#import "JubSDKCore/JUB_SDK.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, JUB_NS_ENUM_DEV_OPT) {
    DEVICE_INFO,
    DEVICE_APPLETS,
    DEVICE_CERT,
    GENERATE_SEED,
    IMPORT_MNEMONIC12,
    IMPORT_MNEMONIC24,
//      IMPORT_SEED,
//      EXPORT_SEED
};

@interface JUBDeviceController : JUBController
@end

NS_ASSUME_NONNULL_END
