#include "JUB_SDK.h"


#include <set>
#include <algorithm>
#include <utility/util.hpp>
#include <token/TokenInterface.hpp>
#include <context/ContextBTC.h>
#include <token/JubiterOneImpl.h>


#ifdef _WIN32 // modify later..
#define HID_MODE
#else
#define BLE_MODE
#endif

static std::set<JUB_CHAR_CPTR> memPtrs;



inline JUB_RV _allocMem(JUB_CHAR_PTR_PTR memPtr, const std::string &strBuf) {

	*memPtr = (new char[strBuf.size() + 1]{ 0 });
	if (nullptr == *memPtr) {
		return JUBR_HOST_MEMORY;
	}

	if (0 == strBuf.size()) {
		return JUBR_OK;
	}

	memcpy(*memPtr, strBuf.data(), strBuf.size());

	if (!memPtrs.insert(*memPtr).second) {
		return JUBR_REPEAT_MEMORY_PTR;
	}

	return JUBR_OK;
}


JUB_RV JUB_FreeMemory(IN JUB_CHAR_CPTR memPtr) {
	JUB_CHECK_NULL(memPtr);

	auto pos = memPtrs.find(memPtr);
	if (memPtrs.end() == pos) {
		return JUBR_INVALID_MEMORY_PTR;
	}

	delete[] memPtr;
	memPtr = nullptr;

	memPtrs.erase(pos);
	return JUBR_OK;
}


/*****************************************************************************
* @function name : Jub_ListDeviceHid
* @in param : 
* @out param : 
* @last change : 
*****************************************************************************/

JUB_RV Jub_ListDeviceHid(OUT JUB_UINT16 deviceIDs[MAX_DEVICE])
{
#ifdef HID_MODE

	auto path_list = jub::JubiterHidDevice::enumDevice();

	for (auto path : path_list)
	{
		jub::JubiterOneImpl* token = new jub::JubiterOneImpl(path);
		jub::TokenManager::GetInstance()->addOne(token);
	}

	auto vDeviceIDs = jub::TokenManager::GetInstance()->getHandleList();
	for (JUB_UINT16 i=0 ; i < std::min((size_t)MAX_DEVICE, vDeviceIDs.size()); i++)
	{
		deviceIDs[i] = vDeviceIDs[i];
	}
#else
	return JUBR_IMPL_NOT_SUPPORT;
#endif

	return JUBR_OK;
}


JUB_RV Jub_ConnetDeviceHid(IN JUB_UINT16 deviceID)
{
#ifdef HID_MODE
	auto token = jub::TokenManager::GetInstance()->getOne(deviceID);
	if (nullptr != token)
	{
		return token->connectToken();
	}
#else
	return JUBR_IMPL_NOT_SUPPORT;
#endif
	return JUBR_ERROR;
}

JUB_RV Jub_DisconnetDeviceHid(IN JUB_UINT16 deviceID)
{
#ifdef HID_MODE
	auto token = jub::TokenManager::GetInstance()->getOne(deviceID);
	if (nullptr != token)
	{
		return token->disconnectToken();
	}
#else
	return JUBR_IMPL_NOT_SUPPORT;
#endif
	return JUBR_ERROR;

}


JUB_RV Jub_CreateContextBTC(IN CONTEXT_CONFIG_BTC cfg, IN JUB_UINT16 deviceID, OUT JUB_UINT16* contextID)
{
	if (nullptr == jub::TokenManager::GetInstance()->getOne(deviceID))
	{
		return JUBR_ERROR;
	}
	jub::ContextBTC* context = new jub::ContextBTC(cfg, deviceID);
	JUB_UINT16 _contextID = jub::ContextManager_BTC::GetInstance()->addOne(context);
	*contextID = _contextID;
	return JUBR_OK;
}


JUB_RV JUB_ClearContext(IN JUB_UINT16 contextID)
{
	jub::ContextManager_BTC::GetInstance()->clearOne(contextID);
	return JUBR_OK;
}


JUB_RV JUB_SignTransactionBTC(IN JUB_UINT16 contextID , IN INPUT_BTC inputs[], IN JUB_UINT16 iCount, IN OUTPUT_BTC outputs[], IN JUB_UINT16 oCount, IN JUB_UINT32 locktime, OUT JUB_CHAR_PTR_PTR raw)
{
	std::vector<INPUT_BTC> vInputs(inputs, inputs + iCount);
	std::vector<OUTPUT_BTC> vOutputs(outputs, outputs + oCount);

	auto context = jub::ContextManager_BTC::GetInstance()->getOne(contextID);
	if (context != nullptr)
	{
		std::string str_raw;
		auto rv = context->signTX(vInputs, vOutputs, locktime, str_raw);
		if (rv == JUBR_OK)
		{
			JUB_VERIFY_RV(_allocMem(raw, str_raw));
			return JUBR_OK;
		}
		return rv;
	}


	return JUBR_ERROR;
}


JUB_RV JUB_ShowVirtualPwd(IN JUB_UINT16 contextID)
{
	auto context = jub::ContextManager_BTC::GetInstance()->getOne(contextID);
	if (context != nullptr)
	{
		return context->showVirtualPwd();
	}

	return JUBR_ERROR;
}


JUB_RV JUB_VerifyPIN(IN JUB_UINT16 contextID, IN JUB_CHAR_PTR pinMix, OUT JUB_ULONG &retry)
{
	auto context = jub::ContextManager_BTC::GetInstance()->getOne(contextID);
	if (context != nullptr)
	{
		return context->verifyPIN(pinMix, retry);
	}

	return JUBR_ERROR;
}


JUB_RV JUB_GetHDNodeBTC(IN JUB_UINT16 contextID, JUB_UINT64	nodeIndex, OUT JUB_CHAR_PTR_PTR xpub)
{
	auto context = jub::ContextManager_BTC::GetInstance()->getOne(contextID);
	JUB_CHECK_NULL(context);
	std::string str_xpub;
	JUB_VERIFY_RV(context->getHDNode(nodeIndex, str_xpub));
	JUB_VERIFY_RV(_allocMem(xpub, str_xpub));
	return JUBR_OK;
	
}


