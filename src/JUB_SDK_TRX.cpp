//
//  JUB_SDK_TRX.cpp
//  JubSDK
//
//  Created by Pan Min on 2020/09/10.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_TRX.h"

#include "utility/mutex.h"
#include "utility/util.h"

#include "context/TRXContextFactory.h"
#include "token/TRX/JubiterBaseTRXImpl.h"
#include <string>

JUB_RV _allocMem(JUB_CHAR_PTR_PTR memPtr, const std::string &strBuf);

/*****************************************************************************
 * @function name : JUB_CreateContextTRX
 * @in  param : cfg
 *          : deviceID - device ID
 * @out param : contextID
 * @last change :
 *****************************************************************************/
JUB_RV JUB_CreateContextTRX(IN CONTEXT_CONFIG_TRX cfg, IN JUB_UINT16 deviceID, OUT JUB_UINT16 *contextID) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::TRXseriesContextFactory::GetInstance()->CreateContext(deviceID, cfg);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->ActiveSelf());
    *contextID = jub::context::ContextManager::GetInstance()->AddOne(context);

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetHDNodeTRX
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX(0x00) for hex;
 *                 JUB_ENUM_PUB_FORMAT::XPUB(0x01)
 *          : path
 * @out param : pubkey
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetHDNodeTRX(IN JUB_UINT16 contextID, IN JUB_ENUM_PUB_FORMAT format, IN BIP44_Path path,
                        //                        IN BIP48_Path path,
                        OUT JUB_CHAR_PTR_PTR pubkey) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::TRXContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_pubkey;
    JUB_VERIFY_RV(context->GetHDNode((JUB_BYTE)format, path, str_pubkey));
    JUB_VERIFY_RV(_allocMem(pubkey, str_pubkey));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetMainHDNodeTRX
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX(0x00) for hex;
 *                 JUB_ENUM_PUB_FORMAT::XPUB(0x01)
 * @out param : xpub
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetMainHDNodeTRX(IN JUB_UINT16 contextID, IN JUB_ENUM_PUB_FORMAT format, OUT JUB_CHAR_PTR_PTR xpub) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::TRXContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_xpub;
    JUB_VERIFY_RV(context->GetMainHDNode((JUB_BYTE)format, str_xpub));
    JUB_VERIFY_RV(_allocMem(xpub, str_xpub));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetAddressTRX
 * @in  param : contextID - context ID
 *            : path
 *            : bShow
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetAddressTRX(IN JUB_UINT16 contextID, IN BIP44_Path path,
                         //                         IN BIP48_Path    path,
                         IN JUB_ENUM_BOOL bShow, OUT JUB_CHAR_PTR_PTR address) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::TRXContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_address;
    JUB_VERIFY_RV(context->GetAddress(path, bShow, str_address));
    JUB_VERIFY_RV(_allocMem(address, str_address));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_CheckAddressTRX
 * @in  param : contextID - context ID
 *          : address
 * @out param : addrInHex: base58 decode
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CheckAddressTRX(IN JUB_UINT16 contextID, IN JUB_CHAR_CPTR address, OUT JUB_CHAR_PTR_PTR addrInHex) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::TRXContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_address;
    JUB_VERIFY_RV(context->CheckAddress(address, str_address));
    JUB_VERIFY_RV(_allocMem(addrInHex, str_address));

    return JUBR_OK;
}

