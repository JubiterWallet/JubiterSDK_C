//
//  JUBCoinTestListBaseController.h
//  JuBiterSDKDemo
//
//  Created by 张川 on 2020/4/28.
//  Copyright © 2020 Jubiter. All rights reserved.
//  首页的基类，用户应该继承本类，去实现自己的业务逻辑

#import <UIKit/UIKit.h>
#import "FTConstant.h"
#import "JUBCoinTestMainView.h"
#import "Tools.h"

NS_ASSUME_NONNULL_BEGIN

@interface JUBCoinTestListBaseController : UIViewController

@property (nonatomic, assign) NSInteger selectedTransmitTypeIndex;

- (void)addMsgData:(NSString *)msgData;

- (void)initUI;

@end

NS_ASSUME_NONNULL_END
