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

/// bio device APIs //////////////////////////////////////////
#define JUBR_BIO_FINGERPRINT_MODALITY_ERROR     0x40009BE0UL
#define JUBR_BIO_SPACE_LIMITATION               0x40009BEAUL
#define JUBR_BIO_TIMEOUT                        0x40009BEEUL

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus


typedef enum {
    VIA_DEVICE = 0x01,  // reserved
    VIA_9GRIDS = 0x02,
    VIA_APDU = 0x03,    // reserved
    VIA_FPGT = 0x04,    // reserved
} JUB_ENUM_IDENTITY_VERIFY_MODE;


/*****************************************************************************
 * @function name : JUB_IdentityVerify
 * @in  param : deviceID - device ID
 *           mode - the mode for verify identity, the following values are valid:
 *                   - JUB_ENUM_IDENTITY_VERIFY_MODE::VIA_DEVICE
 *                   - JUB_ENUM_IDENTITY_VERIFY_MODE::VIA_FPGT
 * @out param : retry - if OK, retry is meaningless value
 * @last change :
 *****************************************************************************/
JUB_RV JUB_IdentityVerify(IN JUB_UINT16 deviceID,
                          IN JUB_ENUM_IDENTITY_VERIFY_MODE mode,
                          OUT JUB_ULONG_PTR pretry);


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
                             OUT JUB_ULONG_PTR pretry);


/*****************************************************************************
 * @function name : JUB_IdentityShowNineGrids
 * @in  param : deviceID - device ID
 *           mode - the mode for verify identity
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_IdentityShowNineGrids(IN JUB_UINT16 deviceID);


/*****************************************************************************
 * @function name : JUB_IdentityCancelNineGrids
 * @in  param : deviceID - device ID
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_IdentityCancelNineGrids(IN JUB_UINT16 deviceID);


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
 *           fpTimeout - timeout for fingerprint
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_EraseFingerprint(IN JUB_UINT16 deviceID,
                            IN JUB_UINT16 fpTimeout);


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
                             IN JUB_BYTE fgptID);


/*****************************************************************************
 * @function name : JUB_VerifyFgptForIntl
 * @in  param : deviceID - device ID
 * @out param : retry
 * @last change : The main security domain testing command.
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_VerifyFgptForIntl(IN JUB_UINT16 deviceID,
                             OUT JUB_ULONG_PTR pretry);


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
#endif /* JUB_SDK_DEV_BIO_h */
