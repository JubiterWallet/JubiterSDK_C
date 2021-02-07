//
//  ContextXRP.cpp
//  JubSDK
//
//  Created by Pan Min on 2019/11/25.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "utility/util.h"

#include "context/XRPContext.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/JubiterLite/JubiterLiteToken.h"
#include "token/interface/XRPTokenInterface.hpp"
#include <Ripple/Signer.h>


namespace jub {
namespace context {


JUB_RV XRPContext::ActiveSelf() {

    auto token = std::dynamic_pointer_cast<jub::token::XRPTokenInterface>(_tokenPtr);
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


JUB_RV XRPContext::GetMainHDNode(JUB_BYTE format, std::string& xpub) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::XRPTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_VERIFY_RV(token->GetHDNode(format, _mainPath, xpub));

    return JUBR_OK;
}


JUB_RV XRPContext::GetAddress(BIP44_Path path, JUB_UINT16 tag, std::string& address) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::XRPTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddress(strPath, tag, address));

    return JUBR_OK;
}


JUB_RV XRPContext::SetMyAddress(BIP44_Path path, std::string& address) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::XRPTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddress(strPath, 0x02, address));

    return JUBR_OK;
}


JUB_RV XRPContext::GetHDNode(JUB_BYTE format, BIP44_Path path, std::string& pubkey) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::XRPTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetHDNode(format, strPath, pubkey));

    return JUBR_OK;
}

JUB_RV XRPContext::CheckAddress(const std::string& address, std::string& addre, std::string& tag) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::XRPTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_VERIFY_RV(token->CheckAddress(address, addre, tag));

    return JUBR_OK;
}


JUB_RV XRPContext::SignTransaction(BIP44_Path path,
                                   const JUB_TX_XRP& tx,
                                   std::string& signedRaw) {

    CONTEXT_CHECK_TYPE_PRIVATE

    auto token = std::dynamic_pointer_cast<jub::token::XRPTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_CHECK_NULL(tx.account);
    JUB_CHECK_NULL(tx.fee);
    JUB_CHECK_NULL(tx.sequence);

    std::string strPath = _FullBip44Path(path);
    std::vector<JUB_BYTE> vPath(strPath.begin(), strPath.end());

    try {
        uchar_vector vPreimage;
        JUB_VERIFY_RV(token->SerializePreimage(tx,
                                               vPreimage));

        std::vector<uchar_vector> vSignatureRaw;
        JUB_VERIFY_RV(token->SignTX(vPath,
                                    vPreimage,
                                    vSignatureRaw));

        // finish transaction
        TW::Ripple::Transaction tx;
        tx.setPreImage(vPreimage);
        if (1 < vSignatureRaw.size()) {
            return JUBR_ARGUMENTS_BAD;
        }
        tx.signature = vSignatureRaw[0];

        uchar_vector vTx(tx.serialize());

#if defined(DEBUG)
        // Verify
        TW::PublicKey verifyPubk(tx.pub_key, TWPublicKeyTypeSECP256k1);
        TW::Ripple::Signer verifySigner;
        if (!verifySigner.verify(verifyPubk, tx)) {
            return JUBR_VERIFY_SIGN_FAILED;
        }
#endif

        signedRaw = vTx.getHex();
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


} // namespace context end
} // namespace jub end
