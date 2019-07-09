#include "JUB_SDK.h"

#include <set>
#include <algorithm>
#include <utility/util.h>
#include <token/interface/TokenInterface.hpp>
#include <context/ContextBTC.h>
#include <context/ContextETH.h>
#include <context/Context.h>
#include <token/JubiterBLDImpl.h>
#include <utility/Singleton.h>
#include <device/JubiterBLEDevice.hpp>
#include <string.h>
#ifdef __ANDROID__
#include <utils/logUtils.h>
#endif

static std::set<JUB_CHAR_CPTR> memPtrs;
static constexpr char const *kVersionFormat = "1.1.0.%02d%02d%02d";
static char Version[20];

inline JUB_RV _allocMem(JUB_CHAR_PTR_PTR memPtr, const std::string &strBuf) {

    *memPtr = (new char[strBuf.size() + 1]{0,});
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

/*****************************************************************************
* @function name : JUB_FreeMemory
* @in  param : memPtr
* @out param :
* @last change :
*****************************************************************************/
JUB_RV JUB_FreeMemory(IN JUB_CHAR_CPTR memPtr) {

    JUB_CHECK_NULL(memPtr);

    auto pos = memPtrs.find(memPtr);
    if (pos == memPtrs.end()) {
        return JUBR_INVALID_MEMORY_PTR;
    }

    delete[] memPtr; memPtr = nullptr;

    memPtrs.erase(pos);

    return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_ListDeviceHid
* @in  param :
* @out param : deviceIDs - device ID list
* @last change :
*****************************************************************************/
JUB_RV JUB_ListDeviceHid(OUT JUB_UINT16 deviceIDs[MAX_DEVICE]) {

#ifdef HID_MODE
    auto path_list = jub::JubiterHidDevice::EnumDevice();
	//std::cout <<"** "<< path_list.size() << std::endl;

	//deal removed key 
	auto vDeviceIDs = jub::TokenManager::GetInstance()->GetHandleList();
	for (JUB_UINT16 i = 0; i < vDeviceIDs.size(); i++) {
		auto token = (jub::JubiterBLDImpl*)jub::TokenManager::GetInstance()->GetOne(vDeviceIDs[i]);
		if (std::end(path_list) == std::find(std::begin(path_list), std::end(path_list), token->getPath())) {
			//removed key
			jub::TokenManager::GetInstance()->ClearOne(vDeviceIDs[i]);
		}
	}

	//deal inserted key
	auto isInManager = [](std::string path)-> bool {
		auto vDeviceIDs = jub::TokenManager::GetInstance()->GetHandleList();
		for (JUB_UINT16 i = 0; i < vDeviceIDs.size(); i++) {
			auto token = (jub::JubiterBLDImpl*)jub::TokenManager::GetInstance()->GetOne(vDeviceIDs[i]);
			if (   token
                && path == token->getPath()
                ) {
				return true;
			}
		}

		return false;
	};

	for (auto path : path_list) {
		if (!isInManager(path)) {
			//new inserted key
			jub::JubiterBLDImpl* token = new jub::JubiterBLDImpl(path);
			jub::TokenManager::GetInstance()->AddOne(token);
		}
	}

	auto _vDeviceIDs = jub::TokenManager::GetInstance()->GetHandleList();
	for (JUB_UINT16 i = 0 ; i < std::min((size_t)MAX_DEVICE, _vDeviceIDs.size()); i++) {
		deviceIDs[i] = _vDeviceIDs[i];
	}

#else
    return JUBR_IMPL_NOT_SUPPORT;
#endif // #ifdef HID_MODE

    return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_ConnetDeviceHid
* @in  param : deviceID - device ID
* @out param :
* @last change :
*****************************************************************************/
JUB_RV JUB_ConnetDeviceHid(IN JUB_UINT16 deviceID) {

#ifdef HID_MODE
    auto token = jub::TokenManager::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->ConnectToken());

    return JUBR_OK;
#else
    return JUBR_IMPL_NOT_SUPPORT;
#endif // #ifdef HID_MODE
}

/*****************************************************************************
* @function name : JUB_DisconnetDeviceHid
* @in  param : deviceID - device ID
* @out param :
* @last change :
*****************************************************************************/
JUB_RV JUB_DisconnetDeviceHid(IN JUB_UINT16 deviceID) {

#ifdef HID_MODE
    auto token = jub::TokenManager::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->DisconnectToken());

    return JUBR_OK;
#else
    return JUBR_IMPL_NOT_SUPPORT;
#endif // #ifdef HID_MODE
}

/*****************************************************************************
* @function name : JUB_CreateContextBTC
* @in  param : cfg
*            : deviceID - device ID
* @out param : contextID
* @last change :
*****************************************************************************/
JUB_RV JUB_CreateContextBTC(IN CONTEXT_CONFIG_BTC cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID) {

    if (nullptr == jub::TokenManager::GetInstance()->GetOne(deviceID)) {
        return JUBR_ERROR_ARGS;
    }

    jub::ContextBTC* context = new jub::ContextBTC(cfg, deviceID);
    JUB_CHECK_NULL(context);

    *contextID = jub::ContextManager::GetInstance()->AddOne(context);

	JUB_VERIFY_RV(context->ActiveSelf());

    return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_ClearContext
* @in  param : contextID - context ID
* @out param :
* @last change :
*****************************************************************************/
JUB_RV JUB_ClearContext(IN JUB_UINT16 contextID) {

    jub::ContextManager::GetInstance()->ClearOne(contextID);

    return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_BuildUSDTOutputs
* @in  param : contextID - context ID
*            : USDTTo - to address
*            : amount
* @out param : outputs
* @last change : build the return0 and dust 2 outputs
*****************************************************************************/
JUB_RV JUB_BuildUSDTOutputs(IN JUB_UINT16 contextID,
                            IN JUB_CHAR_PTR USDTTo,
                            IN JUB_UINT64 amount,
                            OUT OUTPUT_BTC outputs[2]) {

	JUB_CHECK_CONTEXT_BTC(contextID);
	jub::ContextBTC* context = (jub::ContextBTC*)jub::ContextManager::GetInstance()->GetOne(contextID);
	JUB_CHECK_NULL(context);

	JUB_VERIFY_RV(context->BuildUSDTOutputs(USDTTo, amount, outputs));

    return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_SignTransactionBTC
* @in  param : contextID - context ID
*            : inputs
*            : iCount
*            : outputs
*            : oCount
*            : lockTime
* @out param : raw
* @last change :
*****************************************************************************/
JUB_RV JUB_SignTransactionBTC(IN JUB_UINT16 contextID,
                              IN INPUT_BTC inputs[], IN JUB_UINT16 iCount,
                              IN OUTPUT_BTC outputs[], IN JUB_UINT16 oCount,
                              IN JUB_UINT32 lockTime,
                              OUT JUB_CHAR_PTR_PTR raw) {

    JUB_CHECK_CONTEXT_BTC(contextID);

    std::vector<INPUT_BTC> vInputs(inputs, inputs + iCount);
    std::vector<OUTPUT_BTC> vOutputs(outputs, outputs + oCount);

    jub::ContextBTC* context = (jub::ContextBTC*)jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

    std::string str_raw;
    JUB_VERIFY_RV(context->SignTX(vInputs, vOutputs, lockTime, str_raw));

    JUB_VERIFY_RV(_allocMem(raw, str_raw));

    return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_ShowVirtualPwd
* @in  param : contextID - context ID
* @out param :
* @last change :
*****************************************************************************/
JUB_RV JUB_ShowVirtualPwd(IN JUB_UINT16 contextID) {

    auto context = jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->ShowVirtualPwd());

    return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_CancelVirtualPwd
* @in  param : contextID - context ID
* @out param :
* @last change :
*****************************************************************************/
JUB_RV JUB_CancelVirtualPwd(IN JUB_UINT16 contextID) {

	auto context = jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->CancelVirtualPwd());

    return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_VerifyPIN
* @in  param : contextID - context ID
*            : pinMix
* @out param : retry
* @last change :
*****************************************************************************/
JUB_RV JUB_VerifyPIN(IN JUB_UINT16 contextID,
                     IN JUB_CHAR_PTR pinMix,
                     OUT JUB_ULONG_PTR pretry) {

    auto context = jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->VerifyPIN(pinMix, *pretry));

    return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_GetHDNodeBTC
* @in  param : contextID - context ID
*            : path
* @out param : xpub
* @last change :
*****************************************************************************/
JUB_RV JUB_GetHDNodeBTC(IN JUB_UINT16 contextID,
                        IN BIP32_Path path,
                        OUT JUB_CHAR_PTR_PTR xpub) {

	JUB_CHECK_CONTEXT_BTC(contextID);

    auto context = (jub::ContextBTC*)jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

    std::string str_xpub;
    JUB_VERIFY_RV(context->GetHDNode(path, str_xpub));
    JUB_VERIFY_RV(_allocMem(xpub, str_xpub));

    return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_GetMainHDNodeBTC
* @in  param : contextID - context ID
* @out param : xpub
* @last change :
*****************************************************************************/
JUB_RV JUB_GetMainHDNodeBTC(IN JUB_UINT16 contextID,
                            OUT JUB_CHAR_PTR_PTR xpub) {

	JUB_CHECK_CONTEXT_BTC(contextID);

	auto context = (jub::ContextBTC*)jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

	std::string str_xpub;
	JUB_VERIFY_RV(context->GetMainHDNode(str_xpub));
	JUB_VERIFY_RV(_allocMem(xpub, str_xpub));

	return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_SetTimeOut
* @in  param : contextID - context ID
*            : timeout - how many s
* @out param :
* @last change :
*****************************************************************************/
JUB_RV JUB_SetTimeOut(IN JUB_UINT16 contextID,
                      IN JUB_UINT16 timeout) {

	auto context = (jub::ContextBTC*)jub::ContextManager::GetInstance()->GetOne(contextID);
	JUB_CHECK_NULL(context);

	if (600 < timeout) {
		return JUBR_ERROR_ARGS;
	}

	JUB_VERIFY_RV(context->SetTimeout(timeout * 2));

    return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_QueryBattery
* @in  param : deviceID - device ID
* @out param : percent
* @last change :
*****************************************************************************/
JUB_RV JUB_QueryBattery(IN JUB_UINT16 deviceID,
                        OUT JUB_BYTE_PTR percent) {

#ifdef BLE_MODE
	auto token = jub::TokenManager::GetInstance()->GetOne(deviceID);
	JUB_CHECK_NULL(token);

	JUB_VERIFY_RV(token->QueryBattery(*percent));

	return JUBR_OK;

#else
	return JUBR_IMPL_NOT_SUPPORT;
#endif
}

/*****************************************************************************
* @function name : JUB_SetUnitBTC
* @in  param : contextID - context ID
*            : unit
* @out param :
* @last change :
*****************************************************************************/
JUB_RV JUB_SetUnitBTC(IN JUB_UINT16 contextID,
                      IN JUB_BTC_UNIT_TYPE unit) {

	JUB_CHECK_CONTEXT_BTC(contextID);

	auto context = (jub::ContextBTC*)jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

	JUB_VERIFY_RV(context->SetUnit(unit));

    return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_GetAddressBTC
* @in  param : contextID - context ID
*            : path
*            : bShow
* @out param : address
* @last change :
*****************************************************************************/
JUB_RV JUB_GetAddressBTC(IN JUB_UINT16 contextID,
                         IN BIP32_Path path,
                         IN JUB_ENUM_BOOL bshow,
                         OUT JUB_CHAR_PTR_PTR address) {

	JUB_CHECK_CONTEXT_BTC(contextID);

    auto context = (jub::ContextBTC*)jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

    std::string str_address;
    JUB_VERIFY_RV(context->GetAddress(path, bshow, str_address));
    JUB_VERIFY_RV(_allocMem(address, str_address));

    return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_SetMyAddressBTC
* @in  param : contextID - context ID
*            : path
* @out param : address
* @last change :
*****************************************************************************/
JUB_RV JUB_SetMyAddressBTC(IN JUB_UINT16 contextID,
                           IN BIP32_Path path,
                           OUT JUB_CHAR_PTR_PTR address) {

	JUB_CHECK_CONTEXT_BTC(contextID);

	auto context = (jub::ContextBTC*)jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

	std::string str_address;
	JUB_VERIFY_RV(context->SetMyAddress(path, str_address));
	JUB_VERIFY_RV(_allocMem(address, str_address));

	return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_GetDeviceCert
* @in  param : deviceID - device ID
* @out param : cert - device certificate
* @last change :
*****************************************************************************/
JUB_RV JUB_GetDeviceCert(IN JUB_UINT16 deviceID,
                         OUT JUB_CHAR_PTR_PTR cert) {

	auto token = jub::TokenManager::GetInstance()->GetOne(deviceID);
	JUB_CHECK_NULL(token);

    // Let's go to the main security domain,
    // instead of judging the return value,
    // to get the data back
	JUB_IsBootLoader(deviceID);

	std::string str_cert;
	JUB_VERIFY_RV(token->GetDeviceCert(str_cert));
	JUB_VERIFY_RV(_allocMem(cert, str_cert));

	return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_SendOneApdu
* @in  param : deviceID - device ID
*            : apdu - one apdu
* @out param : response
* @last change :
*****************************************************************************/
JUB_RV JUB_SendOneApdu(IN JUB_UINT16 deviceID,
                       IN JUB_CHAR_PTR apdu,
                       OUT JUB_CHAR_PTR_PTR response) {

	auto token = jub::TokenManager::GetInstance()->GetOne(deviceID);
	JUB_CHECK_NULL(token);

	std::string str_response;
	JUB_VERIFY_RV(token->SendOneApdu(apdu, str_response));
	JUB_VERIFY_RV(_allocMem(response, str_response));

	return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_GetDeviceInfo
* @in  param : deviceID - device ID
* @out param : info - device info
* @last change :
*****************************************************************************/
JUB_RV JUB_GetDeviceInfo(IN JUB_UINT16 deviceID,
                         OUT JUB_DEVICE_INFO_PTR info) {

    auto token = jub::TokenManager::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(token);

    /*
    JUB_VERIFY_RV(token->getPinRetry(info.pinRetry));
    JUB_VERIFY_RV(token->getPinMaxRetry(info.pinMaxRetry));
    JUB_VERIFY_RV(token->getSN(sn));
    JUB_VERIFY_RV(token->getLabel(label));*/

    // Let's go to the main security domain,
    // instead of judging the return value,
    // to get the data back
	JUB_IsBootLoader(deviceID);

    JUB_BYTE sn[24] = {0,};
    JUB_BYTE label[32] = {0,};
    JUB_BYTE retry = 0;
    JUB_BYTE maxRetry = 0;
    JUB_BYTE bleVersion[4] = {0,};
    JUB_BYTE fwVersion[4] = {0,};

    token->GetPinRetry(retry);
    token->GetPinMaxRetry(maxRetry);
    token->GetSN(sn);
    token->GetLabel(label);
    token->GetBleVersion(bleVersion);
    token->GetFwVersion(fwVersion);

    memcpy(info->sn, sn, 24);
    memcpy(info->label, label, 32);
    info->pinRetry = retry;
    info->pinMaxRetry = maxRetry;
    memcpy(info->bleVersion, bleVersion, 4);
    memcpy(info->firmwareVersion, fwVersion, 4);

    return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_IsInitialize
* @in  param : deviceID - device ID
* @out param :
* @last change :
*****************************************************************************/
JUB_ENUM_BOOL JUB_IsInitialize(IN JUB_UINT16 deviceID) {

    auto token = jub::TokenManager::GetInstance()->GetOne(deviceID);
    if (nullptr == token) {
        return BOOL_FALSE;
    }

    return (JUB_ENUM_BOOL)token->IsInitialize();
}

/*****************************************************************************
* @function name : JUB_IsBootLoader
* @in  param : deviceID - device ID
* @out param :
* @last change :
*****************************************************************************/
JUB_ENUM_BOOL JUB_IsBootLoader(IN JUB_UINT16 deviceID) {

    auto token = jub::TokenManager::GetInstance()->GetOne(deviceID);
    if (nullptr == token) {
        return BOOL_FALSE;
    }

	jub::ContextManager::GetInstance()->ClearLast();

    return (JUB_ENUM_BOOL)token->IsBootLoader();
}

/*****************************************************************************
* @function name : JUB_EnumApplets
* @in  param : deviceID - device ID
* @out param : appList - applet list
* @last change :
*****************************************************************************/
JUB_RV JUB_EnumApplets(IN JUB_UINT16 deviceID,
                       OUT JUB_CHAR_PTR_PTR appList) {

	auto token = jub::TokenManager::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(token);

	std::string appletList;
	JUB_VERIFY_RV(token->EnumApplet(appletList));
	JUB_VERIFY_RV(_allocMem(appList, appletList));

	return JUBR_OK;
}

/*****************************************************************************
 * @function name : Jub_EnumSupportCoins
 * @in  param : deviceID - device ID
 * @out param : coinsList - coin list
 * @last change :
 *****************************************************************************/
JUB_RV Jub_EnumSupportCoins(IN JUB_UINT16 deviceID,
                            OUT JUB_CHAR_PTR_PTR coinsList) {

    auto token = jub::TokenManager::GetInstance()->GetOne(deviceID);
    if (nullptr == token) {
        return JUBR_ERROR;
    }

    std::string str_coinsList;
    JUB_VERIFY_RV(token->EnumSupportCoins(str_coinsList));
    JUB_VERIFY_RV(_allocMem(coinsList, str_coinsList));

    return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_GetAppletVersion
* @in  param : deviceID - device ID
*            : appID - applet ID
* @out param : version - applet version
* @last change :
*****************************************************************************/
JUB_RV JUB_GetAppletVersion(IN JUB_UINT16 deviceID,
                            IN JUB_CHAR_PTR appID,
                            OUT JUB_CHAR_PTR_PTR version) {

	auto token = jub::TokenManager::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(token);

	std::string str_version;
	JUB_VERIFY_RV(token->GetAppletVersion(appID,str_version));
	JUB_VERIFY_RV(_allocMem(version, str_version));

	return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_CreateContextETH
* @in  param : cfg
*            : deviceID - device ID
* @out param : contextID
* @last change :
*****************************************************************************/
JUB_RV JUB_CreateContextETH(IN CONTEXT_CONFIG_ETH cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID) {

	if (nullptr == jub::TokenManager::GetInstance()->GetOne(deviceID)) {
		return JUBR_ERROR_ARGS;
	}

	jub::ContextETH* context = new jub::ContextETH(cfg, deviceID);
	*contextID = jub::ContextManager::GetInstance()->AddOne(context);
	context->ActiveSelf();

	return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_GetAddressETH
* @in  param : contextID - context ID
*            : path
*            : bShow
* @out param : address
* @last change :
*****************************************************************************/
JUB_RV JUB_GetAddressETH(IN JUB_UINT16 contextID,
                         IN BIP32_Path	path,
                         IN JUB_ENUM_BOOL bShow,
                         OUT JUB_CHAR_PTR_PTR address) {

	JUB_CHECK_CONTEXT_ETH(contextID);

	auto context = (jub::ContextETH*)jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

	std::string str_address;
	JUB_VERIFY_RV(context->GetAddress(path, bShow, str_address));
	JUB_VERIFY_RV(_allocMem(address, str_address));

	return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_SetMyAddressETH
* @in  param : contextID - context ID
*            : path
* @out param : address
* @last change :
*****************************************************************************/
JUB_RV JUB_SetMyAddressETH(IN JUB_UINT16 contextID,
                           IN BIP32_Path path,
                           OUT JUB_CHAR_PTR_PTR address) {

	JUB_CHECK_CONTEXT_ETH(contextID);

	auto context = (jub::ContextETH*)jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

	std::string str_address;
	JUB_VERIFY_RV(context->SetMyAddress(path, str_address));
	JUB_VERIFY_RV(_allocMem(address, str_address));

	return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_GetHDNodeETH
* @in  param : contextID - context ID
*            : format - JUB_ETH_PUB_FORMAT::HEX (0x00) for hex;
*                       JUB_ETH_PUB_FORMAT::XPUB(0x01) for xpub
*            : path
* @out param : pubkey
* @last change :
*****************************************************************************/
JUB_RV JUB_GetHDNodeETH(IN JUB_UINT16 contextID,
                        IN JUB_ETH_PUB_FORMAT format,
                        IN BIP32_Path path,
                        OUT JUB_CHAR_PTR_PTR pubkey) {

	JUB_CHECK_CONTEXT_ETH(contextID);

	auto context = (jub::ContextETH*)jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

	std::string str_pubkey;
	JUB_VERIFY_RV(context->GetHDNode(format,path, str_pubkey));
	JUB_VERIFY_RV(_allocMem(pubkey, str_pubkey));

	return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_GetMainHDNodeETH
* @in  param : contextID - context ID
*            : format - JUB_ETH_PUB_FORMAT::HEX (0x00) for hex;
*                       JUB_ETH_PUB_FORMAT::XPUB(0x01) for xpub
* @out param : xpub
* @last change :
*****************************************************************************/
JUB_RV JUB_GetMainHDNodeETH(IN JUB_UINT16 contextID,
                            IN JUB_ETH_PUB_FORMAT format,
                            OUT JUB_CHAR_PTR_PTR xpub) {

	JUB_CHECK_CONTEXT_ETH(contextID);

	auto context = (jub::ContextETH*)jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

	std::string str_xpub;
	JUB_VERIFY_RV(context->GetMainHDNode(format, str_xpub));
	JUB_VERIFY_RV(_allocMem(xpub, str_xpub));

	return JUBR_OK;
}



JUB_RV JUB_SignTransactionETH(IN JUB_UINT16 contextID,
                              IN BIP32_Path path,
                              IN JUB_UINT32 nonce,
                              IN JUB_UINT32 gasLimit,
                              IN JUB_CHAR_PTR gasPriceInWei,
                              IN JUB_CHAR_PTR to,
                              IN JUB_CHAR_PTR valueInWei,
                              IN JUB_CHAR_PTR input,
                              OUT JUB_CHAR_PTR_PTR raw) {

	JUB_CHECK_CONTEXT_ETH(contextID);

	auto context = (jub::ContextETH*)jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

	std::string str_raw;
	JUB_VERIFY_RV(context->SignTransaction(path,
                                           nonce,
                                           gasLimit,
                                           gasPriceInWei,
                                           to,
                                           valueInWei,
                                           input,
                                           str_raw));
	JUB_VERIFY_RV(_allocMem(raw, str_raw));

	return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_BuildERC20AbiETH
* @in  param : contextID - context ID
*            : tokenTo
*            : tokenValue
* @out param : abi
* @last change :
*****************************************************************************/
JUB_RV JUB_BuildERC20AbiETH(IN JUB_UINT16 contextID,
                            IN JUB_CHAR_PTR tokenTo,
                            IN JUB_CHAR_PTR tokenValue,
                            OUT JUB_CHAR_PTR_PTR abi) {

	JUB_CHECK_CONTEXT_ETH(contextID);

	auto context = (jub::ContextETH*)jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

	std::string strAbi;
	JUB_VERIFY_RV(context->BuildERC20Abi(tokenTo, tokenValue, strAbi));
	JUB_VERIFY_RV(_allocMem(abi, strAbi));

	return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_GetVersion
* @in  param :
* @out param :
* @last change :
*****************************************************************************/
JUB_CHAR_PTR JUB_GetVersion(void) {

	std::string monthes[] = {
		"Jan",
		"Feb",
		"Mar",
		"Apr",
		"May",
		"Jun",
		"Jul",
		"Aug",
		"Sep",
		"Oct",
		"Nov",
		"Dec",
	};

	std::string dateStr = __DATE__;

	int year = atoi(dateStr.substr(dateStr.length() - 2).c_str());
	int month = 0;
	for (int i = 0; i < 12; i++) {
		if (std::string::npos != dateStr.find(monthes[i])) {
			month = i + 1;
			break;
		}
	}

	std::string dayStr = dateStr.substr(4, 2);
	int day = atoi(dayStr.c_str());

	sprintf(Version, kVersionFormat, year, month, day);

	return Version;
}

/// ble device APIs //////////////////////////////////////////
// only works in ble (android and ios)

//#if defined(ANDROID) || defined(TARGET_OS_IPHONE)

using BLE_device_map = Singleton<xManager<JUB_ULONG>>;

JUB_RV JUB_initDevice(IN DEVICE_INIT_PARAM param) {

#ifdef BLE_MODE
    auto bleDevice = Singleton<jub::JubiterBLEDevice>::GetInstance();
    if (!bleDevice) {
        return JUBR_ERROR;
    }

    JUB_VERIFY_RV(bleDevice->Initialize({param.param,
                                         param.callBack,
                                         param.scanCallBack,
                                         param.discCallBack})
    );

    return JUBR_OK;
#else
    return JUBR_IMPL_NOT_SUPPORT;
#endif // #ifdef BLE_MODE
}

JUB_RV JUB_enumDevices(void) {

#ifdef BLE_MODE
    auto bleDevice = Singleton<jub::JubiterBLEDevice>::GetInstance();
    if (!bleDevice) {
        return JUBR_ERROR;
    }

    JUB_VERIFY_RV(bleDevice->Scan());

    return JUBR_OK;
#else
    return JUBR_IMPL_NOT_SUPPORT;
#endif // #ifdef BLE_MODE
}

JUB_RV JUB_stopEnumDevices(void) {

#ifdef BLE_MODE
    auto bleDevice = Singleton<jub::JubiterBLEDevice>::GetInstance();
    if (!bleDevice) {
        return JUBR_ERROR;
    }

    JUB_VERIFY_RV(bleDevice->StopScan());

    return JUBR_OK;
#else
    return JUBR_IMPL_NOT_SUPPORT;
#endif // #ifdef BLE_MODE
}

JUB_RV JUB_connectDevice(JUB_BYTE_PTR bBLEUUID,
                         JUB_UINT32 connectType,
                         JUB_UINT16* pDeviceID,
                         JUB_UINT32 timeout) {

#ifdef BLE_MODE
    auto bleDevice = Singleton<jub::JubiterBLEDevice>::GetInstance();
    if (!bleDevice) {
        return JUBR_ERROR;
    }

    JUB_ULONG * pdevHandle = new JUB_ULONG;
    JUB_RV rv = bleDevice->Connect(bBLEUUID, connectType, pdevHandle, timeout);
//    LOG_INF("JUB_connectDevice rv: %lu", *pdevHandle);
    JUB_VERIFY_RV(rv);

    *pDeviceID = BLE_device_map::GetInstance()->AddOne(pdevHandle);
//    LOG_INF("JUB_connectDevice rv: %hu", *pDeviceID);
    jub::JubiterBLDImpl* token = new jub::JubiterBLDImpl(bleDevice);
    jub::TokenManager::GetInstance()->AddOne(*pDeviceID, token);

    return rv;
#else
    return JUBR_IMPL_NOT_SUPPORT;
#endif // #ifdef BLE_MODE
}

JUB_RV JUB_cancelConnect(JUB_BYTE_PTR bBLEUUID) {

#ifdef BLE_MODE
    auto bleDevice = Singleton<jub::JubiterBLEDevice>::GetInstance();
    if (!bleDevice) {
        return JUBR_ERROR;
    }

    JUB_VERIFY_RV(bleDevice->CancelConnect(bBLEUUID));

    return JUBR_OK;
#else
    return JUBR_IMPL_NOT_SUPPORT;
#endif // #ifdef BLE_MODE
}

JUB_RV JUB_disconnectDevice(JUB_UINT16 deviceID) {

#ifdef BLE_MODE
    auto bleDevice = Singleton<jub::JubiterBLEDevice>::GetInstance();
    if (!bleDevice) {
        return JUBR_ERROR;
    }

    JUB_ULONG *devHandle = BLE_device_map::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(devHandle);
    JUB_VERIFY_RV(bleDevice->Disconnect(*devHandle));

    return JUBR_OK;
#else
    return JUBR_IMPL_NOT_SUPPORT;
#endif // #ifdef BLE_MODE
}

JUB_RV JUB_isDeviceConnect(JUB_UINT16 deviceID) {

#ifdef BLE_MODE
    auto bleDevice = Singleton<jub::JubiterBLEDevice>::GetInstance();
    if (!bleDevice) {
        return JUBR_ERROR;
    }
    JUB_ULONG *devHandle = BLE_device_map::GetInstance()->GetOne(deviceID);
    if (NULL == devHandle) {
        return JUBR_CONNECT_DEVICE_ERROR;
    }

    JUB_VERIFY_RV(bleDevice->IsConnect(*devHandle));

    return JUBR_OK;
#else
    return JUBR_IMPL_NOT_SUPPORT;
#endif // #ifdef BLE_MODE
}
//#endif // #if defined(ANDROID) || defined(TARGET_OS_IPHONE)
