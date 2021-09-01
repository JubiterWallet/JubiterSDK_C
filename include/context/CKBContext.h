#pragma once
#ifndef __ContextCKB__
#define __ContextCKB__

#include <string>
#include <vector>

#include "utility/Singleton.h"
#include "utility/xManager.hpp"

#include "context/BTCContext.h"
#include "token/interface/BaseToken.h"


namespace jub {
namespace context {


class CKBContextBase {
public:
    virtual JUB_RV signTX(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const JUB_UINT32 version, const std::vector<CELL_DEP>& vDeps, const std::vector<CELL_INPUT>& vInputs, const std::vector<CELL_OUTPUT>& vOutputs, std::string& raw) = 0;
}; // class CKBContextBase end


class CKBContext :
    public BTCContext,
    public CKBContextBase {
public:
    CKBContext(const CONTEXT_CONFIG_CKB& cfg,
               std::shared_ptr<token::BaseToken> tokenPtr):
        BTCContext(cfg.mainPath, JUB_ENUM_COINTYPE_BTC::COINCKB, tokenPtr) {
            _transType = p2sh_p2wpkh;
            _coinNet = (!cfg.mainNet ? TWCoinType::TWCoinTypeBitcoinTestNet : TWCoinType::TWCoinTypeBitcoin);
        }
    ~CKBContext() {}

    virtual JUB_RV ActiveSelf() override;

    virtual JUB_RV GetHDNode(const BIP44_Path& path, std::string& xpub) override;
    virtual JUB_RV GetMainHDNode(std::string& xpub) override;

    virtual JUB_RV GetAddress(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const BIP44_Path& path, const JUB_UINT16 tag, std::string& address) override;
    virtual JUB_RV CheckAddress(const std::string& address) override;

    virtual JUB_RV signTX(const JUB_ENUM_BTC_ADDRESS_FORMAT& addrFmt, const JUB_UINT32 version, const std::vector<CELL_DEP>& vDeps, const std::vector<CELL_INPUT>& vInputs, const std::vector<CELL_OUTPUT>& vOutputs, std::string& raw) override;
}; // class CKBContext end


} // namespace context end
} // namespace jub end

#endif // #pragma once
