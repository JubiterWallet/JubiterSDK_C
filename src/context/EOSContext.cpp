//
//  ContextEOS.cpp
//  JubSDK
//
//  Created by Pan Min on 2019/10/09.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "utility/util.h"

#include "context/EOSContext.h"
#include "token/interface/EOSTokenInterface.hpp"
#include <token/interface/HardwareTokenInterface.hpp>
#include "EOS/Signer.h"
#include "EOS/Transaction.h"
#include "EOS/PackedTransaction.h"

namespace jub {
namespace context {


JUB_RV EOSContext::ActiveSelf() {

    JUB_RV rv = _tokenPtr->SelectApplet();
    if (JUBR_EOS_APP_INDEP_OK != rv
        &&            JUBR_OK != rv
        ) {
        return rv;
    }
    bool isIndep = false;
    if (JUBR_EOS_APP_INDEP_OK == rv) {
        isIndep = true;
    }
    auto token = std::dynamic_pointer_cast<token::HardwareTokenInterface>(_tokenPtr);
    if (token) {  JUB_VERIFY_RV(SetTimeout(_timeout)); }
    if (!isIndep) {
        JUB_VERIFY_RV(_tokenPtr->SetCoinType());
    }

    return JUBR_OK;
}


JUB_RV EOSContext::GetMainHDNode(const JUB_BYTE format, std::string& xpub) {

    CONTEXT_CHECK_TYPE_PUBLIC
    JUB_VERIFY_RV(_tokenPtr->GetHDNode(format, _mainPath, xpub));

    return JUBR_OK;
}


JUB_RV EOSContext::GetAddress(const BIP44_Path& path, const JUB_UINT16 tag, std::string& address) {

    CONTEXT_CHECK_TYPE_PUBLIC
    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(_tokenPtr->GetAddress(_eosType, strPath, tag, address));

    return JUBR_OK;
}


JUB_RV EOSContext::SetMyAddress(const BIP44_Path& path, std::string& address) {

    CONTEXT_CHECK_TYPE_PUBLIC
    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(_tokenPtr->GetAddress(_eosType, strPath, 0x02, address));

    return JUBR_OK;
}


JUB_RV EOSContext::GetHDNode(const JUB_BYTE format, const BIP44_Path& path, std::string& pubkey) {

    CONTEXT_CHECK_TYPE_PUBLIC
    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(_tokenPtr->GetHDNode(format, strPath, pubkey));

    return JUBR_OK;
}


JUB_RV EOSContext::GetAddress(const BIP48_Path& path, const JUB_UINT16 tag, std::string& address) {

    CONTEXT_CHECK_TYPE_PUBLIC
    std::string strPath = _FullBip48Path(path);
    JUB_VERIFY_RV(_tokenPtr->GetAddress(_eosType, strPath, tag, address));

    return JUBR_OK;
}


JUB_RV EOSContext::SetMyAddress(const BIP48_Path& path, std::string& address) {

    CONTEXT_CHECK_TYPE_PUBLIC
    std::string strPath = _FullBip48Path(path);
    JUB_VERIFY_RV(_tokenPtr->GetAddress(_eosType, strPath, 0x02, address));

    return JUBR_OK;
}


JUB_RV EOSContext::GetHDNode(const JUB_BYTE format, const BIP48_Path& path, std::string& pubkey) {

    CONTEXT_CHECK_TYPE_PUBLIC
    std::string strPath = _FullBip48Path(path);
    JUB_VERIFY_RV(_tokenPtr->GetHDNode(format, strPath, pubkey));

    return JUBR_OK;
}


JUB_RV EOSContext::SignTransaction(const BIP44_Path& path,
                                   JUB_CHAR_CPTR chainID,
                                   JUB_CHAR_CPTR expiration,
                                   JUB_CHAR_CPTR referenceBlockId,
                                   JUB_CHAR_CPTR referenceBlockTime,
                                   JUB_CHAR_CPTR actionsInJSON,
                                   std::string& rawInJSON) {
    CONTEXT_CHECK_TYPE_PRIVATE

    JUB_CHECK_NULL(expiration);
    JUB_CHECK_NULL(referenceBlockId);
    JUB_CHECK_NULL(referenceBlockTime);
    JUB_CHECK_NULL(actionsInJSON);

    std::string strPath = _FullBip44Path(path);
    std::vector<JUB_BYTE> vPath(strPath.begin(), strPath.end());

    uchar_vector vChainIds;
    if (nullptr == chainID
        ||    0 == strlen(chainID)
        ) {
        uchar_vector vChain(chainIds[4]);
        vChainIds << vChain;
    }
    else {
        uchar_vector vChain(chainID);
        vChainIds << vChain;
    }

    bool bWithType = true;
    uchar_vector vRaw;
    JUB_VERIFY_RV(_tokenPtr->SerializePreimage(expiration,
                                               referenceBlockId,
                                               referenceBlockTime,
                                               actionsInJSON,
                                               vRaw,
                                               bWithType));

    std::vector<uchar_vector> vSignatureRaw;
    JUB_VERIFY_RV(_tokenPtr->SignTX(_eosType,
                                    vPath,
                                    vChainIds,
                                    vRaw,
                                    vSignatureRaw,
                                    bWithType));

    // finish transaction
    try {
        TW::EOS::Transaction tx;
        tx.deserialize(vRaw, bWithType);

        for (const uchar_vector& signatureRaw : vSignatureRaw) {
            TW::EOS::Signature signature(signatureRaw, TW::EOS::Type::ModernK1);
            tx.signatures.push_back(signature);

            //verify
            std::string pubkey;
            JUB_VERIFY_RV(_tokenPtr->GetHDNode(JUB_ENUM_PUB_FORMAT::HEX, strPath, pubkey));
            TW::EOS::Signer signer{ vChainIds };
            if (!signer.verify(TW::PublicKey(TW::Data(uchar_vector(pubkey)), TWPublicKeyType::TWPublicKeyTypeSECP256k1),
                               _eosType,
                               tx)) {
                return JUBR_VERIFY_SIGN_FAILED;
            }
        }

        TW::EOS::PackedTransaction packedTx(tx);
        rawInJSON = packedTx.serialize().dump();
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


JUB_RV EOSContext::SignTransaction(const BIP48_Path& path,
                                   JUB_CHAR_CPTR chainID,
                                   JUB_CHAR_CPTR expiration,
                                   JUB_CHAR_CPTR referenceBlockId,
                                   JUB_CHAR_CPTR referenceBlockTime,
                                   JUB_CHAR_CPTR actionsInJSON,
                                   std::string& rawInJSON) {

    CONTEXT_CHECK_TYPE_PRIVATE
    JUB_CHECK_NULL(expiration);
    JUB_CHECK_NULL(referenceBlockId);
    JUB_CHECK_NULL(referenceBlockTime);
    JUB_CHECK_NULL(actionsInJSON);

    std::string strPath = _FullBip48Path(path);
    std::vector<JUB_BYTE> vPath(strPath.begin(), strPath.end());

    uchar_vector vChinIds;
    if (nullptr == chainID
        ||    0 == strlen(chainID)
        ) {
        uchar_vector vChain(chainIds[4]);
        vChinIds << vChain;
    }
    else {
        uchar_vector vChain(chainID);
        vChinIds << vChain;
    }

    bool bWithType = true;
    uchar_vector vRaw;
    JUB_VERIFY_RV(_tokenPtr->SerializePreimage(expiration,
                                               referenceBlockId,
                                               referenceBlockTime,
                                               actionsInJSON,
                                               vRaw,
                                               bWithType));

    std::vector<uchar_vector> vSignatureRaw;
    JUB_VERIFY_RV(_tokenPtr->SignTX(_eosType,
                                    vPath,
                                    vChinIds,
                                    vRaw,
                                    vSignatureRaw,
                                    bWithType));

    // finish transaction
    try {
        TW::EOS::Transaction tx;
        tx.deserialize(vRaw, bWithType);

        for (const uchar_vector& signatureRaw : vSignatureRaw) {
            TW::EOS::Signature signature(signatureRaw, TW::EOS::Type::ModernK1);
            tx.signatures.push_back(signature);
        }

        TW::EOS::PackedTransaction packedTx(tx);
        rawInJSON = packedTx.serialize().dump();
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


JUB_RV EOSContext::BuildAction(const JUB_ACTION_EOS_PTR actions,
    const JUB_UINT16 actionCount,
    std::string& actionsInJSON) {
    CONTEXT_CHECK_TYPE_PUBLIC

    JUB_CHECK_NULL(actions);

    try {
        JUB_VERIFY_RV(_tokenPtr->SerializeActions(actions, actionCount,
                                                  actionsInJSON));
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}


} // namespace context end
} // namespace jub end
