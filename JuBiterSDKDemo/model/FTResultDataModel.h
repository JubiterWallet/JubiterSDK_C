//
//  FTResultDataModel.h
//  JuBiterSDKDemo
//
//  Created by ID on 2020/3/20.
//  Copyright © 2020 飞天诚信科技股份有限公司. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface FTResultDataModel : NSObject

@property (nonatomic, copy) NSString *time;

@property (nonatomic, copy) NSString *sendData;

@property (nonatomic, copy) NSString *receiveData;

@end

NS_ASSUME_NONNULL_END
