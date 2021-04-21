#pragma once
#include "utility/xFactory.hpp"
#include "utility/Singleton.h"

#include "context/BaseContext.h"
#include "context/FILContext.h"
#include "token/factory/ETHTokenFactory.h"

#include <TrustWalletCore/TWCoinType.h>


namespace jub {
namespace context {

typedef FILContext* (*CreateFILContextFn)(const CONTEXT_CONFIG_FIL& cfg, std::shared_ptr<jub::token::BaseToken>);

class xFILContextFactory :
protected xFactory<FILContext*,
                    TWCoinType,
                    CreateFILContextFn> {
public:
    xFILContextFactory() {
        Register(_type, &FILContext::Create);
    }

    FILContext* CreateContext(const CONTEXT_CONFIG_FIL& cfg, JUB_UINT16 deviceID) {
        auto token = jub::token::ETHseriesTokenFactory::GetInstance()->CreateToken(_type, deviceID);
        return Create(_type, cfg, token);
    }

private:
    TWCoinType _type = TWCoinType::TWCoinTypeFilecoin;
};  // class xFILContextFactory end

using FILseriesContextFactory = Singleton<xFILContextFactory>;
} // namespace context end
} // namespace jub end
