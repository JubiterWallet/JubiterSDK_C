#pragma once
#ifndef __ContextETH__
#define __ContextETH__

#include <JUB_SDK.h>
#include <string>
#include <vector>
#include <utility/Singleton.h>
#include <utility/xManager.hpp>
#include <context/Context.h>




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



		virtual ContextETH* GetClassType(void) { return this; }
		virtual JUB_RV activeSelf();

	private:

		std::string _main_path;
		int _chainID;
	};

}












#endif