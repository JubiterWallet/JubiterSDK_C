#ifndef __JubiterHidDevice__
#define __JubiterHidDevice__

#include "device/DeviceTypeBase.hpp"

#if defined(HID_MODE)
#include <vector>
#include "hidapi/hidapi/hidapi.h"


namespace jub {
namespace device {


#define VID 0x096e
#define PID_BLD 0x0891
#define PID_BIO 0x0893

#define HID_PACKET_SIZE         65
#define HID_PACKET_HEAD_FRIST   8
#define HID_PACKET_HEAD_ROUND   6

#define HID_PACKET_RHEAD_FRIST  7
#define HID_PACKET_RHEAD_ROUND  5

#define HID_PACKET_GNU_SIZE     64
#define FIDO2_WAIT_FLAG         0xbb

class JubiterHidDevice
    : public DeviceTypeBase {

public:
    JubiterHidDevice(const std::string& path, const unsigned short productID);
    ~JubiterHidDevice();

public:
    static  std::vector<std::string> EnumDevice(unsigned short productID, unsigned short vendorID = VID);
    virtual JUB_RV Connect() override;
    virtual JUB_RV Disconnect() override;
    virtual void Reset() override {}

    virtual JUB_RV SendData(IN JUB_BYTE_CPTR sendData, IN JUB_ULONG ulSendLen,
                            OUT JUB_BYTE_PTR retData, INOUT JUB_ULONG_PTR pulRetDataLen,
                            IN JUB_ULONG ulMiliSecondTimeout = 1200000) override;
    std::string getPath() { return _path; }

protected:
    int _Write(const unsigned char *data, size_t length);
    int _Read(unsigned char *data, size_t length);

protected:
    hid_device* _handle;
    unsigned short _vid;
    unsigned short _pid;
    std::string _path;
    bool _bFirstCmd;
}; // class JubiterHidDevice end


class JubiterHidBLDDevice :
public JubiterHidDevice {
public:
    static DeviceTypeBase* Create(const JUB_ENUM_DEVICE& type, std::shared_ptr<device::DeviceTypeBase> devicePtr) {

        switch (type) {
        case JUB_ENUM_DEVICE::BLADE:
            return new JubiterHidBLDDevice(std::dynamic_pointer_cast<device::JubiterHidBLDDevice>(devicePtr)->getPath());
        case JUB_ENUM_DEVICE::BIO:
        default:
            break;
        }   // switch (type) end

        return nullptr;
    }

public:
    JubiterHidBLDDevice(const std::string& path) :
        JubiterHidDevice(path, PID_BLD) {}
    ~JubiterHidBLDDevice() {}

//    explicit operator JubiterHidBLDDevice* () const {
//        if (productID == _pid) {
//            return  const_cast<JubiterHidBLDDevice*>(this);
//        }
//        return nullptr;
//    }
//
//    explicit operator JubiterHidDevice* () const {
//        return  dynamic_cast<JubiterHidDevice*>(const_cast<JubiterHidBLDDevice*>(this));
//    }
//    static unsigned short productID;
};  // class JubiterHidBLDDevice end


class JubiterHidBIODevice :
public JubiterHidDevice {
public:
    static DeviceTypeBase* Create(const JUB_ENUM_DEVICE& type, std::shared_ptr<device::DeviceTypeBase> devicePtr) {

        if (nullptr == devicePtr) {
            return nullptr;
        }

        switch (type) {
        case JUB_ENUM_DEVICE::BIO:
            return new JubiterHidBIODevice(std::dynamic_pointer_cast<device::JubiterHidBIODevice>(devicePtr)->getPath());
        case JUB_ENUM_DEVICE::BLADE:
        default:
            break;
        }   // switch (type) end

        return nullptr;
    }

public:
    JubiterHidBIODevice(const std::string& path) :
        JubiterHidDevice(path, PID_BIO) {}
    ~JubiterHidBIODevice() {}

//    explicit operator JubiterHidBIODevice* () const {
//        if (PID_BIO == _pid) {
//            return  const_cast<JubiterHidBIODevice*>(this);
//        }
//        return nullptr;
//    }
//
//    explicit operator JubiterHidDevice* () const {
//        return  dynamic_cast<JubiterHidDevice*>(const_cast<JubiterHidBIODevice*>(this));
//    }

//    static unsigned short productID;
};  // class JubiterHidBIODevice end


} // namespace device end
} // namespace jub end


#endif  // #if defined(HID_MODE) end
#endif  // __JubiterHidDevice__
