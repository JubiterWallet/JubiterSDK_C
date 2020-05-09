//
//  JUB_SDK_DEV.cpp
//  JubSDK
//
//  Created by Pan Min on 2019/7/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_DEV.h"

#include <string.h>

#include "utility/util.h"
#include "utility/mutex.h"
#include "utility/Singleton.h"

#include <device/JubiterBLEDevice.hpp>
#include <device/JubiterNFCDevice.hpp>

#include <token/JubiterBlade/JubiterBladeToken.h>
#include <token/JubiterNFC/JubiterNFCToken.h>
#include <context/BaseContext.h>

JUB_RV _allocMem(JUB_CHAR_PTR_PTR memPtr, const std::string &strBuf);

// Remove c++ features for swift framework end
//stDevicdInfo::stDevicdInfo() {
//     memset(label, 0x00, sizeof(label)/sizeof(JUB_CHAR));
//     memset(sn, 0x00, sizeof(sn)/sizeof(JUB_CHAR));
//     pinRetry = 0;
//     pinMaxRetry = 0;
//     memset(bleVersion, 0x00, sizeof(bleVersion)/sizeof(JUB_CHAR));
//     memset(firmwareVersion, 0x00, sizeof(firmwareVersion)/sizeof(JUB_CHAR));
//}
// Remove c++ features for swift framework

/*****************************************************************************
 * @function name : JUB_GetDeviceInfo
 * @in  param : deviceID - device ID
 * @out param : info - device info
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetDeviceInfo(IN JUB_UINT16 deviceID,
                         OUT JUB_DEVICE_INFO_PTR info) {

    CREATE_THREAD_LOCK_GUARD
    std::shared_ptr<jub::token::HardwareTokenInterface> token;
#ifdef BLE_MODE
    if (dynamic_cast<jub::device::JubiterBLEDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterBladeToken>(
                         std::make_shared<jub::token::JubiterBladeToken>(deviceID));
    }
#endif
#ifdef NFC_MODE
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
#endif
//    else {
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

/*
    JUB_VERIFY_RV(token->getPinRetry(info.pinRetry));
    JUB_VERIFY_RV(token->getPinMaxRetry(info.pinMaxRetry));
    JUB_VERIFY_RV(token->getSN(sn));
    JUB_VERIFY_RV(token->getLabel(label));*/

    // Let's go to the main security domain,
    // instead of judging the return value,
    // to get the data back
    if (!(JUB_ENUM_BOOL)token->IsBootLoader()) {
        return JUBR_ERROR_ARGS;
    }

    JUB_BYTE sn[24] = {0,};
    JUB_BYTE label[32] = {0,};
    JUB_BYTE retry = 0;
    JUB_BYTE maxRetry = 0;
    JUB_BYTE bleVersion[4] = {0,};
    JUB_BYTE fwVersion[4] = {0,};

    JUB_VERIFY_RV(token->GetPinRetry(retry));
    JUB_VERIFY_RV(token->GetPinMaxRetry(maxRetry));
    JUB_VERIFY_RV(token->GetSN(sn));
    JUB_VERIFY_RV(token->GetLabel(label));
    JUB_VERIFY_RV(token->GetBleVersion(bleVersion));
    JUB_VERIFY_RV(token->GetFwVersion(fwVersion));

    memcpy(info->sn, sn, sizeof(sn)/sizeof(JUB_BYTE));
    memcpy(info->label, label, sizeof(label)/sizeof(JUB_BYTE));
    info->pinRetry = retry;
    info->pinMaxRetry = maxRetry;
    memcpy(info->bleVersion, bleVersion, sizeof(bleVersion)/sizeof(JUB_BYTE));
    memcpy(info->firmwareVersion, fwVersion, sizeof(fwVersion)/sizeof(JUB_BYTE));

    return JUBR_OK;
}


