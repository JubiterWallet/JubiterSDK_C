#ifndef __JubiterHidDevice__
#define __JubiterHidDevice__

#include <JUB_SDK.h>

#ifdef HID_MODE

#include <hidapi/hidapi/hidapi.h>
#include <device/DeviceTypeBase.hpp>
#include <vector>

namespace jub {

#define VID 0x096e
#define PID 0x0891
#define PID_G2 0x0893

#define HID_PACKET_SIZE 65
#define HID_PACKET_HEAD_FRIST 8
#define HID_PACKET_HEAD_ROUND 6

#define HID_PACKET_RHEAD_FRIST 7
#define HID_PACKET_RHEAD_ROUND 5


#define HID_PACKET_GNU_SIZE 64
#define FIDO2_WAIT_FLAG     0xbb

class JubiterHidDevice : public DeviceTypeBase {

public:
    JubiterHidDevice();
    ~JubiterHidDevice();

public:
	static  std::vector<std::string> enumDevice();
    virtual JUB_RV connect(const std::string path);
    virtual JUB_RV disconnect();

    virtual JUB_RV sendData(IN JUB_BYTE_CPTR sendData, IN JUB_ULONG sendLen,
                            OUT JUB_BYTE_PTR pRetData,
                            INOUT JUB_ULONG_PTR pulRetLen,
                            IN JUB_ULONG ulMiliSecondTimeout = 1200000);

protected:
	int write(const unsigned char *data, size_t length);
	int read(unsigned char *data, size_t length);

protected:
    hid_device* m_handle;
    unsigned short vid;
    unsigned short pid;
	std::string _path;
	bool firstCmd;
}; // class JubiterHidDevice end

} // namespace jub end

#endif // HID_MODE
#endif  // __JubiterHidDevice__
