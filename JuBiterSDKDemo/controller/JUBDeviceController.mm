//
//  JUBDeviceController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/6.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBSelectApduView.h"
#import "JUBPinAlertView.h"
#import "JUBSharedData.h"

#import "JUBDeviceController.h"
#import "JUBHomeController.h"


@interface JUBDeviceController ()

@end


@implementation JUBDeviceController


- (void)viewDidLoad {
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.optItem = JUB_NS_ENUM_MAIN::OPT_DEVICE;
    
    NSArray *buttonTitleArray = @[
        BUTTON_TITLE_QUERYBATTERY,
        BUTTON_TITLE_DEVICEINFO,
        BUTTON_TITLE_DEVICEAPPLETS,
        BUTTON_TITLE_DEVICECERT,
        BUTTON_TITLE_DEVICE_CHANGEPIN,
//        BUTTON_TITLE_SENDONEAPDU,
        BUTTON_TITLE_DEVICE_RESET,
        BUTTON_TITLE_GENERATESEED,
        BUTTON_TITLE_IMPORTMNEMONIC12,
        BUTTON_TITLE_IMPORTMNEMONIC18,
        BUTTON_TITLE_IMPORTMNEMONIC24,
        BUTTON_TITLE_EXPORTMNEMONIC,
    ];
    
    NSMutableArray *buttonModelArray = [NSMutableArray array];
    
    for (NSString *title in buttonTitleArray) {
        
        JUBButtonModel *model = [[JUBButtonModel alloc] init];
        
        model.title = title;
        
        //默认支持全部通信类型，不传就是默认，如果传多个通信类型可以直接按照首页顶部的通信类型index传，比如说如果支持NFC和BLE，则直接传@"01"即可，同理如果只支持第一和第三种通信方式，则传@"02"
        if (   [title isEqual:BUTTON_TITLE_DEVICE_CHANGEPIN]
            || [title isEqual:BUTTON_TITLE_DEVICE_RESET]
            || [title isEqual:BUTTON_TITLE_GENERATESEED]
            || [title isEqual:BUTTON_TITLE_IMPORTMNEMONIC12]
            || [title isEqual:BUTTON_TITLE_IMPORTMNEMONIC18]
            || [title isEqual:BUTTON_TITLE_IMPORTMNEMONIC24]
            ) {
            model.transmitTypeOfButton = [NSString stringWithFormat:@"%li",
                                          (long)JUB_NS_ENUM_DEV_TYPE::SEG_NFC];
        }
        else if ([title isEqual:BUTTON_TITLE_QUERYBATTERY]) {
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
}


//测试类型的按钮点击回调
- (void)selectedTestActionTypeIndex:(NSInteger)index {
    
    NSLog(@"JUBDeviceController--selectedTransmitTypeIndex = %ld, Type = %ld, selectedTestActionType = %ld", (long)self.selectedTransmitTypeIndex, (long)self.selectCoinTypeIndex, (long)index);
    
    self.optIndex = index;
    
    switch (self.optIndex) {
    case JUB_NS_ENUM_DEV_OPT::QUERY_BATTERY:
    {
        switch (self.selectedTransmitTypeIndex) {
        case JUB_NS_ENUM_DEV_TYPE::SEG_BLE:
        {
            [self beginBLESession];
            break;
        }
        case JUB_NS_ENUM_DEV_TYPE::SEG_NFC:
        default:
            break;
        }   // switch (self.selectedTransmitTypeIndex) end
        break;
    }
    case JUB_NS_ENUM_DEV_OPT::DEVICE_INFO:
    case JUB_NS_ENUM_DEV_OPT::DEVICE_APPLETS:
    case JUB_NS_ENUM_DEV_OPT::DEVICE_CERT:
    case JUB_NS_ENUM_DEV_OPT::DEVICE_RESET:
    {
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
        break;
    }
//    case JUB_NS_ENUM_DEV_OPT::SEND_ONE_APDU:
//    {
//        [JUBSelectApduView showAPDUArray:@[@"1", @"2", @"3", @"4"]
//                                 AboveVC:self
//                 selectApduCallBackBlock:^(NSString * _Nonnull apdu) {
//
//            NSLog(@"选中的APDU = %@", apdu);
//
//            switch (self.selectedTransmitTypeIndex) {
//            case JUB_NS_ENUM_DEV_TYPE::SEG_NFC:
//            {
//                [self beginNFCSession];
//                break;
//            }
//            case JUB_NS_ENUM_DEV_TYPE::SEG_BLE:
//            {
//                [self beginBLESession];
//                break;
//            }
//            default:
//                break;
//            }   // switch (self.selectedTransmitTypeIndex) end
//        }];
//        break;
//    }
    case JUB_NS_ENUM_DEV_OPT::DEVICE_CHANGEPIN:
    {
        [JUBPinAlertView showChangePinAlert:^(NSString * _Nonnull oldPin,
                                              NSString * _Nonnull newPin) {
            [[JUBSharedData sharedInstance] setUserPin:oldPin];
            [[JUBSharedData sharedInstance] setNeoPin:newPin];
            
            switch (self.selectedTransmitTypeIndex) {
            case JUB_NS_ENUM_DEV_TYPE::SEG_NFC:
            {
                [self beginNFCSession];
                break;
            }
            case JUB_NS_ENUM_DEV_TYPE::SEG_BLE:
            default:
                break;
            }   // switch (self.selectedTransmitTypeIndex) end
        }];
        break;
    }
    case JUB_NS_ENUM_DEV_OPT::GENERATE_SEED:
    case JUB_NS_ENUM_DEV_OPT::IMPORT_MNEMONIC12:
    case JUB_NS_ENUM_DEV_OPT::IMPORT_MNEMONIC18:
    case JUB_NS_ENUM_DEV_OPT::IMPORT_MNEMONIC24:
    case JUB_NS_ENUM_DEV_OPT::EXPORT_MNEMONIC:
    {
        [JUBPinAlertView showInputPinAlert:^(NSString * _Nonnull pin) {
            
            [[JUBSharedData sharedInstance] setUserPin:pin];
            
            switch (self.selectedTransmitTypeIndex) {
            case JUB_NS_ENUM_DEV_TYPE::SEG_NFC:
            {
                [self beginNFCSession];
                break;
            }
            case JUB_NS_ENUM_DEV_TYPE::SEG_BLE:
            default:
                break;
            }   // switch (self.selectedTransmitTypeIndex) end
        }];
        break;
    }
    default:
        break;
    }   // switch (self.optIndex) end
}


#pragma mark - 通讯库寻卡回调
- (void)DeviceOpt:(JUB_UINT16)deviceID {
    
    int wordNum = 24;
    switch (self.optIndex) {
    case JUB_NS_ENUM_DEV_OPT::QUERY_BATTERY:
    {
        [self query_device_battery:deviceID];
        break;
    }
    case JUB_NS_ENUM_DEV_OPT::DEVICE_INFO:
    {
        [self get_device_info_test:deviceID];
        break;
    }
    case JUB_NS_ENUM_DEV_OPT::DEVICE_APPLETS:
    {
        [self get_device_applet_test:deviceID];
        break;
    }
    case JUB_NS_ENUM_DEV_OPT::DEVICE_CERT:
    {
        [self get_device_cert_test:deviceID];
        break;
    }
    case JUB_NS_ENUM_DEV_OPT::DEVICE_CHANGEPIN:
    {
        [self change_device_pin_test:deviceID];
        break;
    }
//    case JUB_NS_ENUM_DEV_OPT::SEND_ONE_APDU:
//    {
//        break;
//    }
    case JUB_NS_ENUM_DEV_OPT::DEVICE_RESET:
    {
        [self device_reset_test:deviceID];
        break;
    }
    case JUB_NS_ENUM_DEV_OPT::GENERATE_SEED:
    {
        [self device_gen_seed_test:deviceID];
        break;
    }
    case JUB_NS_ENUM_DEV_OPT::IMPORT_MNEMONIC12:
    {
        wordNum = 12;
    }
    case JUB_NS_ENUM_DEV_OPT::IMPORT_MNEMONIC24:
    {
        [self device_import_mnemonic:deviceID
                             wordNum:wordNum];
        break;
    }
    case JUB_NS_ENUM_DEV_OPT::EXPORT_MNEMONIC:
    {
        [self device_export_mnemonic_test:deviceID];
        break;
    }
    default:
        break;
    }   // switch (self.optIndex) end
}


#pragma mark - 业务
- (void)query_device_battery:(JUB_UINT16)deviceID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_BYTE percent = 0;
    rv = JUB_QueryBattery(deviceID, &percent);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_QueryBattery() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_QueryBattery() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"Device Battery: %i.", percent]];
}


