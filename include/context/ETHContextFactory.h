#pragma once
#include <utility/xFactory.hpp>
#include <utility/Singleton.h>

#include <context/BaseContext.h>
#include <context/ETHContext.h>
#include <token/factory/MISCTokenFactory.h>

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

    ETHContext* CreateContext(const CONTEXT_CONFIG_ETH& cfg, std::string XPRVorXPUB) {
        auto token = jub::token::MISCseriesTokenFactory::GetInstance()->CreateToken(_type, XPRVorXPUB);
        return Create(_type, cfg, token);
    }

    ETHContext* CreateContext(const CONTEXT_CONFIG_ETH& cfg, JUB_UINT16 deviceID) {
        auto token = jub::token::MISCseriesTokenFactory::GetInstance()->CreateToken(_type, deviceID);
        return Create(_type, cfg, token);
    }

private:
    TWCoinType _type = TWCoinType::TWCoinTypeEthereum;
};  // class xETHContextFactory end

using ETHseriesContextFactory = Singleton<xETHContextFactory>;
} // namespace context end
} // namespace jub end
