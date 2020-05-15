//
//  JUBInputAddressView.h
//  JuBiterSDKDemo
//
//  Created by 张川 on 2020/5/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

typedef void (^JUBGetAddressCallBackBlock)(NSInteger change, NSInteger address);

@interface JUBInputAddressView : UIView

+ (JUBInputAddressView *)showCallBack:(JUBGetAddressCallBackBlock)getAddressCallBackBlock;

@property (nonatomic, copy) NSString *addressHeader;

@end

NS_ASSUME_NONNULL_END
