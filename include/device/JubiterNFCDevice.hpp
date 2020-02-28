#ifndef __JubiterNFCDevice__
#define __JubiterNFCDevice__

#include "JUB_SDK_DEV.h"
#include "JubiterHidDevice.hpp"

#ifdef HID_MODE

namespace jub {
namespace device {

class JubiterNFCDevice
    : public JubiterHidDevice {

public:
    JubiterNFCDevice(const std::string path)
        : JubiterHidDevice(path) {
    };
    ~JubiterNFCDevice();
}; // class JubiterNFCDevice end

} // namespace device end
} // namespace jub end

#endif // HID_MODE
#endif // __JubiterNFCDevice__
