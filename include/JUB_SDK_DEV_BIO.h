//
//  JUB_SDK_DEV_BIO.h
//  JubSDK
//
//  Created by Pan Min on 2020/7/3.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_DEV_BIO_h
#define JUB_SDK_DEV_BIO_h

#include "JUB_SDK_DEV.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus


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
                             OUT JUB_BYTE_PTR fgptID);


/*****************************************************************************
 * @function name : JUB_EnumFingerprint
 * @in  param : deviceID - device ID
 * @out param :fgptList - fingerprint list
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_EnumFingerprint(IN JUB_UINT16 deviceID,
                           OUT JUB_CHAR_PTR_PTR fgptList);


/*****************************************************************************
 * @function name : JUB_EraseFingerprint
 * @in  param : deviceID - device ID
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_EraseFingerprint(IN JUB_UINT16 deviceID);


/*****************************************************************************
 * @function name : JUB_DeleteFingerprint
 * @in  param : deviceID - device ID
 *           fgptID - the modality ID of a fingerprint.
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_DeleteFingerprint(IN JUB_UINT16 deviceID,
                             IN JUB_BYTE fgptID);


/*****************************************************************************
 * @function name : JUB_VerifyFingerprint
 * @in  param : contextID - context ID
 * @out param : retry
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_VerifyFingerprint(IN JUB_UINT16 contextID,
                             OUT JUB_ULONG_PTR pretry);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif /* JUB_SDK_DEV_BLE_h */
