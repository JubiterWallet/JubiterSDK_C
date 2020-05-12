//
//  JUBCoinTestMainView.h
//  JuBiterSDKDemo
//
//  Created by ID on 2020/4/8.
//  Copyright © 2020 Jubiter. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "JUBButtonModel.h"
#import "FTNFCResultDataModel.h"

typedef void (^JUBCoinTestMainViewCallBackBlock)(NSInteger index);

NS_ASSUME_NONNULL_BEGIN

@interface JUBCoinTestMainView : UIView

+ (JUBCoinTestMainView *)coinTestMainViewWithFrame:(CGRect)frame buttonArray:(NSArray<JUBButtonModel *> *)btnArray;

- (void)addMsgData:(NSString *)msgData;

@property (nonatomic, strong) JUBCoinTestMainViewCallBackBlock transmissionViewCallBackBlock;

@end

NS_ASSUME_NONNULL_END
