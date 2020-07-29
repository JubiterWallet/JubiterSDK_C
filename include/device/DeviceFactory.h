//
//  DeviceFactory.h
//  JubSDK
//
//  Created by Pan Min on 2020/7/24.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef DeviceFactory_h
#define DeviceFactory_h

#include <utility/xFactory.hpp>

#include <device/JubiterHidDevice.hpp>
#include <device/JubiterBLEDevice.hpp>
#include <device/JubiterNFCDevice.hpp>


namespace jub {
namespace device {


#if defined(HID_MODE)
typedef std::shared_ptr<JubiterHidDevice>(*CreateHidDeviceFn)(const std::string&);

class xHidDeviceFactory :
public xFactory<std::shared_ptr<JubiterHidDevice>,
                JUB_ENUM_DEVICE,
                CreateHidDeviceFn> {
public:
    xHidDeviceFactory() {
        Register(JUB_ENUM_DEVICE::BLD, &JubiterHidBLDDevice::Create);
        Register(JUB_ENUM_DEVICE::BIO, &JubiterHidBIODevice::Create);
    }
    ~xHidDeviceFactory() = default;


    static std::vector<unsigned short> EnumPID() {
        std::vector<unsigned short> hidList;
        hidList.push_back(PID_BLD);
        hidList.push_back(PID_BIO);
        return std::vector<unsigned short>(hidList);
    }


    static JUB_ENUM_DEVICE PID2EnumDevice(const unsigned short pid) {

        JUB_ENUM_DEVICE enumDevice = DEVICE_NS_ITEM;
        switch (pid) {
        case PID_BLD:
            enumDevice = JUB_ENUM_DEVICE::BLD;
            break;
        case PID_BIO:
            enumDevice = JUB_ENUM_DEVICE::BIO;
            break;
        default:
            break;
        }   // switch (pid) end

        return enumDevice;
    }


    static bool CheckTypeid(const unsigned short pid, const JubiterHidDevice* device) {
        bool b = false;

        JUB_ENUM_DEVICE enumDevice = PID2EnumDevice(pid);
        switch(enumDevice) {
        case JUB_ENUM_DEVICE::BLD:
            b = (typeid(JubiterHidBLDDevice) == typeid(*device));
            break;
        case JUB_ENUM_DEVICE::BIO:
            b = (typeid(JubiterHidBIODevice) == typeid(*device));
            break;
        default:
            break;
        }

        return b;
    }
}; // class xHidDeviceFactory end
#endif  // #if defined(HID_MODE) end


#if defined(BLE_MODE)
typedef std::shared_ptr<JubiterBLEDevice>(*CreateBLEDeviceFn)(const std::string&);

class xBLEDeviceFactory :
public xFactory<std::shared_ptr<JubiterBLEDevice>,
                JUB_ENUM_DEVICE,
                CreateBLEDeviceFn> {
public:
    static JUB_ENUM_DEVICE Prefix2EnumDevice(const std::string& name) {
        JUB_ENUM_DEVICE enumDevice = DEVICE_NS_ITEM;

        if (std::string::npos != name.find(PREFIX_BLD)) {
            enumDevice = JUB_ENUM_DEVICE::BLD;
        }
        else if (std::string::npos != name.find(PREFIX_BIO)) {
            enumDevice = JUB_ENUM_DEVICE::BIO;
        }
        else {
            enumDevice = JUB_ENUM_DEVICE::DEVICE_NS_ITEM;
        }

        return enumDevice;
    }


    static bool CheckTypeid(const DeviceTypeBase* device) {
        bool b = false;

        if (   (typeid(JubiterBLEDevice)    == typeid(*device))
            || (typeid(JubiterBLEBLDDevice) == typeid(*device))
            || (typeid(JubiterBLEBIODevice) == typeid(*device))
            ) {
            b = true;
        }

        return b;
    }


    static bool CheckTypeid(const std::string& uuid, const JubiterBLEDevice* device) {

        bool b = false;

        JUB_ENUM_DEVICE enumDevice = Prefix2EnumDevice(uuid);
        switch (enumDevice) {
        case JUB_ENUM_DEVICE::BLD:
            b = (typeid(JubiterBLEBLDDevice) == typeid(*device));
            break;
        case JUB_ENUM_DEVICE::BIO:
            b = (typeid(JubiterBLEBIODevice) == typeid(*device));
            break;
        default:
            b = (typeid(JubiterBLEDevice) == typeid(*device));
            break;
        }   // switch (enumDevice) end

        return b;
    }


public:
    xBLEDeviceFactory() {
        Register(JUB_ENUM_DEVICE::BLD, &JubiterBLEBLDDevice::Create);
        Register(JUB_ENUM_DEVICE::BIO, &JubiterBLEBIODevice::Create);
    }
    ~xBLEDeviceFactory() = default;
};  // class xBLEDeviceFactory end
#endif  // #if defined(BLE_MODE) end


#if defined(NFC_MODE)
typedef std::shared_ptr<JubiterNFCDevice>(*CreateNFCDeviceFn)();

class xNFCDeviceFactory :
public xFactory<std::shared_ptr<JubiterNFCDevice>,
                JUB_ENUM_DEVICE,
                CreateNFCDeviceFn> {
public:
    static bool CheckTypeid(const DeviceTypeBase* device) {

        return (typeid(JubiterNFCDevice) == typeid(*device));
    }


public:
    xNFCDeviceFactory() {
        Register(JUB_ENUM_DEVICE::NFCARD, &JubiterNFCDevice::Create);
    }
    ~xNFCDeviceFactory() = default;
};  // class xNFCDeviceFactory end
#endif  // #if defined(NFC_MODE) end


class xDeviceFactory {
protected:
#if defined(HID_MODE)
    xHidDeviceFactory hidFactory;
#endif  // #if defined(HID_MODE) end
#if defined(BLE_MODE)
    xBLEDeviceFactory bleFactory;
#endif  // #if defined(BLE_MODE) end
#if defined(NFC_MODE)
    xNFCDeviceFactory nfcFactory;
#endif  // #if defined(NFC_MODE) end


public:
    std::shared_ptr<DeviceTypeBase> CreateDevice(const JUB_ENUM_DEVICE& type, const std::string& arg) {

#if defined(HID_MODE)
        switch (type) {
        case JUB_ENUM_DEVICE::BLD:
            return hidFactory.Create(JUB_ENUM_DEVICE::BLD, arg);
        case JUB_ENUM_DEVICE::BIO:
            return hidFactory.Create(JUB_ENUM_DEVICE::BIO, arg);
        default:
            break;
        }   // witch (type) end
#endif  // #if defined(HID_MODE) end
#if defined(BLE_MODE)
        switch (type) {
        case JUB_ENUM_DEVICE::BLD:
            return bleFactory.Create(JUB_ENUM_DEVICE::BLD, arg);
        case JUB_ENUM_DEVICE::BIO:
            return bleFactory.Create(JUB_ENUM_DEVICE::BIO, arg);
        default:
            break;
        }   // switch (type) end
#endif  // #if defined(BLE_MODE) end
#if defined(NFC_MODE)
        switch (type) {
        case JUB_ENUM_DEVICE::NFCARD:
            return nfcFactory.Create(JUB_ENUM_DEVICE::NFCARD);
        default:
            break;
        }   // switch (type) end
#endif  // #if defined(NFC_MODE) end

        return nullptr;
    }


