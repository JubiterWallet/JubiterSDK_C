/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <JUB_core.h>
#include <string>
#include <TrezorCrypto/bip39.h>

JUB_RV _allocMem(JUB_CHAR_PTR_PTR memPtr, const std::string &strBuf);

JUB_RV JUB_GenerateMnemonic(IN JUB_MNEMONIC_STRENGTH strength, JUB_CHAR_PTR_PTR mnemonic){
    
    JUB_CHAR _mnemonic[240] = {0};
    if(mnemonic_generate(strength, _mnemonic)){
    
        _allocMem(mnemonic,std::string(_mnemonic));
        return JUBR_OK;
    }
    else
        return JUBR_ERROR;
}

JUB_RV JUB_CheckMnemonic(const JUB_CHAR_PTR mnemonic){
    if(mnemonic_check(mnemonic) == 0)
        return JUBR_ERROR;
    else 
        return JUBR_OK;
}

JUB_RV JUB_GenerateSeed(IN JUB_CHAR_CPTR mnemonic, IN JUB_CHAR_CPTR passphrase, OUT JUB_BYTE seed[64], void (*progress_callback)(JUB_UINT32 current, JUB_UINT32 total)){

    mnemonic_to_seed(mnemonic,passphrase,seed,progress_callback);
    return JUBR_OK;
}
