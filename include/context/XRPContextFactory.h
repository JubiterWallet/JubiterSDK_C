#pragma once
#include "utility/xFactory.hpp"
#include "utility/Singleton.h"

#include "context/BaseContext.h"
#include "context/XRPContext.h"
#include "token/factory/MISCTokenFactory.h"

#include <TrustWalletCore/TWCoinType.h>


namespace jub {
namespace context {

typedef XRPContext* (*CreateXRPContextFn)(const CONTEXT_CONFIG_XRP& cfg, std::shared_ptr<jub::token::BaseToken>);

class xXRPContextFactory :
protected xFactory<XRPContext*,
                    TWCoinType,
                    CreateXRPContextFn> {
public:
    xXRPContextFactory() {
        Register(_type, &XRPContext::Create);
    }

    XRPContext* CreateContext(const CONTEXT_CONFIG_XRP& cfg, std::string XPRVorXPUB) {
        auto token = jub::token::MISCseriesTokenFactory::GetInstance()->CreateToken(_type, XPRVorXPUB);
        return Create(_type, cfg, token);
    }

    XRPContext* CreateContext(const CONTEXT_CONFIG_XRP& cfg, JUB_UINT16 deviceID) {
        auto token = jub::token::MISCseriesTokenFactory::GetInstance()->CreateToken(_type, deviceID);
        return Create(_type, cfg, token);
    }

private:
    TWCoinType _type = TWCoinType::TWCoinTypeXRP;
};  // class xXRPContextFactory end

using XRPseriesContextFactory = Singleton<xXRPContextFactory>;
} // namespace context end
} // namespace jub end
