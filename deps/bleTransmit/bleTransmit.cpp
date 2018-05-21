//
//  bleTransmit.m
//  bleTransmit
//
//  Copyright © 1998-2016, FEITIAN Technologies Co., Ltd. All rights reserved.
//

#include <utils/logUtils.h>
#include <bleTransmit/android/BTManager.h>
#include "bleTransmit.h"

#if defined(JSSAFE)
#define __PASTE(x, y) x##y
#define CK_DECLARE_FUNCTION(returnType, name) returnType name
#define CK_FUNCTION_INFO(name) \
    CK_DECLARE_FUNCTION(unsigned int, __PASTE(JS_, name))
#define ADDPRE(name) __PASTE(JS_, name)
#else
#define __PASTE(x, y) x##y
#define CK_DECLARE_FUNCTION(returnType, name) returnType name
#define CK_FUNCTION_INFO(name) \
    CK_DECLARE_FUNCTION(unsigned int, __PASTE(FT_, name))
#define ADDPRE(name) __PASTE(FT_, name)
#endif  // defined(JSSAFE)

#define CK_NEED_ARG_LIST

#ifndef IN
#define IN
#define COMMFRAMEWORKF_IN
#endif

#ifndef OUT
#define OUT
#define COMMFRAMEWORKF_OUT
#endif

CK_FUNCTION_INFO(BLE_Initialize)
#ifdef CK_NEED_ARG_LIST
(BLE_INIT_PARAM param)
#endif
{
    unsigned int ret = IFD_SUCCESS;

    BT_INIT_PARAM mParam;
    mParam.param = param.param;
    mParam.callBack = param.callBack;
    mParam.discCallBack = param.discCallBack;
    mParam.scanCallBack = param.scanCallBack;

    ret = FT_BTManager::GetInstance()->initialize(mParam);
    if (0 != ret)
    {
        LOG_ERR("BLE initialize failed");
    }

    return ret;
}

CK_FUNCTION_INFO(BLE_Finalize)
#ifdef CK_NEED_ARG_LIST
(void)
#endif
{
    unsigned int ret = IFD_SUCCESS;
    return ret;
}

CK_FUNCTION_INFO(BLE_Scan)
#ifdef CK_NEED_ARG_LIST
(void)
#endif
{
    unsigned int ret = IFD_SUCCESS;

    ret = FT_BTManager::GetInstance()->btStartScan(4, 60);
    if (0 != ret)
    {
        LOG_ERR("BLE Scan failed： %8x", ret);
    }

    return ret;
}

CK_FUNCTION_INFO(BLE_StopScan)
#ifdef CK_NEED_ARG_LIST
(void)
#endif
{
    unsigned int ret = IFD_SUCCESS;

    ret = FT_BTManager::GetInstance()->btStopScan();
    if (0 != ret)
    {
        LOG_ERR("BLE Stop Scan Failed");
    }

    return ret;
}

CK_FUNCTION_INFO(BLE_ConnDev)
#ifdef CK_NEED_ARG_LIST
(unsigned char* bBLEUUID,   /**< ble device UUID */
 unsigned int connectType,  /**< ble device connect type */
 unsigned long* pDevHandle, /**< output ble device connect handle */
 unsigned int timeout)
#endif
{
    unsigned int ret = IFD_SUCCESS;

    LOG_ERR("===>>> BLE_ConnDev - connectType: %d", connectType);
    ret = FT_BTManager::GetInstance()->btConnect(bBLEUUID, pDevHandle, timeout);
    if (0 != ret) {
        LOG_ERR("BLE Connect failed: %08X", ret);
    } else {
        LOG_ERR("句柄111： %ld", *pDevHandle);
    }

    return ret;
}

CK_FUNCTION_INFO(BLE_CancelConnDev)
#ifdef CK_NEED_ARG_LIST
(unsigned char* bBLEUUID /**< ble device UUID */
 )
#endif
{
    unsigned int ret = IFD_SUCCESS;

    return ret;
}

CK_FUNCTION_INFO(BLE_DisConn)
#ifdef CK_NEED_ARG_LIST
(unsigned long devHandle /**< output ble device connect handle */
 )
#endif
{
    unsigned int ret = IFD_SUCCESS;

    ret = FT_BTManager::GetInstance()->btDisconnect(devHandle);
    if (0 != ret) {
        LOG_ERR("BLE btDisconnect failed: %08x", ret);
    }

    return ret;
}

/**
 * add by fs - 20170602
 * 该接口按 bool 值处理。 0 - false; 非 0 - true;
 */
CK_FUNCTION_INFO(BLE_IsConn)
#ifdef CK_NEED_ARG_LIST
(unsigned long devHandle /**< output ble device connect handle */
 )
#endif
{
    unsigned int ret = 0;

    return ret;
}

CK_FUNCTION_INFO(BLE_SendAPDU)
#ifdef CK_NEED_ARG_LIST
(unsigned long devHandle, unsigned char* apdu, unsigned int apduLen)
#endif
{
    unsigned int ret = IFD_SUCCESS;

    BYTE recv[2048] = {0};
    ULONG recvLen = 2048;
    ret = FT_BTManager::GetInstance()->btAsyncTransmit(devHandle, apdu, apduLen);
    if (0 != ret)
    {
        LOG_ERR("btSyncTransmit failed: %08X", ret);
    }

    return ret;
}

CK_FUNCTION_INFO(BLE_SendAPDU_Sync)
#ifdef CK_NEED_ARG_LIST
(unsigned long  devHandle,
	unsigned char* apdu,
	unsigned int   apduLen,
	unsigned char* recvData,
	unsigned int* recvDataLen,
	unsigned int timeout/* = 3000*/)
#endif
{
	unsigned int ret = IFD_SUCCESS;

	return ret;
}

#undef CK_NEED_ARG_LIST

#ifdef COMMFRAMEWORKF_IN
#undef IN
#undef COMMFRAMEWORKF_IN
#endif

#ifdef COMMFRAMEWORKF_OUT
#undef OUT
#undef COMMFRAMEWORKF_OUT
#endif

#if defined(FTSAFE) || defined(JSSAFE)
#undef ADDPRE
#undef CK_CALLBACK_FUNC
#undef CK_FUNCTION_INFO
#undef CK_DECLARE_FUNCTION
#undef __PASTE
#endif