/*****************************************************************************
 * @function name : JUB_IsInitialize
 * @in  param : deviceID - device ID
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_ENUM_BOOL JUB_IsInitialize(IN JUB_UINT16 deviceID) {

    CREATE_THREAD_LOCK_GUARD
    std::shared_ptr<jub::token::HardwareTokenInterface> token;
#ifdef BLE_MODE
    if (dynamic_cast<jub::device::JubiterBLEDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterBladeToken>(
                         std::make_shared<jub::token::JubiterBladeToken>(deviceID));
    }
#endif
#ifdef NFC_MODE
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
#endif
//    else {
    if (!token) {
        return JUB_ENUM_BOOL::BOOL_FALSE;
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

    CREATE_THREAD_LOCK_GUARD
    std::shared_ptr<jub::token::HardwareTokenInterface> token;
#ifdef BLE_MODE
    if (dynamic_cast<jub::device::JubiterBLEDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterBladeToken>(
                         std::make_shared<jub::token::JubiterBladeToken>(deviceID));
    }
#endif
#ifdef NFC_MODE
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
#endif
//    else {
    if (!token) {
        return JUB_ENUM_BOOL::BOOL_FALSE;
    }

    jub::context::ContextManager::GetInstance()->ClearLast();

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

    CREATE_THREAD_LOCK_GUARD
    std::shared_ptr<jub::token::HardwareTokenInterface> token;
#ifdef BLE_MODE
    if (dynamic_cast<jub::device::JubiterBLEDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterBladeToken>(
                         std::make_shared<jub::token::JubiterBladeToken>(deviceID));
    }
#endif
#ifdef NFC_MODE
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
#endif
//    else {
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::string appletList;
    JUB_VERIFY_RV(token->EnumApplet(appletList));
    JUB_VERIFY_RV(_allocMem(appList, appletList));

    return JUBR_OK;
}


/*****************************************************************************
 * @function name : JUB_EnumSupportCoins
 * @in  param : deviceID - device ID
 * @out param : coinsList - coin list
 * @last change :
 *****************************************************************************/
JUB_RV JUB_EnumSupportCoins(IN JUB_UINT16 deviceID,
                            OUT JUB_CHAR_PTR_PTR coinsList) {

    CREATE_THREAD_LOCK_GUARD
    std::shared_ptr<jub::token::HardwareTokenInterface> token;
#ifdef BLE_MODE
    if (dynamic_cast<jub::device::JubiterBLEDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterBladeToken>(
                         std::make_shared<jub::token::JubiterBladeToken>(deviceID));
    }
#endif
#ifdef NFC_MODE
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
#endif
//    else {
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
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
                            IN JUB_CHAR_CPTR appID,
                            OUT JUB_CHAR_PTR_PTR version) {

    CREATE_THREAD_LOCK_GUARD
    std::shared_ptr<jub::token::HardwareTokenInterface> token;
#ifdef BLE_MODE
    if (dynamic_cast<jub::device::JubiterBLEDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterBladeToken>(
                         std::make_shared<jub::token::JubiterBladeToken>(deviceID));
    }
#endif
#ifdef NFC_MODE
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
#endif
//    else {
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::string str_version;
    JUB_VERIFY_RV(token->GetAppletVersionBlade(appID,str_version));
    JUB_VERIFY_RV(_allocMem(version, str_version));

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

    CREATE_THREAD_LOCK_GUARD
    auto context = (jub::context::BaseContext*)jub::context::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

    if (600 < timeout) {
        return JUBR_ERROR_ARGS;
    }

    JUB_VERIFY_RV(context->SetTimeout(timeout * 2));

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

    CREATE_THREAD_LOCK_GUARD
    std::shared_ptr<jub::token::HardwareTokenInterface> token;
#ifdef BLE_MODE
    if (dynamic_cast<jub::device::JubiterBLEDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterBladeToken>(
                         std::make_shared<jub::token::JubiterBladeToken>(deviceID));
    }
#endif
#ifdef NFC_MODE
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
#endif
//    else {
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

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
                       IN JUB_CHAR_CPTR apdu,
                       OUT JUB_CHAR_PTR_PTR response) {

    CREATE_THREAD_LOCK_GUARD
    std::shared_ptr<jub::token::HardwareTokenInterface> token;
#ifdef BLE_MODE
    if (dynamic_cast<jub::device::JubiterBLEDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterBladeToken>(
                         std::make_shared<jub::token::JubiterBladeToken>(deviceID));
    }
#endif
#ifdef NFC_MODE
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
#endif
//    else {
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::string str_response;
    JUB_VERIFY_RV(token->SendOneApdu(apdu, str_response));
    JUB_VERIFY_RV(_allocMem(response, str_response));

    return JUBR_OK;
}


