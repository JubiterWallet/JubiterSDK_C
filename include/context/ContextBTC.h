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
			_unit_type = mBTC;
		};
		~ContextBTC() {};


		virtual JUB_RV getHDNode(BIP32_Path path, std::string& xpub);
		virtual JUB_RV getAddress(BIP32_Path path, JUB_UINT16 tag, std::string& address);
		virtual JUB_RV setMyAddress(BIP32_Path path, std::string& address);
		virtual JUB_RV showVirtualPwd();
		virtual JUB_RV verifyPIN(JUB_CHAR_PTR pinMix, OUT JUB_ULONG &retry);
		virtual JUB_RV signTX(std::vector<INPUT_BTC> inputs, std::vector<OUTPUT_BTC> outputs, JUB_UINT32 locktime , std::string& raw);
		virtual JUB_RV setUnit(JUB_BTC_UNIT_TYPE unit_type);

	private:

		std::string full_bip32_path(BIP32_Path path);
		std::string _main_path;
		int _forkid;
		JUB_BTC_TRANS_TYPE _type;
		JUB_UINT16 _deviceID;
		JUB_BTC_UNIT_TYPE _unit_type;
	};


	using ContextManager_BTC = Singleton<xManager<jub::ContextBTC>>;


}










#endif
