//
//  JUB_SDK_BTC.cpp
//  JubSDK
//
//  Created by Pan Min on 2019/7/17.
//  Copyright © 2023 JuBiter. All rights reserved.
//

#include "JUB_SDK_BTC.h"

#include "utility/util.h"
#include "utility/mutex.h"

#include "context/BTCContextFactory.h"


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
 *          : deviceID - device ID
 * @out param : contextID
 * @last change :
 *****************************************************************************/
JUB_RV JUB_CreateContextBTC(IN CONTEXT_CONFIG_BTC cfg,
                            IN JUB_UINT16 deviceID,
                            OUT JUB_UINT16* contextID) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::BTCseriesContextFactory::GetInstance()->CreateContext(deviceID, cfg);
    JUB_CHECK_NULL(context);

    JUB_VERIFY_RV(context->ActiveSelf());
    JUB_VERIFY_RV(context->PreparatoryFlow());
    *contextID = jub::context::ContextManager::GetInstance()->AddOne(context);

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_GetHDNodeBTC
 * @in  param : contextID - context ID
 *          : path
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
 *          : path
 *          : bShow
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
 *          : path
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
 *          : version
 *          : inputs
 *          : iCount
 *          : outputs
 *          : oCount
 *          : lockTime
 * @out param : raw
 * @last change :
 *****************************************************************************/
