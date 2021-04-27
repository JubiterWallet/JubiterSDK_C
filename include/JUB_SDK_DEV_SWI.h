//
//  JUB_SDK_DEV_SWI.h
//  JubSDK
//
//  Created by Pan Min on 2021/03/11.
//  Copyright © 2021 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_DEV_SWI_h
#define JUB_SDK_DEV_SWI_h

#include "JUB_SDK_DEV.h"

#define MAX_DEVICE                  8

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus


/*****************************************************************************
 * @function name : JUB_ConnetDeviceSWI
 * @in  param :
 * @out param : deviceID - device ID
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_ConnetDeviceSWI(JUB_UINT16_PTR pDeviceID);


/*****************************************************************************
 * @function name : JUB_DisconnetDeviceSWI
 * @in  param : deviceID - device ID
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_DisconnetDeviceSWI(IN JUB_UINT16 deviceID);


/*****************************************************************************
 * @function name : JUB_BuildFromMasterPrivateKey
 * @in  param : deviceID - device ID
 *           curve - JUB_ENUM_CURVES::SECP256K1, JUB_ENUM_CURVES::ED25519
 *           masterPrivateKey - Master PrivateKey
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_BuildFromMasterPrivateKey(IN JUB_UINT16 deviceID,
                                     IN JUB_ENUM_CURVES curve,
                                     IN JUB_CHAR_CPTR masterPrivateKey);


/*****************************************************************************
 * @function name : JUB_BuildFromMnemonic
 * @in  param : deviceID - device ID
 *           mnemonic - mnemonic
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_BuildFromMnemonic(IN JUB_UINT16 deviceID,
                             IN JUB_CHAR_CPTR passphrase,
                             IN JUB_CHAR_CPTR mnemonic);
#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif /* JUB_SDK_DEV_SWI_h */
