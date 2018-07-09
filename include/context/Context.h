#pragma once
#ifndef __Context__
#define __Context__


#include <JUB_SDK.h>
#include <string>
#include <vector>
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

		virtual Context* GetClassType(void){return this;}

	protected:
		JUB_UINT16 _deviceID;

	};


	using ContextManager = Singleton<xManager<jub::Context>>;

}




#endif
