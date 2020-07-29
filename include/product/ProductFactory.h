//
//  ProductFactory.h
//  JubSDK
//
//  Created by Pan Min on 2020/7/27.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef ProductFactory_h
#define ProductFactory_h

#include <utility/xFactory.hpp>
#include <utility/Singleton.h>

#include <device/DeviceFactory.h>


namespace jub {
namespace product {


// Remove c++ features for swift framework
typedef enum {
    HID_BLD = (device::JUB_ENUM_COMMODE::HID<<0x10)|(device::JUB_ENUM_DEVICE::BLD),
    HID_BIO = (device::JUB_ENUM_COMMODE::HID<<0x10)|(device::JUB_ENUM_DEVICE::BIO),
    HID_NFC = (device::JUB_ENUM_COMMODE::HID<<0x10)|(device::JUB_ENUM_DEVICE::NFCARD),
    BLE_BLD = (device::JUB_ENUM_COMMODE::BLE<<0x10)|(device::JUB_ENUM_DEVICE::BLD),
    BLE_BIO = (device::JUB_ENUM_COMMODE::BLE<<0x10)|(device::JUB_ENUM_DEVICE::BIO),
    NFC_CARD= (device::JUB_ENUM_COMMODE::NFC<<0x10)|(device::JUB_ENUM_DEVICE::NFCARD),
    PRODUCT_NS_ITEM
} JUB_ENUM_PRODUCT;
// Remove c++ features for swift framework end


typedef device::DeviceTypeBase* (*CreateProductFn)(const device::JUB_ENUM_DEVICE& type, std::shared_ptr<device::DeviceTypeBase>);

class xProductFactory :
protected xFactory<device::DeviceTypeBase*,
                   JUB_ENUM_PRODUCT,
                    CreateProductFn> {
public:
    static device::JUB_ENUM_DEVICE EnumProduct2EnumDevice(JUB_ENUM_PRODUCT product) {
        return (device::JUB_ENUM_DEVICE)(product&0x0000FFFF);
    }

    static        JUB_ENUM_PRODUCT EnumProduct2EnumDevice(device::JUB_ENUM_COMMODE mode, device::JUB_ENUM_DEVICE type) {
        return (JUB_ENUM_PRODUCT)((mode<<0x10)|type);
    }

public:
    xProductFactory() {
#if defined(HID_MODE)
        Register(JUB_ENUM_PRODUCT::HID_BLD,   &device::JubiterHidBLDDevice::Create);
        Register(JUB_ENUM_PRODUCT::HID_BIO,   &device::JubiterHidBIODevice::Create);
#endif  // #if defined(HID_MODE) end
#if defined(BLE_MODE)
        Register(JUB_ENUM_PRODUCT::BLE_BLD,   &device::JubiterBLEBLDDevice::Create);
        Register(JUB_ENUM_PRODUCT::BLE_BIO,   &device::JubiterBLEBIODevice::Create);
#endif  // #if defined(BLE_MODE) end
#if defined(NFC_MODE)
        Register(JUB_ENUM_PRODUCT::NFC_CARD,  &device::JubiterNFCDevice::Create);
#endif  // #if defined(NFC_MODE) end
    };


    device::DeviceTypeBase* CreateProduct(const unsigned short& pid, const std::string& arg) {
        device::JUB_ENUM_DEVICE type = device::JUB_ENUM_DEVICE::DEVICE_NS_ITEM;
        JUB_ENUM_PRODUCT enumProduct = JUB_ENUM_PRODUCT::PRODUCT_NS_ITEM;
#if defined(HID_MODE)
        type = device::xHidDeviceFactory::PID2EnumDevice(pid);
        enumProduct = EnumProduct2EnumDevice(device::JUB_ENUM_COMMODE::HID, type);
#endif  // #if defined(HID_MODE) end
        if (device::JUB_ENUM_DEVICE::DEVICE_NS_ITEM == type
            ||    JUB_ENUM_PRODUCT::PRODUCT_NS_ITEM == enumProduct
            ) {
            return nullptr;
        }

        auto device = device::devFactory::GetInstance()->CreateDevice(type, arg);
        return Create(enumProduct, type, device);
    }


    device::DeviceTypeBase* CreateProduct(const device::JUB_ENUM_COMMODE& mode, const std::string& arg) {

        device::JUB_ENUM_DEVICE type = device::JUB_ENUM_DEVICE::DEVICE_NS_ITEM;
        JUB_ENUM_PRODUCT enumProduct = JUB_ENUM_PRODUCT::PRODUCT_NS_ITEM;
        switch (mode) {
        case device::JUB_ENUM_COMMODE::HID:
        case device::JUB_ENUM_COMMODE::BLE:
#if defined(BLE_MODE)
            type = device::xBLEDeviceFactory::Prefix2EnumDevice(arg);
            enumProduct = EnumProduct2EnumDevice(mode, type);
#endif  // #if defined(BLE_MODE) end
            break;
        case device::JUB_ENUM_COMMODE::NFC:
        default:
            break;
        }   // switch (mode) end
        if (JUB_ENUM_PRODUCT::PRODUCT_NS_ITEM == enumProduct) {
            return nullptr;
        }

        auto device = device::devFactory::GetInstance()->CreateDevice(type, arg);
        return Create(enumProduct, type, device);
    }


    device::DeviceTypeBase* CreateProduct(const device::JUB_ENUM_DEVICE& type) {
        JUB_ENUM_PRODUCT enumProduct = JUB_ENUM_PRODUCT::PRODUCT_NS_ITEM;
#if defined(NFC_MODE)
        enumProduct = EnumProduct2EnumDevice(device::JUB_ENUM_COMMODE::NFC, type);
#endif  // #if defined(NFC_MODE) end
        if (JUB_ENUM_PRODUCT::PRODUCT_NS_ITEM == enumProduct) {
            return nullptr;
        }

        auto device = device::devFactory::GetInstance()->CreateDevice(type);
        return Create(enumProduct, type, device);
    }


    device::DeviceTypeBase* CreateProduct(const device::JUB_ENUM_COMMODE& mode) {

        device::JUB_ENUM_DEVICE type = device::JUB_ENUM_DEVICE::DEVICE_NS_ITEM;
        JUB_ENUM_PRODUCT enumProduct = JUB_ENUM_PRODUCT::PRODUCT_NS_ITEM;
        switch (mode) {
        case device::JUB_ENUM_COMMODE::HID:
#if defined(HID_MODE)
            type = device::JUB_ENUM_DEVICE::BLD;
#endif  // #if defined(HID_MODE) end
            break;
        case device::JUB_ENUM_COMMODE::BLE:
#if defined(BLE_MODE)
            type = device::JUB_ENUM_DEVICE::BLD;
#endif  // #if defined(BLE_MODE) end
            break;
        case device::JUB_ENUM_COMMODE::NFC:
#if defined(NFC_MODE)
            type = device::JUB_ENUM_DEVICE::NFCARD;
#endif  // #if defined(NFC_MODE) end
            break;
        default:
            break;
        }   // switch (mode) end

        enumProduct = EnumProduct2EnumDevice(mode, type);
        if (device::JUB_ENUM_DEVICE::DEVICE_NS_ITEM == type
            ||    JUB_ENUM_PRODUCT::PRODUCT_NS_ITEM == enumProduct
            ) {
            return nullptr;
        }

        auto device = device::devFactory::GetInstance()->CreateDevice(mode);
        return Create(enumProduct, type, device);
    }
};

using prdsFactory = Singleton<xProductFactory>;


} // namespace product end
} // namespace jub end


#endif /* ProductFactory_h */
