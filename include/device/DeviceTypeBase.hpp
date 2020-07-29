#ifndef __DeviceTypeBase__
#define __DeviceTypeBase__

#include "JUB_SDK.h"
#include <string>

#include "utility/Singleton.h"
#include "utility/xManager.hpp"


using device_map = Singleton<xManager<JUB_ULONG>>;


namespace jub {
namespace device {


// Remove c++ features for swift framework
typedef enum {
    HID = 0x00,
    BLE = 0x01,
    NFC = 0x02,
    COMMODE_NS_ITEM
} JUB_ENUM_COMMODE;
// Remove c++ features for swift framework end


// Remove c++ features for swift framework
typedef enum {
    BLD = 0x00,
    BIO,
    NFCARD,
    DEVICE_NS_ITEM
} JUB_ENUM_DEVICE;
// Remove c++ features for swift framework end


class DeviceTypeBase {

public:
    DeviceTypeBase() {
    }
    virtual ~DeviceTypeBase() {
    }

    /* functions */
    virtual JUB_RV Connect() = 0;
    virtual JUB_RV Disconnect() = 0;
    virtual void Reset() = 0;

    virtual JUB_RV SendData(IN JUB_BYTE_CPTR sendData, IN JUB_ULONG ulSendLen,
                            OUT JUB_BYTE_PTR retData, INOUT JUB_ULONG_PTR pulRetDataLen,
                            IN JUB_ULONG ulMiliSecondTimeout = 1200000) = 0;

    virtual void *GetSCP11() {
        return nullptr;
    }
    virtual void *GetSCP03() {
        return nullptr;
    }
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
