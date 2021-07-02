//
//  JUB_SDK_EOS.cpp
//  JubSDK
//
//  Created by Pan Min on 2019/10/09.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_EOS.h"

#include "utility/util.h"
#include "utility/mutex.h"

#include "context/EOSContextFactory.h"
#include <TrustWallet/wallet-core/src/EOS/Serialization.h>
#include <TrustWallet/wallet-core/src/EOS/Deserialization.h>
#include <TrezorCrypto/hasher.h>

#include "token/EOS/JubiterBaseEOSImpl.h"

JUB_RV _allocMem(JUB_CHAR_PTR_PTR memPtr, const std::string &strBuf);

// Remove c++ features for swift framework
//stTransferAction::stTransferAction() {
//    from = nullptr;
//    to = nullptr;
//    asset = nullptr;
//    memo = nullptr;
//}
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
//stDelegateAction::stDelegateAction() {
//    from = nullptr;
//    receiver = nullptr;
//    netQty = nullptr;
//    cpuQty = nullptr;
//    bStake = false;
//}
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
//stBuyRamAction::stBuyRamAction() {
//    payer = nullptr;
//    quant = nullptr;
//    receiver = nullptr;
//}
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
//stSellRamAction::stSellRamAction() {
//    account = nullptr;
//    bytes = nullptr;
//}
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
//stActionEOS::stActionEOS() {
//    type = JUB_ENUM_EOS_ACTION_TYPE::NS_ITEM_EOS_ACTION_TYPE;
//    currency = nullptr;
//    name = nullptr;
//
//    transfer = stTransferAction();
//    delegate = stDelegateAction();
//    buyRam = stBuyRamAction();
//    sellRam = stSellRamAction();
//}
// Remove c++ features for swift framework end

/*****************************************************************************
 * @function name : JUB_CreateContextEOS
 * @in  param : cfg
 *          : deviceID - device ID
 * @out param : contextID
 * @last change :
 *****************************************************************************/
JUB_RV JUB_CreateContextEOS(IN CONTEXT_CONFIG_EOS cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::EOSseriesContextFactory::GetInstance()->CreateContext(cfg, deviceID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->ActiveSelf());
    *contextID = jub::context::ContextManager::GetInstance()->AddOne(context);

    return JUBR_OK;
}


/*****************************************************************************
 * @function name : JUB_GetHDNodeEOS
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX(0x00) for hex;
 *                 JUB_ENUM_PUB_FORMAT::XPUB(0x01)
 *          : path
 * @out param : pubkey
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetHDNodeEOS(IN JUB_UINT16 contextID,
                        IN JUB_ENUM_PUB_FORMAT format,
                        IN BIP44_Path path,
//                        IN BIP48_Path path,
                        OUT JUB_CHAR_PTR_PTR pubkey) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::EOSContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_pubkey;
    JUB_VERIFY_RV(context->GetHDNode((JUB_BYTE)format, path, str_pubkey));
    JUB_VERIFY_RV(_allocMem(pubkey, str_pubkey));

    return JUBR_OK;
}


/*****************************************************************************
 * @function name : JUB_GetMainHDNodeEOS
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX(0x00) for hex;
 *                 JUB_ENUM_PUB_FORMAT::XPUB(0x01)
 * @out param : xpub
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetMainHDNodeEOS(IN JUB_UINT16 contextID,
                            IN JUB_ENUM_PUB_FORMAT format,
                            OUT JUB_CHAR_PTR_PTR xpub) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::EOSContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_xpub;
    JUB_VERIFY_RV(context->GetMainHDNode((JUB_BYTE)format, str_xpub));
    JUB_VERIFY_RV(_allocMem(xpub, str_xpub));

    return JUBR_OK;
}


/*****************************************************************************
 * @function name : JUB_GetAddressEOS
 * @in  param : contextID - context ID
 *          : path
 *          : bShow
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetAddressEOS(IN JUB_UINT16 contextID,
                         IN BIP44_Path    path,
//                         IN BIP48_Path    path,
                         IN JUB_ENUM_BOOL bShow,
                         OUT JUB_CHAR_PTR_PTR address) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::EOSContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_address;
    JUB_VERIFY_RV(context->GetAddress(path, bShow, str_address));
    JUB_VERIFY_RV(_allocMem(address, str_address));

    return JUBR_OK;
}


/*****************************************************************************
 * @function name : JUB_CheckAddressEOS
 * @in  param : contextID - context ID
 *            : address
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CheckAddressEOS(IN JUB_UINT16 contextID, IN JUB_CHAR_CPTR address) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::EOSContext>(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->CheckAddress(address));

    return JUBR_OK;
}


///*****************************************************************************
// * @function name : JUB_SetMyAddressEOS
// * @in  param : contextID - context ID
// *          : path
// * @out param : address
// * @last change :
// *****************************************************************************/
//JUB_RV JUB_SetMyAddressEOS(IN JUB_UINT16 contextID,
//                           IN BIP44_Path path,
////                           IN BIP48_Path path,
//                           OUT JUB_CHAR_PTR_PTR address) {
//
//    CREATE_THREAD_LOCK_GUARD
//    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::EOSContext>(contextID);
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
 * @function name : JUB_SignTransactionEOS
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
JUB_RV JUB_SignTransactionEOS(IN JUB_UINT16 contextID,
                              IN BIP44_Path path,
                              IN JUB_CHAR_CPTR chainID,
                              IN JUB_CHAR_CPTR expiration,
                              IN JUB_CHAR_CPTR referenceBlockId,
                              IN JUB_CHAR_CPTR referenceBlockTime,
                              IN JUB_CHAR_CPTR actionsInJSON,
                              OUT JUB_CHAR_PTR_PTR rawInJSON) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::EOSContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_raw;
    JUB_VERIFY_RV(context->SignTransaction(path,
                                           chainID,
                                           expiration,
                                           referenceBlockId,
                                           referenceBlockTime,
                                           actionsInJSON,
                                           str_raw));
    JUB_VERIFY_RV(_allocMem(rawInJSON, str_raw));

    return JUBR_OK;
}


