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