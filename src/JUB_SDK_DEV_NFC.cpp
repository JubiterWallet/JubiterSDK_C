//
//  JUB_SDK_DEV_NFC.cpp
//  JubSDK
//
//  Created by Pan Min on 2020/3/27.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include "JUB_SDK_DEV_NFC.h"

#include "utility/util.h"
#include "utility/mutex.h"

#include "context/BaseContext.h"
#include "product/ProductFactory.h"
#include "device/JubiterNFCDevice.hpp"
#include "token/JubiterNFC/JubiterNFCToken.h"
#ifdef __ANDROID__
#include "utils/logUtils.h"
#endif

JUB_RV _allocMem(JUB_CHAR_PTR_PTR memPtr, const std::string &strBuf);

JUB_RV JUB_initNFCDevice(IN NFC_DEVICE_INIT_PARAM param) {

#if defined(NFC_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto nfcDevice = jub::product::prdsFactory::GetInstance()->CreateProduct(JUB_ENUM_COMMODE::NFC);
    if (   !nfcDevice
        || !jub::device::xNFCDeviceFactory::CheckTypeid(nfcDevice)
        ) {
        return JUBR_ARGUMENTS_BAD;
    }

    JUB_VERIFY_RV((dynamic_cast<jub::device::JubiterNFCDevice*>(nfcDevice))->Initialize(param));

    return JUBR_OK;
#else   // #if defined(NFC_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(NFC_MODE) end
}


JUB_RV JUB_connectNFCDevice(JUB_BYTE_PTR bUUID,     /**< nfc device UUID */
                            JUB_UINT16* pDeviceID   /**< output nfc device connect handle */) {

#if defined(NFC_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto nfcDevice = jub::product::prdsFactory::GetInstance()->CreateProduct(JUB_ENUM_DEVICE::LITE);
    if (   !nfcDevice
        || !jub::device::xNFCDeviceFactory::CheckTypeid(nfcDevice)
        ) {
        return JUBR_ARGUMENTS_BAD;
    }

    JUB_ULONG * pdevHandle = new JUB_ULONG;
    JUB_RV rv = (dynamic_cast<jub::device::JubiterNFCDevice*>(nfcDevice))->Connect(bUUID, pdevHandle);
//    LOG_INF("JUB_connectNFCDevice rv: %lu", *pdevHandle);
    JUB_VERIFY_RV(rv);

    *pDeviceID = device_map::GetInstance()->AddOne(pdevHandle);
//    LOG_INF("JUB_connectNFCDevice rv: %hu", *pDeviceID);

    jub::device::DeviceManager::GetInstance()->AddOne(*pDeviceID, nfcDevice);

    return rv;
#else   // #if defined(NFC_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(NFC_MODE) end
}


JUB_RV JUB_disconnectNFCDevice(JUB_UINT16 deviceID) {

#if defined(NFC_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto nfcDevice = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    if (   !nfcDevice
        || !jub::device::xNFCDeviceFactory::CheckTypeid(nfcDevice)
        ) {
        return JUBR_ARGUMENTS_BAD;
    }

    JUB_ULONG *devHandle = device_map::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(devHandle);
    JUB_VERIFY_RV((dynamic_cast<jub::device::JubiterNFCDevice*>(nfcDevice))->Disconnect(*devHandle));

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

    return JUBR_OK;
#else   // #if defined(NFC_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(NFC_MODE) end
}


JUB_RV JUB_isDeviceNFCConnect(JUB_UINT16 deviceID) {

#if defined(NFC_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto nfcDevice = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    if (   !nfcDevice
        || !jub::device::xNFCDeviceFactory::CheckTypeid(nfcDevice)
        ) {
        return JUBR_ARGUMENTS_BAD;
    }

    JUB_ULONG *devHandle = device_map::GetInstance()->GetOne(deviceID);
    if (NULL == devHandle) {
        return JUBR_CONNECT_DEVICE_ERROR;
    }

    JUB_VERIFY_RV((dynamic_cast<jub::device::JubiterNFCDevice*>(nfcDevice))->IsConnect(*devHandle));

    return JUBR_OK;
#else   // #if defined(NFC_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(NFC_MODE) end
}


JUB_RV JUB_setNFCAlertMessage(JUB_UINT16 deviceID, JUB_CHAR_CPTR msg) {

#if defined(NFC_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto nfcDevice = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    if (   !nfcDevice
        || !jub::device::xNFCDeviceFactory::CheckTypeid(nfcDevice)
        ) {
        return JUBR_ARGUMENTS_BAD;
    }

    JUB_ULONG *devHandle = device_map::GetInstance()->GetOne(deviceID);
    if (NULL == devHandle) {
        return JUBR_CONNECT_DEVICE_ERROR;
    }

    JUB_VERIFY_RV((dynamic_cast<jub::device::JubiterNFCDevice*>(nfcDevice))->SetAlertMessage(*devHandle, (unsigned char*)msg));

    return JUBR_OK;
#else   // #if defined(NFC_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(NFC_MODE) end
}


