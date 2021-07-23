#pragma once
#ifndef __ContextBTC__
#define __ContextBTC__

#include "JUB_SDK_BTC.h"
#include "JUB_SDK_CKB.h"

#include <string>
#include <vector>

#include "utility/Singleton.h"
#include "utility/xManager.hpp"

#include "context/BaseContext.h"
#include "token/interface/BaseToken.h"

#include "utility/util.h"


namespace jub {
namespace context {


class BTCContext :
    public BaseContext {
public:
    BTCContext(const CONTEXT_CONFIG_BTC& cfg, std::shared_ptr<token::BaseToken> tokenPtr):
        BaseContext(tokenPtr) {
            _mainPath = cfg.mainPath;
           _transType = cfg.transType;
            _unitType = mBTC;
            _coinType = cfg.coinType;
            _timeout = 120 * 2;
            _coinNet = TWCoinType::TWCoinTypeBitcoin;
    }
    BTCContext(JUB_CHAR_CPTR mainPath, const JUB_ENUM_COINTYPE_BTC& coinType, std::shared_ptr<token::BaseToken> tokenPtr):
        BaseContext(tokenPtr) {
            _mainPath = mainPath;
           _transType = p2pkh;//p2sh_p2wpkh;
            _unitType = mBTC;
            _coinType = coinType;
            _timeout = 120 * 2;
    }
    ~BTCContext() {}

    virtual JUB_RV GetHDNode(const BIP44_Path& path, std::string& xpub);
    virtual JUB_RV GetMainHDNode(std::string& xpub);

    virtual JUB_RV GetAddress(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const BIP44_Path& path, const JUB_UINT16 tag, std::string& address);
    virtual JUB_RV CheckAddress(const std::string& address);
    virtual JUB_RV SetMyAddress(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const BIP44_Path& path, std::string& address);
    virtual JUB_RV SignTX(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const JUB_UINT32 version, const std::vector<INPUT_BTC>& vInputs, const std::vector<OUTPUT_BTC>& vOutputs, const JUB_UINT32 lockTime, std::string& raw);
    virtual JUB_RV SetUnit(const JUB_ENUM_BTC_UNIT_TYPE& unitType);

    virtual JUB_RV BuildUSDTOutputs(IN JUB_CHAR_CPTR USDTTo, IN JUB_UINT64 amount, OUT OUTPUT_BTC outputs[2]);
    virtual JUB_RV ActiveSelf() override;

protected:
    JUB_ENUM_COINTYPE_BTC    _coinType{ COINBTC };
    JUB_ENUM_BTC_TRANS_TYPE _transType{ p2pkh };
    JUB_ENUM_BTC_UNIT_TYPE   _unitType{ mBTC };
}; // class BTCContext end


class BCHContext :
    public BTCContext {
public:
    BCHContext(const CONTEXT_CONFIG_BTC& cfg,
               std::shared_ptr<token::BaseToken> tokenPtr):
        BTCContext(cfg, tokenPtr) {}
    ~BCHContext() {}
}; // class BCHContext end


class LTCContext :
    public BTCContext {
public:
    LTCContext(const CONTEXT_CONFIG_BTC& cfg,
               std::shared_ptr<token::BaseToken> tokenPtr):
        BTCContext(cfg, tokenPtr) {}
    ~LTCContext() {}
}; // class LTCContext end


class USDTContext :
    public BTCContext {
public:
    USDTContext(const CONTEXT_CONFIG_BTC& cfg,
                std::shared_ptr<token::BaseToken> tokenPtr):
        BTCContext(cfg, tokenPtr) {}
    ~USDTContext() {}
}; // class USDTContext end


class QTUMContext :
    public BTCContext {
public:
    //for Factory
    static BTCContext* Create(const CONTEXT_CONFIG_BTC& cfg,
                              std::shared_ptr<token::BaseToken> tokenPtr) {
        return new QTUMContext(cfg, tokenPtr);
    }
public:
    QTUMContext(const CONTEXT_CONFIG_BTC& cfg,
                std::shared_ptr<token::BaseToken> tokenPtr):
        BTCContext(cfg, tokenPtr) {}
    ~QTUMContext() {}

	virtual JUB_RV SetQRC20Token(IN JUB_CHAR_CPTR contractAddress, IN JUB_UINT8 decimal, IN JUB_CHAR_CPTR symbol);
	virtual JUB_RV BuildQRC20Outputs(JUB_UINT64 gasLimit, JUB_UINT64 gasPrice, IN JUB_CHAR_CPTR contractAddress, JUB_CHAR_CPTR to, JUB_CHAR_CPTR value, OUT OUTPUT_BTC outputs[1]);
}; // class QTUMContext end


class DASHContext :
    public BTCContext {
public:
    DASHContext(const CONTEXT_CONFIG_BTC& cfg,
                std::shared_ptr<token::BaseToken> tokenPtr):
        BTCContext(cfg, tokenPtr) {}
    ~DASHContext() {}
}; // class DASHContext end


} // namespace context end
} // namespace jub end

#endif // #pragma once
