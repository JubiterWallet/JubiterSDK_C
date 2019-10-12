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

namespace jub {

JUB_RV ContextEOS::ActiveSelf() {

    auto token = dynamic_cast<EOSTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    auto ctoken = dynamic_cast<CommonTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);
    JUB_CHECK_NULL(ctoken);

    JUB_VERIFY_RV(token->SelectAppletEOS());
    JUB_VERIFY_RV(ctoken->SetTimeout(_timeout));

    return JUBR_IMPL_NOT_SUPPORT;
}

JUB_RV ContextEOS::GetMainHDNode(JUB_BYTE format, std::string& xpub) {

    auto token = dynamic_cast<EOSTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    JUB_VERIFY_RV(token->GetHDNodeEOS(format, _mainPath, xpub));

    return JUBR_OK;
}

JUB_RV ContextEOS::GetAddress(BIP44_Path path, JUB_UINT16 tag, std::string& address) {

    auto token = dynamic_cast<EOSTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddressEOS(strPath, tag, address));

    return JUBR_OK;
}

JUB_RV ContextEOS::SetMyAddress(BIP44_Path path, std::string& address) {

    auto token = dynamic_cast<EOSTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetAddressEOS(strPath, 0x02, address));

    return JUBR_OK;
}

JUB_RV ContextEOS::GetHDNode(JUB_BYTE format, BIP44_Path path, std::string& pubkey) {

    auto token = dynamic_cast<EOSTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip44Path(path);
    JUB_VERIFY_RV(token->GetHDNodeEOS(format, strPath, pubkey));

    return JUBR_OK;
}

JUB_RV ContextEOS::GetAddress(BIP48_Path path, JUB_UINT16 tag, std::string& address) {

    auto token = dynamic_cast<EOSTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip48Path(path);
    JUB_VERIFY_RV(token->GetAddressEOS(strPath, tag, address));

    return JUBR_OK;
}

JUB_RV ContextEOS::SetMyAddress(BIP48_Path path, std::string& address) {

    auto token = dynamic_cast<EOSTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip48Path(path);
    JUB_VERIFY_RV(token->GetAddressEOS(strPath, 0x02, address));

    return JUBR_OK;
}

JUB_RV ContextEOS::GetHDNode(JUB_BYTE format, BIP48_Path path, std::string& pubkey) {

    auto token = dynamic_cast<EOSTokenInterface*>(jub::TokenManager::GetInstance()->GetOne(_deviceID));
    JUB_CHECK_NULL(token);

    std::string strPath = _FullBip48Path(path);
    JUB_VERIFY_RV(token->GetHDNodeEOS(format, strPath, pubkey));

    return JUBR_OK;
}

} // namespace jub end
