#include <context/ContextBTC.h>
#include <token/TokenManager.hpp>
#include <JUB_SDK.h>
#include <utility/Singleton.h>


namespace jub {


	JUB_RV ContextBTC::getHDNode(int index, std::string& xpub)
	{
		auto token = Singleton<jub::TokenManager>::GetInstance()->getToken(_deviceID);
		return JUBR_OK;
	}


}