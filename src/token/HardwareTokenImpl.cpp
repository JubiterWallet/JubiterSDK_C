#include <JUB_SDK.h>
#include <token/TokenInterface.hpp>
#include <token/HardwareTokenImpl.hpp>


namespace jub {

	HardwareTokenImpl::HardwareTokenImpl(std::string path) 
		:_apduBuiler(std::make_shared<JubApudBuiler>()),_device(std::make_shared<device_type>()),_path(path){

	};
	HardwareTokenImpl::~HardwareTokenImpl() {};

	JUB_RV HardwareTokenImpl::connectToken()
	{
		return _device->connect(_path);
	}


	JUB_RV HardwareTokenImpl::disconnectToken()
	{
		return _device->disconnect();
	}


}  // namespace jub