/*****************************************************************************
* @function name : JUB_Reset
* @in  param : deviceID - device ID
*                     : curve - curve
* @out param :
* @last change : default User's PIN is '5555'.
*****************************************************************************/
JUB_RV JUB_Reset(IN JUB_UINT16 deviceID) {

    CREATE_THREAD_LOCK_GUARD
    std::shared_ptr<jub::token::HardwareTokenInterface> token;
    #ifdef BLE_MODE
    if (dynamic_cast<jub::device::JubiterBLEDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterBladeToken>(
                         std::make_shared<jub::token::JubiterBladeToken>(deviceID));
    }
    #endif
    #ifdef NFC_MODE
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
    #endif
//    else {
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

    JUB_VERIFY_RV(token->Reset());

    return JUBR_OK;
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

    CREATE_THREAD_LOCK_GUARD
    std::shared_ptr<jub::token::HardwareTokenInterface> token;
#ifdef BLE_MODE
    if (dynamic_cast<jub::device::JubiterBLEDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterBladeToken>(
                         std::make_shared<jub::token::JubiterBladeToken>(deviceID));
    }
#endif
#ifdef NFC_MODE
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
#endif
//    else {
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

    JUB_VERIFY_RV(token->GenerateSeed(pinMix, curve));

    return JUBR_OK;
}


/*****************************************************************************
* @function name : JUB_ImportSeed
* @in  param : deviceID - device ID
*                     : pinMix - User's PIN
*                     : strength - JUB_ENUM_MNEMONIC_STRENGTH
*                     : entropy - entropy
*                     : seed - seed
* @out param :
* @last change :
*****************************************************************************/
JUB_RV JUB_ImportSeed(IN JUB_UINT16 deviceID,
                      IN JUB_CHAR_CPTR pinMix,
                      IN JUB_ENUM_MNEMONIC_STRENGTH strength,
                      IN JUB_CHAR_CPTR entropy,
                      IN JUB_CHAR_CPTR seed) {

    CREATE_THREAD_LOCK_GUARD
    std::shared_ptr<jub::token::HardwareTokenInterface> token;
#ifdef BLE_MODE
    if (dynamic_cast<jub::device::JubiterBLEDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterBladeToken>(
                         std::make_shared<jub::token::JubiterBladeToken>(deviceID));
    }
#endif
#ifdef NFC_MODE
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
#endif
//    else {
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

    JUB_VERIFY_RV(token->SetMnemonic(pinMix,
                                     strength,
                                     entropy,
                                     seed));

    return JUBR_OK;
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

    CREATE_THREAD_LOCK_GUARD
    std::shared_ptr<jub::token::HardwareTokenInterface> token;
#ifdef BLE_MODE
    if (dynamic_cast<jub::device::JubiterBLEDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterBladeToken>(
                         std::make_shared<jub::token::JubiterBladeToken>(deviceID));
    }
#endif
#ifdef NFC_MODE
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
#endif
//    else {
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

    JUB_VERIFY_RV(token->ImportMnemonic(pinMix, mnemonic));

    return JUBR_OK;
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

    CREATE_THREAD_LOCK_GUARD
    std::shared_ptr<jub::token::HardwareTokenInterface> token;
#ifdef BLE_MODE
    if (dynamic_cast<jub::device::JubiterBLEDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterBladeToken>(
                         std::make_shared<jub::token::JubiterBladeToken>(deviceID));
    }
#endif
#ifdef NFC_MODE
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
#endif
//    else {
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::string str_response;
    JUB_VERIFY_RV(token->GetMnemonic(pinMix, str_response));
    JUB_VERIFY_RV(_allocMem(mnemonic, str_response));

    return JUBR_OK;
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

    CREATE_THREAD_LOCK_GUARD
    std::shared_ptr<jub::token::HardwareTokenInterface> token;
#ifdef BLE_MODE
    if (dynamic_cast<jub::device::JubiterBLEDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterBladeToken>(
                         std::make_shared<jub::token::JubiterBladeToken>(deviceID));
    }
#endif
#ifdef NFC_MODE
    if (dynamic_cast<jub::device::JubiterNFCDevice*>(
        jub::device::DeviceManager::GetInstance()->GetOne(deviceID)
        )) {
        token = std::dynamic_pointer_cast<jub::token::JubiterNFCToken>(
                         std::make_shared<jub::token::JubiterNFCToken>(deviceID));
    }
#endif
//    else {
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

    JUB_VERIFY_RV(token->ChangePIN(pinMix, pinNew));

    return JUBR_OK;
}
