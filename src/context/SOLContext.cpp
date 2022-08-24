//
//  ContextSOL.cpp
//  JubSDK
//
//  Created by Pan Min on 2022/7/20.
//  Copyright © 2022 JuBiter. All rights reserved.
//

#include "context/SOLContext.h"
#include "Base58.h"
#include "Data.h"
#include "JUB_SDK_COMM.h"
#include "PublicKey.h"
#include "Solana/Address.h"
#include "Solana/Signer.h"
#include "Solana/Transaction.h"
#include "mSIGNA/stdutils/uchar_vector.h"
#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/interface/SOLTokenInterface.hpp"
//#include <SOL/Signer.h>
//#include <SOL/Transaction.h>
//#include <SOL/PackedTransaction.h>
#include "utility/util.h"
#include <string>
#include <vector>

namespace jub {
namespace context {

JUB_RV SOLContext::ActiveSelf() {

    auto token = std::dynamic_pointer_cast<jub::token::SOLTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_VERIFY_RV(token->GetAppletVersion(_appletVersion));
    JUB_RV rv = token->SelectApplet();
    if (JUBR_OK != rv) {
        return rv;
    }
    JUB_VERIFY_RV(token->SetCoin());

    if (std::dynamic_pointer_cast<token::JubiterBladeToken>(_tokenPtr) ||
        std::dynamic_pointer_cast<token::JubiterBIOToken>(_tokenPtr)) {
        JUB_VERIFY_RV(SetTimeout(_timeout));
    }

#if defined(NFC_MODE)
    // For NFC devices, the session is cleaned up so that the ActiveSelf() function can be started at every session
    // level operation.
    if (std::dynamic_pointer_cast<token::JubiterLiteToken>(_tokenPtr)) {
        jub::context::ContextManager::GetInstance()->ClearLast();
    }
#endif // #if defined(NFC_MODE) end

    return JUBR_OK;
}

JUB_RV SOLContext::GetMainHDNode(const JUB_BYTE format, std::string &xpub) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::SOLTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_VERIFY_RV(token->GetHDNode(format, _mainPath, xpub));

    return JUBR_OK;
}

JUB_RV SOLContext::GetAddress(const std::string &path, const JUB_UINT16 tag, std::string &address) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::SOLTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_VERIFY_RV(token->GetAddress(path, tag, address));

    return JUBR_OK;
}

JUB_RV SOLContext::SetMyAddress(const BIP48_Path &path, std::string &address) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::SOLTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip48Path(path);
    JUB_VERIFY_RV(token->GetAddress(strPath, 0x02, address));

    return JUBR_OK;
}

JUB_RV SOLContext::CheckAddress(const std::string &address) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::SOLTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }
    JUB_VERIFY_RV(token->CheckAddress(address));
    return JUBR_OK;
}

JUB_RV SOLContext::GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::SOLTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_VERIFY_RV(token->GetHDNode(format, path, pubkey));

    return JUBR_OK;
}

namespace {
inline TW::Data a2d(JUB_CHAR_CPTR a) { return TW::Solana::Address{a}.vector(); }
} // namespace

JUB_RV SOLContext::SetToken(JUB_CHAR_CPTR name, JUB_UINT8 decimal, JUB_CHAR_CPTR tokenMint) {
    CONTEXT_CHECK_TYPE_PRIVATE

    auto token = std::dynamic_pointer_cast<jub::token::SOLTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }
    JUB_VERIFY_RV(token->SetTokenInfo(name, decimal, tokenMint));

    return JUBR_OK;
}

JUB_RV SOLContext::SignTransferTx(JUB_CHAR_CPTR path, JUB_CHAR_CPTR recentHash, JUB_CHAR_CPTR dest, JUB_UINT64 amount,
                                  std::string &txRaw) {

    CONTEXT_CHECK_TYPE_PRIVATE

    auto token = std::dynamic_pointer_cast<jub::token::SOLTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    auto raw = TW::Data{};

    JUB_VERIFY_RV(token->SignTransferTx(path, a2d(recentHash), a2d(dest), amount, raw));
#if defined(DEBUG)
    // verify
    std::string pubkey;
    JUB_VERIFY_RV(token->GetHDNode((JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX, path, pubkey));
    auto keys = std::vector<TW::PublicKey>{TW::PublicKey{uchar_vector{pubkey}, TWPublicKeyTypeED25519}};

    auto signer = TW::Solana::Signer{};
    auto tx     = TW::Solana::Transaction::decode(raw);
    JUB_VERIFY_RV(signer.verify(keys, tx));
#endif
    txRaw = TW::Base58::bitcoin.encode(raw);
    return JUBR_OK;
}

JUB_RV SOLContext::SignTokenTransferTx(JUB_CHAR_CPTR path, JUB_CHAR_CPTR recentHash, JUB_CHAR_CPTR tokenMint,
                                       JUB_CHAR_CPTR source, JUB_CHAR_CPTR dest, JUB_UINT64 amount, JUB_UINT8 decimal,
                                       std::string &txRaw) {
    CONTEXT_CHECK_TYPE_PRIVATE

    auto token = std::dynamic_pointer_cast<jub::token::SOLTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }
    auto raw = TW::Data{};

    JUB_VERIFY_RV(token->SignTokenTransferTx(path, a2d(recentHash), a2d(tokenMint), a2d(source), a2d(dest), amount,
                                             decimal, raw));
#if defined(DEBUG)
    // verify
    std::string pubkey;
    JUB_VERIFY_RV(token->GetHDNode((JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX, path, pubkey));
    auto keys = std::vector<TW::PublicKey>{TW::PublicKey{uchar_vector{pubkey}, TWPublicKeyTypeED25519}};

    auto signer = TW::Solana::Signer{};
    auto tx     = TW::Solana::Transaction::decode(raw);
    JUB_VERIFY_RV(signer.verify(keys, tx));
#endif
    txRaw = TW::Base58::bitcoin.encode(raw);
    return JUBR_OK;
}

JUB_RV SOLContext::SignCreateTokenAccountTx(JUB_CHAR_CPTR path, JUB_CHAR_CPTR recentHash, JUB_CHAR_CPTR owner,
                                            JUB_CHAR_CPTR tokenMint, std::string &txRaw) {
    CONTEXT_CHECK_TYPE_PRIVATE

    auto token = std::dynamic_pointer_cast<jub::token::SOLTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    auto raw = TW::Data{};

    JUB_VERIFY_RV(token->SignCreateTokenAccountTx(path, a2d(recentHash), a2d(owner), a2d(tokenMint), raw));
#if defined(DEBUG)
    // verify
    std::string pubkey;
    JUB_VERIFY_RV(token->GetHDNode((JUB_BYTE)JUB_ENUM_PUB_FORMAT::HEX, path, pubkey));
    auto keys = std::vector<TW::PublicKey>{TW::PublicKey{uchar_vector{pubkey}, TWPublicKeyTypeED25519}};

    auto signer = TW::Solana::Signer{};
    auto tx     = TW::Solana::Transaction::decode(raw);
    JUB_VERIFY_RV(signer.verify(keys, tx));
#endif
    txRaw = TW::Base58::bitcoin.encode(raw);
    return JUBR_OK;
}

} // namespace context
} // namespace jub
