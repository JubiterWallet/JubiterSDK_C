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
    FILContext(const CONTEXT_CONFIG_FIL& cfg, std::shared_ptr<token::BaseToken> tokenPtr) :
        BaseContext(tokenPtr) {
            _mainPath = cfg.mainPath;
            _timeout = 120 * 2;
    }
    ~FILContext() {}

    virtual JUB_RV GetAddress(const BIP44_Path& path, const JUB_UINT16 tag, std::string& address);
    virtual JUB_RV SetMyAddress(const BIP44_Path& path, std::string& address);
    virtual JUB_RV CheckAddress(const std::string& address);

    virtual JUB_RV GetHDNode(const JUB_BYTE format, const BIP44_Path& path, std::string& pubkey);
    virtual JUB_RV GetMainHDNode(const JUB_BYTE format, std::string& xpub);

    virtual JUB_RV SignTransaction(const BIP44_Path& path,
                                   const JUB_UINT64 nonce,
                                   const JUB_UINT64 gasLimit,
                                   JUB_CHAR_CPTR gasFeeCapInAtto,
                                   JUB_CHAR_CPTR gasPremiumInAtto,
                                   JUB_CHAR_CPTR to,
                                   JUB_CHAR_CPTR valueInAtto,
                                   JUB_CHAR_CPTR input,
                                   OUT std::string& raw);
    virtual JUB_RV CalculateTransactionCID(const BIP44_Path& path,
                                           const JUB_UINT64 nonce,
                                           const JUB_UINT64 gasLimit,
                                           JUB_CHAR_CPTR gasFeeCapInAtto,
                                           JUB_CHAR_CPTR gasPremiumInAtto,
                                           JUB_CHAR_CPTR to,
                                           JUB_CHAR_CPTR valueInAtto,
                                           JUB_CHAR_CPTR input,
                                           OUT std::string& cid);

    virtual JUB_RV ActiveSelf() override;
}; // class FILContext end


} // namespace context end
} // namespace jub end

#endif // #pragma once
