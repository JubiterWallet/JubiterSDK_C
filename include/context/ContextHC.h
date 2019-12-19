#pragma once
#ifndef __ContextHC__
#define __ContextHC__

#include <JUB_SDK_Hcash.h>
#include <string>
#include <vector>
#include <utility/Singleton.h>
#include <utility/xManager.hpp>
#include <context/ContextBTC.h>
#include "../libHC/libHC.h"

#define JUB_CHECK_CONTEXT_HC(x)                                     \
do {                                                                \
    auto context = jub::ContextManager::GetInstance()->GetOne(x);   \
    JUB_CHECK_NULL(context);                                        \
    const std::type_info& tCtx = typeid(*context);                  \
    const std::type_info& tCtxBTC = typeid(jub::ContextHC);         \
    if (tCtx.hash_code() != tCtxBTC.hash_code()) {                  \
        return JUBR_ERROR_ARGS;                                     \
    }                                                               \
} while(0)

namespace jub {

class HCContextBase {

public:
    virtual JUB_RV signTX(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const std::vector<INPUT_HC>& vInputs, const std::vector<OUTPUT_HC>& vOutputs, const std::string& unsignedTrans, std::string& raw) = 0;
    JUB_RV DeserializeTxHC(const std::string& raw) {
        return hc::DeserializeTx(raw, _tx);
    };
    JUB_RV SerializeTxHC(std::string& raw) {
        return hc::SerializeTx(_tx,raw);
    };

protected:
    hc::TX_Hcash _tx;
}; // class HCContextBase end

class ContextHC : public ContextBTC,
                  public HCContextBase {

public:
// Remove c++ features for swift framework
//    ContextHC(CONTEXT_CONFIG_HC cfg, JUB_UINT16 deviceID) :
//    ContextBTC({COINBTC, cfg.mainPath, p2pkh}, deviceID) {
//        _timeout = 120 * 2;
//    };
    ContextHC(CONTEXT_CONFIG_HC cfg, JUB_UINT16 deviceID) {
        _coinType = COINBTC;
        _mainPath = cfg.mainPath;
        _transType = p2pkh;
        _deviceID = deviceID;

        _timeout = 120 * 2;
    };
// Remove c++ features for swift framework end
    ~ContextHC() {};

    virtual JUB_RV signTX(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const std::vector<INPUT_HC>& vInputs, const std::vector<OUTPUT_HC>& vOutputs, const std::string& unsignedTrans, std::string& raw) override;
    virtual ContextHC* GetClassType(void) {
        return this;
    }
    virtual JUB_RV ActiveSelf() override;
}; // class ContextBTC end

} // namespace jub end

#endif // #pragma once
