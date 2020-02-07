#pragma once

#include <token/BTC/TrezorCryptoBTCImpl.h>
#include <token/interface/QTUMTokenInterface.hpp>


namespace jub {
namespace token {


class TrezorCryptoQTUMImpl :
    public TrezorCryptoBTCImpl,
	virtual public QTUMTokenInterface {

public:
    TrezorCryptoQTUMImpl(const std::string& xprv) :
        TrezorCryptoBTCImpl(xprv) {
        _coin = TWCoinType::TWCoinTypeQtum;
    };
    ~TrezorCryptoQTUMImpl() {};

	//QTUM functions
	virtual JUB_RV SetQRC20ETHToken(const std::string& tokenName,
		const JUB_UINT16 unitDP,
		const std::string& contractAddress) override;

    static std::shared_ptr<BTCTokenInterface> Create(const std::string& xprv) { return std::make_shared<TrezorCryptoQTUMImpl>(xprv);
    }
}; // class TrezorCryptoQTUMImpl end


} // namespace token end
} // namespace jub end
