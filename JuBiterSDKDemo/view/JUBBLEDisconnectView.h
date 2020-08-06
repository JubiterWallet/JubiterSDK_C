//
//  JUBBLEDisconnectView.h
//  JuBiterSDKDemo
//
//  Created by zhangchuan on 2020/5/13.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

typedef void (^JUBDisConnectBLEDeviceCallBack)(NSString *deviceName);

@interface JUBBLEDisconnectView : UIView

+ (JUBBLEDisconnectView *)showCallBack:(JUBDisConnectBLEDeviceCallBack)disConnectBLEDeviceCallBack;

- (void)addDeviceArray:(NSArray<NSString *> *)deviceNameArray;

@end

NS_ASSUME_NONNULL_END
