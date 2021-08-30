//
//  JUB_SDK_CKB.cpp
//  JubSDK
//
//  Created by panmin on 2021/6/15.
//  Copyright © 2021 JuBiter. All rights reserved.
//


#include "JUB_SDK_CKB.h"

#include "utility/util.h"
#include "utility/mutex.h"

#include "context/CKBContextFactory.h"


JUB_RV _allocMem(JUB_CHAR_PTR_PTR memPtr, const std::string &strBuf);


/*****************************************************************************
 * @function name : JUB_CreateContextCKB
 * @in  param : cfg
 *          : deviceID - device ID
 * @out param : contextID
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CreateContextCKB(IN CONTEXT_CONFIG_CKB cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::CKBseriesContextFactory::GetInstance()->CreateContext(cfg, deviceID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->ActiveSelf());
    *contextID = jub::context::ContextManager::GetInstance()->AddOne(context);

    return JUBR_OK;
}


/*****************************************************************************
 * @function name : JUB_GetHDNodeCKB
 * @in  param : contextID - context ID
 *          : path
 * @out param : hex - xpub in HEX
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetHDNodeCKB(IN JUB_UINT16 contextID,
                        IN BIP44_Path path,
                        OUT JUB_CHAR_PTR_PTR hex) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::CKBContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_hex;
    JUB_VERIFY_RV(context->GetHDNode(path, str_hex));
    JUB_VERIFY_RV(_allocMem(hex, str_hex));

    return JUBR_OK;
}


/*****************************************************************************
 * @function name : JUB_GetMainHDNodeCKB
 * @in  param : contextID - context ID
 * @out param : hex - main xpub in HEX
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetMainHDNodeCKB(IN JUB_UINT16 contextID,
                            OUT JUB_CHAR_PTR_PTR hex) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::CKBContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_hex;
    JUB_VERIFY_RV(context->GetMainHDNode(str_hex));
    JUB_VERIFY_RV(_allocMem(hex, str_hex));

    return JUBR_OK;
}


/*****************************************************************************
 * @function name : JUB_GetAddressCKB
 * @in  param : contextID - context ID
 *          : path
 *          : bShow
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetAddressCKB(IN JUB_UINT16 contextID,
                         IN BIP44_Path path,
                         IN JUB_ENUM_BOOL bshow,
                         OUT JUB_CHAR_PTR_PTR address) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::CKBContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_address;
    JUB_VERIFY_RV(context->GetAddress(JUB_ENUM_BTC_ADDRESS_FORMAT::OWN, path, bshow, str_address));
    JUB_VERIFY_RV(_allocMem(address, str_address));

    return JUBR_OK;
}


/*****************************************************************************
 * @function name : JUB_CheckAddressCKB
 * @in  param : contextID - context ID
 *          : address
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CheckAddressCKB(IN JUB_UINT16 contextID,
                           IN JUB_CHAR_CPTR address) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::CKBContext>(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->CheckAddress(address));

    return JUBR_OK;
}


///*****************************************************************************
// * @function name : JUB_SetMyAddressCKB
// * @in  param : contextID - context ID
// *          : path
// * @out param : address
// * @last change :
// *****************************************************************************/
//JUB_RV JUB_SetMyAddressCKB(IN JUB_UINT16 contextID,
//                           IN BIP44_Path path,
//                           OUT JUB_CHAR_PTR_PTR address) {
//
//    CREATE_THREAD_LOCK_GUARD
//    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::CKBContext>(contextID);
//    JUB_CHECK_NULL(context);
//
//    std::string str_address;
//    JUB_VERIFY_RV(context->SetMyAddress(JUB_ENUM_BTC_ADDRESS_FORMAT::OWN, path, str_address));
//    JUB_VERIFY_RV(_allocMem(address, str_address));
//
//    return JUBR_OK;
//}
//
//
/*****************************************************************************
 * @function name : JUB_SignTransactionCKB
 * @in  param : contextID - context ID
 *          : version
 *          : inputs
 *          : iCount
 *          : outputs
 *          : oCount
 * @out param : raw
 * @last change :
 *****************************************************************************/
JUB_RV JUB_SignTransactionCKB(IN JUB_UINT16 contextID,
                              IN JUB_UINT32 version,
                              IN CELL_DEP deps[], IN JUB_UINT16 depCount,
                              IN CELL_INPUT inputs[], IN JUB_UINT16 iCount,
                              IN CELL_OUTPUT outputs[], IN JUB_UINT16 oCount,
                              OUT JUB_CHAR_PTR_PTR raw) {

    CREATE_THREAD_LOCK_GUARD
    std::vector<CELL_DEP> vDeps(deps, deps + depCount);
    std::vector<CELL_INPUT> vInputs(inputs, inputs + iCount);
    std::vector<CELL_OUTPUT> vOutputs(outputs, outputs + oCount);

    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::CKBContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_raw;
    JUB_VERIFY_RV(context->signTX(JUB_ENUM_BTC_ADDRESS_FORMAT::OWN, version, vDeps, vInputs, vOutputs, str_raw));

    JUB_VERIFY_RV(_allocMem(raw, str_raw));

    return JUBR_OK;
}


/*****************************************************************************
 * @function name : JUB_IsValidAddressCKB
 * @in  param : address
 * @out param: format - address format
 *          : codeHashIndex - code hash index
 *          : codeHash - code hash
 *          : args - args to encode
 * @last change :
 *****************************************************************************/
JUB_RV JUB_IsValidAddressCKB(IN JUB_CHAR_CPTR address,
                             OUT JUB_INT32_PTR format,
                             OUT JUB_INT32_PTR codeHashIndex,
                             OUT JUB_CHAR_PTR_PTR codeHash,
                             OUT JUB_CHAR_PTR_PTR args) {

    CREATE_THREAD_LOCK_GUARD

    JUB_VERIFY_RV(jub::token::JubiterBaseCKBImpl::IsValidAddress(address));

    std::string str_codeHash, str_args;
    JUB_VERIFY_RV(jub::token::JubiterBaseCKBImpl::ParseAddress(address,
                                                               format, codeHashIndex,
                                                               str_codeHash, str_args));
    JUB_VERIFY_RV(_allocMem(codeHash, str_codeHash));
    JUB_VERIFY_RV(_allocMem(args, str_args));

    return JUBR_OK;
}
