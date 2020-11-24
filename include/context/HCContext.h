#pragma once
#ifndef __ContextHC__
#define __ContextHC__

#include <string>
#include <vector>

#include "utility/Singleton.h"
#include "utility/xManager.hpp"

#include "context/BTCContext.h"
#include "token/interface/BaseToken.h"


namespace jub {
namespace context {


class HCContextBase {
public:
    virtual JUB_RV signTX(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const JUB_UINT32 version, const std::vector<INPUT_HC>& vInputs, const std::vector<OUTPUT_HC>& vOutputs, const std::string& unsignedTrans, std::string& raw) = 0;
}; // class HCContextBase end


class HCContext :
    public BTCContext,
    public HCContextBase {

public:
    //for Factory
    static HCContext* Create(const CONTEXT_CONFIG_HC& cfg,
                             std::shared_ptr<token::BaseToken> tokenPtr) {
        return new HCContext(cfg, tokenPtr);
    }

    HCContext(CONTEXT_CONFIG_HC cfg,
              std::shared_ptr<token::BaseToken> tokenPtr):
    BTCContext(CONTEXT_CONFIG_BTC{cfg.mainPath, COINBTC, p2pkh}, tokenPtr) {
        _timeout = 120 * 2;
    }
    // Remove c++ features for swift framework end
    ~HCContext() {}

    virtual JUB_RV signTX(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const JUB_UINT32 version, const std::vector<INPUT_HC>& vInputs, const std::vector<OUTPUT_HC>& vOutputs, const std::string& unsignedTrans, std::string& raw) override;
    virtual HCContext* GetClassType(void) {
        return this;
    }
    virtual JUB_RV ActiveSelf() override;
}; // class HCContext end


} // namespace context end
} // namespace jub end

#endif // #pragma once
