/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <JUB_CORE.h>
#include <string>
#include <TrezorCrypto/bip39.h>
#include <TrezorCrypto/bip32.h>
#include <utility/util.h>
#include <HDKey/constant.hpp>
#include <token/TrezorCryptoImpl.h>
#include <context/ContextBTC.h>
#include <context/ContextETH.h>
#include <token/interface/BTCTokenInterface.hpp>

//where to place...
JUB_RV _allocMem(JUB_CHAR_PTR_PTR memPtr, const std::string &strBuf);

JUB_RV _curveToString(JUB_CURVES enumCurve, std::string& strCurve) {

    switch(enumCurve) {
        case secp256k1:
            strCurve = "secp256k1";
            return JUBR_OK;
        case ed25519:
            strCurve = "ed25519";
            return JUBR_OK;
        case nist256p1:
            strCurve = "nist256p1";
            return JUBR_OK;
        default:
            return JUBR_ARGUMENTS_BAD;   
    }
}

JUB_RV JUB_GenerateMnemonic(IN JUB_MNEMONIC_STRENGTH strength,
                            OUT JUB_CHAR_PTR_PTR mnemonic) {

    JUB_CHAR _mnemonic[240] = {0,};
    if(mnemonic_generate(strength, _mnemonic)) {
        JUB_VERIFY_RV(_allocMem(mnemonic,std::string(_mnemonic)));
        return JUBR_OK;
    }
    else {
        return JUBR_ERROR;
    }
}

JUB_RV JUB_CheckMnemonic(IN JUB_CHAR_CPTR mnemonic) {
    if(0 == mnemonic_check(mnemonic)) {
        return JUBR_ERROR;
    }
    else {
        return JUBR_OK;
    }
}

JUB_RV JUB_GenerateSeed(IN JUB_CHAR_CPTR mnemonic, IN JUB_CHAR_CPTR passphrase, OUT JUB_BYTE seed[64],
                        void (*progress_callback)(JUB_UINT32 current, JUB_UINT32 total)) {

    JUB_CHECK_NULL(mnemonic);
    JUB_CHECK_NULL(passphrase);
    mnemonic_to_seed(mnemonic,passphrase,seed,progress_callback);
    return JUBR_OK;
}

JUB_RV JUB_SeedToMasterPrivateKey(IN JUB_BYTE_PTR seed, IN JUB_UINT16 seed_len, IN JUB_CURVES curve,
                                  OUT JUB_CHAR_PTR_PTR prikeyInXprv) {
    JUB_CHECK_NULL(seed);
    std::string curve_name;
    JUB_VERIFY_RV(_curveToString(curve,curve_name));

    HDNode node;
    if(0 == hdnode_from_seed(seed, seed_len, curve_name.c_str(), &node)) {
        return JUBR_ERROR;
    }

    JUB_CHAR str_pri[200] = {0,};
    if(0 == hdnode_serialize_private(&node, 0x00, defaultXPRV, str_pri, 200)) {
        return JUBR_ERROR;
    }

    JUB_VERIFY_RV(_allocMem(prikeyInXprv,std::string(str_pri)));

    return JUBR_OK;
}

template<typename T , typename TBase> class TIsExtended {
public:
    static int t(TBase* base) {
        return 1;
    }
    static char t(void* t2) {
        return 2;
    }
    enum {
        Result = ( sizeof(int) == sizeof(t( (T*)NULL) )),
    };
};

JUB_RV JUB_CreateContextBTC_soft(IN CONTEXT_CONFIG_BTC cfg,
                                 IN JUB_CHAR_PTR masterPriInXPRV,
                                 OUT JUB_UINT16* contextID) {
    static_assert(TIsExtended<jub::TrezorCryptoImpl,
                  jub::BTCTokenInterface>::Result,
                  "Token class should be extended from BTCTokenInterface");

    jub::TrezorCryptoImpl* token = new jub::TrezorCryptoImpl(std::string(masterPriInXPRV));
    JUB_UINT16 deviceID = jub::TokenManager::GetInstance()->AddOne(token);
    jub::ContextBTC* context = new jub::ContextBTC(cfg, deviceID);
    JUB_CHECK_NULL(context);
    *contextID = jub::ContextManager::GetInstance()->AddOne(context);

    return JUBR_OK;
}

JUB_RV JUB_CreateContextETH_soft(IN CONTEXT_CONFIG_ETH cfg,
                                 IN JUB_CHAR_PTR masterPriInXPRV,
                                 OUT JUB_UINT16* contextID) {
    static_assert(TIsExtended<jub::TrezorCryptoImpl,
                  jub::ETHTokenInterface>::Result,
                  "Token class should be extended from ETHTokenInterface");

    jub::TrezorCryptoImpl* token = new jub::TrezorCryptoImpl(std::string(masterPriInXPRV));
    JUB_UINT16 deviceID = jub::TokenManager::GetInstance()->AddOne(token);
    jub::ContextETH* context = new jub::ContextETH(cfg, deviceID);
    JUB_CHECK_NULL(context);
    *contextID = jub::ContextManager::GetInstance()->AddOne(context);

    return JUBR_OK;
}
