#pragma once
#include <memory>

#include "token/BTC/TrezorCryptoBTCImpl.h"


namespace jub {
namespace token {


class TrezorCryptoUSDTImpl :
    public TrezorCryptoBTCImpl{
public:
    TrezorCryptoUSDTImpl(JUB_UINT16 deviceID) :
        TrezorCryptoBTCImpl(deviceID) {}
    ~TrezorCryptoUSDTImpl() {}
}; // class TrezorCryptoUSDTImpl end


} // namespace token end
} // namespace jub end
