//
//  JUB_SDK_XRP.cpp
//  JubSDK
//
//  Created by Pan Min on 2019/11/25.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_XRP.h"

#include "utility/util.h"

#include "context/ContextXRP.h"
#include "token/interface/TokenInterface.hpp"

JUB_RV _allocMem(JUB_CHAR_PTR_PTR memPtr, const std::string &strBuf);

/*****************************************************************************
 * @function name : JUB_CreateContextXRP
 * @in  param : cfg
 *          : deviceID - device ID
 * @out param : contextID
 * @last change :
 *****************************************************************************/
JUB_RV JUB_CreateContextXRP(IN CONTEXT_CONFIG_XRP cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID) {

    if (nullptr == jub::TokenManager::GetInstance()->GetOne(deviceID)) {
        return JUBR_ARGUMENTS_BAD;
    }

    jub::ContextXRP* context = new jub::ContextXRP(cfg, deviceID);
    *contextID = jub::ContextManager::GetInstance()->AddOne(context);
    context->ActiveSelf();

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetAddressXRP
 * @in  param : contextID - context ID
 *          : path
 *          : bShow
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetAddressXRP(IN JUB_UINT16 contextID,
                         IN BIP44_Path    path,
                         IN JUB_ENUM_BOOL bShow,
                         OUT JUB_CHAR_PTR_PTR address) {

    JUB_CHECK_CONTEXT_XRP(contextID);

    auto context = (jub::ContextXRP*)jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

    std::string str_address;
    JUB_VERIFY_RV(context->GetAddress(path, bShow, str_address));
    JUB_VERIFY_RV(_allocMem(address, str_address));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_SetMyAddressXRP
 * @in  param : contextID - context ID
 *            : path
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_RV JUB_SetMyAddressXRP(IN JUB_UINT16 contextID,
                           IN BIP44_Path path,
                           OUT JUB_CHAR_PTR_PTR address) {

    JUB_CHECK_CONTEXT_XRP(contextID);

    auto context = (jub::ContextXRP*)jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

    std::string str_address;
    JUB_VERIFY_RV(context->SetMyAddress(path, str_address));
    JUB_VERIFY_RV(_allocMem(address, str_address));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetHDNodeXRP
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX(0x00) for hex;
 *          : path
 * @out param : pubkey
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetHDNodeXRP(IN JUB_UINT16 contextID,
                        IN JUB_ENUM_PUB_FORMAT format,
                        IN BIP44_Path path,
                        OUT JUB_CHAR_PTR_PTR pubkey) {

    JUB_CHECK_CONTEXT_XRP(contextID);

    auto context = (jub::ContextXRP*)jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

    std::string str_pubkey;
    JUB_VERIFY_RV(context->GetHDNode((JUB_BYTE)format, path, str_pubkey));
    JUB_VERIFY_RV(_allocMem(pubkey, str_pubkey));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetMainHDNodeXRP
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX(0x00) for hex;
 * @out param : xpub
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetMainHDNodeXRP(IN JUB_UINT16 contextID,
                            IN JUB_ENUM_PUB_FORMAT format,
                            OUT JUB_CHAR_PTR_PTR xpub) {

    JUB_CHECK_CONTEXT_XRP(contextID);

    auto context = (jub::ContextXRP*)jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

    std::string str_xpub;
    JUB_VERIFY_RV(context->GetMainHDNode((JUB_BYTE)format, str_xpub));
    JUB_VERIFY_RV(_allocMem(xpub, str_xpub));

    return JUBR_OK;
}

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
                              OUT JUB_CHAR_PTR_PTR raw) {
    JUB_CHECK_CONTEXT_XRP(contextID);

    auto context = (jub::ContextXRP*)jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

    std::string str_raw;
    JUB_VERIFY_RV(context->SignTransaction(path,
                                           tx,
                                           str_raw));
    JUB_VERIFY_RV(_allocMem(raw, str_raw));

    return JUBR_OK;
}
