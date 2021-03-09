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

    JUB_VERIFY_RV(token->GetHDNode(format, _mainPath, xpub));

    return JUBR_OK;
}


JUB_RV DOTContext::GetAddress(BIP44_Path path, JUB_UINT16 tag, std::string& address) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::DOTTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddress(strPath, tag, address));

    return JUBR_OK;
}


JUB_RV DOTContext::SetMyAddress(BIP44_Path path, std::string& address) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::DOTTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddress(strPath, 0x02, address));

    return JUBR_OK;
}


JUB_RV DOTContext::GetHDNode(JUB_BYTE format, BIP44_Path path, std::string& pubkey) {

    CONTEXT_CHECK_TYPE_PUBLIC

    auto token = std::dynamic_pointer_cast<jub::token::DOTTokenInterface>(_tokenPtr);
    if (!token) {
        return JUBR_IMPL_NOT_SUPPORT;
    }

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetHDNode(format, strPath, pubkey));

    return JUBR_OK;
}


JUB_RV DOTContext::SignTransaction(BIP44_Path path,
                                   const JUB_UINT64 nonce,
                                   const JUB_UINT64 gasLimit,
                                   JUB_CHAR_CPTR gasPriceInAtto,
                                   JUB_CHAR_CPTR to,
                                   JUB_CHAR_CPTR valueInAtto,
                                   JUB_CHAR_CPTR input,
                                   OUT std::string& raw) {

//    CONTEXT_CHECK_TYPE_PRIVATE
//
//    auto token = std::dynamic_pointer_cast<jub::token::DOTTokenInterface>(_tokenPtr);
//    if (!token) {
//        return JUBR_IMPL_NOT_SUPPORT;
//    }
//
//    JUB_CHECK_NULL(gasPriceInAtto);
//    JUB_CHECK_NULL(to);
//    JUB_CHECK_NULL(valueInAtto);
//
//    std::string strPath = _FullBip44Path(path);
////    std::vector<JUB_BYTE> vPath(strPath.begin(), strPath.end());
//
//    uint64_t gasPrice;
//    std::istringstream issGasPrice(gasPriceInAtto);
//    issGasPrice >> gasPrice;
//
//    uint64_t value;
//    std::istringstream issValue(valueInAtto);
//    issValue >> value;
//
//    try {
//        std::vector<JUB_BYTE> vSignature;
//        JUB_VERIFY_RV(token->SignTX(nonce,
//                                    gasPrice,
//                                    gasLimit,
//                                    std::string(to),
//                                    value,
//                                    input,
//                                    strPath,
//                                    vSignature));
//
//        // finish transaction
//        std::string from;
//        JUB_VERIFY_RV(GetAddress(path, 0, from));
//
//        TW::Polkadot::Address fromAddr = TW::Polkadot::Address(from);
//        TW::Polkadot::Transaction tx(TW::Polkadot::Address(to),
//                                     fromAddr,
//                                     nonce,
//                                     value,
//                                     gasPrice,
//                                     gasLimit);
//
//#if defined(DEBUG)
//        // Verify
//        std::string pubkey;
//        JUB_VERIFY_RV(token->GetHDNode(JUB_ENUM_PUB_FORMAT::HEX, strPath, pubkey));
//
//        TW::PublicKey verifyPubk(TW::Data(uchar_vector(pubkey)), TWPublicKeyType::TWPublicKeyTypeSECP256k1);
//
//        TW::Polkadot::Address verifyAddr = TW::Polkadot::Address(verifyPubk);
//        if (!(fromAddr == verifyAddr)) {
//            return JUBR_ARGUMENTS_BAD;
//        }
//
//        // verify sign failed for Lite.
//        if (!TW::Polkadot::Signer::verify(verifyPubk, tx, vSignature)) {
//            return JUBR_VERIFY_SIGN_FAILED;
//        }
//#endif
//
//        raw = uchar_vector(tx.serialize(vSignature)).getHex();
//    }
//    catch (...) {
//        return JUBR_ARGUMENTS_BAD;
//    }

    return JUBR_OK;
}


std::string DOTContext::_FullBip44Path(const BIP44_Path& path) {

    return _mainPath + "/"
        + std::to_string(path.change) + "'/"
        + std::to_string(path.addressIndex)+ "'";
}

} // namespace context end
} // namespace jub end