///*****************************************************************************
// * @function name : JUB_SetMyAddressTRX
// * @in  param : contextID - context ID
// *            : path
// * @out param : address
// * @last change :
// *****************************************************************************/
// JUB_RV JUB_SetMyAddressTRX(IN JUB_UINT16 contextID,
//                           IN BIP44_Path path,
////                           IN BIP48_Path path,
//                           OUT JUB_CHAR_PTR_PTR address) {
//
//    CREATE_THREAD_LOCK_GUARD
//    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::TRXContext>(contextID);
//    JUB_CHECK_NULL(context);
//
//    std::string str_address;
//    JUB_VERIFY_RV(context->SetMyAddress(path, str_address));
//    JUB_VERIFY_RV(_allocMem(address, str_address));
//
//    return JUBR_OK;
//}
//
//
/*****************************************************************************
 * @function name : JUB_SignTransactionTRX
 * @in  param : contextID - context ID
 *          : path
 *          : packedContractInPb - packed contract in protobuf
 * @out param : rawInJSON - signed transaction raw in JSON
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTransactionTRX(IN JUB_UINT16 contextID, IN BIP44_Path path, IN JUB_CHAR_CPTR packedContractInPb,
                              OUT JUB_CHAR_PTR_PTR rawInJSON) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::TRXContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_raw;
    JUB_VERIFY_RV(context->SignTransaction(path, packedContractInPb, str_raw));
    JUB_VERIFY_RV(_allocMem(rawInJSON, str_raw));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_SignBytestringTRX（https://github.com/tronprotocol/tips/issues/104）
 * @in  param : contextID - context ID
 *          : path
 *          : data - message to be signed
 * @out param : rawInJSON
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignBytestringTRX(IN JUB_UINT16 contextID, IN BIP44_Path path, IN JUB_CHAR_CPTR data,
                             OUT JUB_CHAR_PTR_PTR rawInJSON) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::TRXContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_raw;
    JUB_VERIFY_RV(context->SignBytestring(path, data, str_raw));
    JUB_VERIFY_RV(_allocMem(rawInJSON, str_raw));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_SetTRC10Asset
 * @in  param : contextID - context ID
 *          : assetName - TRX asset name
 *          : unitDP - unit decimal place
 *          : assetID - asset ID
 * @out param : abi
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetTRC10Asset(IN JUB_UINT16 contextID, IN JUB_CHAR_CPTR assetName, IN JUB_UINT16 unitDP,
                         IN JUB_CHAR_CPTR assetID) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::TRXContext>(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->SetTRC20Token(assetName, unitDP, assetID));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_SetTRC20Token
 * @in  param : contextID - context ID
 *          : tokenName - TRX token name
 *          : unitDP - unit decimal place
 *          : contractAddress - contract address
 *          : tokenTo - token to
 *          : tokenValue - value for token transaction
 * @out param : none.
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetTRC20Token(IN JUB_UINT16 contextID, IN JUB_CHAR_CPTR tokenName, IN JUB_UINT16 unitDP,
                         IN JUB_CHAR_CPTR contractAddress) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::TRXContext>(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->SetTRC20Token(tokenName, unitDP, contractAddress));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_BuildTRC20TransferAbi
 * @in  param : contextID - context ID
 *          : tokenTo - token to
 *          : tokenValue - value for token transaction
 * @out param : abi
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_BuildTRC20TransferAbi(IN JUB_UINT16 contextID, IN JUB_CHAR_CPTR tokenTo, IN JUB_CHAR_CPTR tokenValue,
                                 OUT JUB_CHAR_PTR_PTR abi) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::TRXContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string strAbi;
    JUB_VERIFY_RV(context->BuildTRC20TransferAbi(tokenTo, tokenValue, strAbi));
    JUB_VERIFY_RV(_allocMem(abi, strAbi));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_SetTRC721Token
 * @in  param : contextID - context ID
 *          : nfTokenName - ERC-721 Non-Fungible Token Name
 *          : contractAddress - ERC-721 Non-Fungible Token contract address
 * @out param : none.
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SetTRC721Token(IN JUB_UINT16 contextID, IN JUB_CHAR_CPTR nfTokenName, IN JUB_CHAR_CPTR contractAddress) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::TRXContext>(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->SetTRC721Token(nfTokenName, contractAddress));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_BuildTRC721TransferAbi
 * @in  param : contextID - context ID
 *          : tokenFrom - The current owner of the NFT
 *          : tokenTo - The new owner
 *          : tokenID - The NFT to transfer
 * @out param : abi
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_BuildTRC721TransferAbi(IN JUB_UINT16 contextID, IN JUB_CHAR_CPTR tokenFrom, IN JUB_CHAR_CPTR tokenTo,
                                  IN JUB_CHAR_CPTR tokenID, OUT JUB_CHAR_PTR_PTR abi) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::TRXContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string strAbi;
    JUB_VERIFY_RV(context->BuildTRC721TransferAbi(tokenFrom, tokenTo, tokenID, strAbi));
    JUB_VERIFY_RV(_allocMem(abi, strAbi));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_PackContractTRX
 * @in  param : contextID - context ID
 *          : tx - transaction
 * @out param : packedContractInPB - packed contract in protobuf
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_PackContractTRX(IN JUB_UINT16 contextID, IN JUB_TX_TRX tx, OUT JUB_CHAR_PTR_PTR packedContractInPB) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::TRXContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_packcontract;
    JUB_VERIFY_RV(context->PackTransactionRaw(tx, str_packcontract));
    JUB_VERIFY_RV(_allocMem(packedContractInPB, str_packcontract));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_IsValidAddressTRX
 * @in  param : address
 * @out param : addrInHex: base58 decode
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_IsValidAddressTRX(IN JUB_CHAR_CPTR address, OUT JUB_CHAR_PTR_PTR addrInHex) {

    CREATE_THREAD_LOCK_GUARD

    std::string str_addrInHex;
    JUB_VERIFY_RV(jub::token::JubiterBaseTRXImpl::IsValidAddress(address, str_addrInHex));
    JUB_VERIFY_RV(_allocMem(addrInHex, str_addrInHex));

    return JUBR_OK;
}
