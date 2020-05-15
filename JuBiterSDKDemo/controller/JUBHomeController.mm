//
//  JUBHomeController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/13.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBHomeController.h"
#import "JUBDeviceController.h"
#import "JUBBTCController.h"
#import "JUBETHController.h"
#import "JUBEOSController.h"
#import "JUBXRPController.h"


@interface JUBHomeController ()
@property (nonatomic, weak) UIButton *disConnectBLEButton;
@end


@implementation JUBHomeController


- (void)viewDidLoad {
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    [self initData];
    
    [self initUI];
}


- (void)initData {
    
    NSString *indexStr = [[NSUserDefaults standardUserDefaults] objectForKey:selectedTransmitTypeIndexStr];
    if (!indexStr) {
        [[NSUserDefaults standardUserDefaults] setObject:[NSString stringWithFormat:@"%ld", (long)JUB_NS_ENUM_DEV_TYPE::NFC]
                                                  forKey:selectedTransmitTypeIndexStr];
    }
}


#pragma mark - 初始化UI
- (void)initUI {
    
    [super initUI];
    
    [self setNAVRightBtn];
}


#pragma mark - 页面内部按钮回调方法

- (void)selectTransmitTypeIndex:(NSInteger)index {

//    switch (index) {
//    case JUB_NS_ENUM_DEV_TYPE::BLE:
//    {
////        self.BLEDeviceScanListView = [JUBBLEDeviceScanListView showCallBack:^(NSString * _Nonnull deviceName) {
////        NSLog(@"deviceName = %@", deviceName);
////
////        JUBAlertView *alertView = [JUBAlertView showMsg:@"连接蓝牙设备中..."];
////
////        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
////
////        [alertView dismiss];
////
////        });
////
////        }];
////
////        timer = [NSTimer scheduledTimerWithTimeInterval:1 repeats:YES block:^(NSTimer * _Nonnull timer) {
////
////        [self.BLEDeviceScanListView addBLEDeviceArray:@"123"];
////
////        }];
//        break;
//    }
//    case JUB_NS_ENUM_DEV_TYPE::NFC:
//    default:
//        break;
//    }
//
////    if (index == 1) {
////
////    self.BLEDeviceScanListView = [JUBBLEDeviceScanListView showCallBack:^(NSString * _Nonnull deviceName) {
////
////    NSLog(@"deviceName = %@", deviceName);
////
////    JUBAlertView *alertView = [JUBAlertView showMsg:@"连接蓝牙设备中..."];
////
////    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
////
////    [alertView dismiss];
////
////    });
////
////    }];
////
////    timer = [NSTimer scheduledTimerWithTimeInterval:1 repeats:YES block:^(NSTimer * _Nonnull timer) {
////
////    [self.BLEDeviceScanListView addBLEDeviceArray:@"123"];
////
////    }];
//    }
}


#pragma mark - 懒加载
- (NSInteger)selectedTransmitTypeIndex {

    NSString *indexStr = [[NSUserDefaults standardUserDefaults] objectForKey:selectedTransmitTypeIndexStr];

    return [indexStr integerValue];
}


- (NSArray *)getTransmitTypeArray {
    
    return @[BUTTON_TITLE_NFC,
//             BUTTON_TITLE_BLE
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


- (void)setNAVRightBtn {
    
    UIButton *disConnectBLEButton = [UIButton buttonWithType:UIButtonTypeSystem];
    
    [disConnectBLEButton setTitle:@"断开BLE" forState:UIControlStateNormal];
    
    disConnectBLEButton.hidden = YES;
    
    [disConnectBLEButton setTitleColor:[[Tools defaultTools] colorWithHexString:@"#00ccff"] forState:UIControlStateNormal];
    
    [disConnectBLEButton addTarget:self action:@selector(disConnectBLEButtonClicked) forControlEvents:UIControlEventTouchUpInside];
    
    self.disConnectBLEButton = disConnectBLEButton;
    
    UIBarButtonItem *rightItem = [[UIBarButtonItem alloc]initWithCustomView:disConnectBLEButton];
    
    rightItem.imageInsets = UIEdgeInsetsMake(0, -15,0, 0);//设置向左偏移
    
    self.navigationItem.rightBarButtonItem = rightItem;
}


- (void)setShowDisConnectBLEButton:(BOOL)showDisConnectBLEButton {
    
    if (showDisConnectBLEButton) {
        
        self.disConnectBLEButton.hidden = NO;
        
    } else {
        
        self.disConnectBLEButton.hidden = YES;
        
    }
}


- (void)disConnectBLEButtonClicked {

    NSLog(@"disConnectBLEButtonClicked");

}


@end