JUB_RV JUB_SignTransactionBTC(IN JUB_UINT16 contextID,
                              IN JUB_UINT32 version,
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
    JUB_VERIFY_RV(context->SignTX(JUB_ENUM_BTC_ADDRESS_FORMAT::OWN, version, vInputs, vOutputs, lockTime, str_raw));

    JUB_VERIFY_RV(_allocMem(raw, str_raw));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_ParseTransactionBTC
 * @in  param : contextID - context ID
 *            : incRaw
 * @out param : inputs
 *            : outputs
 *            : plockTime
 * @last change :
 *****************************************************************************/
JUB_COINCORE_DLL_EXPORT
JUB_RV JUB_ParseTransactionBTC(IN JUB_UINT16 contextID,
                               IN JUB_CHAR_PTR incRaw,
                               OUT std::vector<INPUT_BTC>& inputs,
                               OUT std::vector<OUTPUT_BTC>& outputs,
                               OUT JUB_UINT32_PTR plockTime) {

    CREATE_THREAD_LOCK_GUARD

    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::BTCContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string hexIncRaw(incRaw);
    JUB_VERIFY_RV(context->ParseTransaction(hexIncRaw,
                                            inputs, outputs,
                                            plockTime));

    return JUBR_OK;
}

/*****************************************************************************
 * @function name : JUB_SetUnitBTC
 * @in  param : contextID - context ID
 *          : unit
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
 *          : USDTTo - to address
 *          : amount
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
 *            : decimal - decimal for QRC20 token
 *            : symbol - symbol for QRC20 token
 *            : gasLimit - gas limit
 *            : gasPrice - gas price
 *            : to - to address for transfer
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

/*****************************************************************************
 * @function name : JUB_CheckAddressBTC
 * @in  param : contextID - context ID
 *          : address
 * @out param :
 * @last change :
 *****************************************************************************/
JUB_RV JUB_CheckAddressBTC(IN JUB_UINT16 contextID, IN JUB_CHAR_CPTR address) {
    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::BTCContext>(contextID);
    JUB_CHECK_NULL(context);
    
    JUB_VERIFY_RV(context->CheckAddress(address));

    return JUBR_OK;
}

JUB_RV JUB_MergMultiSignedTxBTC(IN JUB_CHAR_CPTR signedTx1, IN JUB_CHAR_CPTR signedTx2, OUT JUB_CHAR_PTR_PTR mergedTx) {
    
    // step 0: decode transaction
    auto decodeTx = [](auto signedTx) {
        auto tx = std::make_unique<TW::Bitcoin::Transaction>();
        auto bytes = uchar_vector(signedTx);
        if (!tx->decode(false, bytes)) {delete tx.release();};
        return tx;
    };
    
    auto tx1 = decodeTx(signedTx1);
    auto tx2 = decodeTx(signedTx2);
    if (!tx1 || !tx2) return JUBR_ARGUMENTS_BAD;
    
    // step 1: verify transaction
    // verify transaction
    // or caller make sure transaction is valided
    
    // step 2: make sure smae transaction signed by co-signer
    auto unsignedTx = [](decltype(tx1)& tx) {
        auto bytes = Data();
        // version
        tx->encodeVersion(bytes);
        // inputs
        encodeVarInt(tx->inputs.size(), bytes);
        for(auto input: tx->inputs) {
            // previous output
            input->previousOutput.encode(bytes);
            // placeholder script
            bytes.push_back(0x00);
            // sequence
            encode32LE(input->sequence, bytes);
        }
        // outputs
        encodeVarInt(tx->outputs.size(), bytes);
        for(auto output: tx->outputs) {
            output->encode(bytes);
        }
        //locktime
        encode32LE(tx->lockTime, bytes);
        return bytes;
    };
    
    if (unsignedTx(tx1) != unsignedTx(tx2)) {
        return JUBR_ARGUMENTS_BAD;
    }
    
    // tx1 tx2 is signd the same `transaction`, and all verified
    // all inputs must be m-n multi. if not can't merge and bordcast
    // step 3: check inputs
    if (!std::all_of(tx1->inputs.begin(), tx1->inputs.end(), [](auto input) {
        return input->script.isMultisigProgram();
    })) {
        return JUBR_ARGUMENTS_BAD;
    }
    
    // step 4: get signatures and public keys from every inputs
    // index, sigs, pks, m, n
    using InputItems = std::tuple<int, std::vector<Data>, std::vector<Data>, int, int>;
    auto items = std::vector<InputItems>();
    for(auto i = 0; i < tx1->inputs.size(); i++) {
        std::vector<Data> sigs, sigs1, sigs2;
        std::vector<Data> pks1, pks2;
        int m1, n1, m2, n2;
        {
            auto input = tx1->inputs[i];
            if(!input->script.matchMultisigScriptSig(sigs1, pks1, m1, n1)) {
                return JUBR_ARGUMENTS_BAD;
            }
        }
        {
            auto input = tx2->inputs[i];
            if(!input->script.matchMultisigScriptSig(sigs2, pks2, m2, n2)) {
                return JUBR_ARGUMENTS_BAD;
            }
        }
        
        if (pks1 != pks2) {
            return JUBR_ARGUMENTS_BAD;
        }
        if (m1 != m2 || n1 != n2) {
            return JUBR_ARGUMENTS_BAD;
        }
        // merge signature
        std::copy(sigs1.begin(), sigs1.end(), std::back_inserter(sigs));
        std::copy(sigs2.begin(), sigs2.end(), std::back_inserter(sigs));
        // pks m n
        items.push_back({i, sigs, pks1, m1, n1});
    }
    
    // step 6: check pks order. not need we trust all tx is signed under same rule
    // for (auto& item: items) {
    //    auto pks = std::get<2>(item);
    //    if (!std::is_sorted(pks.begin(), pks.end())) {
    //        return JUBR_ARGUMENTS_BAD;
    //    }
    // }
    
    // step 7: sort sigs
    auto sortSigs = [&tx1](InputItems& input) {
        // tx1 and tx2 is same
        auto [i, sigs, pks, m, n] = input;
        // get hash type from sig
        // hashType must same
        auto hashType = std::vector<uint32_t>();
        std::transform(sigs.begin(), sigs.end(), std::back_inserter(hashType), [](auto sig){
            return static_cast<uint32_t>(sig.back());
        });
        
        auto redeeScript = TW::Bitcoin::Script::buildRedeemScript(m, n, pks);
        auto preimage = tx1->getPreImage(redeeScript, i, hashType.front()); // hash all
        auto digest = Data(32);
        hasher_Raw(HasherType::HASHER_SHA2D, preimage.data(), preimage.size(), digest.data());
        // make sure tx is verified, so all sigs can verify
        // <matched-key-index, sig>, `zip` can use since c++23, so sad
        auto indexedSigs = std::vector<std::tuple<int, Data>>();
        // can't use std::transferm, pks is local, can't capture
        for(auto& sig: sigs) {
            auto index = 0;
            auto rs = Data(0x40);
            // tx is verified, sig must valided
            ecdsa_der_to_sig(sig.data(), rs.data());
            for (auto& pk: pks) {
                extern ecdsa_curve secp256k1;
                if (0 == ecdsa_verify_digest(&secp256k1, pk.data(), rs.data(), digest.data())) {
                    indexedSigs.push_back({index, sig});
                    break;
                }
                index++;
            }
        }
        
        auto sortedSigs = std::vector<Data>();
        std::sort(indexedSigs.begin(), indexedSigs.end(), [](auto& a, auto& b) {
            // up
            return std::get<0>(a) < std::get<0>(b);
        });
        
        std::transform(indexedSigs.begin(), indexedSigs.end(), std::back_inserter(sortedSigs), [](auto& indexSig){
            return std::get<1>(indexSig);
        });
        
        return InputItems{i, sortedSigs, pks, m, n};
    };
    // replace inplace
    std::transform(items.begin(), items.end(), items.begin(), sortSigs);
    
    // step 8: merge tx
    auto merged = TW::Bitcoin::Transaction(tx1->version, tx1->lockTime);
    for (auto& item: items) {
        auto [i, sigs, pks, m, n] = item;
        auto script = TW::Bitcoin::Script::buildPayToScriptMultisigScriptSig(sigs, pks, m, n);
        auto previousOutput = tx1->inputs[i]->previousOutput;
        auto sequence = tx1->inputs[i]->sequence;
        auto input = new TW::Bitcoin::TransactionInput(previousOutput, script, sequence);
        merged.inputs.push_back(input);
    }
    for (auto output: tx1->outputs) {
        merged.outputs.push_back(new TW::Bitcoin::TransactionOutput(output->value, output->script));
    }
    
    // encode
    auto bytes = uchar_vector();
    merged.encode(false, bytes);
    JUB_VERIFY_RV(_allocMem(mergedTx, bytes.getHex()));
    return JUBR_OK;
}
