//
//  FTResultDataModel.m
//  JuBiterSDKDemo
//
//  Created by ID on 2020/3/20.
//  Copyright © 2020 飞天诚信科技股份有限公司. All rights reserved.
//

#import "FTResultDataModel.h"

@implementation FTResultDataModel

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.time = [self getCurrentTimes];
    }
    return self;
}

- (NSString*)getCurrentTimes{

    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];

    // ----------设置你想要的格式,hh与HH的区别:分别表示12小时制,24小时制

    [formatter setDateFormat:@"YYYY-MM-dd HH:mm:ss"];

    //现在时间,你可以输出来看下是什么格式

    NSDate *datenow = [NSDate date];

    //----------将nsdate按formatter格式转成nsstring

    NSString *currentTimeString = [formatter stringFromDate:datenow];

//    NSLog(@"currentTimeString =  %@",currentTimeString);

    return [@"Time: " stringByAppendingString:currentTimeString];

}

@end
