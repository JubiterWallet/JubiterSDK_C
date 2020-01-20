#pragma once

#include <token/BTC/TrezorCryptoBTCImpl.h>

namespace jub {
namespace token {


class TrezorCryptoDashImpl :
    public TrezorCryptoBTCImpl {

public:
    TrezorCryptoDashImpl(const std::string& xprv) :
        TrezorCryptoBTCImpl(xprv) {
            _coin = TWCoinType::TWCoinTypeDash;
        };
    ~TrezorCryptoDashImpl() {};

    static std::shared_ptr<BTCTokenInterface> Create(const std::string& xprv) { return std::make_shared<TrezorCryptoDashImpl>(xprv);
    }
}; // class TrezorCryptoDashImpl end


} // namespace token end
} // namespace jub end
