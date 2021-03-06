#pragma once
#include <memory>

#include "token/BTC/TrezorCryptoBTCImpl.h"
#include "token/BTC/JubiterBaseDashImpl.h"


namespace jub {
namespace token {


class TrezorCryptoDashImpl :
        public TrezorCryptoBTCImpl,
virtual public JubiterBaseDashImpl {
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(const std::string& XPRVorXPUB) {
        return std::make_shared<TrezorCryptoDashImpl>(XPRVorXPUB);
    }

public:
    TrezorCryptoDashImpl(const std::string& XPRVorXPUB) :
        TrezorCryptoBTCImpl(XPRVorXPUB) {}
    ~TrezorCryptoDashImpl() {}

    virtual JUB_RV SignTX(const JUB_BYTE addrFmt,
                          const JUB_ENUM_BTC_TRANS_TYPE& type,
                          const JUB_UINT16 inputCount,
                          const std::vector<JUB_UINT64>& vInputAmount,
                          const std::vector<std::string>& vInputPath,
                          const std::vector<JUB_UINT16>& vChangeIndex,
                          const std::vector<std::string>& vChangePath,
                          const std::vector<JUB_BYTE>& vUnsigedTrans,
                          std::vector<JUB_BYTE>& vRaw) override;
}; // class TrezorCryptoDashImpl end


} // namespace token end
} // namespace jub end
