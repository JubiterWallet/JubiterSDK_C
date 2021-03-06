/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "JUB_CORE.h"

#include "utility/util.h"
#include "utility/mutex.h"

#include <string>
#include <TrezorCrypto/bip39.h>
#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/curves.h>

#include "context/BTCContextFactory.h"
#include "context/HCContextFactory.h"
#include "context/ETHContextFactory.h"
#include "context/EOSContextFactory.h"
#include "context/XRPContextFactory.h"
#include "context/TRXContextFactory.h"
#include "context/FILContextFactory.h"


//where to place...
JUB_RV _allocMem(JUB_CHAR_PTR_PTR memPtr, const std::string &strBuf);

JUB_RV _curveToString(JUB_ENUM_CURVES enumCurve, std::string& strCurve) {

    switch(enumCurve) {
    case JUB_ENUM_CURVES::SECP256K1:
        strCurve = SECP256K1_NAME;
        return JUBR_OK;
    case JUB_ENUM_CURVES::ED25519:
        strCurve = ED25519_NAME;
        return JUBR_OK;
    case JUB_ENUM_CURVES::NIST256P1:
        strCurve = NIST256P1_NAME;
        return JUBR_OK;
    default:
        return JUBR_ARGUMENTS_BAD;   
    }
}

JUB_RV JUB_GenerateMnemonic_soft(IN JUB_ENUM_MNEMONIC_STRENGTH strength,
                                 OUT JUB_CHAR_PTR_PTR mnemonic) {

    CREATE_THREAD_LOCK_GUARD
    JUB_CHAR _mnemonic[240] = {0,};
    if (mnemonic_generate(strength, _mnemonic)) {
        JUB_VERIFY_RV(_allocMem(mnemonic, std::string(_mnemonic)));
        return JUBR_OK;
    }
    else {
        return JUBR_ERROR;
    }
}

JUB_RV JUB_CheckMnemonic(IN JUB_CHAR_CPTR mnemonic) {

    CREATE_THREAD_LOCK_GUARD
    if (0 == mnemonic_check(mnemonic)) {
        return JUBR_ERROR;
    }
    else {
        return JUBR_OK;
    }
}

JUB_RV JUB_GenerateSeed_soft(IN JUB_CHAR_CPTR mnemonic,
                             IN JUB_CHAR_CPTR passphrase,
                             OUT JUB_BYTE seed[64],
                             void (*progress_callback)(JUB_UINT32 current, JUB_UINT32 total)) {

    CREATE_THREAD_LOCK_GUARD
    JUB_CHECK_NULL(mnemonic);
    JUB_CHECK_NULL(passphrase);
    mnemonic_to_seed(mnemonic, passphrase, seed, progress_callback);
    return JUBR_OK;
}

JUB_RV JUB_SeedToMasterPrivateKey_soft(IN JUB_BYTE_CPTR seed, IN JUB_UINT16 seed_len,
                                       IN JUB_ENUM_CURVES curve,
                                       OUT JUB_CHAR_PTR_PTR prikeyInXprv) {

    CREATE_THREAD_LOCK_GUARD
    JUB_UINT32 hdVersionPrv = TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin);
    JUB_UINT32 hdVersionPub = TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin);

    JUB_CHECK_NULL(seed);
    std::string curve_name;
    JUB_VERIFY_RV(_curveToString(curve, curve_name));

    HDNode node;
    if (0 == hdnode_from_seed(seed, seed_len, curve_name.c_str(), &node)) {
        return JUBR_ERROR;
    }

    JUB_CHAR str_pri[200] = {0,};
    if (0 == hdnode_serialize_private(&node, 0x00, hdVersionPrv, str_pri, sizeof(str_pri) / sizeof(JUB_CHAR))) {
        return JUBR_ERROR;
    }

    JUB_CHAR str_pub[200] = {0,};
    if (0 == hdnode_serialize_public(&node, 0x00, hdVersionPub, str_pub, sizeof(str_pub) / sizeof(JUB_CHAR))) {
        return JUBR_ERROR;
    }

    JUB_VERIFY_RV(_allocMem(prikeyInXprv, std::string(str_pri)));

    return JUBR_OK;
}

