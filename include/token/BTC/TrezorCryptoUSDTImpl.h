#pragma once

#include <token/BTC/TrezorCryptoBTCImpl.h>

namespace jub {
namespace token {


class TrezorCryptoUSDTImpl :
    public TrezorCryptoBTCImpl{

public:
    TrezorCryptoUSDTImpl(const std::string& xprv) :
        TrezorCryptoBTCImpl(xprv) {};
    ~TrezorCryptoUSDTImpl() {};

    static std::shared_ptr<BTCTokenInterface> Create(const std::string& xprv) { return std::make_shared<TrezorCryptoUSDTImpl>(xprv);
    }
}; // class TrezorCryptoUSDTImpl end


} // namespace token end
} // namespace jub end
