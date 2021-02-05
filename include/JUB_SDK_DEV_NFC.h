//
//  JUB_SDK_DEV_NFC.h
//  JubSDK
//
//  Created by Pan Min on 2020/3/27.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_DEV_NFC_h
#define JUB_SDK_DEV_NFC_h

#include "JUB_SDK_DEV.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

typedef void (*NFC_ScanFuncCallBack)(
    unsigned int errorCode, /* 错误码 */
    const char*  uuid,      /* tag uuid */
    unsigned int devType    /* 设备类型 */
);

// Remove c++ features for swift framework
typedef struct stNFCDeviceInitParam {
    void* jvm;
    void* context;
    NFC_ScanFuncCallBack scanCallBack;

    JUB_CHAR_PTR crt;
    JUB_CHAR_PTR sk;
    JUB_UINT8 keyLength;
    JUB_CHAR_PTR hostID;
    JUB_CHAR_PTR cardGroupID;
//
//     stNFCDeviceInitParam();
//    ~stNFCDeviceInitParam() = default;
} NFC_DEVICE_INIT_PARAM;
// Remove c++ features for swift framework end

typedef enum {
    HAS_PIN      = 0x00,    // User's PIN has been setted
    RESETTED     = 0x02,    // NFC has been resetted
    HAS_ROOT_KEY = 0x5A     // root key has been generated
} JUB_ENUM_NFC_ROOT_KEY_STATUS;
typedef JUB_ENUM_NFC_ROOT_KEY_STATUS JUB_PTR JUB_ENUM_NFC_ROOT_KEY_STATUS_PTR;


JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_initNFCDevice(IN NFC_DEVICE_INIT_PARAM param);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_connectNFCDevice(JUB_BYTE_PTR bUUID,     /**< nfc device UUID */
                            JUB_UINT16* pDeviceID   /**< output nfc device connect handle */
);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_disconnectNFCDevice(JUB_UINT16 deviceID);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_isDeviceNFCConnect(JUB_UINT16 deviceID);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_setNFCAlertMessage(JUB_UINT16 deviceID, JUB_CHAR_CPTR msg);

/*****************************************************************************
 * @function name : JUB_SetLabel
 * @in  param : deviceID - device ID
 *          : label - label
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetLabel(IN JUB_UINT16 deviceID,
                    IN JUB_CHAR_CPTR label);

/*****************************************************************************
 * @function name : JUB_Reset
 * @in  param : deviceID - device ID
 *          : curve - curve
 * @out param :
 * @last change : default NO User's PIN.
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_Reset(IN JUB_UINT16 deviceID);

/*****************************************************************************
 * @function name : JUB_GenerateSeed
 * @in  param : deviceID - device ID
 *          : pinMix - User's PIN
 *          : curve - curve
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GenerateSeed(IN JUB_UINT16 deviceID,
                        IN JUB_CHAR_CPTR pinMix,
                        IN JUB_ENUM_CURVES curve);

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
                          IN JUB_CHAR_CPTR mnemonic);

/*****************************************************************************
 * @function name : JUB_ExportMnemonic
 * @in  param : deviceID - device ID
 *          : pinMix - User's PIN
 * @out param : mnemonic - mnemonic
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_ExportMnemonic(IN JUB_UINT16 deviceID,
                          IN JUB_CHAR_CPTR pinMix,
                          OUT JUB_CHAR_PTR_PTR mnemonic);

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
                            OUT JUB_ENUM_NFC_ROOT_KEY_STATUS_PTR status);

/*****************************************************************************
 * @function name : JUB_VerifyPIN
 * @in  param : deviceID - device ID
 *          : pinMix - old PIN
 *          : pinNew - new PIN
 * @out param : retry
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_ChangePIN(IN JUB_UINT16 deviceID,
                     IN JUB_CHAR_CPTR pinMix,
                     IN JUB_CHAR_CPTR pinNew,
                     OUT JUB_ULONG_PTR pretry);

/*****************************************************************************
 * @function name : JUB_GetPINRetries (Heart Beat)
 * @in  param : compID - could be device ID or context ID, both are OK
 * @out param : retry
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetPINRetries(IN JUB_UINT16 compID,
                         OUT JUB_ULONG_PTR pretry);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif /* JUB_SDK_DEV_NFC_h */
