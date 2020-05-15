//
//  JUBSendApduView.h
//  JuBiterSDKDemo
//
//  Created by 张川 on 2020/5/14.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

typedef void (^JUBSelectApduCallBackBlock)(NSString *apdu);

@interface JUBSelectApduView : UIView

+ (JUBSelectApduView *)showAPDUArray:(NSArray *)ApduArray AboveVC:(UIViewController *)superVC  selectApduCallBackBlock:(JUBSelectApduCallBackBlock)selectApduCallBackBlock;

@end

NS_ASSUME_NONNULL_END
