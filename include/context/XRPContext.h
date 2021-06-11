//
//  XRPContext.h
//  JubSDK
//
//  Created by Pan Min on 2019/11/25.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#pragma once
#ifndef __ContextXRP__
#define __ContextXRP__

#include "JUB_SDK_XRP.h"

#include <string>
#include <vector>

#include "utility/Singleton.h"
#include "utility/xManager.hpp"

#include "context/BaseContext.h"
#include "token/interface/BaseToken.h"


namespace jub {
namespace context {


class XRPContext :
    public BaseContext {
public:
    //for Factory
    static XRPContext* Create(const CONTEXT_CONFIG_XRP& cfg,
                              std::shared_ptr<token::BaseToken> tokenPtr) {
        return new XRPContext(cfg, tokenPtr);
    }

    XRPContext(const CONTEXT_CONFIG_XRP cfg, std::shared_ptr<token::BaseToken> tokenPtr) :
        BaseContext(tokenPtr) {
            _mainPath = cfg.mainPath;
            _timeout = 120 * 2;
    }
    ~XRPContext() {}

    virtual JUB_RV GetAddress(const BIP44_Path path, const JUB_UINT16 tag, std::string& address);
    virtual JUB_RV SetMyAddress(const BIP44_Path path, std::string& address);
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const BIP44_Path path, std::string& pubkey);
    virtual JUB_RV CheckAddress(const std::string& address, std::string& addre, std::string& tag);

    virtual JUB_RV GetMainHDNode(const JUB_BYTE format, std::string& xpub);

    virtual JUB_RV SignTransaction(const BIP44_Path path,
                                   const JUB_TX_XRP& tx,
                                   std::string& signedRaw);

    virtual JUB_RV ActiveSelf() override;
}; // class XRPContext end


} // namespace context end
} // namespace jub end

#endif // #pragma once
