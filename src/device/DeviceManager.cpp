#include <device/DeviceManager.hpp>

namespace jub {

std::shared_ptr<DeviceTypeBase> DeviceManager::g_device;
std::mutex DeviceManager::_mutex;

}
