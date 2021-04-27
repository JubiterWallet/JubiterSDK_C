//
//  JUB_SDK_XRP.cpp
//  JubSDK
//
//  Created by Pan Min on 2019/11/25.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_XRP.h"

#include "utility/util.h"
#include "utility/mutex.h"

#include "context/XRPContextFactory.h"


JUB_RV _allocMem(JUB_CHAR_PTR_PTR memPtr, const std::string &strBuf);

// Remove c++ features for swift framework
//stAmount::stAmount() {
//    currency = nullptr;
//    value = nullptr;
//    issuer = nullptr;
//}
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
//bool stAmount::isValid() const {
//    bool bRet = true;
//    if (nullptr == value) {
//        bRet = false;
//    }
//
//    return bRet;
//}
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
//stPaymentXRP::stPaymentXRP() {
//    type = JUB_ENUM_XRP_PYMT_TYPE::NS_ITEM_PYMT_TYPE;
//    amount = stAmount();
//    destination = nullptr;
//    destinationTag = nullptr;
//    invoiceID = nullptr;
//    invoiceID = nullptr;
//    sendMax = stAmount();
//    deliverMin = stAmount();
//}
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
//bool stPaymentXRP::isValid() const {
//    bool bRet = true;
//    if (JUB_ENUM_XRP_PYMT_TYPE::NS_ITEM_PYMT_TYPE == type
//        || !amount.isValid()
//        || nullptr == destination
//        || nullptr == destinationTag
//        ) {
//        bRet = false;
//    }
//
//    return bRet;
//}
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
//stMemoXRP::stMemoXRP() {
//    type = nullptr;
//    data = nullptr;
//    format = nullptr;
//}
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
//bool stMemoXRP::isValid() const {
//    bool bRet = true;
//    if (nullptr == data) {
//        bRet = false;
//    }
//
//    return bRet;
//}
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
//stTxXRP::stTxXRP() {
//    account = nullptr;
//    type = JUB_ENUM_XRP_TX_TYPE::NS_ITEM_TX_TYPE;
//    fee = nullptr;
//    sequence = nullptr;
//    accountTxnID = nullptr;
//    flags = nullptr;
//    lastLedgerSequence = nullptr;
//    memo = JUB_XRP_MEMO();
//    sourceTag = nullptr;
//    pymt = JUB_PYMT_XRP();
//}
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
//bool stTxXRP::isValid() const {
//    bool bRet = true;
//    if (   nullptr == account
//        || JUB_ENUM_XRP_TX_TYPE::NS_ITEM_TX_TYPE == type
//        || nullptr == fee
//        || nullptr == sequence
//        || nullptr == flags
//        || nullptr == lastLedgerSequence
//        ) {
//        bRet = false;
//    }
//
//    return bRet;
//}
// Remove c++ features for swift framework end

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

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::XRPseriesContextFactory::GetInstance()->CreateContext(cfg, deviceID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->ActiveSelf());
    *contextID = jub::context::ContextManager::GetInstance()->AddOne(context);

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

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::XRPContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_address;
    JUB_VERIFY_RV(context->GetAddress(path, bShow, str_address));
    JUB_VERIFY_RV(_allocMem(address, str_address));

    return JUBR_OK;
}

///*****************************************************************************
// * @function name : JUB_SetMyAddressXRP
// * @in  param : contextID - context ID
// *          : path
// * @out param : address
// * @last change :
// *****************************************************************************/
//JUB_RV JUB_SetMyAddressXRP(IN JUB_UINT16 contextID,
//                           IN BIP44_Path path,
//                           OUT JUB_CHAR_PTR_PTR address) {
//
//    CREATE_THREAD_LOCK_GUARD
//	auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::XRPContext>(contextID);
//	JUB_CHECK_NULL(context);
//
//    std::string str_address;
//    JUB_VERIFY_RV(context->SetMyAddress(path, str_address));
//    JUB_VERIFY_RV(_allocMem(address, str_address));
//
//    return JUBR_OK;
//}
//
/*****************************************************************************
 * @function name : JUB_GetHDNodeXRP
 * @in  param : contextID - context ID
 *          : format - JUB_ENUM_PUB_FORMAT::HEX (0x00) for hex;
 *                     JUB_ENUM_PUB_FORMAT::XPUB(0x01) for xpub
 *          : path
 * @out param : pubkey
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetHDNodeXRP(IN JUB_UINT16 contextID,
                        IN JUB_ENUM_PUB_FORMAT format,
                        IN BIP44_Path path,
                        OUT JUB_CHAR_PTR_PTR pubkey) {

    CREATE_THREAD_LOCK_GUARD
	auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::XRPContext>(contextID);
	JUB_CHECK_NULL(context);

    std::string str_pubkey;
    JUB_VERIFY_RV(context->GetHDNode((JUB_BYTE)format, path, str_pubkey));
    JUB_VERIFY_RV(_allocMem(pubkey, str_pubkey));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_CheckAddressXRP
 * @in  param : contextID - context ID
 *          : address
 * @out param : addr: base58 decode
 * @out param : tag: x_address tag
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_CheckAddressXRP(IN JUB_UINT16 contextID, IN JUB_CHAR_CPTR address, OUT JUB_CHAR_PTR_PTR addr,JUB_CHAR_PTR_PTR tag)
{
    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::XRPContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_address;
    std::string str_tag;
    JUB_VERIFY_RV(context->CheckAddress(address, str_address, str_tag));
    JUB_VERIFY_RV(_allocMem(addr, str_address));
    JUB_VERIFY_RV(_allocMem(tag, str_tag));

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

    CREATE_THREAD_LOCK_GUARD
	auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::XRPContext>(contextID);
	JUB_CHECK_NULL(context);

    std::string str_xpub;
    JUB_VERIFY_RV(context->GetMainHDNode((JUB_BYTE)format, str_xpub));
    JUB_VERIFY_RV(_allocMem(xpub, str_xpub));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_SignTransactionXRP
 * @in  param : contextID - context ID
 *          : path
 *          : tx - JUB_TX_XRP
 * @out param : raw
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_SignTransactionXRP(IN JUB_UINT16 contextID,
                              IN BIP44_Path path,
                              IN JUB_TX_XRP tx,
                              OUT JUB_CHAR_PTR_PTR raw) {

    CREATE_THREAD_LOCK_GUARD
	auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::XRPContext>(contextID);
	JUB_CHECK_NULL(context);

    std::string str_raw;
    JUB_VERIFY_RV(context->SignTransaction(path,
                                           tx,
                                           str_raw));
    JUB_VERIFY_RV(_allocMem(raw, str_raw));

    return JUBR_OK;
}