- (void)get_device_info_test:(JUB_UINT16)deviceID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_DEVICE_INFO info;
    rv = JUB_GetDeviceInfo(deviceID, &info);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetDeviceInfo() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetDeviceInfo() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"DeviceInfo.label: %@.", [NSString stringWithUTF8String:info.label]]];
    [self addMsgData:[NSString stringWithFormat:@"DeviceInfo.sn:    %@.", [NSString stringWithUTF8String:info.sn]]];
    [self addMsgData:[NSString stringWithFormat:@"DeviceInfo.pinRetry:    %i.", info.pinRetry]];
    [self addMsgData:[NSString stringWithFormat:@"DeviceInfo.pinMaxRetry: %i.", info.pinMaxRetry]];
//    [self addMsgData:[NSString stringWithFormat:@"DeviceInfo.bleVersion:      %s.", info.bleVersion]];
    [self addMsgData:[NSString stringWithFormat:@"DeviceInfo.firmwareVersion: %s.", info.firmwareVersion]];
}


- (void)get_device_applet_test:(JUB_UINT16)deviceID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_CHAR_PTR appList;
    rv = JUB_EnumApplets(deviceID, &appList);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_EnumApplets() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_EnumApplets() OK.]"]];
    
    std::string appletList = appList;
    JUB_FreeMemory(appList);
    
    [self addMsgData:[NSString stringWithFormat:@"Applets are %@.", [NSString stringWithUTF8String:appletList.c_str()]]];
    
