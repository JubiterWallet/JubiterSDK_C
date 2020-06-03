#ifndef __JubiterHidDevice__
#define __JubiterHidDevice__

#include "JUB_SDK_DEV.h"

#ifdef HID_MODE

#include <vector>

#include "hidapi/hidapi/hidapi.h"
#include "device/DeviceTypeBase.hpp"

namespace jub {
namespace device {

#define VID 0x096e
#define PID 0x0891

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
    JubiterHidDevice(const std::string path);
    ~JubiterHidDevice();

public:
    static  std::vector<std::string> EnumDevice();
    virtual JUB_RV Connect();
    virtual JUB_RV Disconnect();

    virtual JUB_RV SendData(IN JUB_BYTE_CPTR sendData, IN JUB_ULONG ulSendLen,
                            OUT JUB_BYTE_PTR retData, INOUT JUB_ULONG_PTR pulRetDataLen,
                            IN JUB_ULONG ulMiliSecondTimeout = 1200000);
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

} // namespace device end
} // namespace jub end

#endif // HID_MODE
#endif  // __JubiterHidDevice__
