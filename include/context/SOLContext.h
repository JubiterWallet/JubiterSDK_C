//
//  SOLContext.h
//  JubSDK
//
//  Created by Pan Min on 2022/07/12.
//  Copyright © 2022 JuBiter. All rights reserved.
//

#pragma once
#include "JUB_SDK_COMM.h"
#ifndef __ContextSOL__
#define __ContextSOL__

#include "JUB_SDK_SOL.h"

#include <string>

#include "utility/Singleton.h"
#include "utility/xManager.hpp"

#include "context/BaseContext.h"
#include "token/interface/BaseToken.h"

//#include "SOL/Prefixes.h"

namespace jub {
namespace context {
class SOLContext : public BaseContext {
  public:
    SOLContext(const CONTEXT_CONFIG_SOL &cfg, std::shared_ptr<token::BaseToken> tokenPtr) : BaseContext(tokenPtr) {
        _mainPath = cfg.mainPath;
        _timeout  = 120 * 2;
    }
    ~SOLContext() {}

    virtual JUB_RV GetAddress(const std::string &path, const JUB_UINT16 tag, std::string &address);
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey);
    virtual JUB_RV SetMyAddress(const BIP48_Path &path, std::string &address);
    virtual JUB_RV CheckAddress(const std::string &address);

    virtual JUB_RV GetMainHDNode(const JUB_BYTE format, std::string &xpub);
    virtual JUB_RV SetToken(JUB_CHAR_CPTR name, JUB_UINT8 decimal, JUB_CHAR_CPTR tokenMint);
    virtual JUB_RV SignTransferTx(JUB_CHAR_CPTR path, JUB_CHAR_CPTR recentHash, JUB_CHAR_CPTR dest, JUB_UINT64 amount,
                                  std::string &txRaw);
    // Trasfer `Token`
    virtual JUB_RV SignTokenTransferTx(JUB_CHAR_CPTR path, JUB_CHAR_CPTR recentHash, JUB_CHAR_CPTR tokenMint,
                                       JUB_CHAR_CPTR source, JUB_CHAR_CPTR dest, JUB_UINT64 amount, JUB_UINT8 decimal,
                                       std::string &txRaw);
    // create token address
    virtual JUB_RV SignCreateTokenAccountTx(JUB_CHAR_CPTR path, JUB_CHAR_CPTR recentHash, JUB_CHAR_CPTR owner,
                                            JUB_CHAR_CPTR tokenMint, std::string &txRaw);
    virtual JUB_RV ActiveSelf() override;

}; // class SOLContext end

} // namespace context
} // namespace jub

#endif // #pragma once
