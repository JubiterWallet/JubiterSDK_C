#pragma once

#ifndef __ContextBTC__
#define __ContextBTC__

#include <JUB_SDK.h>
#include <string>
#include <vector>
#include <utility/Singleton.h>
#include <utility/xManager.hpp>
#include <context/Context.h>

#define JUB_CHECK_CONTEXT_BTC(x)	do{									\
auto context = jub::ContextManager::GetInstance()->getOne(x);			\
JUB_CHECK_NULL(context);												\
if (typeid(*context->GetClassType()) != typeid(jub::ContextBTC))		\
{	return JUBR_ERROR_ARGS;            }}while(0)

namespace jub {

	class ContextBTC :public Context{
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
		virtual JUB_RV getMainHDNode(std::string& xpub);

		virtual JUB_RV getAddress(BIP32_Path path, JUB_UINT16 tag, std::string& address);
		virtual JUB_RV setMyAddress(BIP32_Path path, std::string& address);
		virtual JUB_RV signTX(std::vector<INPUT_BTC> inputs, std::vector<OUTPUT_BTC> outputs, JUB_UINT32 locktime , std::string& raw);
		virtual JUB_RV setUnit(JUB_BTC_UNIT_TYPE unit_type);
		virtual JUB_RV setTimeout(JUB_UINT16 timeout);



		virtual ContextBTC* GetClassType(void) { return this; }
		virtual JUB_RV activeSelf();

	private:

		int _forkid;
		JUB_BTC_TRANS_TYPE _type;
		JUB_BTC_UNIT_TYPE _unit_type;
		JUB_UINT16 _timeout;
	};

}










#endif
