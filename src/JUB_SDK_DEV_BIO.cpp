//
//  JUB_SDK_DEV_BIO.cpp
//  JubSDK
//
//  Created by Pan Min on 2020/7/3.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include "JUB_SDK_DEV_BIO.h"

#include "utility/util.h"
#include "utility/mutex.h"

#include "context/BaseContext.h"

#include "device/JubiterBLEDevice.hpp"
#include <token/JubiterBIO/JubiterBIOToken.h>
#ifdef __ANDROID__
#include "utils/logUtils.h"
#endif

JUB_RV _allocMem(JUB_CHAR_PTR_PTR memPtr, const std::string &strBuf);

/*****************************************************************************
 * @function name : JUB_EnrollFingerprint
 * @in  param : deviceID - device ID
 * @inout param: fgptIndex - The index of current fingerprint modality.
 *                    If this value is ZERO, indicate enroll a new fingerprint;
 *                      otherwise this value mast be equal to the value in
 *                      response of previous this command.
 * @out param : ptimes -  total number of times that need to enroll for current fingerprint modality.
 *            fgptID - modality ID, assigned by the device
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_EnrollFingerprint(IN JUB_UINT16 deviceID,
                             INOUT JUB_BYTE_PTR fgptIndex, OUT JUB_ULONG_PTR ptimes,
                             OUT JUB_BYTE_PTR fgptID) {

#if defined(BLE_MODE) || defined(HID_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto token = std::make_shared<jub::token::JubiterBIOToken>(deviceID);
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->EnrollFingerprint(fgptIndex, ptimes,
                                           fgptID));

    return JUBR_OK;
#else   // #if defined(BLE_MODE) || defined(HID_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(BLE_MODE) || defined(HID_MODE) end
}


/*****************************************************************************
 * @function name : JUB_EnumFingerprint
 * @in  param : deviceID - device ID
 * @out param :fgptList - fingerprint list
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_EnumFingerprint(IN JUB_UINT16 deviceID,
                           OUT JUB_CHAR_PTR_PTR fgptList) {

#if defined(BLE_MODE) || defined(HID_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto token = std::make_shared<jub::token::JubiterBIOToken>(deviceID);
    JUB_CHECK_NULL(token);

    std::string str_fgpt_list;
    JUB_VERIFY_RV(token->EnumFingerprint(str_fgpt_list));

    JUB_VERIFY_RV(_allocMem(fgptList, str_fgpt_list));

    return JUBR_OK;
#else   // #if defined(BLE_MODE) || defined(HID_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(BLE_MODE) || defined(HID_MODE) end
}


/*****************************************************************************
 * @function name : JUB_EraseFingerprint
 * @in  param : deviceID - device ID
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_EraseFingerprint(IN JUB_UINT16 deviceID) {

#if defined(BLE_MODE) || defined(HID_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto token = std::make_shared<jub::token::JubiterBIOToken>(deviceID);
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->EraseFingerprint());

    return JUBR_OK;
#else   // #if defined(BLE_MODE) || defined(HID_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(BLE_MODE) || defined(HID_MODE) end
}


/*****************************************************************************
 * @function name : JUB_DeleteFingerprint
 * @in  param : deviceID - device ID
 *           fgptID - the modality ID of a fingerprint.
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_DeleteFingerprint(IN JUB_UINT16 deviceID,
                             IN JUB_BYTE fgptID) {

#if defined(BLE_MODE) || defined(HID_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto token = std::make_shared<jub::token::JubiterBIOToken>(deviceID);
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->DeleteFingerprint(fgptID));

    return JUBR_OK;
#else   // #if defined(BLE_MODE) || defined(HID_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(BLE_MODE) || defined(HID_MODE) end
}


/*****************************************************************************
 * @function name : JUB_VerifyFingerprint
 * @in  param : contextID - context ID
 * @out param : retry
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_VerifyFingerprint(IN JUB_UINT16 contextID,
                             OUT JUB_ULONG_PTR pretry) {

    auto context = jub::context::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->VerifyFingerprint(*pretry));

    return JUBR_OK;
}
