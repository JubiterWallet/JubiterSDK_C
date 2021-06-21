//
//  ContextFIL.cpp
//  JubSDK
//
//  Created by Pan Min on 2020/11/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include "context/FILContext.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/JubiterLite/JubiterLiteToken.h"
#include "token/interface/FILTokenInterface.hpp"
#include <Filecoin/Signer.h>


namespace jub {
namespace context {


JUB_RV FILContext::ActiveSelf() {

    auto token = std::dynamic_pointer_cast<jub::token::FILTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_VERIFY_RV(token->SelectApplet());
    JUB_VERIFY_RV(token->SetCoin());
    if (   std::dynamic_pointer_cast<token::JubiterBladeToken>(_tokenPtr)
        || std::dynamic_pointer_cast<token::JubiterBIOToken>(_tokenPtr)
        ) {
        JUB_VERIFY_RV(SetTimeout(_timeout));
    }

#if defined(NFC_MODE)
    // For NFC devices, the session is cleaned up so that the ActiveSelf() function can be started at every session level operation.
    if (std::dynamic_pointer_cast<token::JubiterLiteToken>(_tokenPtr)) {
        jub::context::ContextManager::GetInstance()->ClearLast();
    }
#endif // #if defined(NFC_MODE) end

    return JUBR_OK;
}


JUB_RV FILContext::GetMainHDNode(JUB_BYTE format, std::string& xpub) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::FILTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_VERIFY_RV(token->GetHDNode(format, _mainPath, xpub));

    return JUBR_OK;
}


JUB_RV FILContext::GetAddress(BIP44_Path path, JUB_UINT16 tag, std::string& address) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::FILTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddress(strPath, tag, address));

    return JUBR_OK;
}


JUB_RV FILContext::SetMyAddress(BIP44_Path path, std::string& address) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::FILTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddress(strPath, 0x02, address));

    return JUBR_OK;
}


JUB_RV FILContext::GetHDNode(JUB_BYTE format, BIP44_Path path, std::string& pubkey) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::FILTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetHDNode(format, strPath, pubkey));

    return JUBR_OK;
}


JUB_RV FILContext::SignTransaction(BIP44_Path path,
                                   const JUB_UINT64 nonce,
                                   const JUB_UINT64 gasLimit,
                                   JUB_CHAR_CPTR gasFeeCapInAtto,
                                   JUB_CHAR_CPTR gasPremiumInAtto,
                                   JUB_CHAR_CPTR to,
                                   JUB_CHAR_CPTR valueInAtto,
                                   JUB_CHAR_CPTR input,
                                   OUT std::string& raw) {

    CONTEXT_CHECK_TYPE_PRIVATE

    auto token = std::dynamic_pointer_cast<jub::token::FILTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_CHECK_NULL(gasFeeCapInAtto);
    JUB_CHECK_NULL(gasPremiumInAtto);
    JUB_CHECK_NULL(to);
    JUB_CHECK_NULL(valueInAtto);

    std::string strPath = _FullBip44Path(path);

    uint256_t gasFeeCap(gasFeeCapInAtto, 10);
    uint256_t gasPremium(gasPremiumInAtto, 10);
    uint256_t value(valueInAtto, 10);

    try {
        std::vector<uchar_vector> vSignature;
        JUB_VERIFY_RV(token->SignTX(nonce,
                                    gasLimit,
                                    gasFeeCap,
                                    gasPremium,
                                    std::string(to),
                                    value,
                                    input,
                                    strPath,
                                    vSignature));

        // finish transaction
        std::string from;
        JUB_VERIFY_RV(GetAddress(path, 0, from));

        TW::Filecoin::Address fromAddr = TW::Filecoin::Address(from);
        TW::Filecoin::Transaction tx(TW::Filecoin::Address(to),
                                     fromAddr,
                                     nonce,
                                     value,
                                     gasLimit,
                                     gasFeeCap,
                                     gasPremium);

#if defined(DEBUG)
        // Verify
        std::string pubkey;
        JUB_VERIFY_RV(token->GetHDNode(JUB_ENUM_PUB_FORMAT::HEX, strPath, pubkey));

        TW::PublicKey verifyPubk(TW::Data(uchar_vector(pubkey)), TWPublicKeyType::TWPublicKeyTypeSECP256k1);
        TW::Filecoin::Address verifyAddr = TW::Filecoin::Address(verifyPubk);
        if (verifyPubk.isCompressed()) {
            verifyAddr = TW::Filecoin::Address(verifyPubk.extended());
        }

        if (!(fromAddr == verifyAddr)) {
            return JUBR_ARGUMENTS_BAD;
        }

        // verify sign failed for Lite.
        if (!TW::Filecoin::Signer::verify(verifyPubk, tx, vSignature[0])) {
            return JUBR_VERIFY_SIGN_FAILED;
        }
#endif

        raw = tx.serialize(vSignature[0]);
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


JUB_RV FILContext::CalculateTransactionCID(BIP44_Path path,
                                           const JUB_UINT64 nonce,
                                           const JUB_UINT64 gasLimit,
                                           JUB_CHAR_CPTR gasFeeCapInAtto,
                                           JUB_CHAR_CPTR gasPremiumInAtto,
                                           JUB_CHAR_CPTR to,
                                           JUB_CHAR_CPTR valueInAtto,
                                           JUB_CHAR_CPTR input,
                                           OUT std::string& cid) {

    CONTEXT_CHECK_TYPE_PRIVATE

    auto token = std::dynamic_pointer_cast<jub::token::FILTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_CHECK_NULL(gasFeeCapInAtto);
    JUB_CHECK_NULL(gasPremiumInAtto);
    JUB_CHECK_NULL(to);
    JUB_CHECK_NULL(valueInAtto);

    std::string strPath = _FullBip44Path(path);

    uint256_t gasFeeCap(gasFeeCapInAtto, 10);
    uint256_t gasPremium(gasPremiumInAtto, 10);
    uint256_t value(valueInAtto, 10);

    try {
        std::string from;
        JUB_VERIFY_RV(GetAddress(path, 0, from));

        TW::Filecoin::Address fromAddr = TW::Filecoin::Address(from);
        TW::Filecoin::Transaction tx(TW::Filecoin::Address(to),
                                     fromAddr,
                                     nonce,
                                     value,
                                     gasLimit,
                                     gasFeeCap,
                                     gasPremium);

        cid = TW::Filecoin::Transaction::cid(tx);
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


} // namespace context end
} // namespace jub end
