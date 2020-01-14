#ifndef __DeviceTypeBase__
#define __DeviceTypeBase__

#include "JUB_SDK.h"
#include <string>

#include "utility/Singleton.h"
#include "utility/xManager.hpp"

namespace jub {
namespace device {

class DeviceTypeBase {

public:
    virtual ~DeviceTypeBase() = 0;

    /* functions */
    virtual JUB_RV Connect() = 0;
    virtual JUB_RV Disconnect() = 0;

    virtual JUB_RV SendData(IN JUB_BYTE_CPTR sendData, IN JUB_ULONG ulSendLen,
                            OUT JUB_BYTE_PTR retData, INOUT JUB_ULONG_PTR pulRetDataLen,
                            IN JUB_ULONG ulMiliSecondTimeout = 1200000) = 0;
}; // class DeviceTypeBase end

#define FT3KHN_READWRITE_SIZE_ONCE_NEW 4096

using DeviceManager = Singleton<xManager<DeviceTypeBase>>;

} // namespace device end
} // namespace jub end

#endif // __DeviceTypeBase__
