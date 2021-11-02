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


#define JUBR_NOT_FOUND_INPUT_PUKEY_INFO            0x40006420UL
#define JUBR_NOT_FOUND_BIP32PATH                   0x40006421UL
#define JUBR_NO_CORRECT_BIP32PATH_FORMAT           0x40006422UL
#define JUBR_NOT_FOUND_TRANSCATION_RAW_TEXT        0x40006431UL
#define JUBR_NOT_FOUND_AMOUNTS_DATA                0x40006432UL
#define JUBR_NOT_FOUND_CHANGE_ADDR_IDX             0x40006433UL

#define JUBR_SIGN_FAILED                           0x40006F08UL
//#define JUBR_REJECT                                0x40006F09UL
#define JUBR_INTER_SYSTEM_FAILED                   0x40006F81UL
#define JUBR_ALGORITHM_NOT_SUPPORTED               0x40006F83UL
#define JUBR_PADDING_NOT_SUPPORTED                 0x40006F84UL
#define JUBR_NEED_PERSONALIZED_FINISH              0x40006A91UL
#define JUBR_NEED_MASTER_KEY                       0x40006A92UL
#define JUBR_NEED_SELECT_LANGUAGE                  0x40006A93UL
#define JUBR_DER_DECODE                            0x40006A94UL
#define JUBR_STEPOUT                               0x40006A95UL
#define JUBR_API_USAGE_WRONG                       0x40006A96UL
#define JUBR_OUT_OF_STORAGE                        0x40006A97UL
#define JUBR_NOT_FOUND_OUTPUT                      0x40006A98UL
#define JUBR_NOT_KNOWND_ADDRE_TYPE                 0x40006A99UL
#define JUBR_CHANGE_ADDR_NOT_MATCH                 0x40006A9AUL
#define JUBR_CHANGE_ADDR_NUM_NOT_MATCH             0x40006A9BUL
#define JUBR_CONFIRM_PAGE_INDEX                    0x40006A9CUL
#define JUBR_ADD_OR_SUB_OVERFLOW                   0x40006A9DUL
#define JUBR_REGISTER_FAILED                       0x40006A9EUL
#define JUBR_GET_COIN_DATA_FAILED                  0x40006A9FUL
#define JUBR_OUTPUTS_NUM                           0x40006AA0UL
#define JUBR_ADDCOININFO_FAILED					   0x40006AA1UL
#define JUBR_TRANSATION_TYPE                       0x40006AA2UL
#define JUBR_TRANSATION_STATE                      0x40006AA3UL
#define JUBR_TIMER_OUT                             0x40006AA4UL
#define JUBR_NOT_SUPPORT_COIND_TYPE                0x40006AA5UL
#define JUBR_NOT_SUPPORT_HASH_TYPE                 0x40006AA6UL
#define JUBR_NOT_SUPPORT_FORID                     0x40006AA7UL
#define JUBR_USDT_NUMBER_NOT_MATCH_DUST            0x40006AA8UL
#define JUBR_CHANGE_ADDR_RULE                      0x40006AA9UL
#define JUBR_QUTM_ERC20_TOKEN_ADDR_NOT_MATCH       0x40006AA0UL


#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

typedef enum {
    COINBTC  = 0x00,
    COINBCH  = 0x01,
    COINLTC  = 0x02,
    COINUSDT = 0x03,
    COINDASH = 0x04,
    COINQTUM = 0x05,
    COINCKB  = 0x06,
    Default  = COINBTC
} JUB_ENUM_COINTYPE_BTC;

typedef enum {
    p2pkh = 0,
    p2sh_p2wpkh,
    p2wpkh,
    trans_type_ns_item
/*
     p2sh_multisig,
     p2wsh_multisig,
     p2sh_p2wsh_multisig,
*/
} JUB_ENUM_BTC_TRANS_TYPE;

typedef enum {
    BTC = 0x00,
    cBTC,
    mBTC,
    uBTC,
    Satoshi,
    ns
} JUB_ENUM_BTC_UNIT_TYPE;

typedef enum {
    OWN = 0x00,
    LEGACY,
    DFLT = OWN
} JUB_ENUM_BTC_ADDRESS_FORMAT;

