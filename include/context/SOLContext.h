//
//  SOLContext.h
//  JubSDK
//
//  Created by Pan Min on 2022/07/12.
//  Copyright © 2022 JuBiter. All rights reserved.
//

#pragma once
#ifndef __ContextSOL__
#define __ContextSOL__

#include "JUB_SDK_SOL.h"

#include <string>
#include <vector>

#include "utility/Singleton.h"
#include "utility/xManager.hpp"

#include "context/BaseContext.h"
#include "token/interface/BaseToken.h"
//#include "SOL/Prefixes.h"


namespace jub {
namespace context {


//const std::string chainIds[] = {
//    "Steem",
//    "BitShares",
//    "-",
//    "-",
//    "aca376f206b8fc25a6ed44dbdc66547c36c6c33e3a119ffbeaef943642f0e906", //EOS
//    "FIBOS"
//};

class SOLContext :
    public BaseContext {
public:
    SOLContext(const CONTEXT_CONFIG_SOL& cfg, std::shared_ptr<token::BaseToken> tokenPtr) :
        BaseContext(tokenPtr) {
            _mainPath = cfg.mainPath;
            _timeout = 120 * 2;
    }
    ~SOLContext() {}

    virtual JUB_RV GetAddress(const BIP44_Path& path, const JUB_UINT16 tag, std::string& address);
    virtual JUB_RV SetMyAddress(const BIP44_Path& path, std::string& address);

    virtual JUB_RV GetHDNode(const JUB_BYTE format, const BIP44_Path& path, std::string& pubkey);
    virtual JUB_RV GetAddress(const BIP48_Path& path, const JUB_UINT16 tag, std::string& address);
    virtual JUB_RV SetMyAddress(const BIP48_Path& path, std::string& address);

    virtual JUB_RV CheckAddress(const std::string& address);

    virtual JUB_RV GetHDNode(const JUB_BYTE format, const BIP48_Path& path, std::string& pubkey);
    virtual JUB_RV GetMainHDNode(const JUB_BYTE format, std::string& xpub);

//    virtual JUB_RV SignTransaction(const BIP44_Path& path,
//                                   JUB_CHAR_CPTR chainID,
//                                   JUB_CHAR_CPTR expiration,
//                                   JUB_CHAR_CPTR referenceBlockId,
//                                   JUB_CHAR_CPTR referenceBlockTime,
//                                   JUB_CHAR_CPTR actionsInJSON,
//                                   std::string& rawInJSON);
//
//    virtual JUB_RV SignTransaction(const BIP48_Path& path,
//                                   JUB_CHAR_CPTR chainID,
//                                   JUB_CHAR_CPTR expiration,
//                                   JUB_CHAR_CPTR referenceBlockId,
//                                   JUB_CHAR_CPTR referenceBlockTime,
//                                   JUB_CHAR_CPTR actionsInJSON,
//                                   std::string& rawInJSON);
//
//    virtual JUB_RV BuildAction(const JUB_ACTION_EOS_PTR actions,
//                               const JUB_UINT16 actionCount,
//                               std::string& actionsInJSON);

    virtual JUB_RV ActiveSelf() override;

//private:
//    TW::EOS::Type _eosType{ TW::EOS::Type::Legacy };
}; // class SOLContext end


} // namespace context end
} // namespace jub end

#endif // #pragma once
