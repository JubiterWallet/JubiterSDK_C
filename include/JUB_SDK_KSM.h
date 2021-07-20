//
//  JUB_SDK_KSM.hpp
//  JubSDK
//
//  Created by Administrator on 2021/6/8.
//  Copyright © 2021 ft. All rights reserved.
//

#ifndef JUB_SDK_KSM_hpp
#define JUB_SDK_KSM_hpp

#include "JUB_SDK.h"
#include "JUB_SDK_DOT.h"
#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

/*****************************************************************************
 * @function name : JUB_CreateContextKSM
 * @in  param : cfg
 *          : deviceID - device ID
 * @out param : contextID
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CreateContextKSM(IN CONTEXT_CONFIG_DOT cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID);

/*****************************************************************************
 * @function name : JUB_GetAddressKSM
 * @in  param : contextID - context ID
 *          : path
 *          : bShow
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetAddressKSM(IN JUB_UINT16 contextID,
                         IN JUB_CHAR_PTR path,
                         IN JUB_ENUM_BOOL bShow,
                         OUT JUB_CHAR_PTR_PTR address);

/*****************************************************************************
 * @function name : JUB_GetHDNodeKSM
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX(0x00) for hex;
 *          : path
 * @out param : pubkey
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetHDNodeKSM(IN JUB_UINT16 contextID,
                        IN JUB_ENUM_PUB_FORMAT format,
                        IN JUB_CHAR_PTR path,
                        OUT JUB_CHAR_PTR_PTR pubkey);

/*****************************************************************************
 * @function name : JUB_GetMainHDNodeKSM
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX(0x00) for hex;
 * @out param : xpub
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetMainHDNodeKSM(IN JUB_UINT16 contextID,
                            IN JUB_ENUM_PUB_FORMAT format,
                            OUT JUB_CHAR_PTR_PTR xpub);

/*****************************************************************************
 * @function name : JUB_SetMyAddressKSM
 * @in  param : contextID - context ID
 *          : path
 * @out param : address
 * @last change :
 *****************************************************************************/
//JUB_COINCORE_DLL_EXPORT
//JUB_RV JUB_SetMyAddressKSM(IN JUB_UINT16 contextID,
//                           IN BIP44_Path path,
//                           OUT JUB_CHAR_PTR_PTR address);
//
/*****************************************************************************
 * @function name : JUB_SignTransactionKSM
 * @in  param : contextID - context ID
 *            : tx - JUB_TX_DOT
 * @out param : raw
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTransactionKSM(IN JUB_UINT16 contextID,
                              IN JUB_CHAR_PTR path,
                              IN JUB_TX_DOT tx,
                              OUT JUB_CHAR_PTR_PTR raw);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif /* JUB_SDK_KSM_h */
