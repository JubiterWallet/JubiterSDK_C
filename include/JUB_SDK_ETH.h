//
//  JUB_SDK_ETH.h
//  JubSDK
//
//  Created by Pan Min on 2019/7/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_ETH_h
#define JUB_SDK_ETH_h

#include "JUB_SDK.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

// Remove c++ features for swift framework
//typedef struct stContextCfgETH : stContextCfg {
typedef struct stContextCfgETH {
    JUB_CHAR_PTR            mainPath;
    int                 chainID;
//
//    stContextCfgETH();
//    virtual ~stContextCfgETH() {}
} CONTEXT_CONFIG_ETH;
// Remove c++ features for swift framework end

/*****************************************************************************
 * @function name : JUB_CreateContextETH
 * @in  param : cfg
 *            : deviceID - device ID
 * @out param : contextID
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CreateContextETH(IN CONTEXT_CONFIG_ETH cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID);

/*****************************************************************************
 * @function name : JUB_GetAddressETH
 * @in  param : contextID - context ID
 *            : path
 *            : bShow
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetAddressETH(IN JUB_UINT16 contextID,
                         IN BIP44_Path path,
                         IN JUB_ENUM_BOOL bShow,
                         OUT JUB_CHAR_PTR_PTR address);

/*****************************************************************************
 * @function name : JUB_GetHDNodeETH
 * @in  param : contextID - context ID
 *            : format - JUB_ENUM_PUB_FORMAT::HEX (0x00) for hex;
 *                   JUB_ENUM_PUB_FORMAT::XPUB(0x01) for xpub
 *            : path
 * @out param : pubkey
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetHDNodeETH(IN JUB_UINT16 contextID,
                        IN JUB_ENUM_PUB_FORMAT format,
                        IN BIP44_Path path,
                        OUT JUB_CHAR_PTR_PTR pubkey);

/*****************************************************************************
 * @function name : JUB_GetMainHDNodeETH
 * @in  param : contextID - context ID
 *            : format - JUB_ENUM_PUB_FORMAT::HEX (0x00) for hex;
 *                   JUB_ENUM_PUB_FORMAT::XPUB(0x01) for xpub
 * @out param : xpub
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetMainHDNodeETH(IN JUB_UINT16 contextID,
                            IN JUB_ENUM_PUB_FORMAT format,
                            OUT JUB_CHAR_PTR_PTR xpub);

/*****************************************************************************
 * @function name : JUB_SetMyAddressETH
 * @in  param : contextID - context ID
 *            : path
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetMyAddressETH(IN JUB_UINT16 contextID,
                           IN BIP44_Path path,
                           OUT JUB_CHAR_PTR_PTR address);

/*****************************************************************************
 * @function name : JUB_SignTransactionETH
 * @in  param : contextID - context ID
 *            : path
 *            : nonce - nonce
 *            : gasLimit - gas limit
 *            : gasPriceInWei - gas price in wei
 *            : to
 *            : valueInWei - value in wei
 *            : input
 * @out param : raw
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTransactionETH(IN JUB_UINT16 contextID,
                              IN BIP44_Path path,
                              IN JUB_UINT32 nonce,
                              IN JUB_UINT32 gasLimit,
                              IN JUB_CHAR_CPTR gasPriceInWei,
                              IN JUB_CHAR_CPTR to,
                              IN JUB_CHAR_CPTR valueInWei,
                              IN JUB_CHAR_CPTR input,
                              OUT JUB_CHAR_PTR_PTR raw);

/*****************************************************************************
 * @function name : JUB_BuildERC20AbiETH
 * @in  param : contextID - context ID
 *            : tokenName - ETH token name
 *            : unitDP - unit decimal place
 *            : contractAddress - contract address
 *            : tokenTo - token to
 *            : tokenValue - value for token transaction
 * @out param : abi
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_BuildERC20AbiETH(IN JUB_UINT16 contextID,
                            IN JUB_CHAR_CPTR tokenName,
                            IN JUB_UINT16 unitDP,
                            IN JUB_CHAR_CPTR contractAddress,
                            IN JUB_CHAR_CPTR tokenTo, IN JUB_CHAR_CPTR tokenValue,
                            OUT JUB_CHAR_PTR_PTR abi);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif /* JUB_SDK_ETH_h */
