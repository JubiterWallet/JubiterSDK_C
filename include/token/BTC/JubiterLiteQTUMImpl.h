#pragma once
#include <memory>

#include "token/BTC/JubiterLiteBTCImpl.h"
#include "token/interface/QTUMTokenInterface.hpp"


namespace jub {
namespace token {


class JubiterLiteQTUMImpl :
        public JubiterLiteBTCImpl,
virtual public QTUMTokenInterface {
public:
    JubiterLiteQTUMImpl(const JUB_UINT16 deviceID) :
        JubiterLiteBTCImpl(deviceID) {
            _coin = TWCoinType::TWCoinTypeQtum;
    }
    ~JubiterLiteQTUMImpl() {}

	//QTUM functions
	virtual JUB_RV SetQRC20ETHToken(const std::string& tokenName,
                                    const JUB_UINT16 unitDP,
                                    const std::string& contractAddress) override;
}; // class JubiterLiteQTUMImpl end


} // namespace token end
} // namespace jub end
