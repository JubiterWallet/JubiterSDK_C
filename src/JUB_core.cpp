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