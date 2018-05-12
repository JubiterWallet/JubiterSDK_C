#pragma once

#ifndef __ContextBTC__
#define __ContextBTC__

#include <JUB_SDK.h>
#include <string>
#include <vector>
#include <utility/Singleton.h>
#include <utility/xManager.hpp>

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


		virtual JUB_RV getHDNode(JUB_UINT64 index, std::string& xpub);
		virtual JUB_RV showVirtualPwd();
		virtual JUB_RV verifyPIN(JUB_CHAR_PTR pinMix, OUT JUB_ULONG &retry);
		virtual JUB_RV signTX(std::vector<INPUT_BTC> inputs, std::vector<OUTPUT_BTC> outputs, JUB_UINT32 locktime , std::string& raw);

	private:
		std::string _main_path;
		int _forkid;
		JUB_BTC_TRANS_TYPE _type;
		JUB_UINT16 _deviceID;
	};


	using ContextManager_BTC = Singleton<xManager<jub::ContextBTC>>;


}










#endif
