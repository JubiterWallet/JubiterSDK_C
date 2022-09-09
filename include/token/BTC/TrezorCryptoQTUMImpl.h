#pragma once
#include <memory>

#include "token/BTC/TrezorCryptoBTCImpl.h"
#include "token/interface/QTUMTokenInterface.hpp"


namespace jub {
namespace token {


class TrezorCryptoQTUMImpl :
        public TrezorCryptoBTCImpl,
virtual public QTUMTokenInterface {
public:
    TrezorCryptoQTUMImpl(JUB_UINT16 deviceID, const JUB_ENUM_CURVES curve=JUB_ENUM_CURVES::SECP256K1) :
        TrezorCryptoBTCImpl(deviceID, curve) {
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
