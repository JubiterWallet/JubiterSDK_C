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
#include "token/JubiterBIO/JubiterBIOToken.h"


#ifdef __ANDROID__
#include "utils/logUtils.h"
#endif


JUB_RV _allocMem(JUB_CHAR_PTR_PTR memPtr, const std::string &strBuf);


/*****************************************************************************
 * @function name : JUB_IdentityVerify
 * @in  param : deviceID - device ID
 *           mode - the mode for verify identity, the following values are valid:
 *                   - JUB_ENUM_IDENTITY_VERIFY_MODE::VIA_DEVICE
 *                   - JUB_ENUM_IDENTITY_VERIFY_MODE::VIA_FPGT
 * @out param : retry - if OK, retry is meaningless value
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_IdentityVerify(IN JUB_UINT16 deviceID,
                          IN JUB_ENUM_IDENTITY_VERIFY_MODE mode,
                          OUT JUB_ULONG_PTR pretry) {

#if defined(BLE_MODE) || defined(HID_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto token = std::make_shared<jub::token::JubiterBIOToken>(deviceID);
    JUB_CHECK_NULL(token);

    JUB_ULONG retry = 0;
    JUB_RV rv = token->IdentityVerify(mode, retry);

    *pretry = retry;

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

    return rv;
#else   // #if defined(BLE_MODE) || defined(HID_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(BLE_MODE) || defined(HID_MODE) end
}


/*****************************************************************************
 * @function name : JUB_IdentityVerifyPIN
 * @in  param : deviceID - device ID
 *           mode - the mode for verify identity, the following values are valid:
 *                   - JUB_ENUM_IDENTITY_VERIFY_MODE::VIA_9GRIDS
 *                   - JUB_ENUM_IDENTITY_VERIFY_MODE::VIA_APDU
 *           pinMix: user's PIN
 * @out param : retry - if OK, retry is meaningless value
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_IdentityVerifyPIN(IN JUB_UINT16 deviceID,
                             IN JUB_ENUM_IDENTITY_VERIFY_MODE mode,
                             IN JUB_CHAR_CPTR pinMix,
                             OUT JUB_ULONG_PTR pretry) {

#if defined(BLE_MODE) || defined(HID_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto token = std::make_shared<jub::token::JubiterBIOToken>(deviceID);
    JUB_CHECK_NULL(token);

    JUB_ULONG retry = 0;
    JUB_RV rv = token->IdentityVerifyPIN(mode, pinMix, retry);

    *pretry = retry;

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

    return rv;
#else   // #if defined(BLE_MODE) || defined(HID_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(BLE_MODE) || defined(HID_MODE) end
}


/*****************************************************************************
 * @function name : JUB_IdentityShowNineGrids
 * @in  param : deviceID - device ID
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_IdentityShowNineGrids(IN JUB_UINT16 deviceID) {

#if defined(BLE_MODE) || defined(HID_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto token = std::make_shared<jub::token::JubiterBIOToken>(deviceID);
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->IdentityNineGrids(true));

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

    return JUBR_OK;
#else   // #if defined(BLE_MODE) || defined(HID_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(BLE_MODE) || defined(HID_MODE) end
}


/*****************************************************************************
 * @function name : JUB_IdentityCancelNineGrids
 * @in  param : deviceID - device ID
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_IdentityCancelNineGrids(IN JUB_UINT16 deviceID) {

#if defined(BLE_MODE) || defined(HID_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto token = std::make_shared<jub::token::JubiterBIOToken>(deviceID);
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->IdentityNineGrids(false));

    JUB_VERIFY_RV(token->UIShowMain());

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

    return JUBR_OK;
#else   // #if defined(BLE_MODE) || defined(HID_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(BLE_MODE) || defined(HID_MODE) end
}


/*****************************************************************************
 * @function name : JUB_EnrollFingerprint
 * @in  param : deviceID - device ID
 * @inout param: fpTimeout - timeout for fingerprint
 *            fgptIndex - The index of current fingerprint modality.
 *                    If this value is ZERO, indicate enroll a new fingerprint;
 *                      otherwise this value mast be equal to the value in
 *                      response of previous this command.
 * @out param : ptimes -  total number of times that need to enroll for current fingerprint modality.
 *            fgptID - modality ID, assigned by the device
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_EnrollFingerprint(IN JUB_UINT16 deviceID,
                             IN JUB_UINT16 fpTimeout,
                             INOUT JUB_BYTE_PTR fgptIndex, OUT JUB_ULONG_PTR ptimes,
                             OUT JUB_BYTE_PTR fgptID) {

#if defined(BLE_MODE) || defined(HID_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto token = std::make_shared<jub::token::JubiterBIOToken>(deviceID);
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->EnrollFingerprint(fpTimeout,
                                           fgptIndex, ptimes,
                                           fgptID));

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

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

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

    return JUBR_OK;
#else   // #if defined(BLE_MODE) || defined(HID_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(BLE_MODE) || defined(HID_MODE) end
}


/*****************************************************************************
 * @function name : JUB_EraseFingerprint
 * @in  param : deviceID - device ID
 *           fpTimeout - timeout for fingerprint
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_EraseFingerprint(IN JUB_UINT16 deviceID,
                            IN JUB_UINT16 fpTimeout) {

#if defined(BLE_MODE) || defined(HID_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto token = std::make_shared<jub::token::JubiterBIOToken>(deviceID);
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->EraseFingerprint(fpTimeout));

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

    return JUBR_OK;
#else   // #if defined(BLE_MODE) || defined(HID_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(BLE_MODE) || defined(HID_MODE) end
}


/*****************************************************************************
 * @function name : JUB_DeleteFingerprint
 * @in  param : deviceID - device ID
 *           fpTimeout - timeout for fingerprint
 *           fgptID - the modality ID of a fingerprint.
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_DeleteFingerprint(IN JUB_UINT16 deviceID,
                             IN JUB_UINT16 fpTimeout,
                             IN JUB_BYTE fgptID) {

#if defined(BLE_MODE) || defined(HID_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto token = std::make_shared<jub::token::JubiterBIOToken>(deviceID);
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->DeleteFingerprint(fpTimeout, fgptID));

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

    return JUBR_OK;
#else   // #if defined(BLE_MODE) || defined(HID_MODE)
    return JUBR_IMPL_NOT_SUPPORT;
#endif  // #if defined(BLE_MODE) || defined(HID_MODE) end
}


/*****************************************************************************
 * @function name : JUB_VerifyFgptForIntl
 * @in  param : deviceID - device ID
 * @out param : retry
 * @last change : The main security domain testing command.
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_VerifyFgptForIntl(IN JUB_UINT16 deviceID,
                             OUT JUB_ULONG_PTR pretry) {

#if defined(BLE_MODE) || defined(HID_MODE)
    CREATE_THREAD_LOCK_GUARD
    auto token = std::make_shared<jub::token::JubiterBIOToken>(deviceID);
    JUB_CHECK_NULL(token);

    JUB_ULONG retry = 0;
    JUB_RV rv = token->VerifyFgptForIntl(retry);

    *pretry = retry;

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

    return rv;
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

    // Clean up the session for device in order to force calling ActiveSelf().
    jub::context::ContextManager::GetInstance()->ClearLast();

    return JUBR_OK;
}
