#pragma once
#include <memory>

#include "token/BTC/TrezorCryptoBTCImpl.h"


namespace jub {
namespace token {


class TrezorCryptoUSDTImpl :
    public TrezorCryptoBTCImpl{
public:
    TrezorCryptoUSDTImpl(JUB_UINT16 deviceID, const JUB_ENUM_CURVES curve=JUB_ENUM_CURVES::SECP256K1) :
        TrezorCryptoBTCImpl(deviceID, curve) {}
    ~TrezorCryptoUSDTImpl() {}
}; // class TrezorCryptoUSDTImpl end


} // namespace token end
} // namespace jub end
