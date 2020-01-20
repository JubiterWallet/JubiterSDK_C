#pragma once

#include <token/BTC/TrezorCryptoBTCImpl.h>

namespace jub {
namespace token {


class TrezorCryptoBCHImpl :
    public TrezorCryptoBTCImpl{

public:
    TrezorCryptoBCHImpl(const std::string& xprv) :
        TrezorCryptoBTCImpl(xprv) {
            _coin = TWCoinType::TWCoinTypeBitcoinCash;
        };
    ~TrezorCryptoBCHImpl() {};

    static std::shared_ptr<BTCTokenInterface> Create(const std::string& xprv) { return std::make_shared<TrezorCryptoBCHImpl>(xprv);
    }
}; // class TrezorCryptoBCHImpl end


} // namespace token end
} // namespace jub end