// template<typename T , typename TBase> class TIsExtended {
// public:
//     static int t(TBase* base) {
//         return 1;
//     }
//     static char t(void* t2) {
//         return 2;
//     }
//     enum {
//         Result = ( sizeof(int) == sizeof(t( (T*)NULL) )),
//     };
// };


JUB_RV JUB_CreateContextBTC_soft(IN CONTEXT_CONFIG_BTC cfg,
                                 IN JUB_CHAR_CPTR XPRVorXPUB,
                                 OUT JUB_UINT16* contextID) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::BTCseriesContextFactory::GetInstance()->CreateContext(cfg, XPRVorXPUB);
    JUB_CHECK_NULL(context);

    *contextID = jub::context::ContextManager::GetInstance()->AddOne(context);

    return JUBR_OK;
}

JUB_RV JUB_CreateContextHC_soft(IN CONTEXT_CONFIG_HC cfg,
                                IN JUB_CHAR_CPTR masterPriInXPRV,
                                OUT JUB_UINT16* contextID) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::HCseriesContextFactory::GetInstance()->CreateContext(cfg, masterPriInXPRV);
    JUB_CHECK_NULL(context);

    *contextID = jub::context::ContextManager::GetInstance()->AddOne(context);

    return JUBR_OK;
}

JUB_RV JUB_CreateContextETH_soft(IN CONTEXT_CONFIG_ETH cfg,
                                 IN JUB_CHAR_CPTR masterPriInXPRV,
                                 OUT JUB_UINT16* contextID) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::ETHseriesContextFactory::GetInstance()->CreateContext(cfg, masterPriInXPRV);
    JUB_CHECK_NULL(context);

    *contextID = jub::context::ContextManager::GetInstance()->AddOne(context);

    return JUBR_OK;
}

JUB_RV JUB_CreateContextEOS_soft(IN CONTEXT_CONFIG_EOS cfg,
                                 IN JUB_CHAR_CPTR masterPriInXPRV,
                                 OUT JUB_UINT16* contextID) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::EOSseriesContextFactory::GetInstance()->CreateContext(cfg, masterPriInXPRV);
    JUB_CHECK_NULL(context);

    *contextID = jub::context::ContextManager::GetInstance()->AddOne(context);

    return JUBR_OK;
}

JUB_RV JUB_CreateContextXRP_soft(IN CONTEXT_CONFIG_XRP cfg,
                                 IN JUB_CHAR_CPTR masterPriInXPRV,
                                 OUT JUB_UINT16* contextID) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::XRPseriesContextFactory::GetInstance()->CreateContext(cfg, masterPriInXPRV);
    JUB_CHECK_NULL(context);

    *contextID = jub::context::ContextManager::GetInstance()->AddOne(context);

    return JUBR_OK;
}

JUB_RV JUB_CreateContextTRX_soft(IN CONTEXT_CONFIG_TRX cfg,
                                 IN JUB_CHAR_CPTR masterPriInXPRV,
                                 OUT JUB_UINT16* contextID) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::TRXseriesContextFactory::GetInstance()->CreateContext(cfg, masterPriInXPRV);
    JUB_CHECK_NULL(context);

    *contextID = jub::context::ContextManager::GetInstance()->AddOne(context);

    return JUBR_OK;
}

JUB_RV JUB_CreateContextFIL_soft(IN CONTEXT_CONFIG_FIL cfg,
                                 IN JUB_CHAR_CPTR masterPriInXPRV,
                                 OUT JUB_UINT16* contextID) {

    CREATE_THREAD_LOCK_GUARD
    auto context = jub::context::FILseriesContextFactory::GetInstance()->CreateContext(cfg, masterPriInXPRV);
    JUB_CHECK_NULL(context);

    *contextID = jub::context::ContextManager::GetInstance()->AddOne(context);

    return JUBR_OK;
}
