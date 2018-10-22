#pragma once
#ifndef __Context__
#define __Context__


#include <JUB_SDK.h>
#include <string>
#include <vector>
#include <iostream>
#include <utility/Singleton.h>
#include <utility/xManager.hpp>

namespace jub {


	class Context {
	public:
		Context(JUB_UINT16 deviceID)
		{
			_deviceID = deviceID;
		};
		Context() {};
		~Context() {};

		virtual JUB_RV showVirtualPwd();
		virtual JUB_RV cancelVirtualPwd();
		virtual JUB_RV verifyPIN(JUB_CHAR_PTR pinMix, OUT JUB_ULONG &retry);
		virtual JUB_RV activeSelf() = 0;
		virtual JUB_RV setTimeout(JUB_UINT16 timeout);

		virtual Context* GetClassType(void){return this;}

	protected:
		JUB_UINT16 _deviceID;
		std::string _main_path;
		JUB_UINT16 _timeout;

		std::string full_bip32_path(BIP32_Path path);

	};

	class AutoContextManager :public xManager<jub::Context>
	{
	public:
		jub::Context* getOne(JUB_UINT16 ID)
		{
			auto it = _list.find(ID);
			if (it != _list.end())
			{
				if (_last != it->second)
				{
					it->second->activeSelf();
				}
				_last = it->second;
				return it->second;
			}
			return nullptr;
		}
	};


	using ContextManager = Singleton<AutoContextManager>;
}




#endif
