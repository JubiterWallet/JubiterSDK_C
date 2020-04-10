#ifndef __DeviceTypeBase__
#define __DeviceTypeBase__

#include "JUB_SDK.h"
#include <string>

#include "utility/Singleton.h"
#include "utility/xManager.hpp"

using device_map = Singleton<xManager<JUB_ULONG>>;

namespace jub {
namespace device {

class DeviceTypeBase {

public:
    virtual ~DeviceTypeBase() {
    }

    /* functions */
    virtual JUB_RV Connect() = 0;
    virtual JUB_RV Disconnect() = 0;

    virtual JUB_RV SendData(IN JUB_BYTE_CPTR sendData, IN JUB_ULONG ulSendLen,
                            OUT JUB_BYTE_PTR retData, INOUT JUB_ULONG_PTR pulRetDataLen,
                            IN JUB_ULONG ulMiliSecondTimeout = 1200000) = 0;
}; // class DeviceTypeBase end


class AutoDeviceManager :
    public xManager<DeviceTypeBase> {
public:
    template<typename T>
    T* GetOneSafe(JUB_UINT16 ID) {
        DeviceTypeBase* pDevice = GetOne(ID);
        T* t = dynamic_cast<T*>(pDevice);
        if (t != nullptr) {
            if (_last != pDevice) {
                _last  = pDevice;
            }
        }
        return t;
    }
}; // class class AutoDeviceManager end

#define FT3KHN_READWRITE_SIZE_ONCE_NEW 4096

using DeviceManager = Singleton<AutoDeviceManager>;

} // namespace device end
} // namespace jub end

#endif // __DeviceTypeBase__
