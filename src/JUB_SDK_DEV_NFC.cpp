//
//  JUB_SDK_DEV_NFC.cpp
//  JubSDK
//
//  Created by Pan Min on 2020/3/27.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include <stdio.h>
#include "JUB_SDK_DEV_NFC.h"
//#include "DeviceTypeBase.hpp"

#include "utility/util.h"
#include "utility/mutex.h"

#include "device/JubiterNFCDevice.hpp"
#include <token/JubiterNFC/JubiterNFCToken.h>
#ifdef __ANDROID__
#include "utils/logUtils.h"
#endif


JUB_RV JUB_initNFCDevice(IN NFC_DEVICE_INIT_PARAM param) {

#ifdef NFC_MODE
    CREATE_THREAD_LOCK_GUARD
    auto nfcDevice = Singleton<jub::device::JubiterNFCDevice>::GetInstance();
    if (!nfcDevice) {
        return JUBR_ERROR;
    }

    JUB_VERIFY_RV(nfcDevice->Initialize({param.scanCallBack}));

    return JUBR_OK;
#else
    return JUBR_IMPL_NOT_SUPPORT;
#endif // #ifdef NFC_MODE
}


JUB_RV JUB_connectNFCDevice(JUB_BYTE_PTR bUUID,     /**< nfc device UUID */
                            JUB_UINT16* pDeviceID   /**< output nfc device connect handle */) {

#ifdef NFC_MODE
    CREATE_THREAD_LOCK_GUARD
    auto nfcDevice = Singleton<jub::device::JubiterNFCDevice>::GetInstance();
    if (!nfcDevice) {
        return JUBR_ERROR;
    }

    JUB_ULONG * pdevHandle = new JUB_ULONG;
    JUB_RV rv = nfcDevice->Connect(bUUID, pdevHandle);
//    LOG_INF("JUB_connectNFCDevice rv: %lu", *pdevHandle);
    JUB_VERIFY_RV(rv);

    *pDeviceID = device_map::GetInstance()->AddOne(pdevHandle);
//    LOG_INF("JUB_connectNFCDevice rv: %hu", *pDeviceID);

    jub::device::DeviceManager::GetInstance()->AddOne(*pDeviceID, nfcDevice);

    return rv;
#else
    return JUBR_IMPL_NOT_SUPPORT;
#endif // #ifdef NFC_MODE
}


JUB_RV JUB_disconnectNFCDevice(JUB_UINT16 deviceID) {

#ifdef NFC_MODE
    CREATE_THREAD_LOCK_GUARD
    auto nfcDevice = Singleton<jub::device::JubiterNFCDevice>::GetInstance();
    if (!nfcDevice) {
        return JUBR_ERROR;
    }

    JUB_ULONG *devHandle = device_map::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(devHandle);
    JUB_VERIFY_RV(nfcDevice->Disconnect(*devHandle));

    return JUBR_OK;
#else
    return JUBR_IMPL_NOT_SUPPORT;
#endif // #ifdef NFC_MODE
}


JUB_RV JUB_isDeviceNFCConnect(JUB_UINT16 deviceID) {

#ifdef NFC_MODE
    CREATE_THREAD_LOCK_GUARD
    auto nfcDevice = Singleton<jub::device::JubiterNFCDevice>::GetInstance();
    if (!nfcDevice) {
        return JUBR_ERROR;
    }
    JUB_ULONG *devHandle = device_map::GetInstance()->GetOne(deviceID);
    if (NULL == devHandle) {
        return JUBR_CONNECT_DEVICE_ERROR;
    }

    JUB_VERIFY_RV(nfcDevice->IsConnect(*devHandle));

    return JUBR_OK;
#else
    return JUBR_IMPL_NOT_SUPPORT;
#endif // #ifdef NFC_MODE
}
