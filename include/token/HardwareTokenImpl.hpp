
#ifndef __HardwareTokenImpl__
#define __HardwareTokenImpl__

#include <JUB_SDK.h>
#include <token/TokenInterface.hpp>
#include <device/ApduBuilder.hpp>
#include <device/JubiterHidDevice.hpp>
#include <memory>

namespace jub {

class HardwareTokenImpl : public TokenInterface {
   public:
    HardwareTokenImpl(std::string path);
    ~HardwareTokenImpl();

#ifdef _WIN32 // modify later..
	using device_type = JubiterHidDevice;
#else
	using device_type = JubiterBLEDevice;
#endif

   public:
    /* functions */
    virtual JUB_RV connectToken();
	virtual JUB_RV disconnectToken();

	private:

    std::shared_ptr<ApduBuilder> _apduBuiler;
    std::shared_ptr<device_type> _device;
	std::string _path;

};
}  // namespace jub
#endif  // __HardwareTokenImpl__
