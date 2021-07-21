#pragma once
#include "utility/xFactory.hpp"
#include "utility/Singleton.h"

#include "context/BaseContext.h"
#include "context/DOTContext.h"
#include "token/factory/MISCTokenFactory.h"

#include <TrustWalletCore/TWCoinType.h>


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
                        
    DOTContext* CreateContext(const JUB_UINT16 deviceID, const CONTEXT_CONFIG_DOT& cfg) {
        auto token = jub::token::MISCseriesTokenFactory::GetInstance()->CreateToken(deviceID, _type, cfg.curve);
        return Create(_type, cfg, token);
    }

private:
    TWCoinType _type = TWCoinType::TWCoinTypePolkadot;
};  // class xDOTContextFactory end

using DOTseriesContextFactory = Singleton<xDOTContextFactory>;
} // namespace context end
} // namespace jub end
