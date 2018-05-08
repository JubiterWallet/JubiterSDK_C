#include "JUB_SDK.h"
#include <utility/Singleton.h>
#include <token/TokenManager.hpp>
/*****************************************************************************
* @function name : Jub_ListDeviceHid
* @in param : 
* @out param : 
* @last change : 
*****************************************************************************/
JUB_RV Jub_ListDeviceHid(OUT JUB_UINT16 deviceIDs[MAX_DEVICE])
{
	Singleton<jub::TokenManager>::GetInstance()->enumToken();
	int deviceCount = Singleton<jub::TokenManager>::GetInstance()->getTokenCount();
	if (deviceCount > MAX_DEVICE)
	{
		return JUBR_ERROR;
	}
	for (JUB_UINT16 i = 0; i < deviceCount; i++)
	{
		deviceIDs[i] = i;
	}

	deviceIDs[deviceCount] = 0xffff;

	return JUBR_OK;
}

JUB_RV Jub_ConnetDeviceHid(IN JUB_UINT16 index)
{
	auto token = Singleton<jub::TokenManager>::GetInstance()->getToken(index);
	if (nullptr != token)
	{
		return token->connectToken();
	}
	return JUBR_ERROR;
}

JUB_RV Jub_DisconnetDeviceHid(IN JUB_UINT16 index)
{
	auto token = Singleton<jub::TokenManager>::GetInstance()->getToken(index);
	if (nullptr != token)
	{
		return token->disconnectToken();
	}
	return JUBR_ERROR;

}