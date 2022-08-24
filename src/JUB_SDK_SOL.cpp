//
//  JUB_SDK_SOL.cpp
//  JubSDK
//
//  Created by Pan Min on 2022/07/12.
//  Copyright © 2022 JuBiter. All rights reserved.
//

#include "JUB_SDK_SOL.h"

#include "JUB_SDK_COMM.h"
#include "utility/mutex.h"
#include "utility/util.h"

#include "context/SOLContextFactory.h"
#include <string>

JUB_RV _allocMem(JUB_CHAR_PTR_PTR memPtr, const std::string &strBuf);

/*****************************************************************************
 * @function name : JUB_CreateContextSOL
 * @in  param : cfg
 *          : deviceID - device ID
 * @out param : contextID
 * @last change :
 *****************************************************************************/
JUB_RV JUB_CreateContextSOL(IN CONTEXT_CONFIG_SOL cfg, IN JUB_UINT16 deviceID, OUT JUB_UINT16 *contextID) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::SOLseriesContextFactory::GetInstance()->CreateContext(deviceID, cfg);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->ActiveSelf());
    *contextID = jub::context::ContextManager::GetInstance()->AddOne(context);

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetHDNodeSOL
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX(0x00) for hex;
 *                 JUB_ENUM_PUB_FORMAT::XPUB(0x01)
 *          : path
 * @out param : pubkey
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetHDNodeSOL(IN JUB_UINT16 contextID, IN JUB_ENUM_PUB_FORMAT format, IN JUB_CHAR_PTR path,
                        OUT JUB_CHAR_PTR_PTR pubkey) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::SOLContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_pubkey;
    JUB_VERIFY_RV(context->GetHDNode((JUB_BYTE)format, path, str_pubkey));
    JUB_VERIFY_RV(_allocMem(pubkey, str_pubkey));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetMainHDNodeSOL
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX(0x00) for hex;
 *                 JUB_ENUM_PUB_FORMAT::XPUB(0x01)
 * @out param : xpub
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetMainHDNodeSOL(IN JUB_UINT16 contextID, IN JUB_ENUM_PUB_FORMAT format, OUT JUB_CHAR_PTR_PTR xpub) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::SOLContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_xpub;
    JUB_VERIFY_RV(context->GetMainHDNode((JUB_BYTE)format, str_xpub));
    JUB_VERIFY_RV(_allocMem(xpub, str_xpub));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetAddressSOL
 * @in  param : contextID - context ID
 *          : path
 *          : bShow
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetAddressSOL(IN JUB_UINT16 contextID, IN JUB_CHAR_PTR path, IN JUB_ENUM_BOOL bShow,
                         OUT JUB_CHAR_PTR_PTR address) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::SOLContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_address;
    JUB_VERIFY_RV(context->GetAddress(path, bShow, str_address));
    JUB_VERIFY_RV(_allocMem(address, str_address));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_CheckAddressSOL
 * @in  param : contextID - context ID
 *            : address
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CheckAddressSOL(IN JUB_UINT16 contextID, IN JUB_CHAR_CPTR address) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::SOLContext>(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->CheckAddress(address));

    return JUBR_OK;
}

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
JUB_RV JUB_SetTokenSOL(IN JUB_UINT16 contextID, IN JUB_CHAR_CPTR name, IN JUB_UINT16 decimal,
                       IN JUB_CHAR_CPTR tokenMint) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::SOLContext>(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->SetToken(name, decimal, tokenMint));
    return JUBR_OK;
}

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
JUB_RV JUB_SignTransactionSOL(IN JUB_UINT16 contextID, IN JUB_CHAR_CPTR path, IN JUB_CHAR_CPTR recentHash,
                              IN JUB_CHAR_CPTR dest, IN JUB_UINT64 amount, OUT JUB_CHAR_PTR_PTR txRaw) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::SOLContext>(contextID);
    JUB_CHECK_NULL(context);
    std::string str_raw;
    JUB_VERIFY_RV(context->SignTransferTx(path, recentHash, dest, amount, str_raw));
    JUB_VERIFY_RV(_allocMem(txRaw, str_raw));
    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_SignTransactionTokenSOL
 * @in  param : contextID - context ID
 *          : path - path of funding account
 *          : recentHash - recent block hash of Solana network
 *          : source - the source account
 *          : dest - receipt account
 *          : amount - token transfer amount
 *          : decimal - token decimal
 * @out param : txRaw signed Solana tx
 * @last change :
 *****************************************************************************/
JUB_RV JUB_SignTransactionTokenSOL(IN JUB_UINT16 contextID, IN JUB_CHAR_CPTR path, IN JUB_CHAR_CPTR recentHash,
                                   IN JUB_CHAR_CPTR tokenMint, IN JUB_CHAR_CPTR srouce, IN JUB_CHAR_CPTR dest,
                                   IN JUB_UINT64 amount, IN JUB_UINT8 decimal, OUT JUB_CHAR_PTR_PTR txRaw) {
    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::SOLContext>(contextID);
    JUB_CHECK_NULL(context);
    std::string str_raw;
    JUB_VERIFY_RV(context->SignTokenTransferTx(path, recentHash, tokenMint, srouce, dest, amount, decimal, str_raw));
    JUB_VERIFY_RV(_allocMem(txRaw, str_raw));
    return JUBR_OK;
}
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
JUB_RV JUB_SignTransactionAssociatedTokenCreateSOL(IN JUB_UINT16 contextID, IN JUB_CHAR_CPTR path,
                                                   IN JUB_CHAR_CPTR recentHash, IN JUB_CHAR_CPTR owner,
                                                   IN JUB_CHAR_CPTR tokenMint, OUT JUB_CHAR_PTR_PTR txRaw) {
    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::SOLContext>(contextID);
    JUB_CHECK_NULL(context);
    std::string str_raw;
    JUB_VERIFY_RV(context->SignCreateTokenAccountTx(path, recentHash, owner, tokenMint, str_raw));
    JUB_VERIFY_RV(_allocMem(txRaw, str_raw));
    return JUBR_OK;
};

/*****************************************************************************
 * @function name : JUB_BuildActionSOL
 * @in  param : contextID - context ID
 *          : actions - action array
 *          : actionCount - the count of action array
 * @out param : actionsInJSON
 * @last change :
 *****************************************************************************/
// JUB_COINCORE_DLL_EXPORT
// JUB_RV JUB_BuildActionSOL(IN JUB_UINT16 contextID,
//                           IN JUB_ACTION_EOS_PTR actions,
//                           IN JUB_UINT16 actionCount,
//                           OUT JUB_CHAR_PTR_PTR actionsInJSON) {
//
//     CREATE_THREAD_LOCK_GUARD
//     auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::SOLContext>(contextID);
//     JUB_CHECK_NULL(context);
//
//     std::string str_actions;
//     JUB_VERIFY_RV(context->BuildAction(actions,
//                                        actionCount,
//                                        str_actions));
//     JUB_VERIFY_RV(_allocMem(actionsInJSON, str_actions));
//
//     return JUBR_OK;
// }
//
//
/*****************************************************************************
 * @function name : JUB_IsValidAddressSOL
 * @in  param : address
 * @out param :
 * @last change :
 *****************************************************************************/
// JUB_COINCORE_DLL_EXPORT
// JUB_RV JUB_IsValidAddressSOL(IN JUB_CHAR_CPTR address) {
//
//     CREATE_THREAD_LOCK_GUARD
//
//     JUB_VERIFY_RV(jub::token::JubiterBaseSOLImpl::IsValidAddress(address));
//
//     return JUBR_OK;
// }
