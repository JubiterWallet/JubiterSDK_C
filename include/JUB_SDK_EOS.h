//
//  JUB_SDK_EOS.h
//  JubSDK
//
//  Created by Pan Min on 2019/10/09.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_EOS_h
#define JUB_SDK_EOS_h

#include "JUB_SDK.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

typedef enum class JubEOSPubFormat {
    HEX = 0x00,
    EOS = 0x01
} JUB_ENUM_EOS_PUB_FORMAT;

typedef struct {
    JUB_CHAR_PTR        mainPath;
} CONTEXT_CONFIG_EOS;

/*****************************************************************************
 * @function name : JUB_CreateContextEOS
 * @in  param : cfg
 *            : deviceID - device ID
 * @out param : contextID
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CreateContextEOS(IN CONTEXT_CONFIG_EOS cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID);

/*****************************************************************************
 * @function name : JUB_GetAddressEOS
 * @in  param : contextID - context ID
 *            : path
 *            : bShow
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetAddressEOS(IN JUB_UINT16 contextID,
                         IN BIP44_Path path,
//                         IN BIP48_Path path,
                         IN JUB_ENUM_BOOL bShow,
                         OUT JUB_CHAR_PTR_PTR address);

/*****************************************************************************
 * @function name : JUB_GetHDNodeEOS
 * @in  param : contextID - context ID
 *            : format - JUB_ENUM_EOS_PUB_FORMAT::HEX(0x00) for hex;
 *                       JUB_ENUM_EOS_PUB_FORMAT::EOS(0x01) for Legacy
 *            : path
 * @out param : pubkey
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetHDNodeEOS(IN JUB_UINT16 contextID,
                        IN JUB_ENUM_EOS_PUB_FORMAT format,
                        IN BIP44_Path path,
//                        IN BIP48_Path path,
                        OUT JUB_CHAR_PTR_PTR pubkey);

/*****************************************************************************
 * @function name : JUB_GetMainHDNodeEOS
 * @in  param : contextID - context ID
 *            : format - JUB_ENUM_EOS_PUB_FORMAT::HEX(0x00) for hex;
 *                       JUB_ENUM_EOS_PUB_FORMAT::EOS(0x01) for Legacy
 * @out param : xpub
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetMainHDNodeEOS(IN JUB_UINT16 contextID,
                            IN JUB_ENUM_EOS_PUB_FORMAT format,
                            OUT JUB_CHAR_PTR_PTR xpub);

/*****************************************************************************
 * @function name : JUB_SetMyAddressEOS
 * @in  param : contextID - context ID
 *            : path
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetMyAddressEOS(IN JUB_UINT16 contextID,
                           IN BIP44_Path path,
//                           IN BIP48_Path path,
                           OUT JUB_CHAR_PTR_PTR address);

/*****************************************************************************
 * @function name : JUB_SignTransactionEOS
 * @in  param : contextID - context ID
 *            : path
 *            : referenceBlockId   - reference block ID
 *            : referenceBlockTime - reference block time
 *            : currency - currency
 *            : from
 *            : to
 *            : asset
 *            : memo
 * @out param : rawInJSON
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTransactionEOS(IN JUB_UINT16 contextID,
                              IN BIP44_Path path,
//                              IN BIP48_Path path,
                              IN JUB_CHAR_PTR expiration,
                              IN JUB_CHAR_PTR referenceBlockId,
                              IN JUB_CHAR_PTR referenceBlockTime,
                              IN JUB_CHAR_PTR currency,
                              IN JUB_CHAR_PTR from,
                              IN JUB_CHAR_PTR to,
                              IN JUB_CHAR_PTR asset,
                              IN JUB_CHAR_PTR memo,
                              OUT JUB_CHAR_PTR_PTR rawInJSON);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif /* JUB_SDK_EOS_h */
