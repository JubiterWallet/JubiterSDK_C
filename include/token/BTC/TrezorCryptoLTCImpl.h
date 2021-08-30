#pragma once
#include <memory>

#include "token/BTC/TrezorCryptoBTCImpl.h"


namespace jub {
namespace token {


class TrezorCryptoLTCImpl :
    public TrezorCryptoBTCImpl {
public:
    TrezorCryptoLTCImpl(JUB_UINT16 deviceID, const JUB_ENUM_CURVES curve=JUB_ENUM_CURVES::SECP256K1) :
        TrezorCryptoBTCImpl(deviceID, curve) {
            _coin = TWCoinType::TWCoinTypeLitecoin;
    }
    ~TrezorCryptoLTCImpl() {}
}; // class TrezorCryptoLTCImpl end


} // namespace token end
} // namespace jub end
