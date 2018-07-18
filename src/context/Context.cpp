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

	JUB_RV Context::setTimeout(JUB_UINT16 timeout)
	{
		_timeout = timeout;

		auto token = jub::TokenManager::GetInstance()->getOne(_deviceID);
		JUB_CHECK_NULL(token);

		JUB_VERIFY_RV(token->setTimeout(_timeout));

		return JUBR_OK;
	}

	std::string Context::full_bip32_path(BIP32_Path path)
	{
		return _main_path + "/" + jub::to_string(path.change) + "/" + jub::to_string(path.addressIndex);
	}

}