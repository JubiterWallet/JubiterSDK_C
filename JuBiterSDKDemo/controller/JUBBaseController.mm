//
//  JUBBaseController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/13.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBBaseController.h"
#import "JUBDeviceController.h"
#import "JUBBTCController.h"
#import "JUBETHController.h"
#import "JUBEOSController.h"
#import "JUBXRPController.h"


@interface JUBBaseController ()

@end


@implementation JUBBaseController


- (NSArray *)getTransmitTypeArray {
    
    return @[BUTTON_TITLE_NFC,
             BUTTON_TITLE_BLE
    ];
}


//设置首页列表内容
- (NSArray *)getButtonTitleArray {
    
    NSArray *buttonTapSelectorNameArray = @[BUTTON_TITLE_DEVICE,
                                            BUTTON_TITLE_BTC,
                                            BUTTON_TITLE_ETH,
                                            BUTTON_TITLE_EOS,
                                            BUTTON_TITLE_XRP
    ];
    
//    self.buttonTapSelectorNameArray = buttonTapSelectorNameArray;
    
    return buttonTapSelectorNameArray;
}

//首页按钮点击响应事件
- (void)gotoDetailAccordingCoinSeriesType:(NSInteger)coinSeriesType {
    
    JUBDetailBaseController *vc;
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


- (void)selectTransmitTypeIndex:(NSInteger)index
{
    
}


@end