// Remove c++ features for swift framework
//typedef struct stContextCfgBTC : stContextCfg {
typedef struct stContextCfgBTC {
    JUB_CHAR_PTR             mainPath;
    JUB_ENUM_COINTYPE_BTC    coinType;// = { JUB_ENUM_COINTYPE_BTC::COINBTC };
    JUB_ENUM_BTC_TRANS_TYPE transType;
//
//    stContextCfgBTC();
//    stContextCfgBTC(JUB_ENUM_COINTYPE_BTC _coinType,
//                    JUB_CHAR_PTR _mainPath,
//                    JUB_ENUM_BTC_TRANS_TYPE _transType);
//    virtual ~stContextCfgBTC() {}
} CONTEXT_CONFIG_BTC;
// Remove c++ features for swift framework end

typedef enum {
    P2PKH   = 0x00,
    RETURN0 = 0x01,
//    P2SH_MULTISIG = 0x02,
    QRC20 = 0x03,
} JUB_ENUM_SCRIPT_BTC_TYPE;

// Remove c++ features for swift framework
typedef struct stInput {
    JUB_ENUM_SCRIPT_BTC_TYPE type;
    JUB_CHAR_PTR    preHash;
    JUB_UINT16      preIndex;
    JUB_UINT32      nSequence;
    JUB_UINT64      amount;
    BIP44_Path      path;
//
//     stInput();
//    ~stInput() = default;
} INPUT_BTC;
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
typedef struct stOutput {
    JUB_CHAR_PTR    address;
    JUB_UINT64      amount;
    JUB_ENUM_BOOL   changeAddress;
    BIP44_Path      path;
//
//     stOutput();
//    ~stOutput() = default;
} OUTPUT;
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
typedef struct stOutputReturn0 {
    JUB_UINT64      amount;
    JUB_UINT16      dataLen;
    JUB_BYTE        data[40];
//
//     stOutputReturn0();
//    ~stOutputReturn0() = default;
} OUTPUT_RETURN0;
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
typedef struct stOutputQRC20 {
    JUB_UINT16      dataLen;
    JUB_BYTE        data[200];
//
//     stOutputQRC20();
//    ~stOutputQRC20() = default;
} OUTPUT_QRC20;
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
typedef struct stOutputBTC {
    JUB_ENUM_SCRIPT_BTC_TYPE type;
    union {
        OUTPUT stdOutput;
        OUTPUT_RETURN0 return0;
        OUTPUT_QRC20 qrc20;
    };
//
//     stOutputBTC();
//    ~stOutputBTC() = default;
} OUTPUT_BTC;
// Remove c++ features for swift framework end

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
                        IN BIP44_Path path,
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
                         IN BIP44_Path path,
                         IN JUB_ENUM_BOOL bShow,
                         OUT JUB_CHAR_PTR_PTR address);

/*****************************************************************************
 * @function name : JUB_CheckAddressBTC
 * @in  param : contextID - context ID
 *            : address
 * @out param : 
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CheckAddressBTC(IN JUB_UINT16 contextID, IN JUB_CHAR_CPTR address);

/*****************************************************************************
 * @function name : JUB_SetMyAddressBTC
 * @in  param : contextID - context ID
 *            : path
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetMyAddressBTC(IN JUB_UINT16 contextID,
                           IN BIP44_Path path,
                           OUT JUB_CHAR_PTR_PTR address);

/*****************************************************************************
 * @function name : JUB_SignTransactionBTC
 * @in  param : contextID - context ID
 *            : version
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
                              IN JUB_UINT32 version,
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
                      IN JUB_ENUM_BTC_UNIT_TYPE unit);

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
                            IN JUB_CHAR_CPTR USDTTo,
                            IN JUB_UINT64 amount,
                            OUT OUTPUT_BTC outputs[2]);


/*****************************************************************************
 * @function name : BuildQRC20Outputs
 * @in  param : contextID - context ID
 *          : contractAddress - contract address for QRC20 token
 *          : decimal  - decimal for QRC20 token
 *          : symbol  - symbol for QRC20 token
 *          : gasLimit - gas limit
 *          : gasPrice - gas price
 *          : to  - to address for transfer
 *          : value - amount for transfer
 * @out param : outputs
 * @last change : build the QRC20 outputs
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_BuildQRC20Outputs(IN JUB_UINT16 contextID,
                             IN JUB_CHAR_CPTR contractAddress, IN JUB_UINT8 decimal, IN JUB_CHAR_CPTR symbol,
                             IN JUB_UINT64 gasLimit, IN JUB_UINT64 gasPrice,
                             IN JUB_CHAR_CPTR to, IN JUB_CHAR_CPTR value,
                             OUT OUTPUT_BTC outputs[1]);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif /* JUB_SDK_BTC_h */
