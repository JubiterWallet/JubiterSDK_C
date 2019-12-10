//
//  JUB_SDK_XRP.h
//  JubSDK
//
//  Created by Pan Min on 2019/11/25.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_XRP_h
#define JUB_SDK_XRP_h

#include "JUB_SDK.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

typedef enum class JubXRPPubFormat {
    HEX = 0x00,
    XRP = 0x01,
    NS_ITEM_PUB_FORMAT
} JUB_ENUM_XRP_PUB_FORMAT;

typedef struct {
    JUB_CHAR_PTR        mainPath;
} CONTEXT_CONFIG_XRP;

typedef enum class JubXRPTxType {
    PYMT = 0x00,
    NS_ITEM_TX_TYPE
} JUB_ENUM_XRP_TX_TYPE;

typedef enum class JubXRPPaymentType {
    DXRP   = 0x00,  // Direct XRP payments
//    FX     = 0x01,  // Cross-currency payments(foreign exchange)
//    CHECKS = 0x02,  // Checks
//    ESCROW = 0x03,  // Escrow
//    PRTL   = 0x04,  // Partial Payments
//    CHAN   = 0X05,  // Payment Channels
    NS_ITEM_PYMT_TYPE
} JUB_ENUM_XRP_PYMT_TYPE;

typedef struct stAmount {
    JUB_CHAR_PTR currency;
    JUB_CHAR_PTR value;
    JUB_CHAR_PTR issuer;
} JUB_PYMT_AMOUNT;

typedef struct stPaymentXRP {
    JUB_ENUM_XRP_PYMT_TYPE type;
    JUB_PYMT_AMOUNT amount;
    JUB_CHAR_PTR destination;
    JUB_CHAR_PTR destinationTag;
    JUB_CHAR_PTR invoiceID;     // [Optional]
    JUB_PYMT_AMOUNT sendMax;    // [Optional]
    JUB_PYMT_AMOUNT deliverMin; // [Optional]
} JUB_PYMT_XRP;
typedef JUB_PYMT_XRP* JUB_PYMT_XRP_PTR;

typedef struct stTxXRP {
    JUB_CHAR_PTR account;
    JUB_ENUM_XRP_TX_TYPE type;
    JUB_CHAR_PTR fee;
    JUB_CHAR_PTR sequence;
    JUB_CHAR_PTR accountTxnID;
    JUB_CHAR_PTR flags;
    JUB_CHAR_PTR lastLedgerSequence;
    JUB_CHAR_PTR memos;
    JUB_CHAR_PTR sourceTag;
    union {
        JUB_PYMT_XRP pymt;
    };
} JUB_TX_XRP;
typedef JUB_TX_XRP* JUB_TX_XRP_PTR;

/*****************************************************************************
 * @function name : JUB_CreateContextXRP
 * @in  param : cfg
 *          : deviceID - device ID
 * @out param : contextID
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CreateContextXRP(IN CONTEXT_CONFIG_XRP cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID);

/*****************************************************************************
 * @function name : JUB_GetAddressXRP
 * @in  param : contextID - context ID
 *          : path
 *          : bShow
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetAddressXRP(IN JUB_UINT16 contextID,
                         IN BIP44_Path path,
                         IN JUB_ENUM_BOOL bShow,
                         OUT JUB_CHAR_PTR_PTR address);

/*****************************************************************************
 * @function name : JUB_GetHDNodeXRP
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_XRP_PUB_FORMAT::HEX(0x00) for hex;
 *                 JUB_ENUM_XRP_PUB_FORMAT::XRP(0x01) for Legacy
 *          : path
 * @out param : pubkey
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetHDNodeXRP(IN JUB_UINT16 contextID,
                        IN JUB_ENUM_XRP_PUB_FORMAT format,
                        IN BIP44_Path path,
                        OUT JUB_CHAR_PTR_PTR pubkey);

/*****************************************************************************
 * @function name : JUB_GetMainHDNodeXRP
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_XRP_PUB_FORMAT::HEX(0x00) for hex;
 *                 JUB_ENUM_XRP_PUB_FORMAT::XRP(0x01) for Legacy
 * @out param : xpub
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetMainHDNodeXRP(IN JUB_UINT16 contextID,
                            IN JUB_ENUM_XRP_PUB_FORMAT format,
                            OUT JUB_CHAR_PTR_PTR xpub);

/*****************************************************************************
 * @function name : JUB_SetMyAddressXRP
 * @in  param : contextID - context ID
 *          : path
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetMyAddressXRP(IN JUB_UINT16 contextID,
                           IN BIP44_Path path,
                           OUT JUB_CHAR_PTR_PTR address);

/*****************************************************************************
* @function name : JUB_SignTransactionXRP
* @in  param : contextID - context ID
*                     : path
*                     : tx - JUB_TX_XRP
* @out param : raw
* @last change :
*****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTransactionXRP(IN JUB_UINT16 contextID,
                              IN BIP44_Path path,
                              IN JUB_TX_XRP tx,
                              OUT JUB_CHAR_PTR_PTR raw);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif /* JUB_SDK_XRP_h */
