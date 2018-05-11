#pragma once

#ifndef __ContextBTC__
#define __ContextBTC__

#include <JUB_SDK.h>
#include <string>
#include <vector>

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


		JUB_RV getHDNode(int index, std::string& xpub);
		JUB_RV showVirtualPwd();
		JUB_RV verifyPIN(JUB_CHAR_PTR pinMix, OUT JUB_ULONG &retry);
		JUB_RV signTX(std::vector<INPUT_BTC> inputs, std::vector<OUTPUT_BTC> outputs, JUB_UINT32 locktime , std::string& raw);

	private:
		std::string _main_path;
		int _forkid;
		JUB_BTC_TRANS_TYPE _type;
		JUB_UINT16 _deviceID;
	};








}










#endif
