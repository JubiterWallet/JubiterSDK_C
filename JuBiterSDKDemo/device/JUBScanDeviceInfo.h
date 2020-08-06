//
//  JUBScanDeviceInfo.h
//  JubSDKDemo
//
//  Created by Pan Min on 2019/6/26.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#ifndef JUBScanDeviceInfo_h
#define JUBScanDeviceInfo_h
#import <Foundation/Foundation.h>


#define SCAN_DEVICE_INFO_UUID       @"uuid"
#define SCAN_DEVICE_INFO_DEVICE     @"device"


@interface JUBScanDeviceInfo : NSObject
@property (nonatomic, strong) NSString* name;
@property (nonatomic, strong) NSString* uuid;
@property (nonatomic, assign) NSUInteger type;
//@property (nonatomic, assign) NSInteger handle;
//@property (nonatomic, assign) NSUInteger ethContextID;

- (NSString*)getName;
@end

#endif /* Object_h */
