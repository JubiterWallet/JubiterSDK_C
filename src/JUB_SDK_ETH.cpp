//
//  JUB_SDK_ETH.cpp
//  JubSDK
//
//  Created by Pan Min on 2019/7/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_ETH.h"

#include "utility/util.h"
#include "utility/mutex.h"

#include <context/ETHContextFactory.h>


JUB_RV _allocMem(JUB_CHAR_PTR_PTR memPtr, const std::string &strBuf);

// Remove c++ features for swift framework
//stContextCfgETH::stContextCfgETH() {
//    mainPath = nullptr;
//    chainID = 0;
//}
// Remove c++ features for swift framework end

/*****************************************************************************
 * @function name : JUB_CreateContextETH
 * @in  param : cfg
 *          : deviceID - device ID
 * @out param : contextID
 * @last change :
 *****************************************************************************/
JUB_RV JUB_CreateContextETH(IN CONTEXT_CONFIG_ETH cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ETHseriesContextFactory::GetInstance()->CreateContext(cfg, deviceID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->ActiveSelf());
    *contextID = jub::context::ContextManager::GetInstance()->AddOne(context);

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetAddressETH
 * @in  param : contextID - context ID
 *          : path
 *          : bShow
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetAddressETH(IN JUB_UINT16 contextID,
                         IN BIP44_Path    path,
                         IN JUB_ENUM_BOOL bShow,
                         OUT JUB_CHAR_PTR_PTR address) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::ETHContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_address;
    JUB_VERIFY_RV(context->GetAddress(path, bShow, str_address));
    JUB_VERIFY_RV(_allocMem(address, str_address));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_SetMyAddressETH
 * @in  param : contextID - context ID
 *          : path
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_RV JUB_SetMyAddressETH(IN JUB_UINT16 contextID,
                           IN BIP44_Path path,
                           OUT JUB_CHAR_PTR_PTR address) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::ETHContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_address;
    JUB_VERIFY_RV(context->SetMyAddress(path, str_address));
    JUB_VERIFY_RV(_allocMem(address, str_address));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetHDNodeETH
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX (0x00) for hex;
 *                 JUB_ENUM_PUB_FORMAT::XPUB(0x01) for xpub
 *          : path
 * @out param : pubkey
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetHDNodeETH(IN JUB_UINT16 contextID,
                        IN JUB_ENUM_PUB_FORMAT format,
                        IN BIP44_Path path,
                        OUT JUB_CHAR_PTR_PTR pubkey) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::ETHContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_pubkey;
    JUB_VERIFY_RV(context->GetHDNode((JUB_BYTE)format, path, str_pubkey));
    JUB_VERIFY_RV(_allocMem(pubkey, str_pubkey));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetMainHDNodeETH
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX (0x00) for hex;
 *                 JUB_ENUM_PUB_FORMAT::XPUB(0x01) for xpub
 * @out param : xpub
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetMainHDNodeETH(IN JUB_UINT16 contextID,
                            IN JUB_ENUM_PUB_FORMAT format,
                            OUT JUB_CHAR_PTR_PTR xpub) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::ETHContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_xpub;
    JUB_VERIFY_RV(context->GetMainHDNode((JUB_BYTE)format, str_xpub));
    JUB_VERIFY_RV(_allocMem(xpub, str_xpub));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_SignTransactionETH
 * @in  param : contextID - context ID
 *          : path
 *          : nonce - nonce
 *          : gasLimit - gas limit
 *          : gasPriceInWei - gas price in wei
 *          : to
 *          : valueInWei - value in wei
 *          : input
 * @out param : raw
 * @last change :
 *****************************************************************************/
JUB_RV JUB_SignTransactionETH(IN JUB_UINT16 contextID,
                              IN BIP44_Path path,
                              IN JUB_UINT32 nonce,
                              IN JUB_UINT32 gasLimit,
                              IN JUB_CHAR_CPTR gasPriceInWei,
                              IN JUB_CHAR_CPTR to,
                              IN JUB_CHAR_CPTR valueInWei,
                              IN JUB_CHAR_CPTR input,
                              OUT JUB_CHAR_PTR_PTR raw) {

    CREATE_THREAD_LOCK_GUARD
    JUB_CHECK_NULL(gasPriceInWei);
    JUB_CHECK_NULL(to);
//    JUB_CHECK_NULL(valueInWei);// it can be nullptr
    JUB_CHECK_NULL(input);

	auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::ETHContext>(contextID);
	JUB_CHECK_NULL(context);

    std::string str_raw;
    JUB_VERIFY_RV(context->SignTransaction(path,
                                           nonce,
                                           gasLimit,
                                           gasPriceInWei,
                                           to,
                                           valueInWei,
                                           input,
                                           str_raw));
    JUB_VERIFY_RV(_allocMem(raw, str_raw));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_SignBytestringETH
 * @in  param : contextID - context ID
 *          : path
 *          : data - typed structured data
 * @out param : raw
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignBytestringETH(IN JUB_UINT16 contextID,
                             IN BIP44_Path path,
                             IN JUB_CHAR_CPTR data,
                             OUT JUB_CHAR_PTR_PTR signature) {

    CREATE_THREAD_LOCK_GUARD
    JUB_CHECK_NULL(data);

    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::ETHContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_signature;
    JUB_VERIFY_RV(context->SignBytestring(path,
                                          data,
                                          str_signature));
    JUB_VERIFY_RV(_allocMem(signature, str_signature));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_BuildERC20AbiETH
 * @in  param : contextID - context ID
 *          : tokenName - ETH token name
 *          : unitDP - unit decimal place
 *          : contractAddress - contract address
 *          : tokenTo - token to
 *          : tokenValue - value for token transaction
 * @out param : abi
 * @last change :
 *****************************************************************************/
JUB_RV JUB_BuildERC20AbiETH(IN JUB_UINT16 contextID,
                            IN JUB_CHAR_CPTR tokenName,
                            IN JUB_UINT16 unitDP,
                            IN JUB_CHAR_CPTR contractAddress,
                            IN JUB_CHAR_CPTR tokenTo,
                            IN JUB_CHAR_CPTR tokenValue,
                            OUT JUB_CHAR_PTR_PTR abi) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::ETHContext>(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->SetERC20ETHToken(tokenName, unitDP, contractAddress));

    std::string strAbi;
    JUB_VERIFY_RV(context->BuildERC20Abi(tokenTo, tokenValue, strAbi));
    JUB_VERIFY_RV(_allocMem(abi, strAbi));

    return JUBR_OK;
}
