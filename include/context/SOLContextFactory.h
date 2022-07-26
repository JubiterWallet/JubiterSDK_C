#pragma once
#include "utility/xFactory.hpp"
#include "utility/Singleton.h"

#include "context/BaseContext.h"
#include "context/SOLContext.h"
#include "token/factory/MISCTokenFactory.h"

#include <TrustWalletCore/TWCoinType.h>


namespace jub {
namespace context {

typedef SOLContext* (*CreateSOLContextFn)(const CONTEXT_CONFIG_SOL& cfg, std::shared_ptr<jub::token::BaseToken>);

class xSOLContextFactory :
protected xFactory<SOLContext*,
                    TWCoinType,
                    CreateSOLContextFn> {
public:
    xSOLContextFactory() {
        Register(_type, &SOLContext::Create);
    }

    SOLContext* CreateContext(const JUB_UINT16 deviceID, const CONTEXT_CONFIG_SOL& cfg) {
        auto token = jub::token::MISCseriesTokenFactory::GetInstance()->CreateToken(deviceID, _type);
        return Create(_type, cfg, token);
    }

private:
    TWCoinType _type = TWCoinType::TWCoinTypeSolana;
};  // class xSOLContextFactory end

using SOLseriesContextFactory = Singleton<xSOLContextFactory>;
} // namespace context end
} // namespace jub end
