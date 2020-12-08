//
//  ContextTRX.cpp
//  JubSDK
//
//  Created by Pan Min on 2020/09/10.
//  Copyright © 2020 JuBiter. All rights reserved.
//


#include "context/TRXContext.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/JubiterLite/JubiterLiteToken.h"
#include "token/interface/TRXTokenInterface.hpp"
#include <Tron/Signer.h>
#include "utility/util.h"


namespace jub {
namespace context {


JUB_RV TRXContext::ActiveSelf() {

    auto token = std::dynamic_pointer_cast<jub::token::TRXTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_RV rv = token->SelectApplet();
    JUB_VERIFY_RV(token->SetCoin());
    if (JUBR_OK != rv) {
        return rv;
    }

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


JUB_RV TRXContext::GetMainHDNode(const JUB_BYTE format, std::string& xpub) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::TRXTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_VERIFY_RV(token->GetHDNode(format, _mainPath, xpub));

    return JUBR_OK;
}


JUB_RV TRXContext::GetAddress(const BIP44_Path& path, const JUB_UINT16 tag, std::string& address) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::TRXTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddress(strPath, tag, address));

    return JUBR_OK;
}


JUB_RV TRXContext::SetMyAddress(const BIP44_Path& path, std::string& address) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::TRXTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddress(strPath, 0x02, address));

    return JUBR_OK;
}


JUB_RV TRXContext::GetHDNode(const JUB_BYTE format, const BIP44_Path& path, std::string& pubkey) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::TRXTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetHDNode(format, strPath, pubkey));

    return JUBR_OK;
}


JUB_RV TRXContext::GetAddress(const BIP48_Path& path, const JUB_UINT16 tag, std::string& address) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::TRXTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip48Path(path);
    JUB_VERIFY_RV(token->GetAddress(strPath, tag, address));

    return JUBR_OK;
}


JUB_RV TRXContext::SetMyAddress(const BIP48_Path& path, std::string& address) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::TRXTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip48Path(path);
    JUB_VERIFY_RV(token->GetAddress(strPath, 0x02, address));

    return JUBR_OK;
}


JUB_RV TRXContext::GetHDNode(const JUB_BYTE format, const BIP48_Path& path, std::string& pubkey) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::TRXTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip48Path(path);
    JUB_VERIFY_RV(token->GetHDNode(format, strPath, pubkey));

    return JUBR_OK;
}


JUB_RV TRXContext::SignTransaction(const BIP44_Path& path,
                                   JUB_CHAR_CPTR packedContractInPb,
                                   std::string& rawInJSON) {
    CONTEXT_CHECK_TYPE_PRIVATE

    auto token = std::dynamic_pointer_cast<jub::token::TRXTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_CHECK_NULL(packedContractInPb);

    std::string strPath = _FullBip44Path(path);
    std::vector<JUB_BYTE> vPath(strPath.begin(), strPath.end());

    uchar_vector vRaw;
    JUB_VERIFY_RV(token->SerializePreimage(packedContractInPb,
                                           vRaw));

    std::vector<uchar_vector> vSignatureRaw;
    JUB_VERIFY_RV(token->SignTX(vPath,
                                vRaw,
                                vSignatureRaw));

    // finish transaction
    try {
        TW::Tron::Transaction tx;
        tx.raw_data.deserialize(vRaw);
        tx.signature = vSignatureRaw[0];

#if defined(DEBUG)
        //verify
        std::string pubkey;
        JUB_VERIFY_RV(token->GetHDNode(JUB_ENUM_PUB_FORMAT::HEX, strPath, pubkey));

        TW::Tron::Signer signer;
        if (!signer.verify(TW::PublicKey(TW::Data(uchar_vector(pubkey)), TWPublicKeyType::TWPublicKeyTypeSECP256k1),
                           tx)) {
            return JUBR_VERIFY_SIGN_FAILED;
        }
#endif

        rawInJSON = tx.serialize().dump();
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


JUB_RV TRXContext::PackTransactionRaw(const JUB_TX_TRX& tx,
                                      std::string& packedContractInPB) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::TRXTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    try {
        JUB_VERIFY_RV(token->PackTransactionRaw(tx,
                                                packedContractInPB));
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


} // namespace context end
} // namespace jub end
