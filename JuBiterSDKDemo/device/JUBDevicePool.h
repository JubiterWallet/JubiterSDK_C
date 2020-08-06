//
//  JUBDevicePool.h
//  JubSDKDemo
//
//  Created by Pan Min on 2019/6/22.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>

@interface JUBDevicePool : NSObject<CBCentralManagerDelegate>

+ (instancetype)shareDevicePool;

@end
