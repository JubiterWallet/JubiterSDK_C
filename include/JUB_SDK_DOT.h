//
//  JUB_SDK_DOT.h
//  JubSDK
//
//  Created by Pan Min on 2020/11/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_DOT_h
#define JUB_SDK_DOT_h

#include "JUB_SDK.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

//typedef CONTEXT_CONFIG_DOT CONTEXT_CONFIG_DOT;
typedef enum {
    COINDOT  = 0x00,
    COINKSM  = 0x01,
} JUB_ENUM_COINTYPE_DOT;

typedef struct stContextCfgDOT {
    JUB_CHAR_PTR             mainPath;
    JUB_ENUM_COINTYPE_DOT    coinType;// = { JUB_ENUM_COINTYPE_DOT::COINDOT };
    JUB_ENUM_CURVES          curve; //(sr2551&ed25519)
} CONTEXT_CONFIG_DOT;

//"adress":"14Ztd3KJDaB9xyJtRkREtSZDdhLSbm7UUKt8Z7AwSv7q85G2",
//"genesisHash":"91b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c3",
//"blockHash":"0x5d2143bb808626d63ad7e1cda70fa8697059d670a992e82cd440fbb95ea40351",
//"nonce":3,
//"specVersion":26,
//"private_key":"70a794d4f1019c3ce002f33062f45029c4f930a56b3d20ec477f7668c6bbc37f",
//"network":0,
//"transaction_version":5,
//"blockNumber":3541050,
//"eraPeriod":64
//"to":"13ZLCqJNPsRZYEbwjtZZFpWt9GyFzg5WahXCVWKpWdUJqrQ5",
//"value":"2000000000"
typedef struct stTxDOT {
    JUB_CHAR_PTR genesisHash;
    JUB_CHAR_PTR blockHash;
    JUB_CHAR_PTR to;
    JUB_UINT64 nonce;
    JUB_UINT32 specVersion;
    JUB_UINT64 network;
    JUB_UINT32 transaction_version;
    JUB_UINT64 blockNumber;
    JUB_CHAR_PTR value;
    JUB_UINT64 eraPeriod; // [Optional]
    JUB_UINT64 tip;       // [Optional]
} JUB_TX_DOT;
/*****************************************************************************
 * @function name : JUB_CreateContextDOT
 * @in  param : cfg
 *          : deviceID - device ID
 * @out param : contextID
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CreateContextDOT(IN CONTEXT_CONFIG_DOT cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID);

/*****************************************************************************
 * @function name : JUB_GetAddressDOT
 * @in  param : contextID - context ID
 *          : path
 *          : bShow
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetAddressDOT(IN JUB_UINT16 contextID,
                         IN JUB_CHAR_PTR path,
                         IN JUB_ENUM_BOOL bShow,
                         OUT JUB_CHAR_PTR_PTR address);

/*****************************************************************************
 * @function name : JUB_GetHDNodeDOT
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX(0x00) for hex;
 *          : path
 * @out param : pubkey
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetHDNodeDOT(IN JUB_UINT16 contextID,
                        IN JUB_ENUM_PUB_FORMAT format,
                        IN JUB_CHAR_PTR path,
                        OUT JUB_CHAR_PTR_PTR pubkey);

/*****************************************************************************
 * @function name : JUB_GetMainHDNodeDOT
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX(0x00) for hex;
 * @out param : xpub
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetMainHDNodeDOT(IN JUB_UINT16 contextID,
                            IN JUB_ENUM_PUB_FORMAT format,
                            OUT JUB_CHAR_PTR_PTR xpub);

/*****************************************************************************
 * @function name : JUB_SetMyAddressDOT
 * @in  param : contextID - context ID
 *          : path
 * @out param : address
 * @last change :
 *****************************************************************************/
//JUB_COINCORE_DLL_EXPORT
//JUB_RV JUB_SetMyAddressDOT(IN JUB_UINT16 contextID,
//                           IN BIP44_Path path,
//                           OUT JUB_CHAR_PTR_PTR address);
//
/*****************************************************************************
 * @function name : JUB_SignTransactionDOT
 * @in  param : contextID - context ID
 *            : tx - JUB_TX_DOT
 * @out param : raw
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTransactionDOT(IN JUB_UINT16 contextID,
                              IN JUB_CHAR_PTR path,
                              IN JUB_TX_DOT tx,
                              OUT JUB_CHAR_PTR_PTR raw);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif /* JUB_SDK_DOT_h */
