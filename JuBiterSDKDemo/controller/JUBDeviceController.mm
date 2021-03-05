//
//  JUBDeviceController.mm
//  JuBiterSDKDemo
//
//  Created by panmin on 2020/5/6.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#import "JUBErrorCode.h"
#import "JUBSharedData.h"

#import "JUBHomePageController.h"
#import "JUBDeviceController.h"


@interface JUBDeviceController ()

@end


@implementation JUBDeviceController


- (void) viewDidLoad {
    
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.title = @"Device options";
    
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
- (void) selectedTestActionTypeIndex:(NSInteger)index {
    
    NSLog(@"JUBDeviceController--selectedTransmitTypeIndex = %ld, Type = %ld, selectedTestActionTypeIndex = %ld", (long)self.selectedTransmitTypeIndex, (long)self.selectedMenuIndex, (long)index);
    
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    
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
    case JUB_NS_ENUM_DEV_OPT::DEVICE_CHANGEPIN:
    {
        [JUBPinAlert showChangePinCallBack:^(NSString * _Nonnull oldPin,
                                              NSString * _Nonnull newPin) {
            if (   nil == oldPin
                || nil == newPin
                ) {
//                rv = JUBR_USER_CANCEL;
                return;
            }
            [sharedData setUserPin:oldPin];
            [sharedData setNeoPin:newPin];
            
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
        [JUBPinAlert showInputPinCallBack:^(NSString * _Nonnull pin) {
            if (!pin) {
//                rv = JUBR_USER_CANCEL;
                return;
            }
            [sharedData setUserPin:pin];
            
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
- (void) DeviceOpt:(NSUInteger)deviceID {
    
    if (JUB_ENUM_BOOL::BOOL_FALSE == JUB_IsBootLoader(deviceID)) {
        [self addMsgData:@"[JUB_IsBootLoader() return JUB_ENUM_BOOL::BOOL_FALSE.]"];
        return;
    }
    [self addMsgData:@"[JUB_IsBootLoader() return JUB_ENUM_BOOL::BOOL_TRUE.]"];
    
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
        [self device_import_mnemonic:deviceID
                             wordNum:12];
        break;
    }
    case JUB_NS_ENUM_DEV_OPT::IMPORT_MNEMONIC18:
    {
        [self device_import_mnemonic:deviceID
                             wordNum:18];
        break;
    }
    case JUB_NS_ENUM_DEV_OPT::IMPORT_MNEMONIC24:
    {
        [self device_import_mnemonic:deviceID
                             wordNum:24];
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
- (void) query_device_battery:(NSUInteger)deviceID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_BYTE percent = 0;
    rv = JUB_QueryBattery(deviceID, &percent);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_QueryBattery() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_QueryBattery() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"Device Battery: %i.", percent]];
}


- (void) get_device_info_test:(NSUInteger)deviceID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_DEVICE_INFO info;
    rv = JUB_GetDeviceInfo(deviceID, &info);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetDeviceInfo() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetDeviceInfo() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"DeviceInfo.label: %@.", [NSString stringWithUTF8String:info.label]]];
    [self addMsgData:[NSString stringWithFormat:@"DeviceInfo.sn:    %@.", [NSString stringWithUTF8String:info.sn]]];
    [self addMsgData:[NSString stringWithFormat:@"DeviceInfo.pinRetry:    %i.", info.pinRetry]];
    [self addMsgData:[NSString stringWithFormat:@"DeviceInfo.pinMaxRetry: %i.", info.pinMaxRetry]];
//    [self addMsgData:[NSString stringWithFormat:@"DeviceInfo.bleVersion:      %s.", info.bleVersion]];
    [self addMsgData:[NSString stringWithFormat:@"DeviceInfo.firmwareVersion: %s.", info.firmwareVersion]];
    
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    
    JUB_ENUM_COMMODE commode = JUB_ENUM_COMMODE::COMMODE_NS_ITEM;
    JUB_ENUM_DEVICE deviceClass = JUB_ENUM_DEVICE::DEVICE_NS_ITEM;
    rv = JUB_GetDeviceType(deviceID, &commode, &deviceClass);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetDeviceType() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetDeviceType() OK.]"]];
    [sharedData setComMode:commode];
    [sharedData setDeviceClass:deviceClass];
    
    switch ([sharedData comMode]) {
    case JUB_ENUM_COMMODE::NFC:
    {
        JUB_ENUM_DEVICE_ROOT_KEY_STATUS status;
        rv = JUB_GetDeviceRootKeyStatus(deviceID, &status);
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_GetRootKeyStatus() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
            return;
        }
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetRootKeyStatus() return %d.]", status]];
        break;
    }
    default:
        break;
    }
}


