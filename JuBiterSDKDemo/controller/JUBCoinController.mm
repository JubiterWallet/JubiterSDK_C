//
//  JUBCoinController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/9.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBPinAlertView.h"
#import "JUBSharedData.h"

#import "JUBCoinController.h"


@interface JUBCoinController ()

@end


@implementation JUBCoinController
@synthesize inputAddrView;

- (void)viewDidLoad {
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    NSArray *buttonTitleArray = @[
        BUTTON_TITLE_TRANSACTION,
        BUTTON_TITLE_GETADDRESS,
        BUTTON_TITLE_SHOWADDRESS,
        BUTTON_TITLE_SETMYADDRESS,
        BUTTON_TITLE_SETTIMEOUT,
    ];
    
    NSMutableArray *buttonModelArray = [NSMutableArray array];
    
    for (NSString *title in buttonTitleArray) {
        JUBButtonModel *model = [[JUBButtonModel alloc] init];
        
        model.title = title;
        
        //默认支持全部通信类型，不传就是默认，如果传多个通信类型可以直接按照首页顶部的通信类型index传，比如说如果支持NFC和BLE，则直接传@"01"即可，同理如果只支持第一和第三种通信方式，则传@"02"
        if (   [title isEqual:BUTTON_TITLE_SHOWADDRESS]
            || [title isEqual:BUTTON_TITLE_SETMYADDRESS]
            || [title isEqual:BUTTON_TITLE_SETTIMEOUT]
            ) {
            model.transmitTypeOfButton = [NSString stringWithFormat:@"%li",
                                          (long)JUB_NS_ENUM_DEV_TYPE::SEG_BLE];
        }
        else {
            model.transmitTypeOfButton = [NSString stringWithFormat:@"%li%li",
                                          (long)JUB_NS_ENUM_DEV_TYPE::SEG_NFC,
                                          (long)JUB_NS_ENUM_DEV_TYPE::SEG_BLE];
        }
        
        [buttonModelArray addObject:model];
    }
    
    self.buttonArray = buttonModelArray;
        
    [[JUBSharedData sharedInstance] setCurrContextID:0];
    
}


- (void)dealloc {
    
    JUB_UINT16 contextID = [[[JUBSharedData sharedInstance] currContextID] intValue];
    if (!contextID) {
        JUB_ClearContext(contextID);
    }
    
    [[JUBSharedData sharedInstance] setCurrContextID:0];
}


//选中币种之后的回调，你这可能用不到这个回调，先开放给你，用不到就可以直接删掉
- (void)selectCoinType {
    
    NSLog(@"JUBBTCController--selectCoinTypeIndex %ld", (long)self.selectCoinTypeIndex);
    
    self.optCoinType = (long)self.selectCoinTypeIndex;
}


//测试类型的按钮点击回调
- (void)selectedTestActionTypeIndex:(NSInteger)index {
    
    NSLog(@"JUBCoinController--selectedTransmitTypeIndex = %ld, CoinType = %ld, selectedTestActionType = %ld", (long)self.selectedTransmitTypeIndex, (long)self.selectCoinTypeIndex, (long)index);
    
    self.optIndex = index;
    
    switch (self.optIndex) {
    case JUB_NS_ENUM_OPT::TRANSACTION:
    {
        switch (self.selectedTransmitTypeIndex) {
        case JUB_NS_ENUM_DEV_TYPE::SEG_NFC:
        {
            [JUBPinAlertView showInputPinAlert:^(NSString * _Nonnull pin) {
                JUBSharedData *data = [JUBSharedData sharedInstance];
                [data setUserPin:pin];
                [data setVerifyMode:JUB_NS_ENUM_VERIFY_MODE::PIN];
                [self beginNFCSession];
            }];
            break;
        }
        case JUB_NS_ENUM_DEV_TYPE::SEG_BLE:
        {
            JUBSharedData *data = [JUBSharedData sharedInstance];
            [data setVerifyMode:JUB_NS_ENUM_VERIFY_MODE::VKPIN];
            [self beginBLESession];
            break;
        }
        default:
            break;
        }   // switch (self.selectedTransmitTypeIndex) end
        break;
    }
    case JUB_NS_ENUM_OPT::GET_ADDRESS:
    case JUB_NS_ENUM_OPT::SHOW_ADDRESS:
    case JUB_NS_ENUM_OPT::SET_MY_ADDRESS:
    {
        inputAddrView = [JUBInputAddressView showCallBack:^(NSInteger change, NSInteger address) {
            
            NSLog(@"showCallBack change = %ld, address = %ld", (long)change, (long)address);
            
            self.change = change;
            self.addressIndex = address;
            
            switch (self.selectedTransmitTypeIndex) {
            case JUB_NS_ENUM_DEV_TYPE::SEG_NFC:
            {
                [self beginNFCSession];
                break;
            }
            case JUB_NS_ENUM_DEV_TYPE::SEG_BLE:
            {
                [self beginBLESession];
                break;
            }
            default:
                break;
            }   // switch (self.selectedTransmitTypeIndex) end
        }];
        inputAddrView.addressHeader = @"m/purpose'/coin_type'/account'";
        break;
    }
    case JUB_NS_ENUM_OPT::SET_TIMEOUT:
    {
        switch (self.selectedTransmitTypeIndex) {
        case JUB_NS_ENUM_DEV_TYPE::SEG_BLE:
        {
            break;
        }
        case JUB_NS_ENUM_DEV_TYPE::SEG_NFC:
        default:
            break;
        }   // switch (self.selectedTransmitTypeIndex) end
        break;
    }
    default:
        break;
    }   // switch (self.optIndex) end
}


#pragma mark - 业务


- (JUB_RV)show_virtualKeyboard:(JUB_UINT16)contextID {
    
    JUB_RV rv = JUBR_ERROR;
    
    rv = JUB_ShowVirtualPwd(contextID);
    if (   JUBR_OK               != rv
        && JUBR_IMPL_NOT_SUPPORT != rv
        ) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_ShowVirtualPwd() return 0x%2lx.]", rv]];
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_ShowVirtualPwd() OK.]"]];
    
    return rv;
}


- (JUB_RV)verify_pin:(JUB_UINT16)contextID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_ULONG retry = 0;
    rv = JUB_VerifyPIN(contextID, [[[JUBSharedData sharedInstance] userPin] UTF8String], &retry);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_VerifyPIN() return 0x%2lx.]", rv]];
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_VerifyPIN() OK.]"]];
    
    return rv;
}


@end
