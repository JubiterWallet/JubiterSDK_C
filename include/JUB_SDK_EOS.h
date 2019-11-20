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

typedef enum class JubEOSActionType {
    XFER   = 0x00, // transfer
      DELE = 0x01, //   delegatebw
    UNDELE = 0x02, // undelegatebw
    BUYRAM = 0x03, //  buyrambytes
   SELLRAM = 0x04, // sellrambytes
    NS_ITEM_ACTION_TYPE
} JUB_ENUM_EOS_ACTION_TYPE;

typedef struct stTransferAction {
    JUB_CHAR_PTR from;
    JUB_CHAR_PTR to;
    JUB_CHAR_PTR asset;
    JUB_CHAR_PTR memo;
} JUB_ACTION_TRANSFER;

typedef struct stDelegateAction {
    JUB_CHAR_PTR from;
    JUB_CHAR_PTR receiver;
    JUB_CHAR_PTR netQty; // stake_net_quantity/unstake_net_quantity
    JUB_CHAR_PTR cpuQty; // stake_cpu_quantity/unstake_cpu_quantity
    bool bStake;
} JUB_ACTION_DELEGATE;

typedef struct stBuyRamAction {
    JUB_CHAR_PTR payer;
    JUB_CHAR_PTR quant;
    JUB_CHAR_PTR receiver;
} JUB_ACTION_BUYRAM;

typedef struct stSellRamAction {
    JUB_CHAR_PTR account;
    JUB_CHAR_PTR bytes;
} JUB_ACTION_SELLRAM;

typedef struct stActionEOS {
    JUB_ENUM_EOS_ACTION_TYPE type;
    JUB_CHAR_PTR currency;
    JUB_CHAR_PTR name;
    union {
        JUB_ACTION_TRANSFER transfer;
        JUB_ACTION_DELEGATE delegate;
        JUB_ACTION_BUYRAM buyRam;
        JUB_ACTION_SELLRAM sellRam;
    };
} JUB_ACTION_EOS;
typedef JUB_ACTION_EOS* JUB_ACTION_EOS_PTR;

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
 *          : path
 *          : chainID - chain ID
 *          : expiration - expiration, eg, 300(s)
 *          : referenceBlockId   - reference block ID
 *          : referenceBlockTime - reference block time
 *          : actionsInJSON - array of actions
 * @out param : rawInJSON
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTransactionEOS(IN JUB_UINT16 contextID,
                              IN BIP44_Path path,
                              IN JUB_CHAR_PTR chainID,
                              IN JUB_CHAR_PTR expiration,
                              IN JUB_CHAR_PTR referenceBlockId,
                              IN JUB_CHAR_PTR referenceBlockTime,
                              IN JUB_CHAR_PTR actionsInJSON,
                              OUT JUB_CHAR_PTR_PTR rawInJSON);

/*****************************************************************************
 * @function name : JUB_BuildActionEOS
 * @in  param : contextID - context ID
 *          : actions - action array
 *          : actionCount - the count of action array
 * @out param : actionsInJSON
 * @last change :
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_BuildActionEOS(IN JUB_UINT16 contextID,
                          IN JUB_ACTION_EOS_PTR actions,
                          IN JUB_UINT16 actionCount,
                          OUT JUB_CHAR_PTR_PTR actionsInJSON);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif /* JUB_SDK_EOS_h */
