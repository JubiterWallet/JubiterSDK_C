//
//  JUB_SDK_BTC.h
//  JubSDK
//
//  Created by Pan Min on 2019/7/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_BTC_h
#define JUB_SDK_BTC_h

#include "JUB_SDK.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

typedef enum {
    COINBTC,
    COINBCH,
    COINLTC,
    COINUSDT,
    Default = COINBTC
} JUB_ENUM_COINTYPE_BTC;

typedef enum {
    p2pkh = 0,
    //p2pwpkh,
    p2sh_p2wpkh
/*
     p2sh_multisig,
     p2wsh_multisig,
     p2sh_p2wsh_multisig,
*/
} JUB_BTC_TRANS_TYPE;

typedef enum {
    BTC = 0x00,
    cBTC,
    mBTC,
    uBTC,
    Satoshi
} JUB_BTC_UNIT_TYPE;

typedef struct {
    JUB_ENUM_COINTYPE_BTC   coinType;// = { JUB_ENUM_COINTYPE_BTC::COINBTC };

    JUB_CHAR_PTR            mainPath;
    JUB_BTC_TRANS_TYPE      transType;
} CONTEXT_CONFIG_BTC;

typedef struct {
    JUB_CHAR_PTR    preHash;
    JUB_UINT16      preIndex;
    JUB_UINT64      amount;
    BIP32_Path      path;
} INPUT_BTC;

typedef enum {
    P2PKH = 0x00,
    RETURN0 = 0x01
} OUTPUT_BTC_TYPE;

typedef struct {
    JUB_CHAR_PTR    address;
    JUB_UINT64      amount;
    JUB_ENUM_BOOL   changeAddress;
    BIP32_Path      path;
} OUTPUT_P2PKH;

typedef struct {
    JUB_UINT64      amount;
    JUB_UINT16      dataLen;
    JUB_BYTE        data[40];
} OUTPUT_RETURN0;

typedef struct {
    OUTPUT_BTC_TYPE type;
    union {
        OUTPUT_P2PKH   outputP2pkh;
        OUTPUT_RETURN0 outputReturn0;
    };
} OUTPUT_BTC;

/*****************************************************************************
 * @function name : JUB_CreateContextBTC
 * @in  param : cfg
 *            : deviceID - device ID
 * @out param : contextID
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CreateContextBTC(IN CONTEXT_CONFIG_BTC cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID);

/*****************************************************************************
 * @function name : JUB_GetHDNodeBTC
 * @in  param : contextID - context ID
 *            : path
 * @out param : xpub
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetHDNodeBTC(IN JUB_UINT16 contextID,
                        IN BIP32_Path path,
                        OUT JUB_CHAR_PTR_PTR xpub);

/*****************************************************************************
 * @function name : JUB_GetMainHDNodeBTC
 * @in  param : contextID - context ID
 * @out param : xpub
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetMainHDNodeBTC(IN JUB_UINT16 contextID,
                            OUT JUB_CHAR_PTR_PTR xpub);

/*****************************************************************************
 * @function name : JUB_GetAddressBTC
 * @in  param : contextID - context ID
 *            : path
 *            : bShow
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetAddressBTC(IN JUB_UINT16 contextID,
                         IN BIP32_Path path,
                         IN JUB_ENUM_BOOL bShow,
                         OUT JUB_CHAR_PTR_PTR address);

/*****************************************************************************
 * @function name : JUB_SetMyAddressBTC
 * @in  param : contextID - context ID
 *            : path
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetMyAddressBTC(IN JUB_UINT16 contextID,
                           IN BIP32_Path path,
                           OUT JUB_CHAR_PTR_PTR address);

/*****************************************************************************
 * @function name : JUB_SignTransactionBTC
 * @in  param : contextID - context ID
 *            : inputs
 *            : iCount
 *            : outputs
 *            : oCount
 *            : lockTime
 * @out param : raw
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTransactionBTC(IN JUB_UINT16 contextID,
                              IN INPUT_BTC inputs[], IN JUB_UINT16 iCount,
                              IN OUTPUT_BTC outputs[], IN JUB_UINT16 oCount,
                              IN JUB_UINT32 lockTime,
                              OUT JUB_CHAR_PTR_PTR raw);

/*****************************************************************************
 * @function name : JUB_SetUnitBTC
 * @in  param : contextID - context ID
 *            : unit
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetUnitBTC(IN JUB_UINT16 contextID,
                      IN JUB_BTC_UNIT_TYPE unit);

/*****************************************************************************
 * @function name : JUB_BuildUSDTOutputs
 * @in  param : contextID - context ID
 *            : USDTTo - to address
 *            : amount
 * @out param : outputs
 * @last change : build the return0 and dust 2 outputs
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_BuildUSDTOutputs(IN JUB_UINT16 contextID,
                            IN JUB_CHAR_PTR USDTTo,
                            IN JUB_UINT64 amount,
                            OUT OUTPUT_BTC outputs[2]);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif /* JUB_SDK_BTC_h */
