/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   JUB_core.h
 * Author: zxc
 *
 * Created on 2019年7月18日, 上午10:58
 */

#ifndef JUB_CORE_H
#define JUB_CORE_H

//硬件接口 + 软件接口的头文件，外部使用，只需要包含这一个

#include "JUB_SDK.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

typedef enum {
    STRENGTH128 = 128,
    STRENGTH192 = 192,
    STRENGTH256 = 256
} JUB_ENUM_MNEMONIC_STRENGTH;

// The definition is the same as the COS definition
typedef enum {
    secp256k1 = 0,
    nist256p1,
    ed25519
} JUB_ENUM_CURVES;

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GenerateMnemonic(IN JUB_ENUM_MNEMONIC_STRENGTH strength,
                            OUT JUB_CHAR_PTR_PTR mnemonic);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CheckMnemonic(IN JUB_CHAR_CPTR mnemonic);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GenerateSeed(IN JUB_CHAR_CPTR mnemonic, IN JUB_CHAR_CPTR passphrase,
                        OUT JUB_BYTE seed[64],
                        void (*progress_callback)(JUB_UINT32 current, JUB_UINT32 total));

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SeedToMasterPrivateKey(IN JUB_BYTE_CPTR seed, IN JUB_UINT16 seed_len, IN JUB_ENUM_CURVES curve,
                                  OUT JUB_CHAR_PTR_PTR prikeyInXPRV);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CreateContextBTC_soft(IN CONTEXT_CONFIG_BTC cfg,
                                 IN JUB_CHAR_CPTR XPRVorXPUB,
                                 OUT JUB_UINT16* contextID);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CreateContextHC_soft(IN CONTEXT_CONFIG_HC cfg,
                                IN JUB_CHAR_CPTR masterPriInXPRV,
                                OUT JUB_UINT16* contextID);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CreateContextETH_soft(IN CONTEXT_CONFIG_ETH cfg,
                                 IN JUB_CHAR_CPTR masterPriInXPRV,
                                 OUT JUB_UINT16* contextID);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CreateContextEOS_soft(IN CONTEXT_CONFIG_EOS cfg,
                                 IN JUB_CHAR_CPTR masterPriInXPRV,
                                 OUT JUB_UINT16* contextID);

JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CreateContextXRP_soft(IN CONTEXT_CONFIG_XRP cfg,
                                 IN JUB_CHAR_CPTR masterPriInXPRV,
                                 OUT JUB_UINT16* contextID);
#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif /* JUB_CORE_H */
