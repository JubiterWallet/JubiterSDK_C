#pragma once
#include "utility/xFactory.hpp"
#include "utility/Singleton.h"

#include "context/BaseContext.h"
#include "context/ETHContext.h"
#include "token/factory/ETHTokenFactory.h"

#include <TrustWalletCore/TWCoinType.h>


namespace jub {
namespace context {

typedef ETHContext* (*CreateETHContextFn)(const CONTEXT_CONFIG_ETH& cfg, std::shared_ptr<jub::token::BaseToken>);

class xETHContextFactory :
protected xFactory<ETHContext*,
                    TWCoinType,
                    CreateETHContextFn> {
public:
    xETHContextFactory() {
        Register(TWCoinType::TWCoinTypeEthereum,        &ETHContext::Create);
//        Register(TWCoinType::TWCoinTypeEthereumClassic, &ETHContext::Create);
    }

    ETHContext* CreateContext(const JUB_UINT16 deviceID, const CONTEXT_CONFIG_ETH& cfg) {
        auto token = jub::token::ETHseriesTokenFactory::GetInstance()->CreateToken(deviceID, _type);
        return Create(_type, cfg, token);
    }

private:
    TWCoinType _type = TWCoinType::TWCoinTypeEthereum;
};  // class xETHContextFactory end

using ETHseriesContextFactory = Singleton<xETHContextFactory>;
} // namespace context end
} // namespace jub end
