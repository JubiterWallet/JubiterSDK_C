#pragma once
#include <utility/xFactory.hpp>
#include <utility/Singleton.h>

#include <context/BaseContext.h>
#include <token/interface/BTCTokenInterface.hpp>
#include <context/BTCContext.h>
#include <JUB_SDK_BTC.h>

#include <token/TokenFactory.h>


namespace jub {
namespace context {

typedef  BTCContext* (*CreateBTCContextFn)(const CONTEXT_CONFIG_BTC& cfg, std::shared_ptr<jub::token::BTCTokenInterface>);

class xBTCContextFactory :
protected xFactory<BTCContext*,JUB_ENUM_COINTYPE_BTC, CreateBTCContextFn> {
public:
    xBTCContextFactory() {
        Register(JUB_ENUM_COINTYPE_BTC::COINBTC, &BTCContext::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINBCH, &BCHContext::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINLTC, &LTCContext::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINUSDT, &USDTContext::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINDASH, &DASHContext::Create);
        Register(JUB_ENUM_COINTYPE_BTC::COINQTUM, &QTUMContext::Create);
    };

    BTCContext* CreateContext(const CONTEXT_CONFIG_BTC& cfg, std::string XPRVorXPUB) {
        auto token = jub::token::BTCseriesTokenFactory::GetInstance()->CreateToken(cfg.coinType, XPRVorXPUB);
        return Create(cfg.coinType,cfg,token);
    }

    BTCContext* CreateContext(const CONTEXT_CONFIG_BTC& cfg, JUB_UINT16 devieID) {
        auto token = jub::token::BTCseriesTokenFactory::GetInstance()->CreateToken(cfg.coinType, devieID);
        return Create(cfg.coinType,cfg,token);
    }
};

using BTCseriesContextFactory = Singleton<xBTCContextFactory>;
} // namespace context end
} // namespace jub end
