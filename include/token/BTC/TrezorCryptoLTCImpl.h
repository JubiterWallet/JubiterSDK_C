#pragma once

#include <token/BTC/TrezorCryptoBTCImpl.h>

namespace jub {
namespace token {


class TrezorCryptoLTCImpl :
    public TrezorCryptoBTCImpl {

public:
    TrezorCryptoLTCImpl(const std::string& xprv) :
        TrezorCryptoBTCImpl(xprv) {
            _coin = TWCoinType::TWCoinTypeLitecoin;
        };
    ~TrezorCryptoLTCImpl() {};

    static std::shared_ptr<BTCTokenInterface> Create(const std::string& xprv) { return std::make_shared<TrezorCryptoLTCImpl>(xprv);
    }
}; // class TrezorCryptoLTCImpl end


} // namespace token end
} // namespace jub end
