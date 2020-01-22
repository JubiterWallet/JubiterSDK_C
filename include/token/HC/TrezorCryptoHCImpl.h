#pragma once

#include <token/interface/BTCTokenInterface.hpp>
#include <token/BTC/TrezorCryptoBTCImpl.h>

namespace jub {
namespace token {


class TrezorCryptoHCImpl :
    public TrezorCryptoBTCImpl,
    virtual public BTCTokenInterface {

public:
    TrezorCryptoHCImpl(const std::string& xprv) :
        TrezorCryptoBTCImpl(xprv) {
            _coin = TWCoinType::TWCoinTypeHcash;
        };

    ~TrezorCryptoHCImpl() {};

    //HC functions
    JUB_RV GetAddress(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, const JUB_UINT16 tag, std::string& address) override;
}; // class TrezorCryptoHCImpl end


} // namespace token end
} // namespace jub end
