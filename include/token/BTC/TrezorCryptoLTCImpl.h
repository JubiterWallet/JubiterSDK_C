#pragma once
#include <memory>

#include "token/BTC/TrezorCryptoBTCImpl.h"


namespace jub {
namespace token {


class TrezorCryptoLTCImpl :
    public TrezorCryptoBTCImpl {
public:
    TrezorCryptoLTCImpl(JUB_UINT16 deviceID) :
        TrezorCryptoBTCImpl(deviceID) {
            _coin = TWCoinType::TWCoinTypeLitecoin;
    }
    ~TrezorCryptoLTCImpl() {}
}; // class TrezorCryptoLTCImpl end


} // namespace token end
} // namespace jub end
