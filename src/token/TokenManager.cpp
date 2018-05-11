#include <token/TokenManager.hpp>
#include <hidapi/hidapi/hidapi.h>
#include <device/JubiterHidDevice.hpp>
#include <token/HardwareTokenImpl.hpp>

namespace jub {

	TokenManager::TokenManager()
	{

	}
	TokenManager::~TokenManager() { 
#ifdef WIN32
		hid_exit();
#endif
		clearToken();
	};


	bool TokenManager::enumToken() {

#ifdef WIN32
		return _enumTokenHid();
#endif
	}

	bool TokenManager::_enumTokenHid() {
		clearToken();
		if (0 != hid_init())
		{
			return false;
		}
	
		auto hid_dev = hid_enumerate(VID, PID);
		auto hid_dev_head = hid_dev;
		while (hid_dev)
		{
			HardwareTokenImpl* token = new HardwareTokenImpl(hid_dev->path);
			JUB_UINT16 handle = rand();
			m_token_list.insert(std::make_pair(handle, token));
			hid_dev = hid_dev->next;
		}

		hid_free_enumeration(hid_dev_head);
		return true;
	}


	void TokenManager::clearToken()
	{
		for (auto it : m_token_list)
		{
			delete it.second;
		}
	}


	void TokenManager::getHandleList(JUB_UINT16 hlist[MAX_DEVICE]) {
		int index = 0;
		for (auto it : m_token_list)
		{
			hlist[index] = it.first;
			index++;
		}
	}
	TokenInterface* TokenManager::getToken(JUB_UINT16 index) {
		auto it = m_token_list.find(index);
		if (it != m_token_list.end())
		{
			return it->second;
		}
		return nullptr;
	}

}