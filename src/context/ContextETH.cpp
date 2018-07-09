#include <context/ContextETH.h>
#include <JUB_SDK.h>
#include <utility/util.hpp>
#include <token/interface/TokenInterface.hpp>

namespace jub {

	JUB_RV ContextETH::activeSelf()
	{
		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);
		return token->selectApplet_ETH();
	}

}
