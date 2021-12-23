#pragma once
#include "utility/xFactory.hpp"
#include "utility/Singleton.h"

#include "context/BaseContext.h"
#include "context/KSMContext.h"
#include "token/factory/MISCTokenFactory.h"

#include <TrustWalletCore/TWCoinType.h>


namespace jub {
namespace context {

typedef KSMContext* (*CreateKSMContextFn)(const CONTEXT_CONFIG_DOT& cfg, std::shared_ptr<jub::token::BaseToken>);

class xKSMContextFactory :
protected xFactory<KSMContext*,
                    TWCoinType,
                    CreateKSMContextFn> {
public:
    xKSMContextFactory() {
        Register(_type, &KSMContext::Create);
    }

    KSMContext* CreateContext(const JUB_UINT16 deviceID, const CONTEXT_CONFIG_DOT& cfg) {
        auto token = jub::token::MISCseriesTokenFactory::GetInstance()->CreateToken(deviceID, _type, cfg.curve);
        return Create(_type, cfg, token);
    }

private:
    TWCoinType _type = TWCoinType::TWCoinTypeKusama;
};  // class xKSMContextFactory end

using KSMseriesContextFactory = Singleton<xKSMContextFactory>;
} // namespace context end
} // namespace jub end
