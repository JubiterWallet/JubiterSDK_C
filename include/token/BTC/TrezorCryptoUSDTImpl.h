#pragma once
#include <token/BTC/TrezorCryptoBTCImpl.h>

namespace jub {
namespace token {


class TrezorCryptoUSDTImpl :
    public TrezorCryptoBTCImpl{

public:
    TrezorCryptoUSDTImpl(const std::string& XPRVorXPUB) :
        TrezorCryptoBTCImpl(XPRVorXPUB) {};
    ~TrezorCryptoUSDTImpl() {};

    static std::shared_ptr<BTCTokenInterface> Create(const std::string& XPRVorXPUB) { return std::make_shared<TrezorCryptoUSDTImpl>(XPRVorXPUB);
    }
}; // class TrezorCryptoUSDTImpl end


} // namespace token end
} // namespace jub end
