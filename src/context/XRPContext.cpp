//
//  ContextXRP.cpp
//  JubSDK
//
//  Created by Pan Min on 2019/11/25.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "utility/util.h"

#include "context/XRPContext.h"
#include "Ripple/Signer.h"
#include <token/interface/HardwareTokenInterface.hpp>

namespace jub {
namespace context {


JUB_RV XRPContext::ActiveSelf() {

    JUB_VERIFY_RV(_tokenPtr->SelectApplet());
    JUB_VERIFY_RV(_tokenPtr->SetCoinType());
                auto token = std::dynamic_pointer_cast<token::HardwareTokenInterface>(_tokenPtr);
    if (token) {  JUB_VERIFY_RV(SetTimeout(_timeout)); }

    return JUBR_OK;
}


JUB_RV XRPContext::GetMainHDNode(JUB_BYTE format, std::string& xpub) {

    JUB_VERIFY_RV(_tokenPtr->GetHDNode(format, _mainPath, xpub));

    return JUBR_OK;
}


JUB_RV XRPContext::GetAddress(BIP44_Path path, JUB_UINT16 tag, std::string& address) {

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(_tokenPtr->GetAddress(strPath, tag, address));

    return JUBR_OK;
}


JUB_RV XRPContext::SetMyAddress(BIP44_Path path, std::string& address) {

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(_tokenPtr->GetAddress(strPath, 0x02, address));

    return JUBR_OK;
}


JUB_RV XRPContext::GetHDNode(JUB_BYTE format, BIP44_Path path, std::string& pubkey) {

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(_tokenPtr->GetHDNode(format, strPath, pubkey));

    return JUBR_OK;
}


JUB_RV XRPContext::SignTransaction(BIP44_Path path,
                                   const JUB_TX_XRP& tx,
                                   std::string& signedRaw) {

    JUB_CHECK_NULL(tx.account);
    JUB_CHECK_NULL(tx.fee);
    JUB_CHECK_NULL(tx.sequence);

    std::string strPath = _FullBip44Path(path);
    std::vector<JUB_BYTE> vPath(strPath.begin(), strPath.end());

    try {
        uchar_vector vPreimage;
        JUB_VERIFY_RV(_tokenPtr->SerializePreimage(tx,
                                                   vPreimage));

        std::vector<uchar_vector> vSignatureRaw;
        JUB_VERIFY_RV(_tokenPtr->SignTX(vPath,
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


} // namespace context end
} // namespace jub end
