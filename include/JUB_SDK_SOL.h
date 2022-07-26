//
//  JUB_SDK_SOL.h
//  JubSDK
//
//  Created by Pan Min on 2022/07/12.
//  Copyright © 2022 JuBiter. All rights reserved.
//

#ifndef JUB_SDK_SOL_h
#define JUB_SDK_SOL_h

#include "JUB_SDK.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

typedef CONTEXT_CONFIG CONTEXT_CONFIG_SOL;

typedef struct stTxSOL {
    struct stHeader {
        // The number of signatures required for this message to be considered
        // valid. The signatures must match the first `numRequiredSignatures` of
        // `accountKeys`.
        JUB_UINT8 numRequiredSignatures;
        // The last numCreditOnlySignedAccounts of the signed keys are
        // credit-only accounts.
        JUB_UINT8 numCreditOnlySignedAccounts;
        // The last numCreditOnlyUnsignedAccounts of the unsigned keys are
        // credit-only accounts.
        JUB_UINT8 numCreditOnlyUnsignedAccounts;
    };
    stHeader header;

//    JUB_CHAR_PTR ref_block_bytes;
//    JUB_CHAR_PTR ref_block_num; // [Optional]
//    JUB_CHAR_PTR ref_block_hash;
//    JUB_CHAR_PTR expiration;
//    JUB_CHAR_PTR data;          // [Optional]
//    JUB_CONTRACT_TRX_PTR contracts;
//    JUB_UINT16 contractCount;
//    JUB_CHAR_PTR scripts;       // [Optional]
//    JUB_CHAR_PTR timestamp;
//    JUB_CHAR_PTR fee_limit;     // [Optional]
//
//     stTxSOL();
//    ~stTxSOL() = default;
//
//    bool isValid() const;
} JUB_TX_SOL;
//// Remove c++ features for swift framework end


/*****************************************************************************
 * @function name : JUB_CreateContextSOL
 * @in  param : cfg
 *          : deviceID - device ID
 * @out param : contextID
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CreateContextSOL(IN CONTEXT_CONFIG_SOL cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID);


/*****************************************************************************
 * @function name : JUB_GetHDNodeSOL
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX(0x00) for hex;
 *                 JUB_ENUM_PUB_FORMAT::XPUB(0x01)
 *          : path
 * @out param : pubkey
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetHDNodeSOL(IN JUB_UINT16 contextID,
                        IN JUB_ENUM_PUB_FORMAT format,
                        IN BIP44_Path path,
//                        IN BIP48_Path path,
                        OUT JUB_CHAR_PTR_PTR pubkey);


/*****************************************************************************
 * @function name : JUB_GetMainHDNodeSOL
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX(0x00) for hex;
 *                 JUB_ENUM_PUB_FORMAT::XPUB(0x01)
 * @out param : xpub
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetMainHDNodeSOL(IN JUB_UINT16 contextID,
                            IN JUB_ENUM_PUB_FORMAT format,
                            OUT JUB_CHAR_PTR_PTR xpub);


/*****************************************************************************
 * @function name : JUB_GetAddressSOL
 * @in  param : contextID - context ID
 *            : path
 *            : bShow
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_GetAddressSOL(IN JUB_UINT16 contextID,
                         IN BIP44_Path path,
//                         IN BIP48_Path path,
                         IN JUB_ENUM_BOOL bShow,
                         OUT JUB_CHAR_PTR_PTR address);


/*****************************************************************************
 * @function name : JUB_CheckAddressSOL
 * @in  param : contextID - context ID
 *            : address
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CheckAddressSOL(IN JUB_UINT16 contextID, IN JUB_CHAR_CPTR address);


///*****************************************************************************
// * @function name : JUB_SetMyAddressSOL
// * @in  param : contextID - context ID
// *            : path
// * @out param : address
// * @last change :
// *****************************************************************************/
//JUB_COINCORE_DLL_EXPORT
//JUB_RV JUB_SetMyAddressSOL(IN JUB_UINT16 contextID,
//                           IN BIP44_Path path,
////                           IN BIP48_Path path,
//                           OUT JUB_CHAR_PTR_PTR address);
//
//
/*****************************************************************************
 * @function name : JUB_SignTransactionSOL
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
JUB_RV JUB_SignTransactionSOL(IN JUB_UINT16 contextID,
                              IN BIP44_Path path,
                              IN JUB_CHAR_CPTR chainID,
                              IN JUB_CHAR_CPTR expiration,
                              IN JUB_CHAR_CPTR referenceBlockId,
                              IN JUB_CHAR_CPTR referenceBlockTime,
                              IN JUB_CHAR_CPTR actionsInJSON,
                              OUT JUB_CHAR_PTR_PTR rawInJSON);


///*****************************************************************************
// * @function name : JUB_BuildActionSOL
// * @in  param : contextID - context ID
// *          : actions - action array
// *          : actionCount - the count of action array
// * @out param : actionsInJSON
// * @last change :
// *****************************************************************************/
//JUB_COINCORE_DLL_EXPORT
//JUB_RV JUB_BuildActionSOL(IN JUB_UINT16 contextID,
//                          IN JUB_ACTION_EOS_PTR actions,
//                          IN JUB_UINT16 actionCount,
//                          OUT JUB_CHAR_PTR_PTR actionsInJSON);
//
//
/*****************************************************************************
 * @function name : JUB_IsValidAddressSOL
 * @in  param : address
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_IsValidAddressSOL(IN JUB_CHAR_CPTR address);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif /* JUB_SDK_SOL_h */
