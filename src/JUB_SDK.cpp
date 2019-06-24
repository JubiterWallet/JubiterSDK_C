#include "JUB_SDK.h"

#include <set>
#include <algorithm>
#include <utility/util.hpp>
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
static constexpr char const *Version_Format = "1.1.0.%02d%02d%02d";
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
    auto path_list = jub::JubiterHidDevice::enumDevice();
	//std::cout <<"** "<< path_list.size() << std::endl;

	//deal removed key 
	auto vDeviceIDs = jub::TokenManager::GetInstance()->getHandleList();
	for (JUB_UINT16 i = 0; i < vDeviceIDs.size(); i++) {
		auto token = (jub::JubiterBLDImpl*)jub::TokenManager::GetInstance()->getOne(vDeviceIDs[i]);
		if (std::end(path_list) == std::find(std::begin(path_list), std::end(path_list), token->getPath())) {
			//removed key
			jub::TokenManager::GetInstance()->clearOne(vDeviceIDs[i]);
		}
	}

	//deal inserted key
	auto isInManager = [](std::string path)-> bool {
		auto vDeviceIDs = jub::TokenManager::GetInstance()->getHandleList();
		for (JUB_UINT16 i = 0; i < vDeviceIDs.size(); i++) {
			auto token = (jub::JubiterBLDImpl*)jub::TokenManager::GetInstance()->getOne(vDeviceIDs[i]);
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
			jub::TokenManager::GetInstance()->addOne(token);
		}
	}

	auto _vDeviceIDs = jub::TokenManager::GetInstance()->getHandleList();
	for (JUB_UINT16 i=0 ; i < std::min((size_t)MAX_DEVICE, _vDeviceIDs.size()); i++) {
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
    auto token = jub::TokenManager::GetInstance()->getOne(deviceID);
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->connectToken());

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
    auto token = jub::TokenManager::GetInstance()->getOne(deviceID);
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->disconnectToken());

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

    if (nullptr == jub::TokenManager::GetInstance()->getOne(deviceID)) {
        return JUBR_ERROR_ARGS;
    }

    jub::ContextBTC* context = new jub::ContextBTC(cfg, deviceID);
    JUB_CHECK_NULL(context);

    JUB_UINT16 _contextID = jub::ContextManager::GetInstance()->addOne(context);
    *contextID = _contextID;

	JUB_VERIFY_RV(context->activeSelf());

    return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_ClearContext
