#pragma once
#include <memory>

#include <token/BTC/TrezorCryptoBTCImpl.h>


namespace jub {
namespace token {


class TrezorCryptoLTCImpl :
    public TrezorCryptoBTCImpl {
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(const std::string& XPRVorXPUB) {
        return std::make_shared<TrezorCryptoLTCImpl>(XPRVorXPUB);
    }

public:
    TrezorCryptoLTCImpl(const std::string& XPRVorXPUB) :
        TrezorCryptoBTCImpl(XPRVorXPUB) {
            _coin = TWCoinType::TWCoinTypeLitecoin;
    }
    ~TrezorCryptoLTCImpl() {}
}; // class TrezorCryptoLTCImpl end


} // namespace token end
} // namespace jub end
