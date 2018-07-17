#pragma once
#ifndef __ContextETH__
#define __ContextETH__

#include <JUB_SDK.h>
#include <string>
#include <vector>
#include <utility/Singleton.h>
#include <utility/xManager.hpp>
#include <context/Context.h>


#define JUB_CHECK_CONTEXT_ETH(x)	do{									\
auto context = jub::ContextManager::GetInstance()->getOne(x);			\
JUB_CHECK_NULL(context);												\
if (typeid(*context->GetClassType()) != typeid(jub::ContextETH))		\
{	return JUBR_ERROR_ARGS;            }}while(0)


namespace jub {

	class ContextETH :public Context {
	public:
		ContextETH(CONTEXT_CONFIG_ETH cfg, JUB_UINT16 deviceID)
		{
			_main_path = cfg.main_path;
			_chainID = cfg.chainID;
			_deviceID = deviceID;

		};
		~ContextETH() {};


		virtual JUB_RV getAddress(BIP32_Path path, JUB_UINT16 tag, std::string& address);
		virtual JUB_RV getHDNode(JUB_BYTE format,BIP32_Path path, std::string& pubkey);

		virtual JUB_RV signTransaction(IN BIP32_Path path, IN JUB_UINT32 nonce,
			IN JUB_UINT32 gasLimit,
			IN JUB_CHAR_PTR gasPriceInWei,
			IN JUB_CHAR_PTR to,
			IN JUB_CHAR_PTR valueInWei,
			IN JUB_CHAR_PTR input,
			OUT std::string& raw
		);

		virtual JUB_RV buildERC20Abi(JUB_CHAR_PTR to, JUB_CHAR_PTR value,std::string& abi);
		virtual ContextETH* GetClassType(void) { return this; }
		virtual JUB_RV activeSelf();

		 

	private:

		int _chainID;
	};

}












#endif