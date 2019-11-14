//
//  ContextEOS.h
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

#include "context/Context.h"
#include "EOS/Prefixes.h"

#define JUB_CHECK_CONTEXT_EOS(x)                                    \
do {                                                                \
    auto context = jub::ContextManager::GetInstance()->GetOne(x);   \
    JUB_CHECK_NULL(context);                                        \
    const std::type_info& tCtx = typeid(*context);                  \
    const std::type_info& tCtxEOS = typeid(jub::ContextEOS);        \
    if (tCtx.hash_code() != tCtxEOS.hash_code()) {                  \
        return JUBR_ARGUMENTS_BAD;                                  \
    }                                                               \
} while(0)

namespace jub {

const std::string chainIds[] = {
    "Steem",
    "BitShares",
    "-",
    "-",
    "aca376f206b8fc25a6ed44dbdc66547c36c6c33e3a119ffbeaef943642f0e906", //EOS
    "FIBOS"
};

class ContextEOS : public Context {
public:
    ContextEOS(CONTEXT_CONFIG_EOS cfg, JUB_UINT16 deviceID) {
        _mainPath = cfg.mainPath;
        _deviceID = deviceID;
        _timeout = 120 * 2;
    };
    ~ContextEOS() {};

    virtual JUB_RV GetAddress(BIP44_Path path, JUB_UINT16 tag, std::string& address);
    virtual JUB_RV SetMyAddress(BIP44_Path path, std::string& address);
    virtual JUB_RV GetHDNode(JUB_BYTE format, BIP44_Path path, std::string& pubkey);
    virtual JUB_RV GetAddress(BIP48_Path path, JUB_UINT16 tag, std::string& address);
    virtual JUB_RV SetMyAddress(BIP48_Path path, std::string& address);
    virtual JUB_RV GetHDNode(JUB_BYTE format, BIP48_Path path, std::string& pubkey);
    virtual JUB_RV GetMainHDNode(JUB_BYTE format, std::string& xpub);

    virtual JUB_RV SignTransaction(BIP44_Path path,
                                   const JUB_CHAR_PTR chainID,
                                   const JUB_CHAR_PTR expiration,
                                   const JUB_CHAR_PTR referenceBlockId,
                                   const JUB_CHAR_PTR referenceBlockTime,
                                   const JUB_CHAR_PTR actionsInJSON,
                                   std::string& rawInJSON);

    virtual JUB_RV SignTransaction(BIP48_Path path,
                                   const JUB_CHAR_PTR chainID,
                                   const JUB_CHAR_PTR expiration,
                                   const JUB_CHAR_PTR referenceBlockId,
                                   const JUB_CHAR_PTR referenceBlockTime,
                                   const JUB_CHAR_PTR actionsInJSON,
                                   std::string& rawInJSON);

    virtual JUB_RV BuildAction(const JUB_ACTION_EOS_PTR actions,
                               const JUB_UINT16 actionCount,
                               std::string& actionsInJSON);

    virtual JUB_RV ActiveSelf() override;

private:
    TW::EOS::Type _eosType{ TW::EOS::Type::Legacy };
}; // class ContextEOS end

} // namespace jub end

#endif // #pragma once
