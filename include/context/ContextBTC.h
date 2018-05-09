#pragma once

#ifndef __ContextBTC__
#define __ContextBTC__

#include <JUB_SDK.h>
#include <string>


namespace jub {

	class ContextBTC {
	public:
		ContextBTC(CONTEXT_CONFIG_BTC cfg,JUB_UINT16 deviceID)
		{
			_main_path = cfg.main_path;
			_forkid = cfg.forkID;
			_type = cfg.type;
			_deviceID = deviceID;
		};
		~ContextBTC() {};

	private:
		std::string _main_path;
		int _forkid;
		JUB_BTC_TRANS_TYPE _type;
		JUB_UINT16 _deviceID;
	};



}










#endif
