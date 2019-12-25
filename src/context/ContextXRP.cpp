//
//  ContextXRP.cpp
//  JubSDK
//
//  Created by Pan Min on 2019/11/25.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "utility/util.h"

#include "context/ContextXRP.h"
#include "token/interface/TokenInterface.hpp"
#include "libXRP/libXRP.hpp"
#include "Ripple/Transaction.h"
#include "Ripple/Signer.h"

namespace jub {

JUB_RV ContextXRP::ActiveSelf() {

    auto token = dynamic_cast<XRPTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    auto ctoken = dynamic_cast<CommonTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);
    JUB_CHECK_NULL(ctoken);

    JUB_VERIFY_RV(token->SelectAppletXRP());
    JUB_VERIFY_RV(ctoken->SetTimeout(_timeout));
    JUB_VERIFY_RV(token->SetCoinTypeXRP());

    return JUBR_OK;
}

JUB_RV ContextXRP::GetMainHDNode(JUB_BYTE format, std::string& xpub) {

    auto token = dynamic_cast<XRPTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->GetHDNodeXRP(format, _mainPath, xpub));

    return JUBR_OK;
}

JUB_RV ContextXRP::GetAddress(BIP44_Path path, JUB_UINT16 tag, std::string& address) {

    auto token = dynamic_cast<XRPTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddressXRP(strPath, tag, address));

    return JUBR_OK;
}

JUB_RV ContextXRP::SetMyAddress(BIP44_Path path, std::string& address) {

    auto token = dynamic_cast<XRPTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddressXRP(strPath, 0x02, address));

    return JUBR_OK;
}

JUB_RV ContextXRP::GetHDNode(JUB_BYTE format, BIP44_Path path, std::string& pubkey) {

    auto token = dynamic_cast<XRPTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetHDNodeXRP(format, strPath, pubkey));

    return JUBR_OK;
}

JUB_RV ContextXRP::SignTransaction(BIP44_Path path,
                                   const JUB_TX_XRP& tx,
                                   std::string& signedRaw) {

    JUB_CHECK_NULL(tx.account);
    JUB_CHECK_NULL(tx.fee);
    JUB_CHECK_NULL(tx.sequence);

    auto token = dynamic_cast<XRPTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip44Path(path);
    std::vector<JUB_BYTE> vPath(strPath.begin(), strPath.end());

    try {
        uchar_vector vPreimage;
        JUB_VERIFY_RV(jub::xrp::serializePreimage(tx,
                                                  vPreimage));

        std::vector<uchar_vector> vSignatureRaw;
        JUB_VERIFY_RV(token->SignTXXRP(vPath,
                                       vPreimage,
                                       vSignatureRaw));

        // finish transaction
        TW::Ripple::Transaction tx;
        tx.setPreImage(vPreimage);
        if (1 < vSignatureRaw.size()) {
            return JUBR_ARGUMENTS_BAD;
        }
        tx.signature = vSignatureRaw[0];

        // Verify
        uchar_vector vTx(tx.serialize());
        TW::PublicKey verifyPubk(tx.pub_key, TWPublicKeyTypeSECP256k1);
        TW::Ripple::Signer verifySigner;
        if (!verifySigner.verify(verifyPubk, tx)) {
            return JUBR_ERROR;
        }

        signedRaw = vTx.getHex();
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}

} // namespace jub end
