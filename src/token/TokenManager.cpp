#include <token/TokenManager.hpp>
#include <hidapi/hidapi/hidapi.h>
#include <device/JubiterHidDevice.hpp>
#include <token/HardwareTokenImpl.hpp>

namespace jub {

	bool TokenManager::enumToken() {

#ifdef WIN32
		return _enumTokenHid();
#endif
	}

	TokenManager::~TokenManager() { 
#ifdef WIN32
		hid_exit();
#endif
		for (auto token : m_token_list)
		{
			delete token;
		}
	};

	bool TokenManager::_enumTokenHid() {
		if (0 != hid_init())
		{
			return false;
		}
	
		auto hid_dev = hid_enumerate(0, 0);
		auto hid_dev_head = hid_dev;
		while (hid_dev)
		{
			HardwareTokenImpl* token = new HardwareTokenImpl(hid_dev->path);

			m_token_list.push_back(token);
			hid_dev = hid_dev->next;
		}

		hid_free_enumeration(hid_dev_head);
		return true;
	}

}