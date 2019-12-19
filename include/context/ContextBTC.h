#pragma once
#ifndef __ContextBTC__
#define __ContextBTC__

#include "JUB_SDK_BTC.h"

#include <string>
#include <vector>

#include "utility/Singleton.h"
#include "utility/xManager.hpp"

#include "context/Context.h"
#include "utility/util.h"

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

class ContextBTC : public Context {

public:
    ContextBTC() {};
    ContextBTC(const CONTEXT_CONFIG_BTC& cfg, const JUB_UINT16 deviceID) {
        _mainPath = cfg.mainPath;
        _transType = cfg.transType;
        _deviceID = deviceID;
        _unitType = mBTC;
        _coinType = cfg.coinType;
        _timeout = 120 * 2;
    };
    ~ContextBTC() {};

    virtual JUB_RV GetHDNode(const BIP44_Path& path, std::string& xpub);
    virtual JUB_RV GetMainHDNode(std::string& xpub);

    virtual JUB_RV GetAddress(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const BIP44_Path& path, const JUB_UINT16 tag, std::string& address);
    virtual JUB_RV SetMyAddress(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const BIP44_Path& path, std::string& address);
    virtual JUB_RV SignTX(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const std::vector<INPUT_BTC>& vInputs, const std::vector<OUTPUT_BTC>& vOutputs, const JUB_UINT32 lockTime, std::string& raw);
    virtual JUB_RV SetUnit(const JUB_ENUM_BTC_UNIT_TYPE& unitType);

    virtual JUB_RV BuildUSDTOutputs(IN JUB_CHAR_CPTR USDTTo, IN JUB_UINT64 amount, OUT OUTPUT_BTC outputs[2]);
    virtual JUB_RV SetQRC20Token(IN JUB_CHAR_CPTR contractAddress,IN JUB_UINT8 decimal,IN JUB_CHAR_CPTR symbol);
    virtual JUB_RV BuildQRC20Outputs(JUB_UINT64 gasLimit,JUB_UINT64 gasPrice,IN JUB_CHAR_CPTR contractAddress, JUB_CHAR_CPTR to, JUB_CHAR_CPTR value, OUT OUTPUT_BTC outputs[1]);
    virtual JUB_RV ActiveSelf() override;

protected:
    JUB_BYTE _RealAddressFormat(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt) {
        return ((addrFmt&0x0F) << 4);
    }
    JUB_ENUM_COINTYPE_BTC   _coinType{ COINBTC };
    JUB_ENUM_BTC_TRANS_TYPE _transType{ p2pkh };
    JUB_ENUM_BTC_UNIT_TYPE  _unitType{ mBTC };
}; // class ContextBTC end

} // namespace jub end

#endif // #pragma once
