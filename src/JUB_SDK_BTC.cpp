//
//  JUB_SDK_BTC.cpp
//  JubSDK
//
//  Created by Pan Min on 2019/7/17.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "JUB_SDK_BTC.h"

#include "utility/util.h"
#include "utility/mutex.h"

#include "context/BTCContext.h"
#include "token/BTC/JubiterBladeBTCImpl.h"

#include <context/ContextFactory.h>

JUB_RV _allocMem(JUB_CHAR_PTR_PTR memPtr, const std::string &strBuf);

// Remove c++ features for swift framework
//stContextCfgBTC::stContextCfgBTC() {
//    mainPath = nullptr;
//     coinType = JUB_ENUM_COINTYPE_BTC::Default;
//    transType = JUB_ENUM_BTC_TRANS_TYPE::p2pkh;
//}
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
//stContextCfgBTC::stContextCfgBTC(JUB_ENUM_COINTYPE_BTC _coinType,
//                                 JUB_CHAR_PTR _mainPath,
//                                 JUB_ENUM_BTC_TRANS_TYPE _transType) {
//     mainPath = _mainPath;
//     coinType =  _coinType;
//    transType = _transType;
//}
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
//stInput::stInput() {
//    type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
//
//    preHash = NULL;
//    preIndex = 0;
//    nSequence = jub::btc::sequence;
//    amount = 0;
//    path = BIP44_Path();
//}
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
//stOutput::stOutput() {
//    address = NULL;
//    amount = 0;
//    changeAddress = JUB_ENUM_BOOL::BOOL_FALSE;
//    path = BIP44_Path();
//}
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
//stOutputReturn0::stOutputReturn0() {
//    amount = 0;
//    dataLen = 0;
//    memset(data, 0x00, sizeof(data)/sizeof(JUB_BYTE));
//}
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
//stOutputQRC20::stOutputQRC20() {
//    dataLen = 0;
//    memset(data, 0x00, sizeof(data)/sizeof(JUB_BYTE));
//}
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
//stOutputBTC::stOutputBTC() {
//    type = JUB_ENUM_SCRIPT_BTC_TYPE::P2PKH;
//
//    stdOutput = OUTPUT();
//    return0   = OUTPUT_RETURN0();
//    qrc20     = OUTPUT_QRC20();
//}
// Remove c++ features for swift framework end

/*****************************************************************************
 * @function name : JUB_CreateContextBTC
 * @in  param : cfg
 *            : deviceID - device ID
 * @out param : contextID
 * @last change :
 *****************************************************************************/
