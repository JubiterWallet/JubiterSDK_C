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

#include "context/BaseContext.h"
#include "product/ProductFactory.h"
#include <string>
#include <cstring>


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
 * @function name : JUB_GetDeviceType
 * @in  param : deviceID - device ID
 * @out param : commode - device communication mode
 *           : deviceClass - device class
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetDeviceType(IN JUB_UINT16 deviceID,
                         OUT JUB_ENUM_COMMODE_PTR commode, OUT JUB_ENUM_DEVICE_PTR deviceClass) {

    CREATE_THREAD_LOCK_GUARD

    return jub::product::xProductFactory::GetDeviceType(deviceID,
                                                        commode, deviceClass);
}


/*****************************************************************************
 * @function name : JUB_GetDeviceInfo
 * @in  param : deviceID - device ID
 * @out param : info - device info
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetDeviceInfo(IN JUB_UINT16 deviceID,
                         OUT JUB_DEVICE_INFO_PTR info) {

    CREATE_THREAD_LOCK_GUARD
    auto device = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(device);

    std::shared_ptr<jub::token::HardwareTokenInterface> token = jub::product::xProductFactory::GetDeviceToken(deviceID);
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
    JUB_VERIFY_RV(token->SelectMainSecurityDomain());

    JUB_BYTE sn[25] = {0,};
    JUB_BYTE label[33] = {0,};
    JUB_BYTE retry = 0;
    JUB_BYTE maxRetry = 0;
    JUB_BYTE bleVersion[5] = {0,};
    JUB_BYTE fwVersion[5] = {0,};

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

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

    return JUBR_OK;
}


/*****************************************************************************
 * @function name : JUB_GetDeviceRootKeyStatus
 * @in  param : deviceID - device ID
 * @out param : status - 00 - User's PIN has been setted
 *                 02 - NFC has been resetted
 *                 5A - root key has been generated
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetDeviceRootKeyStatus(IN JUB_UINT16 deviceID,
                                  OUT JUB_ENUM_DEVICE_ROOT_KEY_STATUS_PTR status) {

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
}


/*****************************************************************************
 * @function name : JUB_IsBootLoader
 * @in  param : deviceID - device ID
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_ENUM_BOOL JUB_IsBootLoader(IN JUB_UINT16 deviceID) {

    CREATE_THREAD_LOCK_GUARD
    auto device = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    if (!device) {
        return JUB_ENUM_BOOL::BOOL_FALSE;
    }

    std::shared_ptr<jub::token::HardwareTokenInterface> token = jub::product::xProductFactory::GetDeviceToken(deviceID);
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
    auto device = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(device);

    std::shared_ptr<jub::token::HardwareTokenInterface> token = jub::product::xProductFactory::GetDeviceToken(deviceID);
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

    // Let's go to the main security domain,
    // instead of judging the return value,
    // to get the data back
    JUB_VERIFY_RV(token->SelectMainSecurityDomain());

    std::string appletList;
    JUB_VERIFY_RV(token->EnumApplet(appletList));
    JUB_VERIFY_RV(_allocMem(appList, appletList));

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

    return JUBR_OK;
}


/*****************************************************************************
 * @function name : JUB_EnumAppletInfo
 * @in  param : deviceID - device ID
 * @out param : appInfoListInfoInJSON - applet info list in JSON
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_EnumAppletInfo(IN JUB_UINT16 deviceID,
                          OUT JUB_CHAR_PTR_PTR appInfoListInJSON) {

    CREATE_THREAD_LOCK_GUARD
    auto device = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(device);

    std::shared_ptr<jub::token::HardwareTokenInterface> token = jub::product::xProductFactory::GetDeviceToken(deviceID);
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

    // Let's go to the main security domain,
    // instead of judging the return value,
    // to get the data back
    JUB_VERIFY_RV(token->SelectMainSecurityDomain());

    std::string appletInfoList;
    JUB_VERIFY_RV(token->EnumAppletInfo(appletInfoList));
    JUB_VERIFY_RV(_allocMem(appInfoListInJSON, appletInfoList));

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

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
    auto device = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(device);

    std::shared_ptr<jub::token::HardwareTokenInterface> token = jub::product::xProductFactory::GetDeviceToken(deviceID);
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

    // Let's go to the main security domain,
    // instead of judging the return value,
    // to get the data back
    JUB_VERIFY_RV(token->SelectMainSecurityDomain());

    std::string str_coinsList;
    JUB_VERIFY_RV(token->EnumSupportCoins(str_coinsList));
    JUB_VERIFY_RV(_allocMem(coinsList, str_coinsList));

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

    return JUBR_OK;
}


/*****************************************************************************
 * @function name : JUB_GetAppletVersion
 * @in  param : deviceID - device ID
 *            : appID - applet ID
 * @out param : version - applet three-part version number
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetAppletVersion(IN JUB_UINT16 deviceID,
                            IN JUB_CHAR_CPTR appID,
                            OUT JUB_VERSION_PTR version) {

    CREATE_THREAD_LOCK_GUARD
    auto device = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(device);

    std::shared_ptr<jub::token::HardwareTokenInterface> token = jub::product::xProductFactory::GetDeviceToken(deviceID);
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

    JUB_VERIFY_RV(token->GetAppletVersion(appID, *version));

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

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
    auto device = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(device);

    std::shared_ptr<jub::token::HardwareTokenInterface> token = jub::product::xProductFactory::GetDeviceToken(deviceID);
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

    // Let's go to the main security domain,
    // instead of judging the return value,
    // to get the data back
    JUB_VERIFY_RV(token->SelectMainSecurityDomain());

    std::string str_cert;
    JUB_VERIFY_RV(token->GetDeviceCert(str_cert));
    JUB_VERIFY_RV(_allocMem(cert, str_cert));

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

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
    auto device = jub::device::DeviceManager::GetInstance()->GetOne(deviceID);
    JUB_CHECK_NULL(device);

    std::shared_ptr<jub::token::HardwareTokenInterface> token = jub::product::xProductFactory::GetDeviceToken(deviceID);
    if (!token) {
        return JUBR_ARGUMENTS_BAD;
    }

    std::string str_response;
    JUB_VERIFY_RV(token->SendOneApdu(apdu, str_response));
    JUB_VERIFY_RV(_allocMem(response, str_response));

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

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
