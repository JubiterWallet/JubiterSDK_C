//
//  JUB_SDK_EOS.cpp
//  JubSDK
//
//  Created by Pan Min on 2019/10/09.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_EOS.h"

#include "utility/util.h"

#include "context/ContextEOS.h"
#include "token/interface/TokenInterface.hpp"

JUB_RV _allocMem(JUB_CHAR_PTR_PTR memPtr, const std::string &strBuf);

/*****************************************************************************
 * @function name : JUB_CreateContextEOS
 * @in  param : cfg
 *            : deviceID - device ID
 * @out param : contextID
 * @last change :
 *****************************************************************************/
JUB_RV JUB_CreateContextEOS(IN CONTEXT_CONFIG_EOS cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID) {

    if (nullptr == jub::TokenManager::GetInstance()->GetOne(deviceID)) {
        return JUBR_ARGUMENTS_BAD;
    }

    jub::ContextEOS* context = new jub::ContextEOS(cfg, deviceID);
    *contextID = jub::ContextManager::GetInstance()->AddOne(context);
    context->ActiveSelf();

    return JUBR_IMPL_NOT_SUPPORT;
}

/*****************************************************************************
 * @function name : JUB_GetAddressEOS
 * @in  param : contextID - context ID
 *            : path
 *            : bShow
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetAddressEOS(IN JUB_UINT16 contextID,
                         IN BIP32_Path    path,
                         IN JUB_ENUM_BOOL bShow,
                         OUT JUB_CHAR_PTR_PTR address) {

    JUB_CHECK_CONTEXT_EOS(contextID);

    auto context = (jub::ContextEOS*)jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

    std::string str_address;
    JUB_VERIFY_RV(context->GetAddress(path, bShow, str_address));
    JUB_VERIFY_RV(_allocMem(address, str_address));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_SetMyAddressEOS
 * @in  param : contextID - context ID
 *            : path
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_RV JUB_SetMyAddressEOS(IN JUB_UINT16 contextID,
                           IN BIP32_Path path,
                           OUT JUB_CHAR_PTR_PTR address) {

    JUB_CHECK_CONTEXT_EOS(contextID);

    auto context = (jub::ContextEOS*)jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

    std::string str_address;
    JUB_VERIFY_RV(context->SetMyAddress(path, str_address));
    JUB_VERIFY_RV(_allocMem(address, str_address));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetHDNodeEOS
 * @in  param : contextID - context ID
 *            : format - JUB_EOS_PUB_FORMAT::HEX(0x00) for hex;
 *                       JUB_EOS_PUB_FORMAT::EOS(0x01) for Legacy
 *            : path
 * @out param : pubkey
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetHDNodeEOS(IN JUB_UINT16 contextID,
                        IN JUB_EOS_PUB_FORMAT format,
                        IN BIP32_Path path,
                        OUT JUB_CHAR_PTR_PTR pubkey) {

    JUB_CHECK_CONTEXT_EOS(contextID);

    auto context = (jub::ContextEOS*)jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

    std::string str_pubkey;
    JUB_VERIFY_RV(context->GetHDNode((JUB_BYTE)format, path, str_pubkey));
    JUB_VERIFY_RV(_allocMem(pubkey, str_pubkey));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetMainHDNodeEOS
 * @in  param : contextID - context ID
 *            : format - JUB_EOS_PUB_FORMAT::HEX(0x00) for hex;
 *                       JUB_EOS_PUB_FORMAT::EOS(0x01) for Legacy
 * @out param : xpub
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetMainHDNodeEOS(IN JUB_UINT16 contextID,
                            IN JUB_EOS_PUB_FORMAT format,
                            OUT JUB_CHAR_PTR_PTR xpub) {

    JUB_CHECK_CONTEXT_EOS(contextID);

    auto context = (jub::ContextEOS*)jub::ContextManager::GetInstance()->GetOne(contextID);
    JUB_CHECK_NULL(context);

    std::string str_xpub;
    JUB_VERIFY_RV(context->GetMainHDNode((JUB_BYTE)format, str_xpub));
    JUB_VERIFY_RV(_allocMem(xpub, str_xpub));

    return JUBR_OK;
}
