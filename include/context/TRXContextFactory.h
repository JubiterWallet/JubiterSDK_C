#pragma once
#include "utility/xFactory.hpp"
#include "utility/Singleton.h"

#include "context/BaseContext.h"
#include "context/TRXContext.h"
#include "token/factory/MISCTokenFactory.h"

#include <TrustWalletCore/TWCoinType.h>


namespace jub {
namespace context {

typedef TRXContext* (*CreateTRXContextFn)(const CONTEXT_CONFIG_TRX& cfg, std::shared_ptr<jub::token::BaseToken>);

class xTRXContextFactory :
protected xFactory<TRXContext*,
                    TWCoinType,
                    CreateTRXContextFn> {
public:
    xTRXContextFactory() {
        Register(_type, &TRXContext::Create);
    }

    TRXContext* CreateContext(const JUB_UINT16 deviceID, const CONTEXT_CONFIG_TRX& cfg) {
        auto token = jub::token::MISCseriesTokenFactory::GetInstance()->CreateToken(deviceID, _type);
        return Create(_type, cfg, token);
    }

private:
    TWCoinType _type = TWCoinType::TWCoinTypeTron;
};  // class xTRXContextFactory end

using TRXseriesContextFactory = Singleton<xTRXContextFactory>;
} // namespace context end
} // namespace jub end
