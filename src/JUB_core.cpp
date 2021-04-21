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


JUB_RV JUB_GenerateMnemonic(IN JUB_ENUM_MNEMONIC_STRENGTH strength,
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
