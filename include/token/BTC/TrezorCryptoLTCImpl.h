#pragma once
#include <token/BTC/TrezorCryptoBTCImpl.h>

namespace jub {
namespace token {


class TrezorCryptoLTCImpl :
    public TrezorCryptoBTCImpl {

public:
    TrezorCryptoLTCImpl(const std::string& XPRVorXPUB) :
        TrezorCryptoBTCImpl(XPRVorXPUB) {
        _coin = TWCoinType::TWCoinTypeLitecoin;
    };
    ~TrezorCryptoLTCImpl() {};

    static std::shared_ptr<BTCTokenInterface> Create(const std::string& XPRVorXPUB) { return std::make_shared<TrezorCryptoLTCImpl>(XPRVorXPUB);
    }
}; // class TrezorCryptoLTCImpl end


} // namespace token end
} // namespace jub end