* @in  param : contextID - context ID
* @out param :
* @last change :
*****************************************************************************/
JUB_RV JUB_ClearContext(IN JUB_UINT16 contextID) {

    jub::ContextManager::GetInstance()->clearOne(contextID);

    return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_BuildUSDTOutputs
* @in  param : contextID - context ID
*            : USDT_to - to address
*            : amount
* @out param : outputs
* @last change : build the return0 and dust 2 outputs
*****************************************************************************/
JUB_RV JUB_BuildUSDTOutputs(IN JUB_UINT16 contextID,
                            IN JUB_CHAR_PTR USDT_to,
                            IN JUB_UINT64 amount,
                            OUT OUTPUT_BTC outputs[2]) {

	JUB_CHECK_CONTEXT_BTC(contextID);
	jub::ContextBTC* context = (jub::ContextBTC*)jub::ContextManager::GetInstance()->getOne(contextID);
	JUB_CHECK_NULL(context);

	JUB_VERIFY_RV(context->buildUSDTOutputs(USDT_to, amount, outputs));

    return JUBR_ERROR;
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

    jub::ContextBTC* context = (jub::ContextBTC*)jub::ContextManager::GetInstance()->getOne(contextID);
    JUB_CHECK_NULL(context);

    if (nullptr != context) {
        std::string str_raw;
        auto rv = context->signTX(vInputs, vOutputs, lockTime, str_raw);
        if (JUBR_OK == rv) {
            JUB_VERIFY_RV(_allocMem(raw, str_raw));
            return JUBR_OK;
        }
        return rv;
    }

    return JUBR_ERROR;
}

/*****************************************************************************
* @function name : JUB_ShowVirtualPwd
* @in  param : contextID - context ID
* @out param :
* @last change :
*****************************************************************************/
JUB_RV JUB_ShowVirtualPwd(IN JUB_UINT16 contextID) {

    auto context = jub::ContextManager::GetInstance()->getOne(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->showVirtualPwd());

    return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_CancelVirtualPwd
* @in  param : contextID - context ID
* @out param :
* @last change :
*****************************************************************************/
JUB_RV JUB_CancelVirtualPwd(IN JUB_UINT16 contextID) {

	auto context = jub::ContextManager::GetInstance()->getOne(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->cancelVirtualPwd());

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
                     OUT JUB_ULONG_PTR retry) {

    auto context = jub::ContextManager::GetInstance()->getOne(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->verifyPIN(pinMix, *retry));

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

    auto context = (jub::ContextBTC*)jub::ContextManager::GetInstance()->getOne(contextID);
    JUB_CHECK_NULL(context);

    std::string str_xpub;
    JUB_VERIFY_RV(context->getHDNode(path, str_xpub));
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

	auto context = (jub::ContextBTC*)jub::ContextManager::GetInstance()->getOne(contextID);
    JUB_CHECK_NULL(context);

	std::string str_xpub;
	JUB_VERIFY_RV(context->getMainHDNode(str_xpub));
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

	auto context = (jub::ContextBTC*)jub::ContextManager::GetInstance()->getOne(contextID);
	JUB_CHECK_NULL(context);

	if (600 < timeout) {
		return JUBR_ERROR_ARGS;
	}

	JUB_VERIFY_RV(context->setTimeout(timeout * 2));

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
	auto token = jub::TokenManager::GetInstance()->getOne(deviceID);
	JUB_CHECK_NULL(token);

	JUB_VERIFY_RV(token->queryBattery(*percent));

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

	auto context = (jub::ContextBTC*)jub::ContextManager::GetInstance()->getOne(contextID);
    JUB_CHECK_NULL(context);

	JUB_VERIFY_RV(context->setUnit(unit));

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

    auto context = (jub::ContextBTC*)jub::ContextManager::GetInstance()->getOne(contextID);
    JUB_CHECK_NULL(context);

    std::string str_address;
    JUB_VERIFY_RV(context->getAddress(path, bshow, str_address));
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

	auto context = (jub::ContextBTC*)jub::ContextManager::GetInstance()->getOne(contextID);
    JUB_CHECK_NULL(context);

	std::string str_address;
	JUB_VERIFY_RV(context->setMyAddress(path, str_address));
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

	auto token = jub::TokenManager::GetInstance()->getOne(deviceID);
	JUB_CHECK_NULL(token);

    // Let's go to the main security domain,
    // instead of judging the return value,
    // to get the data back
	JUB_IsBootLoader(deviceID);

	std::string str_cert;
	JUB_VERIFY_RV(token->getDeviceCert(str_cert));
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

	auto token = jub::TokenManager::GetInstance()->getOne(deviceID);
	JUB_CHECK_NULL(token);

	std::string str_response;
	JUB_VERIFY_RV(token->sendOneApdu(apdu, str_response));
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

    auto token = jub::TokenManager::GetInstance()->getOne(deviceID);
    JUB_CHECK_NULL(token);

    /*
    JUB_VERIFY_RV(token->getPinRetry(info.pin_retry));
    JUB_VERIFY_RV(token->getPinMaxRetry(info.pin_max_retry));
    JUB_VERIFY_RV(token->getSN(sn));
    JUB_VERIFY_RV(token->getLabel(label));*/

    // Let's go to the main security domain,
    // instead of judging the return value,
    // to get the data back
	JUB_IsBootLoader(deviceID);

    JUB_BYTE sn[24] = {0,};
    JUB_BYTE label[32] = {0,};
    JUB_BYTE retry = 0;
    JUB_BYTE max_retry = 0;
    JUB_BYTE ble_version[4] = {0,};
    JUB_BYTE fw_version[4] = {0,};

    token->getPinRetry(retry);
    token->getPinMaxRetry(max_retry);
    token->getSN(sn);
    token->getLabel(label);
    token->getBleVersion(ble_version);
    token->getFwVersion(fw_version);

    memcpy(info->sn, sn, 24);
    memcpy(info->label, label, 32);
    info->pin_retry = retry;
    info->pin_max_retry = max_retry;
    memcpy(info->ble_version, ble_version, 4);
    memcpy(info->firmware_version, fw_version, 4);

    return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_IsInitialize
* @in  param : deviceID - device ID
* @out param :
* @last change :
*****************************************************************************/
JUB_ENUM_BOOL JUB_IsInitialize(IN JUB_UINT16 deviceID) {

    auto token = jub::TokenManager::GetInstance()->getOne(deviceID);
    if (nullptr == token) {
        return BOOL_FALSE;
    }

    return (JUB_ENUM_BOOL)token->isInitialize();
}

/*****************************************************************************
* @function name : JUB_IsBootLoader
* @in  param : deviceID - device ID
* @out param :
* @last change :
*****************************************************************************/
JUB_ENUM_BOOL JUB_IsBootLoader(IN JUB_UINT16 deviceID) {

    auto token = jub::TokenManager::GetInstance()->getOne(deviceID);
    if (nullptr == token) {
        return BOOL_FALSE;
    }

	jub::ContextManager::GetInstance()->clearLast();

    return (JUB_ENUM_BOOL)token->isBootLoader();
}

/*****************************************************************************
* @function name : JUB_EnumApplets
* @in  param : deviceID - device ID
* @out param : appList - applet list
* @last change :
*****************************************************************************/
JUB_RV JUB_EnumApplets(IN JUB_UINT16 deviceID,
                       OUT JUB_CHAR_PTR_PTR applist) {

	auto token = jub::TokenManager::GetInstance()->getOne(deviceID);
    JUB_CHECK_NULL(token);

	std::string str_applist;
	JUB_VERIFY_RV(token->enumApplet(str_applist));
	JUB_VERIFY_RV(_allocMem(applist, str_applist));

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

    auto token = jub::TokenManager::GetInstance()->getOne(deviceID);
    if (nullptr == token) {
        return JUBR_ERROR;
    }

    std::string str_coinsList;
    JUB_VERIFY_RV(token->enumSupportCoins(str_coinsList));
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

	auto token = jub::TokenManager::GetInstance()->getOne(deviceID);
    JUB_CHECK_NULL(token);

	std::string str_version;
	JUB_VERIFY_RV(token->getAppletVersion(appID,str_version));
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

	if (nullptr == jub::TokenManager::GetInstance()->getOne(deviceID)) {
		return JUBR_ERROR_ARGS;
	}

	jub::ContextETH* context = new jub::ContextETH(cfg, deviceID);
	JUB_UINT16 _contextID = jub::ContextManager::GetInstance()->addOne(context);
	*contextID = _contextID;
	context->activeSelf();

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

	auto context = (jub::ContextETH*)jub::ContextManager::GetInstance()->getOne(contextID);
    JUB_CHECK_NULL(context);

	std::string str_address;
	JUB_VERIFY_RV(context->getAddress(path, bShow, str_address));
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

	auto context = (jub::ContextETH*)jub::ContextManager::GetInstance()->getOne(contextID);
    JUB_CHECK_NULL(context);

	std::string str_address;
	JUB_VERIFY_RV(context->setMyAddress(path, str_address));
	JUB_VERIFY_RV(_allocMem(address, str_address));

	return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_GetHDNodeETH
* @in  param : contextID - context ID
*            : format - 0x00 for hex;
*                       0x01 for xpub
*            : path
* @out param : pubkey
* @last change :
*****************************************************************************/
JUB_RV JUB_GetHDNodeETH(IN JUB_UINT16 contextID,
                        IN JUB_ETH_PUB_FORMAT format,
                        IN BIP32_Path path,
                        OUT JUB_CHAR_PTR_PTR pubkey) {

	JUB_CHECK_CONTEXT_ETH(contextID);

	auto context = (jub::ContextETH*)jub::ContextManager::GetInstance()->getOne(contextID);
    JUB_CHECK_NULL(context);

	std::string str_pubkey;
	JUB_VERIFY_RV(context->getHDNode(format,path, str_pubkey));
	JUB_VERIFY_RV(_allocMem(pubkey, str_pubkey));

	return JUBR_OK;
}

/*****************************************************************************
* @function name : JUB_GetMainHDNodeETH
* @in  param : contextID - context ID
*            : format - 0x00 for hex;
*                       0x01 for xpub
* @out param : xpub
* @last change :
*****************************************************************************/
JUB_RV JUB_GetMainHDNodeETH(IN JUB_UINT16 contextID,
                            IN JUB_ETH_PUB_FORMAT format,
                            OUT JUB_CHAR_PTR_PTR xpub) {

	JUB_CHECK_CONTEXT_ETH(contextID);

	auto context = (jub::ContextETH*)jub::ContextManager::GetInstance()->getOne(contextID);
    JUB_CHECK_NULL(context);

	std::string str_xpub;
	JUB_VERIFY_RV(context->getMainHDNode(format, str_xpub));
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

	auto context = (jub::ContextETH*)jub::ContextManager::GetInstance()->getOne(contextID);
    JUB_CHECK_NULL(context);

	std::string str_raw;
	JUB_VERIFY_RV(context->signTransaction(path,
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
*            : token_to
*            : token_value
* @out param : abi
* @last change :
*****************************************************************************/
JUB_RV JUB_BuildERC20AbiETH(IN JUB_UINT16 contextID,
                            IN JUB_CHAR_PTR token_to,
                            IN JUB_CHAR_PTR token_value,
                            OUT JUB_CHAR_PTR_PTR abi) {

	JUB_CHECK_CONTEXT_ETH(contextID);

	auto context = (jub::ContextETH*)jub::ContextManager::GetInstance()->getOne(contextID);
    JUB_CHECK_NULL(context);

	std::string str_abi;
	JUB_VERIFY_RV(context->buildERC20Abi(token_to, token_value, str_abi));
	JUB_VERIFY_RV(_allocMem(abi, str_abi));

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

	sprintf(Version, Version_Format, year, month, day);

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

    JUB_VERIFY_RV(bleDevice->initialize({param.param,
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

    JUB_VERIFY_RV(bleDevice->scan());

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

    JUB_VERIFY_RV(bleDevice->stopScan());

    return JUBR_OK;
#else
    return JUBR_IMPL_NOT_SUPPORT;
#endif // #ifdef BLE_MODE
}

JUB_RV JUB_connectDevice(JUB_BYTE_PTR bBLEUUID,
                         JUB_UINT32 connectType,
                         JUB_UINT16* pDevice_ID,
                         JUB_UINT32 timeout) {

#ifdef BLE_MODE
    auto bleDevice = Singleton<jub::JubiterBLEDevice>::GetInstance();
    if (!bleDevice) {
        return JUBR_ERROR;
    }

    JUB_ULONG * pDevHandle = new JUB_ULONG;
    JUB_RV rv = bleDevice->connect(bBLEUUID, connectType, pDevHandle, timeout);
//    LOG_INF("JUB_connectDevice rv: %lu", *pDevHandle);
    JUB_VERIFY_RV(rv);

    *pDevice_ID = BLE_device_map::GetInstance()->addOne(pDevHandle);
//    LOG_INF("JUB_connectDevice rv: %hu", *pDevice_ID);
    jub::JubiterBLDImpl* token = new jub::JubiterBLDImpl(bleDevice);
    jub::TokenManager::GetInstance()->addOne(*pDevice_ID, token);

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

    JUB_VERIFY_RV(bleDevice->cancelConnect(bBLEUUID));

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

    JUB_ULONG *devHandle = BLE_device_map::GetInstance()->getOne(deviceID);
    JUB_CHECK_NULL(devHandle);
    JUB_VERIFY_RV(bleDevice->disconnect(*devHandle));

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
    JUB_ULONG *devHandle = BLE_device_map::GetInstance()->getOne(deviceID);
    if (NULL == devHandle) {
        return JUBR_CONNECT_DEVICE_ERROR;
    }

    JUB_VERIFY_RV(bleDevice->isConnect(*devHandle));

    return JUBR_OK;
#else
    return JUBR_IMPL_NOT_SUPPORT;
#endif // #ifdef BLE_MODE
}
//#endif // #if defined(ANDROID) || defined(TARGET_OS_IPHONE)