/*****************************************************************************
 * @function name : JUB_SetLabel
 * @in  param : deviceID - device ID
 *          : label - label
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetLabel(IN JUB_UINT16 deviceID,
                    IN JUB_CHAR_CPTR label) {

#if defined(NFC_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto nfcDevice = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    if (   !nfcDevice
        || !jub::device::xNFCDeviceFactory::CheckTypeid(nfcDevice)
        ) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::shared_ptr<jub::token::HardwareTokenInterface> token;
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(nfcDevice)) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

//    // Let's go to the main security domain,
//    // instead of judging the return value,
//    // to get the data back
//    JUB_VERIFY_RV(token->SelectMainSecurityDomain());

    JUB_VERIFY_RV(token->SetLabel(label));

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

    return JUBR_OK;
#else   // #if defined(NFC_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(NFC_MODE) end
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
    auto nfcDevice = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    if (   !nfcDevice
        || !jub::device::xNFCDeviceFactory::CheckTypeid(nfcDevice)
        ) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::shared_ptr<jub::token::HardwareTokenInterface> token;
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(nfcDevice)) {
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

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

    return JUBR_OK;
#else   // #if defined(NFC_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(NFC_MODE) end
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
    auto nfcDevice = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    if (   !nfcDevice
        || !jub::device::xNFCDeviceFactory::CheckTypeid(nfcDevice)
        ) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::shared_ptr<jub::token::HardwareTokenInterface> token;
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(nfcDevice)) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

//    // Let's go to the main security domain,
//    // instead of judging the return value,
//    // to get the data back
//    JUB_VERIFY_RV(token->SelectMainSecurityDomain());

    JUB_VERIFY_RV(token->GenerateSeed(pinMix, curve));

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

    return JUBR_OK;
#else   // #if defined(NFC_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(NFC_MODE) end
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
    auto nfcDevice = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    if (   !nfcDevice
        || !jub::device::xNFCDeviceFactory::CheckTypeid(nfcDevice)
        ) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::shared_ptr<jub::token::HardwareTokenInterface> token;
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(nfcDevice)) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

//    // Let's go to the main security domain,
//    // instead of judging the return value,
//    // to get the data back
//    JUB_VERIFY_RV(token->SelectMainSecurityDomain());

    JUB_VERIFY_RV(token->ImportMnemonic(pinMix, mnemonic));

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

    return JUBR_OK;
#else   // #if defined(NFC_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(NFC_MODE) end
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
    auto nfcDevice = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    if (   !nfcDevice
        || !jub::device::xNFCDeviceFactory::CheckTypeid(nfcDevice)
        ) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::shared_ptr<jub::token::HardwareTokenInterface> token;
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(nfcDevice)) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

//    // Let's go to the main security domain,
//    // instead of judging the return value,
//    // to get the data back
//    JUB_VERIFY_RV(token->SelectMainSecurityDomain());

    std::string str_response;
    JUB_VERIFY_RV(token->ExportMnemonic(pinMix, str_response));
    JUB_VERIFY_RV(_allocMem(mnemonic, str_response));

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

    return JUBR_OK;
#else   // #if defined(NFC_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(NFC_MODE) end
}

/*****************************************************************************
 * @function name : JUB_GetRootKeyStatus
 * @in  param : deviceID - device ID
 * @out param : status - 00 - User's PIN has been setted
 *                 02 - NFC has been resetted
 *                 5A - root key has been generated
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetRootKeyStatus(IN JUB_UINT16 deviceID,
                            OUT JUB_ENUM_NFC_ROOT_KEY_STATUS_PTR status) {

#if defined(NFC_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto device = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    if (!device) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::shared_ptr<jub::token::HardwareTokenInterface> token = jub::product::xProductFactory::GetDeviceToken(deviceID);
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

    JUB_VERIFY_RV(token->GetRootKeyStatus(status));

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

    return JUBR_OK;
#else   // #if defined(NFC_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(NFC_MODE) end
}

/*****************************************************************************
* @function name : JUB_VerifyPIN
* @in  param : deviceID - device ID
*                     : pinMix - old PIN
*                     : pinNew - new PIN
* @out param : retry
* @last change :
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_ChangePIN(IN JUB_UINT16 deviceID,
                     IN JUB_CHAR_CPTR pinMix,
                     IN JUB_CHAR_CPTR pinNew,
                     OUT JUB_ULONG_PTR pretry) {

#if defined(NFC_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto nfcDevice = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    if (   !nfcDevice
        || !jub::device::xNFCDeviceFactory::CheckTypeid(nfcDevice)
        ) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::shared_ptr<jub::token::HardwareTokenInterface> token;
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(nfcDevice)) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

//    // Let's go to the main security domain,
//    // instead of judging the return value,
//    // to get the data back
//    JUB_VERIFY_RV(token->SelectMainSecurityDomain());

    JUB_RV rv = token->ChangePIN(pinMix, pinNew);

    JUB_BYTE retry = 0;
    JUB_VERIFY_RV(token->GetPinRetry(retry));
    *pretry = retry;

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

    JUB_VERIFY_RV(rv);

    return JUBR_OK;
#else   // #if defined(NFC_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(NFC_MODE) end
}
