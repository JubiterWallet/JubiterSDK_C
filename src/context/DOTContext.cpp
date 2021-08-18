//
//  ContextDOT.cpp
//  JubSDK
//
//  Created by Pan Min on 2020/11/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#include "context/DOTContext.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/JubiterLite/JubiterLiteToken.h"
#include "token/interface/DOTTokenInterface.hpp"
#include <Polkadot/Signer.h>

namespace jub {
namespace context {


JUB_RV DOTContext::ActiveSelf() {

    auto token = std::dynamic_pointer_cast<jub::token::DOTTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }
    JUB_VERIFY_RV(token->SetCoinCurvePublicKeyType(_coinType, _curve));
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

JUB_RV DOTContext::GetMainHDNode(JUB_BYTE format, std::string& xpub) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::DOTTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_VERIFY_RV(token->GetHDNode(format, "", xpub));

    return JUBR_OK;
}


JUB_RV DOTContext::GetAddress(std::string path, JUB_UINT16 tag, std::string& address) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::DOTTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_VERIFY_RV(token->GetAddress(path, tag, address));

    return JUBR_OK;
}


JUB_RV DOTContext::SetMyAddress(std::string path, std::string& address) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::DOTTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    JUB_VERIFY_RV(token->GetAddress(path, 0x02, address));

    return JUBR_OK;
}


JUB_RV DOTContext::GetHDNode(JUB_BYTE format, std::string path, std::string& pubkey) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::DOTTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    
    JUB_VERIFY_RV(token->GetHDNode(format, path, pubkey));

    return JUBR_OK;
}
JUB_RV DOTContext::SignTransaction(std::string path,
                                   const JUB_TX_DOT& tx,
                                   std::string& signedRaw) {
    CONTEXT_CHECK_TYPE_PRIVATE

    auto token = std::dynamic_pointer_cast<jub::token::DOTTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    try {
        std::vector<JUB_BYTE> vSignature;
        std::string genesisHash = tx.genesisHash;
        std::string blockHash = tx.blockHash;
        std::string to = tx.to;
        uint64_t nonce = tx.nonce;
        uint32_t specVersion = tx.specVersion;
        uint64_t network = tx.network;
        uint32_t transaction_version = tx.transaction_version;
        uint64_t blockNumber = tx.blockNumber;
        std::string value = tx.value;
        uint64_t eraPeriod = tx.eraPeriod;
        std::string tip = tx.tip;
        
        JUB_VERIFY_RV(token->SignTX(path,genesisHash, blockHash, to, nonce, specVersion, network, transaction_version, blockNumber, value, eraPeriod, tip, vSignature));
        
        signedRaw = uchar_vector(vSignature).getHex();

    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }
    return JUBR_OK;
}


} // namespace context end
} // namespace jub end