JUB_RV JUB_CreateContextBTC(IN CONTEXT_CONFIG_BTC cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::BTCseriesContextFactory::GetInstance()->CreateContext(cfg, deviceID);
    JUB_CHECK_NULL(context);
    context->ActiveSelf();
    *contextID = jub::context::ContextManager::GetInstance()->AddOne(context);

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetHDNodeBTC
 * @in  param : contextID - context ID
 *            : path
 * @out param : xpub
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetHDNodeBTC(IN JUB_UINT16 contextID,
                        IN BIP44_Path path,
                        OUT JUB_CHAR_PTR_PTR xpub) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::BTCContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_xpub;
    JUB_VERIFY_RV(context->GetHDNode(path, str_xpub));
    JUB_VERIFY_RV(_allocMem(xpub, str_xpub));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetMainHDNodeBTC
 * @in  param : contextID - context ID
 * @out param : xpub
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetMainHDNodeBTC(IN JUB_UINT16 contextID,
                            OUT JUB_CHAR_PTR_PTR xpub) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::BTCContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_xpub;
    JUB_VERIFY_RV(context->GetMainHDNode(str_xpub));
    JUB_VERIFY_RV(_allocMem(xpub, str_xpub));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetAddressBTC
 * @in  param : contextID - context ID
 *            : path
 *            : bShow
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_RV JUB_GetAddressBTC(IN JUB_UINT16 contextID,
                         IN BIP44_Path path,
                         IN JUB_ENUM_BOOL bshow,
                         OUT JUB_CHAR_PTR_PTR address) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::BTCContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_address;
    JUB_VERIFY_RV(context->GetAddress(JUB_ENUM_BTC_ADDRESS_FORMAT::OWN, path, bshow, str_address));
    JUB_VERIFY_RV(_allocMem(address, str_address));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_SetMyAddressBTC
 * @in  param : contextID - context ID
 *            : path
 * @out param : address
 * @last change :
 *****************************************************************************/
JUB_RV JUB_SetMyAddressBTC(IN JUB_UINT16 contextID,
                           IN BIP44_Path path,
                           OUT JUB_CHAR_PTR_PTR address) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::BTCContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string str_address;
    JUB_VERIFY_RV(context->SetMyAddress(JUB_ENUM_BTC_ADDRESS_FORMAT::OWN, path, str_address));
    JUB_VERIFY_RV(_allocMem(address, str_address));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_SignTransactionBTC
 * @in  param : contextID - context ID
 *            : inputs
 *            : iCount
 *            : outputs
 *            : oCount
 *            : lockTime
 * @out param : raw
 * @last change :
 *****************************************************************************/
JUB_RV JUB_SignTransactionBTC(IN JUB_UINT16 contextID,
                              IN INPUT_BTC inputs[], IN JUB_UINT16 iCount,
                              IN OUTPUT_BTC outputs[], IN JUB_UINT16 oCount,
                              IN JUB_UINT32 lockTime,
                              OUT JUB_CHAR_PTR_PTR raw) {

    CREATE_THREAD_LOCK_GUARD
    std::vector<INPUT_BTC> vInputs(inputs, inputs + iCount);
    std::vector<OUTPUT_BTC> vOutputs(outputs, outputs + oCount);
    bool isQrc20    = false;
    int changeCount = 0;

    for (OUTPUT_BTC output : vOutputs) {
        if (QRC20 == output.type) {
            isQrc20 = true;
            continue;
        }
        if (P2PKH == output.type) {
            if (BOOL_TRUE == output.stdOutput.changeAddress) {
                changeCount++;
            }
        }
    }
    if (isQrc20) {
        if (   2 == vOutputs.size()
            && 1 == changeCount
            ) {
        }
        else {
            return JUBR_QRC20_WRONG_FORMAT;
        }
    }

	auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::BTCContext>(contextID);
	JUB_CHECK_NULL(context);

    std::string str_raw;
    JUB_VERIFY_RV(context->SignTX(JUB_ENUM_BTC_ADDRESS_FORMAT::OWN, vInputs, vOutputs, lockTime, str_raw));

    JUB_VERIFY_RV(_allocMem(raw, str_raw));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_SetUnitBTC
 * @in  param : contextID - context ID
 *            : unit
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_RV JUB_SetUnitBTC(IN JUB_UINT16 contextID,
                      IN JUB_ENUM_BTC_UNIT_TYPE unit) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::BTCContext>(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->SetUnit(unit));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_BuildUSDTOutputs
 * @in  param : contextID - context ID
 *            : USDTTo - to address
 *            : amount
 * @out param : outputs
 * @last change : build the return0 and dust 2 outputs
 *****************************************************************************/
JUB_RV JUB_BuildUSDTOutputs(IN JUB_UINT16 contextID,
                            IN JUB_CHAR_CPTR USDTTo,
                            IN JUB_UINT64 amount,
                            OUT OUTPUT_BTC outputs[2]) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::BTCContext>(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->BuildUSDTOutputs(USDTTo, amount, outputs));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : BuildQRC20Outputs
 * @in  param : contextID - context ID
 *            : contractAddress - contract address for QRC20 token
 *            : decimal         - decimal for QRC20 token
 *            : symbol          - symbol for QRC20 token
 *            : gasLimit - gas limit
 *            : gasPrice - gas price
 *            : to    - to address for transfer
 *            : value - amount for transfer
 * @out param : outputs
 * @last change : build the QRC20 outputs
 *****************************************************************************/
JUB_RV JUB_BuildQRC20Outputs(IN JUB_UINT16 contextID,
                             IN JUB_CHAR_CPTR contractAddress, IN JUB_UINT8 decimal, IN JUB_CHAR_CPTR symbol,
                             IN JUB_UINT64 gasLimit, IN JUB_UINT64 gasPrice,
                             IN JUB_CHAR_CPTR to, IN JUB_CHAR_CPTR value,
                             OUT OUTPUT_BTC outputs[1]) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::QTUMContext>(contextID);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->SetQRC20Token(contractAddress,decimal,symbol));
    JUB_VERIFY_RV(context->BuildQRC20Outputs(gasLimit, gasPrice, contractAddress,to,value,outputs));

    return JUBR_OK;
}

JUB_RV JUB_CheckAddressBTC(IN JUB_UINT16 contextID,IN JUB_CHAR_CPTR address){
    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::BTCContext>(contextID);
    JUB_CHECK_NULL(context);
    
    JUB_VERIFY_RV(context->CheckAddress(address));

    return JUBR_OK;
}