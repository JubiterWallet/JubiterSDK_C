#pragma once
#include <memory>

#include <token/BTC/TrezorCryptoBTCImpl.h>


namespace jub {
namespace token {


class TrezorCryptoUSDTImpl :
    public TrezorCryptoBTCImpl{
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(const std::string& XPRVorXPUB) {
        return std::make_shared<TrezorCryptoUSDTImpl>(XPRVorXPUB);
    }

public:
    TrezorCryptoUSDTImpl(const std::string& XPRVorXPUB) :
        TrezorCryptoBTCImpl(XPRVorXPUB) {}
    ~TrezorCryptoUSDTImpl() {}
}; // class TrezorCryptoUSDTImpl end


} // namespace token end
} // namespace jub end
