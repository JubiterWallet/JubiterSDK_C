#include "JUB_SDK.h"
#include <utility/Singleton.h>
#include <token/TokenManager.hpp>
#include <context/ContextManager.hpp>
#include <context/ContextBTC.h>
/*****************************************************************************
* @function name : Jub_ListDeviceHid
* @in param : 
* @out param : 
* @last change : 
*****************************************************************************/
JUB_RV Jub_ListDeviceHid(OUT JUB_UINT16 deviceIDs[MAX_DEVICE])
{
	Singleton<jub::TokenManager>::GetInstance()->enumToken();
	Singleton<jub::TokenManager>::GetInstance()->getHandleList(deviceIDs);

	return JUBR_OK;
}

JUB_RV Jub_ConnetDeviceHid(IN JUB_UINT16 deviceID)
{
	auto token = Singleton<jub::TokenManager>::GetInstance()->getToken(deviceID);
	if (nullptr != token)
	{
		return token->connectToken();
	}
	return JUBR_ERROR;
}

JUB_RV Jub_DisconnetDeviceHid(IN JUB_UINT16 deviceID)
{
	auto token = Singleton<jub::TokenManager>::GetInstance()->getToken(deviceID);
	if (nullptr != token)
	{
		return token->disconnectToken();
	}
	return JUBR_ERROR;

}


JUB_RV Jub_CreateContextBTC(IN CONTEXT_CONFIG_BTC cfg, IN JUB_UINT16 deviceID, OUT JUB_UINT16* contextID)
{
	jub::ContextBTC* context = new jub::ContextBTC(cfg,deviceID);
	if (nullptr == Singleton<jub::TokenManager>::GetInstance()->getToken(deviceID))
	{
		return JUBR_ERROR;
	}
	JUB_UINT16 _contextID = Singleton<jub::ContextManager<jub::ContextBTC> >::GetInstance()->addContext(context);
	*contextID = _contextID;
	return JUBR_OK;
}


JUB_RV JUB_ClearContext(IN JUB_UINT16 contextID)
{
	Singleton<jub::ContextManager<jub::ContextBTC> >::GetInstance()->clearContext(contextID);
	return JUBR_OK;
}


JUB_RV JUB_SignTransactionBTC(IN JUB_UINT16 contextID , IN INPUT_BTC inputs[], IN JUB_UINT16 iCount, IN OUTPUT_BTC outputs[], IN JUB_UINT16 oCount, IN JUB_UINT32 locktime, OUT JUB_CHAR_PTR_PTR raw)
{
	std::vector<INPUT_BTC> vInputs(inputs, inputs + iCount);
	std::vector<OUTPUT_BTC> vOutputs(outputs, outputs + oCount);

	auto context = Singleton<jub::ContextManager<jub::ContextBTC> >::GetInstance()->getContext(contextID);
	if (context != nullptr)
	{
		std::string raw;
		auto rv = context->signTX(vInputs, vOutputs, locktime,raw);
		if (rv == JUBR_OK)
		{
			//
		}
	}


	return JUBR_ERROR;
}