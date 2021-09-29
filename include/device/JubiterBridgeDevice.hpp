//
//  JubiterBridgeDevice.hpp
//  JubSDK
//
//  Created by Chen ruquan on 2020/12/01.
//  Copyright © 2020 JuBiter. All rights reserved.
//
#if defined(SIM_MODE)
#ifndef __JubiterBridgeDevice__
#define __JubiterBridgeDevice__

#if defined(_WIN32)
#include <winscard.h>
#elif defined(__APPLE__)
#include <PCSC/winscard.h>
#include <PCSC/wintypes.h>
#endif
#include "device/DeviceTypeBase.hpp"
#include "scp03/scp03.hpp"
#include "scp11/scp11c.hpp"

#include <vector>

namespace jub {
namespace device {

class JubiterBridgeDevice
    : public DeviceTypeBase {
private:
    class Impl;

public:
    //for Factory
    template<typename T>
    static std::shared_ptr<JubiterBridgeDevice> Create(const std::string& ip, const std::string& name) {
        return std::make_shared<T>(ip, name);
    }

public:
    JubiterBridgeDevice(const std::string& ip, const std::string& name);
    virtual ~JubiterBridgeDevice();
    static  std::vector<std::string> EnumDevice(const std::string& ip);
    virtual JUB_RV Connect();
    virtual JUB_RV Disconnect();
    virtual JUB_RV IsConnect();
    virtual void Reset() {}

    virtual JUB_RV SendData(IN JUB_BYTE_CPTR sendData, IN JUB_ULONG ulSendLen,
        OUT JUB_BYTE_PTR retData, INOUT JUB_ULONG_PTR pulRetDataLen,
        IN JUB_ULONG ulMiliSecondTimeout = 1200000);
    std::string getIP() { return ip_; }
    std::string getName() { return name_; }

private:
    Impl *impl_;
    unsigned long handle_;
    std::string ip_;
    std::string name_;
}; // class JubiterBridgeDevice end


class JubiterBridgeBLDDevice :
public JubiterBridgeDevice {
public:
    static DeviceTypeBase* Create(const JUB_ENUM_DEVICE& type, std::shared_ptr<device::DeviceTypeBase> devicePtr) {

        switch (type) {
        case JUB_ENUM_DEVICE::BLADE:
            return new JubiterBridgeBLDDevice(
                std::dynamic_pointer_cast<device::JubiterBridgeDevice>(devicePtr)->getIP(),
                std::dynamic_pointer_cast<device::JubiterBridgeBLDDevice>(devicePtr)->getName());
        case JUB_ENUM_DEVICE::BIO:
        case JUB_ENUM_DEVICE::LITE:
        default:
            break;
        }   // switch (type) end

        return nullptr;
    }

public:
    JubiterBridgeBLDDevice(const std::string& ip, const std::string& name) :
        JubiterBridgeDevice(ip, name) {}
    ~JubiterBridgeBLDDevice() {}

//    explicit operator JubiterBridgeBLDDevice* () const {
//        return  dynamic_cast<JubiterBridgeBLDDevice*>(const_cast<JubiterBridgeBLDDevice*>(this));
//    }
//
//    explicit operator JubiterBridgeDevice* () const {
//        return  dynamic_cast<JubiterBridgeDevice*>(const_cast<JubiterBridgeBLDDevice*>(this));
//    }
//    static unsigned short productID;
};  // class JubiterBridgeBLDDevice end


class JubiterBridgeBIODevice :
public JubiterBridgeDevice {
public:
    static DeviceTypeBase* Create(const JUB_ENUM_DEVICE& type, std::shared_ptr<device::DeviceTypeBase> devicePtr) {

        switch (type) {
        case JUB_ENUM_DEVICE::BIO:
            return new JubiterBridgeBIODevice(
                std::dynamic_pointer_cast<device::JubiterBridgeDevice>(devicePtr)->getIP(),
                std::dynamic_pointer_cast<device::JubiterBridgeBIODevice>(devicePtr)->getName());
        case JUB_ENUM_DEVICE::BLADE:
        case JUB_ENUM_DEVICE::LITE:
        default:
            break;
        }   // switch (type) end

        return nullptr;
    }

public:
    JubiterBridgeBIODevice(const std::string& ip, const std::string& name) :
        JubiterBridgeDevice(ip, name) {}
    ~JubiterBridgeBIODevice() {}

//    explicit operator JubiterBridgeBIODevice* () const {
//        return  dynamic_cast<JubiterBridgeBIODevice*>(const_cast<JubiterBridgeBIODevice*>(this));
//    }
//
//    explicit operator JubiterBridgeDevice* () const {
//        return  dynamic_cast<JubiterBridgeDevice*>(const_cast<JubiterBridgeBIODevice*>(this));
//    }
//    static unsigned short productID;
};  // class JubiterBridgeBIODevice end


class JubiterBridgeLITEDevice :
public JubiterBridgeDevice {
public:
    static DeviceTypeBase* Create(const JUB_ENUM_DEVICE& type, std::shared_ptr<device::DeviceTypeBase> devicePtr) {

        switch (type) {
        case JUB_ENUM_DEVICE::LITE:
            return new JubiterBridgeLITEDevice(
                std::dynamic_pointer_cast<device::JubiterBridgeDevice>(devicePtr)->getIP(),
                std::dynamic_pointer_cast<device::JubiterBridgeLITEDevice>(devicePtr)->getName());
        case JUB_ENUM_DEVICE::BLADE:
        case JUB_ENUM_DEVICE::BIO:
        default:
            break;
        }   // switch (type) end

        return nullptr;
    }

public:
    JubiterBridgeLITEDevice(const std::string& ip, const std::string& name) :
        JubiterBridgeDevice(ip, name) {}
    ~JubiterBridgeLITEDevice() {}

//    explicit operator JubiterBridgeLITEDevice* () const {
//        return  dynamic_cast<JubiterBridgeLITEDevice*>(const_cast<JubiterBridgeLITEDevice*>(this));
//    }
//
//    explicit operator JubiterBridgeDevice* () const {
//        return  dynamic_cast<JubiterBridgeDevice*>(const_cast<JubiterBridgeLITEDevice*>(this));
//    }
//    static unsigned short productID;

public:
    virtual unsigned int Initialize(const LITE_DEVICE_INIT_PARAM& params);

    virtual void *GetSCP11() override {
        return &_scp11;
    }
    virtual void *GetSCP03() override {
        return &_scp03;
    }

protected:
    // for secure channel
    virtual unsigned int SetSCP11Param(const std::string& crt,
                                       const std::string& rk,
                                       const std::string& hostID,
                                       const uint8_t keyLength);

private:
    scp03  _scp03;
    scp11c _scp11;
};  // class JubiterBridgeLITEDevice end


} // namespace device end
} // namespace jub end


#endif  // #if defined(SIM_MODE) end
#endif  // __JubiterBridgeDevice__
