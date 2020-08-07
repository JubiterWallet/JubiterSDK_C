#pragma once
#include <memory>

#include <token/BTC/TrezorCryptoBTCImpl.h>
#include <token/interface/QTUMTokenInterface.hpp>


namespace jub {
namespace token {


class TrezorCryptoQTUMImpl :
        public TrezorCryptoBTCImpl,
virtual public QTUMTokenInterface {
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(const std::string& XPRVorXPUB) {
        return std::make_shared<TrezorCryptoQTUMImpl>(XPRVorXPUB);
    }

public:
    TrezorCryptoQTUMImpl(const std::string& XPRVorXPUB) :
        TrezorCryptoBTCImpl(XPRVorXPUB) {
            _coin = TWCoinType::TWCoinTypeQtum;
    }
    ~TrezorCryptoQTUMImpl() {}

	//QTUM functions
	virtual JUB_RV SetQRC20ETHToken(const std::string& tokenName,
		const JUB_UINT16 unitDP,
		const std::string& contractAddress) override;
}; // class TrezorCryptoQTUMImpl end


} // namespace token end
} // namespace jub end
