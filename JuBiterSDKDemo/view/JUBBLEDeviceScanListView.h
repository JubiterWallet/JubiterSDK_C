//
//  JUBBLEDeviceScanListView.h
//  JuBiterSDKDemo
//
//  Created by 张川 on 2020/5/13.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

typedef void (^JUBSelectBLEDeviceCallBackBlock)(NSString *deviceName);

@interface JUBBLEDeviceScanListView : UIView

+ (JUBBLEDeviceScanListView *)showCallBack:(JUBSelectBLEDeviceCallBackBlock)getSelectBLEDeviceCallBackBlock;

- (void)addBLEDeviceArray:(NSString *)deviceName;
- (void)cleanBLEDeviceArray;

@end

NS_ASSUME_NONNULL_END
