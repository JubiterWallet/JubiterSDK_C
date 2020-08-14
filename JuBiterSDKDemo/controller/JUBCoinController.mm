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
    
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    [sharedData setCurrMainPath:nil];
    [sharedData setCurrCoinType:-1];
    [sharedData setCurrContextID:0];
}


- (void)dealloc {
    
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    
    JUB_UINT16 contextID = [sharedData currContextID];
    if (!contextID) {
        JUB_ClearContext(contextID);
    }
    
    [sharedData setCurrMainPath:nil];
    [sharedData setCurrCoinType:-1];
    [sharedData setCurrContextID:0];
}


//选中币种之后的回调，你这可能用不到这个回调，先开放给你，用不到就可以直接删掉
- (void)selectCoinType {
    
    NSLog(@"JUBBTCController--selectCoinTypeIndex %ld", (long)self.selectCoinTypeIndex);
    
    self.optCoinType = (long)self.selectCoinTypeIndex;
}


//测试类型的按钮点击回调
- (void)selectedTestActionTypeIndex:(NSInteger)index {
    
    NSLog(@"JUBCoinController--selectedTransmitTypeIndex = %ld, CoinType = %ld, selectedTestActionType = %ld", (long)self.selectedTransmitTypeIndex, (long)self.selectCoinTypeIndex, (long)index);
    
    __block
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    
    self.optIndex = index;
    
    switch (self.optIndex) {
    case JUB_NS_ENUM_OPT::TRANSACTION:
    {
        switch (self.selectedTransmitTypeIndex) {
        case JUB_NS_ENUM_DEV_TYPE::SEG_NFC:
        {
            [JUBPinAlertView showInputPinAlert:^(NSString * _Nonnull pin) {
                if (nil == pin) {
                    [self addMsgData:[NSString stringWithFormat:@"[User CANCELED"]];
//                    rv = JUBR_USER_CANCEL;
                    return;
                }
                [sharedData setUserPin:pin];
                [sharedData setVerifyMode:JUB_NS_ENUM_VERIFY_MODE::PIN];
                [self beginNFCSession];
            }];
            break;
        }
        case JUB_NS_ENUM_DEV_TYPE::SEG_BLE:
        {
            JUB_ENUM_COMMODE comMode = JUB_ENUM_COMMODE::COMMODE_NS_ITEM;
            JUB_ENUM_DEVICE deviceClass = JUB_ENUM_DEVICE::DEVICE_NS_ITEM;
            JUB_RV rv = JUB_GetDeviceType([sharedData currDeviceID], &comMode, &deviceClass);
            if (JUBR_OK != rv) {
                [self addMsgData:[NSString stringWithFormat:@"[JUB_GetDeviceType() return 0x%2lx.]", rv]];
                return;
            }
            [self addMsgData:[NSString stringWithFormat:@"[JUB_GetDeviceType() OK.]"]];
            
            switch (deviceClass) {
            case JUB_ENUM_DEVICE::BIO:
                [sharedData setVerifyMode:JUB_NS_ENUM_VERIFY_MODE::FGPT];
                break;
            case JUB_ENUM_DEVICE::BLADE:
            default:
                [sharedData setVerifyMode:JUB_NS_ENUM_VERIFY_MODE::VKPIN];
                break;
            }
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
- (NSUInteger)show_virtualKeyboard:(NSUInteger)contextID {
    
    JUB_RV rv = JUBR_ERROR;
    
    rv = JUB_ShowVirtualPwd(contextID);
    if (   JUBR_OK               != rv
        && JUBR_IMPL_NOT_SUPPORT != rv
        ) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_ShowVirtualPwd() return 0x%2lx.]", rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_ShowVirtualPwd() OK.]"]];
    
    return rv;
}


- (NSUInteger)cancel_virtualKeyboard:(NSUInteger)contextID {
    
    JUB_RV rv = JUBR_ERROR;
    
    rv = JUB_CancelVirtualPwd(contextID);
    if (   JUBR_OK               != rv
        && JUBR_IMPL_NOT_SUPPORT != rv
        ) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_CancelVirtualPwd() return 0x%2lx.]", rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_CancelVirtualPwd() OK.]"]];
    
    return rv;
}


- (NSUInteger)verify_pin:(NSUInteger)contextID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return rv;
    }
    
    JUB_ULONG retry = 0;
    rv = JUB_VerifyPIN(contextID, [[sharedData userPin] UTF8String], &retry);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_VerifyPIN(%lu) return 0x%2lx.]", retry, rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_VerifyPIN() OK.]"]];
    
    return rv;
}


- (NSUInteger)verify_fgpt:(NSUInteger)contextID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_ULONG retry = 0;
    rv = JUB_VerifyFingerprint(contextID, &retry);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_VerifyFingerprint(%lu) return 0x%2lx.]", retry, rv]];
        return rv;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_VerifyFingerprint() OK.]"]];
    
    return rv;
}


@end
