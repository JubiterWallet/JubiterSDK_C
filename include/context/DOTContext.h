//
//  DOTContext.h
//  JubSDK
//
//  Created by Pan Min on 2020/11/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#pragma once
#ifndef __ContextDOT__
#define __ContextDOT__

#include "JUB_SDK_DOT.h"

#include <string>
#include <vector>

#include "utility/Singleton.h"
#include "utility/xManager.hpp"

#include "context/BaseContext.h"
#include "token/interface/BaseToken.h"


namespace jub {
namespace context {


class DOTContext :
    public BaseContext {
public:
    DOTContext(CONTEXT_CONFIG_DOT cfg, std::shared_ptr<token::BaseToken> tokenPtr) :
        BaseContext(tokenPtr) {
            _mainPath = cfg.mainPath;
            _coinType = cfg.coinType;
            _curve = cfg.curve;
            _timeout = 120 * 2;
    }
    ~DOTContext() {}

    virtual JUB_RV GetAddress(std::string path, JUB_UINT16 tag, std::string& address);
    virtual JUB_RV SetMyAddress(std::string path, std::string& address);
    virtual JUB_RV GetHDNode(JUB_BYTE format, std::string path, std::string& pubkey);
    virtual JUB_RV GetMainHDNode(JUB_BYTE format, std::string& xpub);

    virtual JUB_RV SignTransaction(std::string path,
                                   const JUB_TX_DOT& tx,
                                   std::string& signedRaw);

    virtual JUB_RV ActiveSelf() override;
        
protected:
        JUB_ENUM_COINTYPE_DOT _coinType;
        JUB_ENUM_CURVES _curve;

}; // class DOTContext end


} // namespace context end
} // namespace jub end

#endif // #pragma once
