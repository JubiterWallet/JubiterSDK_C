//
//  JUB_SDK_FIL.h
//  JubSDK
//
//  Created by Pan Min on 2020/11/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_FIL_h
#define JUB_SDK_FIL_h

#include "JUB_SDK.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

typedef CONTEXT_CONFIG CONTEXT_CONFIG_FIL;


/*****************************************************************************
 * @function name : JUB_CreateContextFIL
 * @in  param : cfg
 *          : deviceID - device ID
 * @out param : contextID
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CreateContextFIL(IN CONTEXT_CONFIG_FIL cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID);

/*****************************************************************************
 * @function name : JUB_GetAddressFIL
 * @in  param : contextID - context ID
 *          : path
 *          : bShow
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetAddressFIL(IN JUB_UINT16 contextID,
                         IN BIP44_Path path,
                         IN JUB_ENUM_BOOL bShow,
                         OUT JUB_CHAR_PTR_PTR address);

/*****************************************************************************
 * @function name : JUB_GetHDNodeFIL
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX(0x00) for hex;
 *          : path
 * @out param : pubkey
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetHDNodeFIL(IN JUB_UINT16 contextID,
                        IN JUB_ENUM_PUB_FORMAT format,
                        IN BIP44_Path path,
                        OUT JUB_CHAR_PTR_PTR pubkey);

/*****************************************************************************
 * @function name : JUB_GetMainHDNodeFIL
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX(0x00) for hex;
 * @out param : xpub
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetMainHDNodeFIL(IN JUB_UINT16 contextID,
                            IN JUB_ENUM_PUB_FORMAT format,
                            OUT JUB_CHAR_PTR_PTR xpub);

///*****************************************************************************
// * @function name : JUB_SetMyAddressFIL
// * @in  param : contextID - context ID
// *          : path
// * @out param : address
// * @last change :
// *****************************************************************************/
//JUB_COINCORE_DLL_EXPORT
//JUB_RV JUB_SetMyAddressFIL(IN JUB_UINT16 contextID,
//                           IN BIP44_Path path,
//                           OUT JUB_CHAR_PTR_PTR address);
//
/*****************************************************************************
 * @function name : JUB_SignTransactionFIL
 * @in  param : contextID - context ID
 *          : path
 *          : nonce - nonce
 *          : gasLimit - gas limit
 *          : gasPriceInAtto - gas price in attoFIL
 *          : to
 *          : valueInAtto - value in attoFIL
 *          : input
 * @out param : raw
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTransactionFIL(IN JUB_UINT16 contextID,
                              IN BIP44_Path path,
                              IN JUB_UINT64 nonce,
                              IN JUB_UINT64 gasLimit,
                              IN JUB_CHAR_CPTR gasFeeCapInAtto,
                              IN JUB_CHAR_CPTR gasPremiumInAtto,
                              IN JUB_CHAR_CPTR to,
                              IN JUB_CHAR_CPTR valueInAtto,
                              IN JUB_CHAR_CPTR input,
                              OUT JUB_CHAR_PTR_PTR raw);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif /* JUB_SDK_XRP_h */
