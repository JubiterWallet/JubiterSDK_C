#pragma once
#include <memory>

#include "token/BTC/JubiterNFCBTCImpl.h"
#include "token/interface/QTUMTokenInterface.hpp"


namespace jub {
namespace token {


class JubiterNFCQTUMImpl :
        public JubiterNFCBTCImpl,
virtual public QTUMTokenInterface {
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterNFCQTUMImpl>(deviceID);
    }

public:
    JubiterNFCQTUMImpl(JUB_UINT16 deviceID) :
        JubiterNFCBTCImpl(deviceID) {
            _coin = TWCoinType::TWCoinTypeQtum;
    }
    ~JubiterNFCQTUMImpl() {}

	//QTUM functions
	virtual JUB_RV SetQRC20ETHToken(const std::string& tokenName,
                                    const JUB_UINT16 unitDP,
                                    const std::string& contractAddress) override;
}; // class JubiterBladeQTUMImpl end


} // namespace token end
} // namespace jub end
