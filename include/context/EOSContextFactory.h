#pragma once
#include <utility/xFactory.hpp>
#include <utility/Singleton.h>

#include <context/BaseContext.h>
#include <context/EOSContext.h>
#include <token/factory/MISCTokenFactory.h>

#include <TrustWalletCore/TWCoinType.h>


namespace jub {
namespace context {

typedef EOSContext* (*CreateEOSContextFn)(const CONTEXT_CONFIG_EOS& cfg, std::shared_ptr<jub::token::BaseToken>);

class xEOSContextFactory :
protected xFactory<EOSContext*,
                    TWCoinType,
                    CreateEOSContextFn> {
public:
    xEOSContextFactory() {
        Register(_type, &EOSContext::Create);
    }

    EOSContext* CreateContext(const CONTEXT_CONFIG_EOS& cfg, std::string XPRVorXPUB) {
        auto token = jub::token::MISCseriesTokenFactory::GetInstance()->CreateToken(_type, XPRVorXPUB);
        return Create(_type, cfg, token);
    }

    EOSContext* CreateContext(const CONTEXT_CONFIG_EOS& cfg, JUB_UINT16 deviceID) {
        auto token = jub::token::MISCseriesTokenFactory::GetInstance()->CreateToken(_type, deviceID);
        return Create(_type, cfg, token);
    }

private:
    TWCoinType _type = TWCoinType::TWCoinTypeEOS;
};  // class xEOSContextFactory end

using EOSseriesContextFactory = Singleton<xEOSContextFactory>;
} // namespace context end
} // namespace jub end
