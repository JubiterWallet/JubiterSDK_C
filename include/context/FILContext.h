//
//  FILContext.h
//  JubSDK
//
//  Created by Pan Min on 2020/11/12.
//  Copyright © 2020 JuBiter. All rights reserved.
//

#pragma once
#ifndef __ContextFIL__
#define __ContextFIL__

#include "JUB_SDK_FIL.h"

#include <string>
#include <vector>

#include "utility/Singleton.h"
#include "utility/xManager.hpp"

#include "context/BaseContext.h"
#include "token/interface/BaseToken.h"


namespace jub {
namespace context {


class FILContext :
    public BaseContext {
public:
    //for Factory
    static FILContext* Create(const CONTEXT_CONFIG_FIL& cfg,
                              std::shared_ptr<token::BaseToken> tokenPtr) {
        return new FILContext(cfg, tokenPtr);
    }

    FILContext(CONTEXT_CONFIG_FIL cfg, std::shared_ptr<token::BaseToken> tokenPtr) :
        BaseContext(tokenPtr) {
            _mainPath = cfg.mainPath;
            _timeout = 120 * 2;
    }
    ~FILContext() {}

    virtual JUB_RV GetAddress(BIP44_Path path, JUB_UINT16 tag, std::string& address);
    virtual JUB_RV SetMyAddress(BIP44_Path path, std::string& address);
    virtual JUB_RV GetHDNode(JUB_BYTE format, BIP44_Path path, std::string& pubkey);
    virtual JUB_RV GetMainHDNode(JUB_BYTE format, std::string& xpub);

    virtual JUB_RV SignTransaction(BIP44_Path path,
                                   const JUB_UINT64 nonce,
                                   const JUB_UINT64 gasLimit,
                                   JUB_CHAR_CPTR gasPriceInAtto,
                                   JUB_CHAR_CPTR to,
                                   JUB_CHAR_CPTR valueInAtto,
                                   JUB_CHAR_CPTR input,
                                   OUT std::string& raw);

    virtual JUB_RV ActiveSelf() override;
}; // class FILContext end


} // namespace context end
} // namespace jub end

#endif // #pragma once
