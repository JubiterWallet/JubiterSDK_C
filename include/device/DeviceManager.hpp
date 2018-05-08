
#ifndef __DeviceManager__
#define __DeviceManager__

#include <memory>
#include <mutex>

//#include <device/JubiterBLEDevice.hpp>
#include <device/JubiterHidDevice.hpp>

namespace jub {

class DeviceManager
{
public:
    DeviceManager() = delete;
    ~DeviceManager() = delete;

#ifdef _WIN32 // modify later..
	using device_type = JubiterHidDevice;
#else
	using device_type = JubiterBLEDevice;
#endif

public:
        /* functions */
	// we only support one device at the same time currently.
    static std::shared_ptr<DeviceTypeBase> getDevice() {
        if (!g_device) {
            std::lock_guard<std::mutex> lock(_mutex);
            if (!g_device) {
				g_device = std::make_shared<device_type>();
            }
        }
        return g_device;
    }

private:
    /* data */
	static std::mutex _mutex;
    static std::shared_ptr<DeviceTypeBase> g_device;

};

}

#define IS_BLE_MODE (std::is_same<jub::DeviceManager::device_type,  \
                                  jub::JubiterBLEDevice>::value)

#endif // __DeviceManager__
