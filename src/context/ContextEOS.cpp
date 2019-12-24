//
//  ContextEOS.cpp
//  JubSDK
//
//  Created by Pan Min on 2019/10/09.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#include "utility/util.h"

#include "context/ContextEOS.h"
#include "token/interface/TokenInterface.hpp"
#include "libEOS/libEOS.hpp"
#include "EOS/Transaction.h"
#include "EOS/PackedTransaction.h"

namespace jub {

JUB_RV ContextEOS::ActiveSelf() {

    auto token = dynamic_cast<EOSTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    auto ctoken = dynamic_cast<CommonTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);
    JUB_CHECK_NULL(ctoken);

    JUB_RV rv = token->SelectAppletEOS();
    if (                 JUBR_OK != rv
        && JUBR_EOS_APP_INDEP_OK != rv
        ) {
        return rv;
    }
    bool isIndep = false;
    if (JUBR_EOS_APP_INDEP_OK == rv) {
        isIndep = true;
    }
    JUB_VERIFY_RV(ctoken->SetTimeout(_timeout));
    if (!isIndep) {
        JUB_VERIFY_RV(token->SetCoinTypeEOS());
    }

    return JUBR_OK;
}

JUB_RV ContextEOS::GetMainHDNode(const JUB_BYTE format, std::string& xpub) {

    auto token = dynamic_cast<EOSTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->GetHDNodeEOS(format, _mainPath, xpub));

    return JUBR_OK;
}

JUB_RV ContextEOS::GetAddress(const BIP44_Path& path, const JUB_UINT16 tag, std::string& address) {

    auto token = dynamic_cast<EOSTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddressEOS(_eosType, strPath, tag, address));

    return JUBR_OK;
}

JUB_RV ContextEOS::SetMyAddress(const BIP44_Path& path, std::string& address) {

    auto token = dynamic_cast<EOSTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddressEOS(_eosType, strPath, 0x02, address));

    return JUBR_OK;
}

JUB_RV ContextEOS::GetHDNode(const JUB_BYTE format, const BIP44_Path& path, std::string& pubkey) {

    auto token = dynamic_cast<EOSTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetHDNodeEOS(format, strPath, pubkey));

    return JUBR_OK;
}

JUB_RV ContextEOS::GetAddress(const BIP48_Path& path, const JUB_UINT16 tag, std::string& address) {

    auto token = dynamic_cast<EOSTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip48Path(path);
    JUB_VERIFY_RV(token->GetAddressEOS(_eosType, strPath, tag, address));

    return JUBR_OK;
}

JUB_RV ContextEOS::SetMyAddress(const BIP48_Path& path, std::string& address) {

    auto token = dynamic_cast<EOSTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip48Path(path);
    JUB_VERIFY_RV(token->GetAddressEOS(_eosType, strPath, 0x02, address));

    return JUBR_OK;
}

JUB_RV ContextEOS::GetHDNode(const JUB_BYTE format, const BIP48_Path& path, std::string& pubkey) {

    auto token = dynamic_cast<EOSTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip48Path(path);
    JUB_VERIFY_RV(token->GetHDNodeEOS(format, strPath, pubkey));

    return JUBR_OK;
}

JUB_RV ContextEOS::SignTransaction(const BIP44_Path& path,
                                   JUB_CHAR_CPTR chainID,
                                   JUB_CHAR_CPTR expiration,
                                   JUB_CHAR_CPTR referenceBlockId,
                                   JUB_CHAR_CPTR referenceBlockTime,
                                   JUB_CHAR_CPTR actionsInJSON,
                                   std::string& rawInJSON) {

    JUB_CHECK_NULL(expiration);
    JUB_CHECK_NULL(referenceBlockId);
    JUB_CHECK_NULL(referenceBlockTime);
    JUB_CHECK_NULL(actionsInJSON);

    auto token = dynamic_cast<EOSTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip44Path(path);
    std::vector<JUB_BYTE> vPath(strPath.begin(), strPath.end());

    uchar_vector vChinIds;
    if (nullptr == chainID
        || 0 == strlen(chainID)
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
    JUB_VERIFY_RV(jub::eos::serializePreimage(expiration,
                                              referenceBlockId,
                                              referenceBlockTime,
                                              actionsInJSON,
                                              vRaw,
                                              bWithType));

    std::vector<uchar_vector> vSignatureRaw;
    JUB_VERIFY_RV(token->SignTXEOS(_eosType,
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

JUB_RV ContextEOS::SignTransaction(const BIP48_Path& path,
                                   JUB_CHAR_CPTR chainID,
                                   JUB_CHAR_CPTR expiration,
                                   JUB_CHAR_CPTR referenceBlockId,
                                   JUB_CHAR_CPTR referenceBlockTime,
                                   JUB_CHAR_CPTR actionsInJSON,
                                   std::string& rawInJSON) {

    JUB_CHECK_NULL(expiration);
    JUB_CHECK_NULL(referenceBlockId);
    JUB_CHECK_NULL(referenceBlockTime);
    JUB_CHECK_NULL(actionsInJSON);

    auto token = dynamic_cast<EOSTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip48Path(path);
    std::vector<JUB_BYTE> vPath(strPath.begin(), strPath.end());

    uchar_vector vChinIds;
    if (nullptr == chainID
        || 0 == strlen(chainID)
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
    JUB_VERIFY_RV(jub::eos::serializePreimage(expiration,
                                              referenceBlockId,
                                              referenceBlockTime,
                                              actionsInJSON,
                                              vRaw,
                                              bWithType));

    std::vector<uchar_vector> vSignatureRaw;
    JUB_VERIFY_RV(token->SignTXEOS(_eosType,
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

JUB_RV ContextEOS::BuildAction(const JUB_ACTION_EOS_PTR actions,
                               const JUB_UINT16 actionCount,
                               std::string& actionsInJSON) {

    JUB_CHECK_NULL(actions);

    try {
        JUB_VERIFY_RV(jub::eos::serializeActions(actions, actionCount,
                                                 actionsInJSON));
    }
    catch (...) {
        return JUBR_ARGUMENTS_BAD;
    }

    return JUBR_OK;
}

} // namespace jub end
