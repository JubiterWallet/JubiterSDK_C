#pragma once
#include "utility/xFactory.hpp"
#include "utility/Singleton.h"

#include "context/BaseContext.h"
#include "context/BTCContext.h"
#include "context/CKBContext.h"
#include "token/factory/BTCTokenFactory.h"


namespace jub {
namespace context {

typedef CKBContext* (*CreateCKBContextFn)(const CONTEXT_CONFIG_CKB& cfg, std::shared_ptr<jub::token::BaseToken>);

class xCKBContextFactory :
protected xFactory<CKBContext*,
                    JUB_ENUM_COINTYPE_BTC,
                    CreateCKBContextFn> {
public:
    xCKBContextFactory() {
        Register(JUB_ENUM_COINTYPE_BTC::COINCKB,   &CKBContext::Create);
    }

    CKBContext* CreateContext(const CONTEXT_CONFIG_CKB& cfg, JUB_UINT16 deviceID) {
        auto token = jub::token::BTCseriesTokenFactory::GetInstance()->CreateToken(deviceID, JUB_ENUM_COINTYPE_BTC::COINCKB);
        return Create(JUB_ENUM_COINTYPE_BTC::COINCKB, cfg, token);
    }
};  // class xCKBContextFactory end

using CKBseriesContextFactory = Singleton<xCKBContextFactory>;


} // namespace context end
} // namespace jub end
