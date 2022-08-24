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
#include "JUB_SDK_COMM.h"

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
                        IN JUB_CHAR_PTR path,
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
                         IN JUB_CHAR_PTR path,
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

/*****************************************************************************
 * @function name : JUB_SetTokenSOL
 * @in  param : contextID - context ID
 *          : tokenName - TRX token name
 *          : decimal
 *          : tokenMint
 * @out param : none.
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetTokenSOL(IN JUB_UINT16 contextID,
                       IN JUB_CHAR_CPTR name,
                       IN JUB_UINT16 decimal,
                       IN JUB_CHAR_CPTR tokenMint);

/*****************************************************************************
 * @function name : JUB_SignTransactionSOL
 * @in  param : contextID - context ID
 *          : path - path of funding account
 *          : recentHash - recent block hash of Solana network
 *          : dest - receipt account
 *          : amount - transfer amount in lamports
 * @out param : txRaw signed Solana tx
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTransactionSOL(IN JUB_UINT16 contextID,
                              IN JUB_CHAR_CPTR path,
                              IN JUB_CHAR_CPTR recentHash,
                              IN JUB_CHAR_CPTR dest,
                              IN JUB_UINT64 amount,
                              OUT JUB_CHAR_PTR_PTR txRaw);

/*****************************************************************************
 * @function name : JUB_SignTransactionTokenSOL
 * @in  param : contextID - context ID
 *          : path - path of funding account
 *          : recentHash - recent block hash of Solana network
 *          : tokenMint - the token mint
 *          : source - the source account
 *          : dest - receipt account
 *          : amount - token transfer amount
 *          : decimal - token decimal
 * @out param : txRaw signed Solana tx
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTransactionTokenSOL(IN JUB_UINT16 contextID,
                                   IN JUB_CHAR_CPTR path,
                                   IN JUB_CHAR_CPTR recentHash,
                                   IN JUB_CHAR_CPTR tokenMint,
                                   IN JUB_CHAR_CPTR srouce,
                                   IN JUB_CHAR_CPTR dest,
                                   IN JUB_UINT64 amount,
                                   IN JUB_UINT8 decimal,
                                   OUT JUB_CHAR_PTR_PTR txRaw);

/*****************************************************************************
 * @function name : JUB_SignTransactionAssociatedTokenSOL
 * @in  param : contextID - context ID
 *          : path - path of funding account
 *          : recentHash - recent block hash of Solana network
 *          : owner - owner address
 *          : token - token address
 * @out param : txRaw signed Solana tx
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTransactionAssociatedTokenCreateSOL(IN JUB_UINT16 contextID,
                                                   IN JUB_CHAR_CPTR path,
                                                   IN JUB_CHAR_CPTR recentHash,
                                                   IN JUB_CHAR_CPTR owner,
                                                   IN JUB_CHAR_CPTR tokenMint,
                                                   OUT JUB_CHAR_PTR_PTR txRaw);

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
