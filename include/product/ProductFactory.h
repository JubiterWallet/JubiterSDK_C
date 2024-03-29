//
//  ProductFactory.h
//  JubSDK
//
//  Created by Pan Min on 2020/7/27.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef ProductFactory_h
#define ProductFactory_h

#include "JUB_SDK_DEV.h"

#include "utility/xFactory.hpp"
#include "utility/Singleton.h"

#include "device/DeviceFactory.h"
#include "device/DeviceTypeBase.hpp"

#include "token/TrezorCrypto/TrezorCryptoToken.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/JubiterLite/JubiterLiteToken.h"

namespace jub {
namespace product {


// Remove c++ features for swift framework
typedef enum {
    PRODUCT_NS_ITEM = (JUB_ENUM_COMMODE::COMMODE_NS_ITEM<<0x10)|(JUB_ENUM_DEVICE::DEVICE_NS_ITEM),

    SIM_BLD  = (JUB_ENUM_COMMODE::SIM<<0x10)|(JUB_ENUM_DEVICE::BLADE),
    SIM_BIO  = (JUB_ENUM_COMMODE::SIM<<0x10)|(JUB_ENUM_DEVICE::BIO),
    SIM_LITE = (JUB_ENUM_COMMODE::SIM<<0x10)|(JUB_ENUM_DEVICE::LITE),

    SWI_VD   = (JUB_ENUM_COMMODE::SWI<<0x10)|(JUB_ENUM_DEVICE::VD),

    HID_BLD  = (JUB_ENUM_COMMODE::HID<<0x10)|(JUB_ENUM_DEVICE::BLADE),
    HID_BIO  = (JUB_ENUM_COMMODE::HID<<0x10)|(JUB_ENUM_DEVICE::BIO),
    HID_NFC  = (JUB_ENUM_COMMODE::HID<<0x10)|(JUB_ENUM_DEVICE::LITE),

    BLE_BLD  = (JUB_ENUM_COMMODE::BLE<<0x10)|(JUB_ENUM_DEVICE::BLADE),
    BLE_BIO  = (JUB_ENUM_COMMODE::BLE<<0x10)|(JUB_ENUM_DEVICE::BIO),

    NFC_LITE = (JUB_ENUM_COMMODE::NFC<<0x10)|(JUB_ENUM_DEVICE::LITE),
} JUB_ENUM_PRODUCT;
// Remove c++ features for swift framework end


typedef device::DeviceTypeBase* (*CreateProductFn)(const JUB_ENUM_DEVICE& type, std::shared_ptr<device::DeviceTypeBase>);


class xProductFactory :
protected xFactory<device::DeviceTypeBase*,
                   JUB_ENUM_PRODUCT,
                    CreateProductFn> {
public:
    static JUB_ENUM_DEVICE  EnumProduct2EnumDevice(const JUB_ENUM_PRODUCT& product) {
        return (JUB_ENUM_DEVICE)(product&0x0000FFFF);
    }

    static JUB_ENUM_PRODUCT EnumProduct2EnumDevice(const JUB_ENUM_COMMODE& mode, const JUB_ENUM_DEVICE& type) {
        return (JUB_ENUM_PRODUCT)((mode<<0x10)|type);
    }


public:
    xProductFactory() {
#if defined(SIM_MODE)
        Register(JUB_ENUM_PRODUCT::SIM_BLD,  &device::JubiterBridgeBLDDevice::Create);
        Register(JUB_ENUM_PRODUCT::SIM_BIO,  &device::JubiterBridgeBIODevice::Create);
        Register(JUB_ENUM_PRODUCT::SIM_LITE, &device::JubiterBridgeLITEDevice::Create);
#endif  // #if defined(SIM_MODE) end
#if defined(SWI_MODE)
        Register(JUB_ENUM_PRODUCT::SWI_VD,   &device::JubiterSWIImplDevice::Create);
#endif  // #if defined(SWI_MODE) end
#if defined(HID_MODE)
        Register(JUB_ENUM_PRODUCT::HID_BLD,  &device::JubiterHidBLDDevice::Create);
        Register(JUB_ENUM_PRODUCT::HID_BIO,  &device::JubiterHidBIODevice::Create);
#endif  // #if defined(HID_MODE) end
#if defined(BLE_MODE)
        Register(JUB_ENUM_PRODUCT::BLE_BLD,  &device::JubiterBLEBLDDevice::Create);
        Register(JUB_ENUM_PRODUCT::BLE_BIO,  &device::JubiterBLEBIODevice::Create);
#endif  // #if defined(BLE_MODE) end
#if defined(NFC_MODE)
        Register(JUB_ENUM_PRODUCT::NFC_LITE, &device::JubiterNFCDevice::Create);
#endif  // #if defined(NFC_MODE) end
    };


    static JUB_RV GetDeviceType(JUB_UINT16 deviceID,
                                OUT JUB_ENUM_COMMODE_PTR commode, OUT JUB_ENUM_DEVICE_PTR deviceClass) {

        auto device = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
        JUB_CHECK_NULL(device);

        *commode = JUB_ENUM_COMMODE::COMMODE_NS_ITEM;
        *deviceClass = JUB_ENUM_DEVICE::DEVICE_NS_ITEM;
#if defined(SIM_MODE)
        if (   JUB_ENUM_COMMODE::COMMODE_NS_ITEM == (*commode)
            &&  JUB_ENUM_DEVICE::DEVICE_NS_ITEM  == (*deviceClass)
            ) {
            *commode = JUB_ENUM_COMMODE::SIM;
            switch (jub::device::xSIMDeviceFactory::GetEnumDevice(device)) {
            case JUB_ENUM_DEVICE::BLADE:
                *deviceClass = JUB_ENUM_DEVICE::BLADE;
                break;
            case JUB_ENUM_DEVICE::BIO:
                *deviceClass = JUB_ENUM_DEVICE::BIO;
                break;
            case JUB_ENUM_DEVICE::LITE:
                *deviceClass = JUB_ENUM_DEVICE::LITE;
                break;
            default:
                *commode = JUB_ENUM_COMMODE::COMMODE_NS_ITEM;
                break;
            }
        }
#endif  // #if defined(SIM_MODE) end
#if defined(SWI_MODE)
        if (   JUB_ENUM_COMMODE::COMMODE_NS_ITEM == (*commode)
            &&  JUB_ENUM_DEVICE::DEVICE_NS_ITEM  == (*deviceClass)
            ) {
            *commode = JUB_ENUM_COMMODE::SWI;
            switch (jub::device::xSWIDeviceFactory::GetEnumDevice(device)) {
            case JUB_ENUM_DEVICE::VD:
                *deviceClass = JUB_ENUM_DEVICE::VD;
                break;
            default:
                *commode = JUB_ENUM_COMMODE::COMMODE_NS_ITEM;
                break;
            }
        }
#endif  // #if defined(SWI_MODE) end
#if defined(HID_MODE)
        if (   JUB_ENUM_COMMODE::COMMODE_NS_ITEM == (*commode)
            &&  JUB_ENUM_DEVICE::DEVICE_NS_ITEM  == (*deviceClass)
            ) {
            *commode = JUB_ENUM_COMMODE::HID;
            switch (jub::device::xHidDeviceFactory::GetEnumDevice(device)) {
            case JUB_ENUM_DEVICE::BLADE:
                *deviceClass = JUB_ENUM_DEVICE::BLADE;
                break;
            case JUB_ENUM_DEVICE::BIO:
                *deviceClass = JUB_ENUM_DEVICE::BIO;
                break;
            default:
                *commode = JUB_ENUM_COMMODE::COMMODE_NS_ITEM;
                break;
            }
        }
#endif  // #if defined(HID_MODE) end
#if defined(BLE_MODE)
        if (   JUB_ENUM_COMMODE::COMMODE_NS_ITEM == (*commode)
            &&  JUB_ENUM_DEVICE::DEVICE_NS_ITEM  == (*deviceClass)
            ) {
            *commode = JUB_ENUM_COMMODE::BLE;
            switch (jub::device::xBLEDeviceFactory::GetEnumDevice(device)) {
            case JUB_ENUM_DEVICE::BLADE:
                *deviceClass = JUB_ENUM_DEVICE::BLADE;
                break;
            case JUB_ENUM_DEVICE::BIO:
                *deviceClass = JUB_ENUM_DEVICE::BIO;
                break;
            default:
                *commode = JUB_ENUM_COMMODE::COMMODE_NS_ITEM;
                break;
            }
        }
#endif  // #if defined(BLE_MODE) end
#if defined(NFC_MODE)
        if (   JUB_ENUM_COMMODE::COMMODE_NS_ITEM == (*commode)
            && JUB_ENUM_DEVICE::DEVICE_NS_ITEM   == (*deviceClass)
            ) {
            *commode = JUB_ENUM_COMMODE::NFC;
            switch (jub::device::xNFCDeviceFactory::GetEnumDevice(device)) {
            case JUB_ENUM_DEVICE::LITE:
                *deviceClass = JUB_ENUM_DEVICE::LITE;
                break;
            default:
                *commode = JUB_ENUM_COMMODE::COMMODE_NS_ITEM;
                break;
            }
        }
#endif  // #if defined(NFC_MODE) end

        if (  JUB_ENUM_COMMODE::COMMODE_NS_ITEM == (*commode)
            || JUB_ENUM_DEVICE::DEVICE_NS_ITEM  == (*deviceClass)
            ) {
            return JUBR_ARGUMENTS_BAD;
        }

        return JUBR_OK;
    }


    static std::shared_ptr<token::HardwareTokenInterface> GetDeviceToken(JUB_UINT16 deviceID) {

        std::shared_ptr<token::HardwareTokenInterface> token;

        JUB_ENUM_COMMODE commode     = JUB_ENUM_COMMODE::COMMODE_NS_ITEM;
        JUB_ENUM_DEVICE  deviceClass =  JUB_ENUM_DEVICE::DEVICE_NS_ITEM;
        if (JUBR_OK != GetDeviceType(deviceID,
                                     &commode, &deviceClass)) {
            return token;
        }

        switch (commode) {
        case JUB_ENUM_COMMODE::SIM:
        {
            switch (deviceClass) {
            case JUB_ENUM_DEVICE::BLADE:
                token = std::make_shared<token::JubiterBladeToken>(deviceID);
                break;
            case JUB_ENUM_DEVICE::BIO:
                token = std::make_shared<token::JubiterBIOToken>(deviceID);
                break;
            case JUB_ENUM_DEVICE::LITE:
                token = std::make_shared<token::JubiterLiteToken>(deviceID);
                break;
            default:
                break;
            }
            break;
        }
        case JUB_ENUM_COMMODE::HID:
        case JUB_ENUM_COMMODE::BLE:
        {
            switch (deviceClass) {
            case JUB_ENUM_DEVICE::BLADE:
                token = std::make_shared<token::JubiterBladeToken>(deviceID);
                break;
            case JUB_ENUM_DEVICE::BIO:
                token = std::make_shared<token::JubiterBIOToken>(deviceID);
                break;
            default:
                break;
            }
            break;
        }
        case JUB_ENUM_COMMODE::NFC:
        {
            switch (deviceClass) {
            case JUB_ENUM_DEVICE::LITE:
                token = std::make_shared<token::JubiterLiteToken>(deviceID);
                break;
            default:
                break;
            }
            break;
        }
        default:
            break;
        }

        return token;
    }


    device::DeviceTypeBase* CreateProduct(const unsigned short& pid, const std::string& arg) {
        JUB_ENUM_DEVICE         type =  JUB_ENUM_DEVICE::DEVICE_NS_ITEM;
        JUB_ENUM_PRODUCT enumProduct = JUB_ENUM_PRODUCT::PRODUCT_NS_ITEM;
#if defined(HID_MODE)
        type = device::xHidDeviceFactory::PID2EnumDevice(pid);
        enumProduct = EnumProduct2EnumDevice(JUB_ENUM_COMMODE::HID, type);
#endif  // #if defined(HID_MODE) end
        if (    JUB_ENUM_DEVICE::DEVICE_NS_ITEM  == type
            || JUB_ENUM_PRODUCT::PRODUCT_NS_ITEM == enumProduct
            ) {
            return nullptr;
        }

        auto device = device::devFactory::GetInstance()->CreateDevice(type, arg);
        return Create(enumProduct, type, device);
    }


    device::DeviceTypeBase* CreateProduct(const JUB_ENUM_COMMODE& mode, const std::string& arg) {

        JUB_ENUM_DEVICE         type =  JUB_ENUM_DEVICE::DEVICE_NS_ITEM;
        JUB_ENUM_PRODUCT enumProduct = JUB_ENUM_PRODUCT::PRODUCT_NS_ITEM;
        switch (mode) {
        case JUB_ENUM_COMMODE::SWI:
#if defined(SWI_MODE)
            type = JUB_ENUM_DEVICE::VD;
            enumProduct = EnumProduct2EnumDevice(mode, type);
#endif  // #if defined(SWI_MODE) end
            break;
        case JUB_ENUM_COMMODE::HID:
        case JUB_ENUM_COMMODE::BLE:
#if defined(BLE_MODE)
            type = device::xBLEDeviceFactory::Prefix2EnumDevice(arg);
            enumProduct = EnumProduct2EnumDevice(mode, type);
#endif  // #if defined(BLE_MODE) end
            break;
        case JUB_ENUM_COMMODE::SIM:
        case JUB_ENUM_COMMODE::NFC:
        default:
            break;
        }   // switch (mode) end
        if (JUB_ENUM_PRODUCT::PRODUCT_NS_ITEM == enumProduct) {
            return nullptr;
        }

        auto device = device::devFactory::GetInstance()->CreateDevice(type, arg);
        return Create(enumProduct, type, device);
    }


    device::DeviceTypeBase* CreateProduct(const JUB_ENUM_DEVICE& type) {
        JUB_ENUM_PRODUCT enumProduct = JUB_ENUM_PRODUCT::PRODUCT_NS_ITEM;
#if defined(NFC_MODE)
        enumProduct = EnumProduct2EnumDevice(JUB_ENUM_COMMODE::NFC, type);
#endif  // #if defined(NFC_MODE) end
        if (JUB_ENUM_PRODUCT::PRODUCT_NS_ITEM == enumProduct) {
            return nullptr;
        }

        auto device = device::devFactory::GetInstance()->CreateDevice(type);
        return Create(enumProduct, type, device);
    }


    device::DeviceTypeBase* CreateProduct(const JUB_ENUM_COMMODE& mode) {

        JUB_ENUM_DEVICE         type =  JUB_ENUM_DEVICE::DEVICE_NS_ITEM;
        JUB_ENUM_PRODUCT enumProduct = JUB_ENUM_PRODUCT::PRODUCT_NS_ITEM;
        switch (mode) {
        case JUB_ENUM_COMMODE::SWI:
#if defined(SWI_MODE)
            type = JUB_ENUM_DEVICE::VD;
#endif  // #if defined(SWI_MODE) end
            break;
        case JUB_ENUM_COMMODE::HID:
#if defined(HID_MODE)
            type = JUB_ENUM_DEVICE::BLADE;
#endif  // #if defined(HID_MODE) end
            break;
        case JUB_ENUM_COMMODE::BLE:
#if defined(BLE_MODE)
            type = JUB_ENUM_DEVICE::BLADE;
#endif  // #if defined(BLE_MODE) end
            break;
        case JUB_ENUM_COMMODE::NFC:
#if defined(NFC_MODE)
            type = JUB_ENUM_DEVICE::LITE;
#endif  // #if defined(NFC_MODE) end
            break;
        default:
            break;
        }   // switch (mode) end

        enumProduct = EnumProduct2EnumDevice(mode, type);
        if (    JUB_ENUM_DEVICE::DEVICE_NS_ITEM  == type
            || JUB_ENUM_PRODUCT::PRODUCT_NS_ITEM == enumProduct
            ) {
            return nullptr;
        }

        auto device = device::devFactory::GetInstance()->CreateDevice(mode);
        return Create(enumProduct, type, device);
    }


    device::DeviceTypeBase* CreateProduct(const JUB_ENUM_COMMODE& mode, const JUB_ENUM_DEVICE& type, const std::string& arg1, const std::string& arg2) {

        if (  JUB_ENUM_COMMODE::COMMODE_NS_ITEM == mode
            || JUB_ENUM_DEVICE::DEVICE_NS_ITEM  == type
            ) {
            return nullptr;
        }

        JUB_ENUM_PRODUCT enumProduct = JUB_ENUM_PRODUCT::PRODUCT_NS_ITEM;
        switch (mode) {
        case JUB_ENUM_COMMODE::SIM:
            enumProduct = (JUB_ENUM_PRODUCT)((mode<<0x10)|type);
            break;
        case JUB_ENUM_COMMODE::SWI:
        case JUB_ENUM_COMMODE::HID:
        case JUB_ENUM_COMMODE::BLE:
        case JUB_ENUM_COMMODE::NFC:
        default:
            break;
        }   // switch (mode) end

        if (JUB_ENUM_PRODUCT::PRODUCT_NS_ITEM == enumProduct) {
            return nullptr;
        }

        auto device = device::devFactory::GetInstance()->CreateDevice(mode, type, arg1, arg2);
        return Create(enumProduct, type, device);
    }
};


using prdsFactory = Singleton<xProductFactory>;


} // namespace product end
} // namespace jub end


#endif /* ProductFactory_h */
