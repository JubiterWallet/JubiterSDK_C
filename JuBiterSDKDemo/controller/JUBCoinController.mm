//
//  JUBCoinController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/9.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBCoinController.h"


@interface JUBCoinController ()

@end


@implementation JUBCoinController


- (void)viewDidLoad {
    
    NSArray *buttonTitleArray = @[@"TRANSACTION",
                                  @"   GET_ADDRESS",
                                  @"  SHOW_ADDRESS",
                                  @"SET_MY_ADDRESS",
                                  @"SET_TIMEOUT"
    ];
    
    NSMutableArray *buttonModelArray = [NSMutableArray array];
    
    for (NSString *title in buttonTitleArray) {
        JUBButtonModel *model = [[JUBButtonModel alloc] init];
        
        model.title = title;
        
        //默认支持全部通信类型，不传就是默认，如果传多个通信类型可以直接按照首页顶部的通信类型index传，比如说如果支持NFC和BLE，则直接传@"01"即可，同理如果只支持第一和第三种通信方式，则传@"02"
        if (   [title isEqual:@"  SHOW_ADDRESS"]
            || [title isEqual:@"SET_MY_ADDRESS"]
            || [title isEqual:@"SET_TIMEOUT"]
            ) {
            model.transmitTypeOfButton = @"1";
        }
        else {
            model.transmitTypeOfButton = @"01";
        }
        
        [buttonModelArray addObject:model];
    }
    
    self.buttonArray = buttonModelArray;
    
//    [self.navigationController pushViewController:vc animated:YES];
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}


//选中币种之后的回调，你这可能用不到这个回调，先开放给你，用不到就可以直接删掉
- (void)selectCoinType {
    
    NSLog(@"JUBBTCController--selectCoinTypeIndex %ld", (long)self.selectCoinTypeIndex);
    
    self.optCoinType = (long)self.selectCoinTypeIndex;
}


//测试类型的按钮点击回调
- (void)selectedTestActionTypeIndex:(NSInteger)index {
    
    NSLog(@"JUBBTCController--Type = %ld, selectedTestActionType %ld",
          (long)self.selectCoinTypeIndex, (long)index);
    
    self.optIndex = index;
    
    switch (self.optIndex) {
    case JUB_NS_ENUM_OPT::TRANSACTION:
    {
        [[Tools defaultTools] showPinAlertAboveVC:self
                              getPinCallBackBlock:^(NSString *pin) {
            
            self.userPIN = pin;
            [self beginNFCSession];
        }];
        break;
    }
    case JUB_NS_ENUM_OPT::GET_ADDRESS:
    case JUB_NS_ENUM_OPT::SHOW_ADDRESS:
    {
//        [[Tools defaultTools] showPinAlertAboveVC:self
//                              getPinCallBackBlock:^(NSString *path) {
//            self.userPath = path;
            [self beginNFCSession];
//        }];
        break;
    }
    case JUB_NS_ENUM_OPT::SET_MY_ADDRESS:
    case JUB_NS_ENUM_OPT::SET_TIMEOUT:
    {
        [self beginNFCSession];
        break;
    }
    default:
        break;
    }
}


#pragma mark - 业务
- (JUB_RV)verify_pin:(JUB_UINT16)contextID
                 pin:(std::string)pin {
    
    JUB_RV rv = JUBR_ERROR;
//    while (rv) {
//        //����pin��λ�ã�������123456789
//        cout << "1 2 3" << endl;
//        cout << "4 5 6" << endl;
//        cout << "7 8 9" << endl;
//
//        cout << "to cancel the virtualpwd iput 'c'" << endl;
//        rv = JUB_ShowVirtualPwd(contextID);
//        if (   JUBR_OK               != rv
//            && JUBR_IMPL_NOT_SUPPORT != rv
//            ) {
//            cout << "JUB_ShowVirtualPwd() return " << GetErrMsg(rv) << endl;
//            break;
//        }
//
//        char str[9] = {0,};
//
//        cin >> str;
//        cout << str << endl;
//
//        if (   'c' == str[0]
//            || 'C' == str[0]
//            ) {
//            cout << "cancel the VirtualPwd "<< endl;
//            rv = JUB_CancelVirtualPwd(contextID);
//            if (JUBR_OK != rv) {
//                cout << "JUB_CancelVirtualPwd() return " << GetErrMsg(rv) << endl;
//                break;
//            }
//            return rv;
//        }
//
//        JUB_ULONG retry;
//        rv = JUB_VerifyPIN(contextID, [selfClass.userPIN UTF8String], &retry);
//        if (JUBR_OK != rv) {
//            cout << "[JUB_VerifyPIN() return " << GetErrMsg(rv) << ".]" << endl;
//            [selfClass addMsgData:[NSString stringWithFormat:@"[JUB_VerifyPIN() return 0x%2lx.]", rv]];
//            break;
//        }
//        [self addMsgData:[NSString stringWithFormat:@"[JUB_VerifyPIN() OK.]"]];
//    }
    
    JUB_ULONG retry;
    rv = JUB_VerifyPIN(contextID, pin.c_str(), &retry);
    if (JUBR_OK != rv) {
//        cout << "[JUB_VerifyPIN() return " << GetErrMsg(rv) << ".]" << endl;
        [self addMsgData:[NSString stringWithFormat:@"[JUB_VerifyPIN() return 0x%2lx.]", rv]];
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_VerifyPIN() OK.]"]];
    
    return rv;
}


@end
