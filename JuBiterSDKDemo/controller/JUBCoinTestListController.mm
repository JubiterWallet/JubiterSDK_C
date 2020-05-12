//
//  JUBCoinTestListController.m
//  JuBiterSDKDemo
//
//  Created by 张川 on 2020/4/28.
//  Copyright © 2020 Jubiter. All rights reserved.
//

#import "JUBCoinTestListController.h"
#import "JUBDeviceController.h"
#import "JUBBTCController.h"
#import "JUBETHController.h"
#import "JUBEOSController.h"
#import "JUBXRPController.h"

@interface JUBCoinTestListController ()

@property (nonatomic, strong) NSArray *buttonTapSelectorNameArray;

@end

@implementation JUBCoinTestListController


- (void)viewDidLoad {
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (NSArray *)getTransmitTypeArray {
    
    return @[@"NFC", @"蓝牙"];
//    return @[@"NFC"];
}


//设置首页列表内容
- (NSArray *)getButtonTitleArray {
    
    NSArray *buttonTapSelectorNameArray = @[@"device",
                                            @"BTC",
                                            @"ETH",
                                            @"EOS",
                                            @"XRP",
//                                            @"HC_Test"
    ];
    
    self.buttonTapSelectorNameArray = buttonTapSelectorNameArray;
    
    return buttonTapSelectorNameArray;
}


//首页按钮点击响应事件
- (void)gotoDetailAccordingCoinSeriesType:(NSInteger)coinSeriesType {
    
    JUBController *vc;
    switch (coinSeriesType) {
    case JUB_NS_ENUM_MAIN::OPT_DEVICE:
    {
        vc = [[JUBDeviceController alloc] init];
        break;
    }
    case JUB_NS_ENUM_MAIN::OPT_BTC:
    {
        vc = [[JUBBTCController alloc] init];
        break;
    }
    case JUB_NS_ENUM_MAIN::OPT_ETH:
    {
        vc = [[JUBETHController alloc] init];
        break;
    }
    case JUB_NS_ENUM_MAIN::OPT_EOS:
    {
        vc = [[JUBEOSController alloc] init];
        break;
    }
    case JUB_NS_ENUM_MAIN::OPT_XRP:
    {
        vc = [[JUBXRPController alloc] init];
        break;
    }
    default:
        return;
    }
    
    [self.navigationController pushViewController:vc
                                         animated:YES];
}


@end