    std::shared_ptr<DeviceTypeBase> CreateDevice(const JUB_ENUM_DEVICE& type) {

        switch (type) {
        case JUB_ENUM_DEVICE::NFCARD:
#if defined(NFC_MODE)
            return nfcFactory.Create(JUB_ENUM_DEVICE::NFCARD);
#endif  // #if defined(NFC_MODE) end
        case JUB_ENUM_DEVICE::BLD:
        case JUB_ENUM_DEVICE::BIO:
        default:
            break;
        }   // switch (type) end

        return nullptr;
    }


    // for communication library
    std::shared_ptr<DeviceTypeBase> CreateDevice(const JUB_ENUM_COMMODE& mode) {
        switch (mode) {
        case JUB_ENUM_COMMODE::HID:
#if defined(HID_MODE)
            return hidFactory.Create(JUB_ENUM_DEVICE::BLD, "");
#endif  // #if defined(HID_MODE) end
        case JUB_ENUM_COMMODE::BLE:
#if defined(BLE_MODE)
            return bleFactory.Create(JUB_ENUM_DEVICE::BLD, "");
#endif  // #if defined(BLE_MODE) end
        case JUB_ENUM_COMMODE::NFC:
#if defined(NFC_MODE)
            return nfcFactory.Create(JUB_ENUM_DEVICE::NFCARD);
#endif  // #if defined(NFC_MODE) end
        default:
            break;
        }
        return nullptr;
    }
};  // class xDeviceFactory end

using devFactory = Singleton<xDeviceFactory>;


} // namespace device end
} // namespace jub end

#endif /* DeviceFactory_h */
