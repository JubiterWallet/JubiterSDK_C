#pragma once
#ifndef __ContextBTC__
#define __ContextBTC__

#include "JUB_SDK_BTC.h"

#include <string>
#include <vector>

#include "utility/Singleton.h"
#include "utility/xManager.hpp"

#include "context/Context.h"

#define JUB_CHECK_CONTEXT_BTC(x)                                    \
do {                                                                \
    auto context = jub::ContextManager::GetInstance()->GetOne(x);   \
    JUB_CHECK_NULL(context);                                        \
    const std::type_info& tCtx = typeid(*context);                  \
    const std::type_info& tCtxBTC = typeid(jub::ContextBTC);        \
    if (tCtx.hash_code() != tCtxBTC.hash_code()) {                  \
        return JUBR_ARGUMENTS_BAD;                                  \
    }                                                               \
} while(0)

namespace jub {

class ContextBTC : public Context{

public:
    ContextBTC(CONTEXT_CONFIG_BTC cfg, JUB_UINT16 deviceID) {
        _mainPath = cfg.mainPath;
        _transType = cfg.transType;
        _deviceID = deviceID;
        _unitType = mBTC;
        _coinType = cfg.coinType;
        _timeout = 120 * 2;
    };
    ~ContextBTC() {};

    virtual JUB_RV GetHDNode(BIP44_Path path, std::string& xpub);
    virtual JUB_RV GetMainHDNode(std::string& xpub);

    virtual JUB_RV GetAddress(BIP44_Path path, JUB_UINT16 tag, std::string& address);
    virtual JUB_RV SetMyAddress(BIP44_Path path, std::string& address);
    virtual JUB_RV SignTX(std::vector<INPUT_BTC> vInputs, std::vector<OUTPUT_BTC> vOutputs, JUB_UINT32 lockTime,  std::string& raw);
    virtual JUB_RV SetUnit(JUB_ENUM_BTC_UNIT_TYPE unitType);

    virtual JUB_RV BuildUSDTOutputs(IN JUB_CHAR_PTR USDTTo, IN JUB_UINT64 amount, OUT OUTPUT_BTC outputs[2]);
    virtual JUB_RV ActiveSelf();

private:
    JUB_ENUM_COINTYPE_BTC   _coinType{ COINBTC };
    JUB_ENUM_BTC_TRANS_TYPE _transType{ p2pkh };
    JUB_ENUM_BTC_UNIT_TYPE  _unitType{ mBTC };
}; // class ContextBTC end

} // namespace jub end

#endif // #pragma once
