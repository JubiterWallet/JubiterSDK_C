#pragma once
#include "utility/xFactory.hpp"
#include "utility/Singleton.h"

#include "context/BaseContext.h"
#include "context/DOTContext.h"
#include "token/factory/ETHTokenFactory.h"

#include <TrustWalletCore/TWCoinType.h>
extern "C" {
#include <sr25519/sr25519.h>
}


namespace jub {
namespace context {

typedef DOTContext* (*CreateDOTContextFn)(const CONTEXT_CONFIG_DOT& cfg, std::shared_ptr<jub::token::BaseToken>);

class xDOTContextFactory :
protected xFactory<DOTContext*,
                    TWCoinType,
                    CreateDOTContextFn> {
public:
    xDOTContextFactory() {
        Register(_type, &DOTContext::Create);
    }

    DOTContext* CreateContext(const CONTEXT_CONFIG_DOT& cfg, std::string XPRVorXPUB) {
        auto token = jub::token::ETHseriesTokenFactory::GetInstance()->CreateToken(_type, XPRVorXPUB);
        return Create(_type, cfg, token);
    }

    DOTContext* CreateContext(const CONTEXT_CONFIG_DOT& cfg, JUB_UINT16 deviceID) {
        auto token = jub::token::ETHseriesTokenFactory::GetInstance()->CreateToken(_type, deviceID);
        return Create(_type, cfg, token);
    }

private:
    TWCoinType _type = TWCoinType::TWCoinTypePolkadot;
};  // class xDOTContextFactory end

using DOTseriesContextFactory = Singleton<xDOTContextFactory>;
} // namespace context end
} // namespace jub end
