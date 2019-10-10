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

class ContextEOS : public Context {
public:
    ContextEOS(CONTEXT_CONFIG_EOS cfg, JUB_UINT16 deviceID) {
        _mainPath = cfg.mainPath;
        _chainID = cfg.chainID;
        _deviceID = deviceID;
        _timeout = 120 * 2;
    };
    ~ContextEOS() {};

    virtual JUB_RV GetAddress(BIP32_Path path, JUB_UINT16 tag, std::string& address);
    virtual JUB_RV SetMyAddress(BIP32_Path path, std::string& address);
    virtual JUB_RV GetHDNode(JUB_BYTE format, BIP32_Path path, std::string& pubkey);
    virtual JUB_RV GetMainHDNode(JUB_BYTE format, std::string& xpub);

    virtual JUB_RV ActiveSelf() override;
protected:
    std::string _FullBip32Path(BIP32_Path path) override;
private:
    int _chainID;
}; // class ContextEOS end

} // namespace jub end

#endif // #pragma once