/*
    auto vAppList = split(appletList, ' ');
    
    for (auto appID : vAppList) {
        char* version;
        auto rv = JUB_GetAppletVersion(deviceID, (char*)appID.c_str(), &version);
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAppletVersion return 0x%2lx.]", rv]];
            return;
        }
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAppletVersion() OK.]"]];
        
        cout << appID << "Applet Version : " << version << endl;
    }
*/
    
    JUB_CHAR_PTR coinList;
    rv = JUB_EnumSupportCoins(deviceID, &coinList);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_EnumSupportCoins() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_EnumSupportCoins() OK.]"]];
    
    std::string coinSupportList = coinList;
    JUB_FreeMemory(coinList);
    
    [self addMsgData:[NSString stringWithFormat:@"Support coins are %@.", [NSString stringWithUTF8String:coinSupportList.c_str()]]];
}


- (void)get_device_cert_test:(JUB_UINT16)deviceID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_CHAR_PTR cert;
    rv = JUB_GetDeviceCert(deviceID, &cert);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetDeviceCert() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetDeviceCert() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"Device Cert is %s.", cert]];
    JUB_FreeMemory(cert);
}


- (void)change_device_pin_test:(JUB_UINT16)deviceID {
    
    JUB_RV rv = JUBR_ERROR;
    
    rv = JUB_ChangePIN(deviceID,
                       [[[JUBSharedData sharedInstance] userPin] UTF8String],
                       [[[JUBSharedData sharedInstance]  neoPin] UTF8String]);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_ChangePIN() return 0x%2lx.]", rv]];
        return;
    }
    
    [self addMsgData:[NSString stringWithFormat:@"Device change PIN OK."]];
}


- (void)send_one_apdu_test:(JUB_UINT16)deviceID {
    
//    JUB_RV rv = JUBR_ERROR;
//
//    JUB_CHAR_PTR cert;
//    rv = JUB_GetDeviceCert(deviceID, &cert);
//    if (JUBR_OK != rv) {
//        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetDeviceCert() return 0x%2lx.]", rv]];
//        return;
//    }
//
//    [self addMsgData:[NSString stringWithFormat:@"Device Cert is %s.", cert]];
//    JUB_FreeMemory(cert);
}


- (void)device_reset_test:(JUB_UINT16)deviceID {
    
    JUB_RV rv = JUBR_ERROR;
    
    rv = JUB_Reset(deviceID);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_Reset() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_Reset() OK.]"]];
}


- (void)device_gen_seed_test:(JUB_UINT16)deviceID {
    
    JUB_RV rv = JUBR_ERROR;
    
    rv = JUB_GenerateSeed(deviceID,
                          [[[JUBSharedData sharedInstance] userPin] UTF8String],
                          JUB_ENUM_CURVES::SECP256K1);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GenerateSeed() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GenerateSeed() OK.]"]];
}


- (void)device_import_mnemonic:(JUB_UINT16)deviceID
                       wordNum:(int)wordNum {
    
    JUB_RV rv = JUBR_ERROR;
    
    NSString *filePath = [[NSBundle mainBundle] pathForResource:@"testMnemonic"
                                                         ofType:@"json"];
    Json::Value root = readJSON([filePath UTF8String]);
    std::string keyword = "mnemonic12";
    if (18 == wordNum) {
        keyword = "mnemonic18";
    }
    else if (24 == wordNum) {
        keyword = "mnemonic24";
    }
    char *mnemonic = (char*)root[keyword.c_str()].asCString();
    
    rv = JUB_ImportMnemonic(deviceID,
                            [[[JUBSharedData sharedInstance] userPin] UTF8String],
                            mnemonic);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_ImportMnemonic() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_ImportMnemonic() OK.]"]];
}


- (void)device_export_mnemonic_test:(JUB_UINT16)deviceID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_CHAR_PTR mnemonic;
    rv = JUB_ExportMnemonic(deviceID,
                            [[[JUBSharedData sharedInstance] userPin] UTF8String],
                            &mnemonic);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_ExportMnemonic() return 0x%2lx.]", rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_ExportMnemonic() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"Device seed is %s.", mnemonic]];
    JUB_FreeMemory(mnemonic);
}


@end
