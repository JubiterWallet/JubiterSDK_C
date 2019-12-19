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
    COINBTC  = 0x00,
    COINBCH  = 0x01,
    COINLTC  = 0x02,
    COINUSDT = 0x03,
    COINDASH = 0x04,
    COINQTUM = 0x05,
    Default  = COINBTC
} JUB_ENUM_COINTYPE_BTC;

typedef enum {
    p2pkh = 0,
    //p2pwpkh,
    p2sh_p2wpkh,
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
    JUB_CHAR_PTR            mainPath;
    JUB_ENUM_COINTYPE_BTC    coinType;// = { JUB_ENUM_COINTYPE_BTC::COINBTC };
    JUB_ENUM_BTC_TRANS_TYPE transType;

//    stContextCfgBTC() = default;
//    stContextCfgBTC(JUB_ENUM_COINTYPE_BTC _coinType,
//                    JUB_CHAR_PTR _mainPath,
//                    JUB_ENUM_BTC_TRANS_TYPE _transType) {
//         mainPath = _mainPath;
//         coinType =  _coinType;
//        transType = _transType;
//    }
//
//    virtual ~stContextCfgBTC() {}

//    stContextCfgBTC();
//    stContextCfgBTC(JUB_ENUM_COINTYPE_BTC _coinType,
//                    JUB_CHAR_PTR _mainPath,
//                    JUB_ENUM_BTC_TRANS_TYPE _transType);
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

//     stOutput();
//    ~stOutput() = default;
} OUTPUT;
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
typedef struct stOutputReturn0 {
    JUB_UINT64      amount;
    JUB_UINT16      dataLen;
    JUB_BYTE        data[40];

//     stOutputReturn0();
//    ~stOutputReturn0() = default;
} OUTPUT_RETURN0;
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
typedef struct stOutputQRC20 {
    JUB_UINT16      dataLen;
    JUB_BYTE        data[200];

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
 *            : contractAddress - contract address for QRC20 token
 *            : decimal         - decimal for QRC20 token
 *            : symbol          - symbol for QRC20 token
 *            : gasLimit - gas limit
 *            : gasPrice - gas price
 *            : to    - to address for transfer
 *            : value - amount for transfer
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