- (void) get_device_applet_test:(NSUInteger)deviceID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_CHAR_PTR appList;
    rv = JUB_EnumApplets(deviceID, &appList);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_EnumApplets() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_EnumApplets() OK.]"]];
    
    std::string appletList = appList;
    rv = JUB_FreeMemory(appList);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"Applets are %@.", [NSString stringWithUTF8String:appletList.c_str()]]];
    
    auto vAppList = [[NSString stringWithCString:appletList.c_str()
                                        encoding:[NSString defaultCStringEncoding]]
                     componentsSeparatedByString:@" "];
    for (id appID : vAppList) {
        if (NSComparisonResult::NSOrderedSame == [appID compare:[NSString stringWithCString:""
                                                                                   encoding:[NSString defaultCStringEncoding]]]) {
            continue;
        }
        JUB_VERSION version;
        auto rv = JUB_GetAppletVersion(deviceID, [appID UTF8String], &version);
        if (JUBR_OK != rv) {
            [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAppletVersion return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
            return;
        }
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetAppletVersion() OK.]"]];
        [self addMsgData:[NSString stringWithFormat:@"Applet Version: %i.%i.%i", version.major, version.minor, version.patch]];
    }
    
    JUB_CHAR_PTR coinList;
    rv = JUB_EnumSupportCoins(deviceID, &coinList);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_EnumSupportCoins() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_EnumSupportCoins() OK.]"]];
    
    std::string coinSupportList = coinList;
    rv = JUB_FreeMemory(coinList);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"Support coins are %@.", [NSString stringWithUTF8String:coinSupportList.c_str()]]];
}


- (void) get_device_cert_test:(NSUInteger)deviceID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUB_CHAR_PTR cert;
    rv = JUB_GetDeviceCert(deviceID, &cert);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GetDeviceCert() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GetDeviceCert() OK.]"]];
    [self addMsgData:[NSString stringWithFormat:@"Device Cert is %s.", cert]];
    
    rv = JUB_FreeMemory(cert);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return %@ 0x%2lx.]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
}


- (void) change_device_pin_test:(NSUInteger)deviceID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    
    JUB_ULONG retry = 0;
    rv = JUB_ChangePIN(deviceID,
                       [[sharedData userPin] UTF8String],
                       [[sharedData  neoPin] UTF8String],
                       &retry);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_ChangePIN(%lu) return %@ (0x%2lx).]", retry, [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    
    [self addMsgData:[NSString stringWithFormat:@"Device change PIN OK."]];
}


- (void) device_reset_test:(NSUInteger)deviceID {
    
    JUB_RV rv = JUBR_ERROR;
    
    rv = JUB_Reset(deviceID);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_Reset() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_Reset() OK.]"]];
}


- (void) device_gen_seed_test:(NSUInteger)deviceID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    
    rv = JUB_GenerateSeed(deviceID,
                          [[sharedData userPin] UTF8String],
                          JUB_ENUM_CURVES::SECP256K1);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_GenerateSeed() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_GenerateSeed() OK.]"]];
}


- (void) device_import_mnemonic:(NSUInteger)deviceID
                        wordNum:(int)wordNum {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    
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
                            [[sharedData userPin] UTF8String],
                            mnemonic);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_ImportMnemonic() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_ImportMnemonic() OK.]"]];
}


- (void) device_export_mnemonic_test:(NSUInteger)deviceID {
    
    JUB_RV rv = JUBR_ERROR;
    
    JUBSharedData *sharedData = [JUBSharedData sharedInstance];
    if (nil == sharedData) {
        return;
    }
    
    JUB_CHAR_PTR mnemonic;
    rv = JUB_ExportMnemonic(deviceID,
                            [[sharedData userPin] UTF8String],
                            &mnemonic);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_ExportMnemonic() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_ExportMnemonic() OK.]"]];
    
    [self addMsgData:[NSString stringWithFormat:@"Device Entropy is (%s).", mnemonic]];
    rv = JUB_FreeMemory(mnemonic);
    if (JUBR_OK != rv) {
        [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() return %@ (0x%2lx).]", [JUBErrorCode GetErrMsg:rv], rv]];
        return;
    }
    [self addMsgData:[NSString stringWithFormat:@"[JUB_FreeMemory() OK.]"]];
}


@end
