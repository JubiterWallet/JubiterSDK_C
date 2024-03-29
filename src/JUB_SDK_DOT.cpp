//
//  JUB_SDK_DOT.cpp
//  JubSDK
//
//  Created by Pan Min on 2020/11/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include "JUB_SDK_DOT.h"

#include "utility/util.h"
#include "utility/mutex.h"

#include "context/DOTContextFactory.h"


JUB_RV _allocMem(JUB_CHAR_PTR_PTR memPtr, const std::string &strBuf);

/*****************************************************************************
 * @function name : JUB_CreateContextDOT
 * @in  param : cfg
 *          : deviceID - device ID
 * @out param : contextID
 * @last change :
 *****************************************************************************/
JUB_RV JUB_CreateContextDOT(IN CONTEXT_CONFIG_DOT cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::DOTseriesContextFactory::GetInstance()->CreateContext(deviceID, cfg);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->ActiveSelf());
    *contextID = jub::context::ContextManager::GetInstance()->AddOne(context);

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetAddressDOT
 * @in  param : contextID - context ID
 *          : path
 *          : bShow
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetAddressDOT(IN JUB_UINT16 contextID,
                         IN JUB_CHAR_PTR    path,
                         IN JUB_ENUM_BOOL bShow,
                         OUT JUB_CHAR_PTR_PTR address) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::DOTContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_address;
    JUB_VERIFY_RV(context->GetAddress(path, bShow, str_address));
    JUB_VERIFY_RV(_allocMem(address, str_address));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetHDNodeDOT
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX(0x00) for hex;
 *          : path
 * @out param : pubkey
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetHDNodeDOT(IN JUB_UINT16 contextID,
                        IN JUB_ENUM_PUB_FORMAT format,
                        IN JUB_CHAR_PTR path,
                        OUT JUB_CHAR_PTR_PTR pubkey) {

    CREATE_THREAD_LOCK_GUARD
	auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::DOTContext>(contextID);
	JUB_CHECK_NULL(context);

    std::string str_pubkey;
    JUB_VERIFY_RV(context->GetHDNode((JUB_BYTE)format, path, str_pubkey));
    JUB_VERIFY_RV(_allocMem(pubkey, str_pubkey));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetMainHDNodeDOT
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX(0x00) for hex;
 * @out param : xpub
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetMainHDNodeDOT(IN JUB_UINT16 contextID,
                            IN JUB_ENUM_PUB_FORMAT format,
                            OUT JUB_CHAR_PTR_PTR xpub) {

    CREATE_THREAD_LOCK_GUARD
	auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::DOTContext>(contextID);
	JUB_CHECK_NULL(context);

    std::string str_xpub;
    JUB_VERIFY_RV(context->GetMainHDNode((JUB_BYTE)format, str_xpub));
    JUB_VERIFY_RV(_allocMem(xpub, str_xpub));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_CheckAddressDOT
 * @in  param : contextID - context ID
 *            : address
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_RV JUB_CheckAddressDOT(IN JUB_UINT16 contextID, IN JUB_CHAR_CPTR address) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::DOTContext>(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->CheckAddress(address));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_SignTransactionDOT
 * @in  param : contextID - context ID
 *          : path
 *          : nonce - nonce
 *          : gasLimit - gas limit
 *          : gasPriceInAtto- gas price in attoDOT
 *          : to
 *          : valueInAtto - value in attoDOT
 *          : input
 * @out param : raw
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTransactionDOT(IN JUB_UINT16 contextID,
                              IN JUB_CHAR_PTR path,
                              IN JUB_TX_DOT tx,
                              OUT JUB_CHAR_PTR_PTR raw) {

    CREATE_THREAD_LOCK_GUARD
	auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::DOTContext>(contextID);
	JUB_CHECK_NULL(context);

    std::string str_raw;
    JUB_VERIFY_RV(context->SignTransaction(path,
                                           tx,
                                           str_raw));
    JUB_VERIFY_RV(_allocMem(raw, str_raw));

    return JUBR_OK;
}


/*****************************************************************************
 * @function name : JUB_IsValidAddressDOT
 * @in  param : address
 * @out param : none
 * @last change :
 *****************************************************************************/
JUB_RV JUB_IsValidAddressDOT(IN JUB_CHAR_CPTR address) {

    CREATE_THREAD_LOCK_GUARD

    if (JUBR_OK != jub::token::JubiterBaseDOTImpl::IsValidAddress(address, TWCoinType::TWCoinTypePolkadot)) {
        JUB_VERIFY_RV(jub::token::JubiterBaseDOTImpl::IsValidAddress(address, TWCoinType::TWCoinTypePolkadot, true));
    }

    return JUBR_OK;
}
