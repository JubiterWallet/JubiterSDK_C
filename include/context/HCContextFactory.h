#pragma once
#include "utility/xFactory.hpp"
#include "utility/Singleton.h"

#include "context/BaseContext.h"
#include "context/HCContext.h"
#include "token/factory/MISCTokenFactory.h"

#include <TrustWalletCore/TWCoinType.h>


namespace jub {
namespace context {

typedef HCContext* (*CreateHCContextFn)(const CONTEXT_CONFIG_HC& cfg, std::shared_ptr<jub::token::BaseToken>);

class xHCContextFactory :
protected xFactory<HCContext*,
                    TWCoinType,
                    CreateHCContextFn> {
public:
    xHCContextFactory() {
        Register(_type, &BaseContext::Create<HCContext, CONTEXT_CONFIG_HC>);
    }

    HCContext* CreateContext(const CONTEXT_CONFIG_HC& cfg, JUB_UINT16 deviceID) {
        auto token = jub::token::MISCseriesTokenFactory::GetInstance()->CreateToken(_type, deviceID);
        return Create(_type, cfg, token);
    }

private:
    TWCoinType _type = TWCoinType::TWCoinTypeHcash;
};  // class xHCContextFactory end

using HCseriesContextFactory = Singleton<xHCContextFactory>;
} // namespace context end
} // namespace jub end