/*****************************************************************************
 * @function name : JUB_BuildActionEOS
 * @in  param : contextID - context ID
 *          : actions - action array
 *          : actionCount - the count of action array
 * @out param : actionsInJSON
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_BuildActionEOS(IN JUB_UINT16 contextID,
                          IN JUB_ACTION_EOS_PTR actions,
                          IN JUB_UINT16 actionCount,
                          OUT JUB_CHAR_PTR_PTR actionsInJSON) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::EOSContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_actions;
    JUB_VERIFY_RV(context->BuildAction(actions,
                                       actionCount,
                                       str_actions));
    JUB_VERIFY_RV(_allocMem(actionsInJSON, str_actions));

    return JUBR_OK;
}


/*****************************************************************************
 * @function name : JUB_CalculateMemoHash
 * @in  param : memo - memo
 * @out param : memoHash - memo hash
 * @last change :
 *****************************************************************************/
JUB_RV JUB_CalculateMemoHash(IN JUB_CHAR_CPTR memo,
                             OUT JUB_CHAR_PTR_PTR memoHash) {

    CREATE_THREAD_LOCK_GUARD
    std::string strMemo = std::string(memo);
    TW::Data encode;
    TW::EOS::encodeString(strMemo, encode);

    int varIntByteSize = 0;
    std::string s;
    TW::EOS::decodeString(encode, s, varIntByteSize);
    if (0 != s.compare(strMemo)) {
        return JUBR_HOST_MEMORY;
    }

    TW::Data toHash(encode.begin()+varIntByteSize, encode.end());

    const auto begin = reinterpret_cast<const uint8_t*>(toHash.data());
    TW::Data hash = TW::Hash::sha256(begin, toHash.size());

    uchar_vector memoh = uchar_vector(hash.begin(), hash.begin()+4);
    std::string str_memoHash = memoh.getHex();
    JUB_VERIFY_RV(_allocMem(memoHash, str_memoHash));

    return JUBR_OK;
}


/*****************************************************************************
 * @function name : JUB_IsValidAddressEOS
 * @in  param : address
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_IsValidAddressEOS(IN JUB_CHAR_CPTR address) {

    CREATE_THREAD_LOCK_GUARD

    JUB_VERIFY_RV(jub::token::JubiterBaseEOSImpl::IsValidAddress(address));

    return JUBR_OK;
}
