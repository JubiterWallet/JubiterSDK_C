#include <JUB_SDK_Hcash.h>
#include <token/HC/JubiterBladeHCImpl.h>
#include "utility/mutex.h"

#include <context/HCContext.h>
#include <string>

JUB_RV _allocMem(JUB_CHAR_PTR_PTR memPtr, const std::string &strBuf);

// Remove c++ features for swift framework
//stInputHC::stInputHC() {
//    amount = 0;
//    path = BIP44_Path();
//}
// Remove c++ features for swift framework end

// Remove c++ features for swift framework
//stOutputHC::stOutputHC() {
//    changeAddress = JUB_ENUM_BOOL::BOOL_FALSE;
//    path = BIP44_Path();
//}
// Remove c++ features for swift framework end

JUB_RV JUB_CreateContextHC(IN CONTEXT_CONFIG_HC cfg,
                           IN JUB_UINT16 deviceID,
                           OUT JUB_UINT16* contextID) {

    CREATE_THREAD_LOCK_GUARD
    auto token = std::make_shared<jub::token::JubiterBladeHCImpl>(deviceID);

    jub::context::HCContext* context = new jub::context::HCContext(cfg, std::dynamic_pointer_cast<jub::token::BTCTokenInterface>(token));
    JUB_CHECK_NULL(context);

	*contextID = jub::context::ContextManager::GetInstance()->AddOne(context);
    JUB_VERIFY_RV(context->ActiveSelf());

    return JUBR_OK;
}

JUB_RV JUB_GetAddressHC(IN JUB_UINT16 contextID,
                        IN BIP44_Path path,
                        IN JUB_ENUM_BOOL bshow,
                        OUT JUB_CHAR_PTR_PTR address) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::HCContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string strAddress;
    JUB_VERIFY_RV(context->GetAddress(JUB_ENUM_BTC_ADDRESS_FORMAT::OWN, path, bshow, strAddress));
    JUB_VERIFY_RV(_allocMem(address, strAddress));

    return JUBR_OK;
}

JUB_RV JUB_GetHDNodeHC(IN JUB_UINT16 contextID,
                       IN BIP44_Path path,
                       OUT JUB_CHAR_PTR_PTR xpub) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::HCContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string strXpub;
    JUB_VERIFY_RV(context->GetHDNode(path, strXpub));
    JUB_VERIFY_RV(_allocMem(xpub, strXpub));

    return JUBR_OK;
}

JUB_RV JUB_GetMainHDNodeHC(IN JUB_UINT16 contextID,
                           OUT JUB_CHAR_PTR_PTR xpub) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::HCContext>(contextID);
    JUB_CHECK_NULL(context);

    std::string strXpub;
    JUB_VERIFY_RV(context->GetMainHDNode(strXpub));
    JUB_VERIFY_RV(_allocMem(xpub, strXpub));

    return JUBR_OK;
}

JUB_RV JUB_SignTransactionHC(IN JUB_UINT16 contextID,
                             IN INPUT_HC inputs[], IN JUB_UINT16 iCount,
                             IN OUTPUT_HC outputs[], IN JUB_UINT16 oCount,
                             IN JUB_CHAR_CPTR unsignedTrans,
                             OUT JUB_CHAR_PTR_PTR raw) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ContextManager::GetInstance()->GetOneSafe<jub::context::HCContext>(contextID);
    JUB_CHECK_NULL(context);
    JUB_CHECK_NULL(unsignedTrans);

    std::vector<INPUT_HC> vInputs(inputs, inputs + iCount);
    std::vector<OUTPUT_HC> vOutputs(outputs, outputs + oCount);

    std::string strRaw;
    auto rv = context->signTX(JUB_ENUM_BTC_ADDRESS_FORMAT::OWN, vInputs, vOutputs, unsignedTrans, strRaw);
    if (   JUBR_OK          == rv
        || JUBR_MULTISIG_OK == rv
        ) {
        if (JUBR_OK != _allocMem(raw, strRaw)) {
            JUB_VERIFY_RV(JUBR_HOST_MEMORY);
        }
        else {
            JUB_VERIFY_RV(rv);
        }
    }

    return rv;
}
