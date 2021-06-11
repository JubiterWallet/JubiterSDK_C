//
//  EOSContext.h
//  JubSDK
//
//  Created by Pan Min on 2019/10/09.
//  Copyright © 2019 JuBiter. All rights reserved.
//

#pragma once
#ifndef __ContextEOS__
#define __ContextEOS__

#include "JUB_SDK_EOS.h"

#include <string>
#include <vector>

#include "utility/Singleton.h"
#include "utility/xManager.hpp"

#include "context/BaseContext.h"
#include "token/interface/BaseToken.h"
#include "EOS/Prefixes.h"


namespace jub {
namespace context {


const std::string chainIds[] = {
    "Steem",
    "BitShares",
    "-",
    "-",
    "aca376f206b8fc25a6ed44dbdc66547c36c6c33e3a119ffbeaef943642f0e906", //EOS
    "FIBOS"
};

class EOSContext :
    public BaseContext {
public:
    //for Factory
    static EOSContext* Create(const CONTEXT_CONFIG_EOS& cfg,
                              std::shared_ptr<token::BaseToken> tokenPtr) {
        return new EOSContext(cfg, tokenPtr);
    }

    EOSContext(const CONTEXT_CONFIG_EOS cfg, std::shared_ptr<token::BaseToken> tokenPtr) :
        BaseContext(tokenPtr) {
            _mainPath = cfg.mainPath;
            _timeout = 120 * 2;
    }
    ~EOSContext() {}

    virtual JUB_RV GetAddress(const BIP44_Path& path, const JUB_UINT16 tag, std::string& address);
    virtual JUB_RV SetMyAddress(const BIP44_Path& path, std::string& address);
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const BIP44_Path& path, std::string& pubkey);
    virtual JUB_RV GetAddress(const BIP48_Path& path, const JUB_UINT16 tag, std::string& address);
    virtual JUB_RV SetMyAddress(const BIP48_Path& path, std::string& address);
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const BIP48_Path& path, std::string& pubkey);
    virtual JUB_RV GetMainHDNode(const JUB_BYTE format, std::string& xpub);

    virtual JUB_RV SignTransaction(const BIP44_Path& path,
                                   JUB_CHAR_CPTR chainID,
                                   JUB_CHAR_CPTR expiration,
                                   JUB_CHAR_CPTR referenceBlockId,
                                   JUB_CHAR_CPTR referenceBlockTime,
                                   JUB_CHAR_CPTR actionsInJSON,
                                   std::string& rawInJSON);

    virtual JUB_RV SignTransaction(const BIP48_Path& path,
                                   JUB_CHAR_CPTR chainID,
                                   JUB_CHAR_CPTR expiration,
                                   JUB_CHAR_CPTR referenceBlockId,
                                   JUB_CHAR_CPTR referenceBlockTime,
                                   JUB_CHAR_CPTR actionsInJSON,
                                   std::string& rawInJSON);

    virtual JUB_RV BuildAction(const JUB_ACTION_EOS_PTR actions,
                               const JUB_UINT16 actionCount,
                               std::string& actionsInJSON);

    virtual JUB_RV ActiveSelf() override;

private:
    TW::EOS::Type _eosType{ TW::EOS::Type::Legacy };
}; // class EOSContext end


} // namespace context end
} // namespace jub end

#endif // #pragma once
