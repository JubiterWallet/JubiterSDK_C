//
//  JUBDevicePool.m
//  JubSDKDemo
//
//  Created by Pan Min on 2019/6/24.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>
#import "JUBNotification.h"

@interface JUBDevicePool : NSObject<CBCentralManagerDelegate>
@property (nonatomic, strong) CBCentralManager *central;
@end

static JUBDevicePool *g_devicePool = nil;

@implementation JUBDevicePool

+ (instancetype) shareDevicePool {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        g_devicePool = [[self alloc] init];
    });
    return g_devicePool;
}

- (instancetype) init {
    if (self = [super init]) {
        self.central = [[CBCentralManager alloc] initWithDelegate:self
                                                            queue:nil];
    }
    return self;
}

- (void) centralManagerDidUpdateState:(CBCentralManager *)central {
    
//    NSLog(@"booltooth state： %ld", central.state);
    switch (central.state) {
    case CBManagerStateUnknown:
        NSLog(@"[Current BLE status: %@]", @"CBManagerStateUnknown");
        break;
    case CBManagerStateResetting:
        NSLog(@"[Current BLE status: %@]", @"CBManagerStateResetting");
        break;
    case CBManagerStateUnsupported:
        NSLog(@"[Current BLE status: %@]", @"CBManagerStateUnsupported");
        break;
    case CBManagerStateUnauthorized:
        NSLog(@"[Current BLE status: %@]", @"CBManagerStateUnauthorized");
        break;
    case CBManagerStatePoweredOff:
        NSLog(@"[Current BLE status: %@]", @"CBManagerStatePoweredOff");
        break;
    case CBManagerStatePoweredOn:
        NSLog(@"[Current BLE status: %@]", @"CBManagerStatePoweredOn");
        break;
    }   // switch (central.state) end
    
    [[NSNotificationCenter defaultCenter] postNotificationName:NOTIF_DEVICE_ARRVL
                                                        object:self
                                                      userInfo:@{@"name":@(central.state)}];
}

@end


