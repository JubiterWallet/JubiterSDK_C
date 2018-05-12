#include "JUB_SDK.h"
#include <utility/Singleton.h>
#include <token/TokenManager.hpp>
#include <context/ContextManager.hpp>
#include <context/ContextBTC.h>
#include <set>
#include <utility/util.hpp>

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
	auto context = Singleton<jub::ContextManager<jub::ContextBTC> >::GetInstance()->getContext(contextID);
	if (context != nullptr)
	{
		return context->showVirtualPwd();
	}

	return JUBR_ERROR;
}


JUB_RV JUB_VerifyPIN(IN JUB_UINT16 contextID, IN JUB_CHAR_PTR pinMix, OUT JUB_ULONG &retry)
{
	auto context = Singleton<jub::ContextManager<jub::ContextBTC> >::GetInstance()->getContext(contextID);
	if (context != nullptr)
	{
		return context->verifyPIN(pinMix, retry);
	}

	return JUBR_ERROR;
}


JUB_RV JUB_GetHDNodeBTC(IN JUB_UINT16 contextID, JUB_UINT64	nodeIndex, OUT JUB_CHAR_PTR_PTR xpub)
{
	auto context = Singleton<jub::ContextManager<jub::ContextBTC> >::GetInstance()->getContext(contextID);
	JUB_CHECK_NULL(context);
	std::string str_xpub;
	JUB_VERIFY_RV(context->getHDNode(nodeIndex, str_xpub));
	JUB_VERIFY_RV(_allocMem(xpub, str_xpub));
	return JUBR_OK;
	
}