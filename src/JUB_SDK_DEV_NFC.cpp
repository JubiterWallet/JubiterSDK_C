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

JUB_RV _allocMem(JUB_CHAR_PTR_PTR memPtr, const std::string &strBuf);

JUB_RV JUB_initNFCDevice(IN NFC_DEVICE_INIT_PARAM param) {

#if defined(NFC_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto nfcDevice = Singleton<jub::device::JubiterNFCDevice>::GetInstance();
    if (!nfcDevice) {
        return JUBR_ERROR;
    }

    JUB_VERIFY_RV(nfcDevice->Initialize(param));

    return JUBR_OK;
#else
    return JUBR_IMPL_NOT_SUPPORT;
#endif // #ifdef NFC_MODE
}


JUB_RV JUB_connectNFCDevice(JUB_BYTE_PTR bUUID,     /**< nfc device UUID */
                            JUB_UINT16* pDeviceID   /**< output nfc device connect handle */) {

#if defined(NFC_MODE)
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

#if defined(NFC_MODE)
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

#if defined(NFC_MODE)
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


/*****************************************************************************
* @function name : JUB_Reset
* @in  param : deviceID - device ID
*                     : curve - curve
* @out param :
* @last change : default User's PIN is '5555'.
*****************************************************************************/
JUB_RV JUB_Reset(IN JUB_UINT16 deviceID) {

#if defined(NFC_MODE)
    CREATE_THREAD_LOCK_GUARD
    std::shared_ptr<jub::token::HardwareTokenInterface> token;
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

    // Let's go to the main security domain,
    // instead of judging the return value,
    // to get the data back
    JUB_VERIFY_RV(token->SelectMainSecurityDomain());

    JUB_VERIFY_RV(token->Reset());

    return JUBR_OK;
#else
    return JUBR_IMPL_NOT_SUPPORT;
#endif
}


/*****************************************************************************
 * @function name : JUB_GenerateSeed
 * @in  param : deviceID - device ID
 *          : pinMix - User's PIN
 *          : curve - curve
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GenerateSeed(IN JUB_UINT16 deviceID,
                        IN JUB_CHAR_CPTR pinMix,
                        IN JUB_ENUM_CURVES curve) {

#if defined(NFC_MODE)
    CREATE_THREAD_LOCK_GUARD
    std::shared_ptr<jub::token::HardwareTokenInterface> token;

    if (dynamic_cast<jub::device::JubiterNFCDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

    // Let's go to the main security domain,
    // instead of judging the return value,
    // to get the data back
    JUB_VERIFY_RV(token->SelectMainSecurityDomain());

    JUB_VERIFY_RV(token->GenerateSeed(pinMix, curve));

    return JUBR_OK;
#else
    return JUBR_IMPL_NOT_SUPPORT;
#endif
}


/*****************************************************************************
 * @function name : JUB_ImportMnemonic
 * @in  param : deviceID - device ID
 *          : pinMix - User's PIN
 *          : mnemonic - mnemonic
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_ImportMnemonic(IN JUB_UINT16 deviceID,
                          IN JUB_CHAR_CPTR pinMix,
                          IN JUB_CHAR_CPTR mnemonic) {

#if defined(NFC_MODE)
    CREATE_THREAD_LOCK_GUARD
    std::shared_ptr<jub::token::HardwareTokenInterface> token;
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

    // Let's go to the main security domain,
    // instead of judging the return value,
    // to get the data back
    JUB_VERIFY_RV(token->SelectMainSecurityDomain());

    JUB_VERIFY_RV(token->ImportMnemonic(pinMix, mnemonic));

    return JUBR_OK;
#else
    return JUBR_IMPL_NOT_SUPPORT;
#endif
}


/*****************************************************************************
 * @function name : JUB_ExportMnemonic
 * @in  param : deviceID - device ID
 *          : pinMix - User's PIN
 * @out param : mnemonic - mnemonic
 * @last change :
 *****************************************************************************/
JUB_RV JUB_ExportMnemonic(IN JUB_UINT16 deviceID,
                          IN JUB_CHAR_CPTR pinMix,
                          OUT JUB_CHAR_PTR_PTR mnemonic) {

#if defined(NFC_MODE)
    CREATE_THREAD_LOCK_GUARD
    std::shared_ptr<jub::token::HardwareTokenInterface> token;
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

    // Let's go to the main security domain,
    // instead of judging the return value,
    // to get the data back
    JUB_VERIFY_RV(token->SelectMainSecurityDomain());

    std::string str_response;
    JUB_VERIFY_RV(token->GetMnemonic(pinMix, str_response));
    JUB_VERIFY_RV(_allocMem(mnemonic, str_response));

    return JUBR_OK;
#else
    return JUBR_IMPL_NOT_SUPPORT;
#endif
}

/*****************************************************************************
* @function name : JUB_VerifyPIN
* @in  param : deviceID - device ID
*                     : pinMix - old PIN
*                     : pinNew - new PIN
* @out param :
* @last change :
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_ChangePIN(IN JUB_UINT16 deviceID,
                     IN JUB_CHAR_CPTR pinMix,
                     IN JUB_CHAR_CPTR pinNew) {

#if defined(NFC_MODE)
    CREATE_THREAD_LOCK_GUARD
    std::shared_ptr<jub::token::HardwareTokenInterface> token;
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

    // Let's go to the main security domain,
    // instead of judging the return value,
    // to get the data back
    JUB_VERIFY_RV(token->SelectMainSecurityDomain());

    JUB_VERIFY_RV(token->ChangePIN(pinMix, pinNew));

    return JUBR_OK;
#else
    return JUBR_IMPL_NOT_SUPPORT;
#endif
}
