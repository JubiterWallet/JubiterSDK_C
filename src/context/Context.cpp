#include <context/Context.h>
#include <utility/Singleton.h>
#include <utility/xManager.hpp>
#include <token/interface/TokenInterface.hpp>
#include <utility/util.hpp>


namespace jub {


	JUB_RV Context::showVirtualPwd()
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);
		return token->showVirtualPwd();
	}

	JUB_RV Context::cancelVirtualPwd()
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);
		return token->cancelVirtualPwd();
	}

	JUB_RV Context::verifyPIN(JUB_CHAR_PTR pinMix, OUT JUB_ULONG &retry)
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);
		return token->verifyPIN(pinMix, retry);
	}

}