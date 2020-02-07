#pragma once

#include <token/BTC/JubiterBladeBTCImpl.h>
#include <token/interface/QTUMTokenInterface.hpp>

namespace jub {
namespace token {


class JubiterBladeQTUMImpl :
    public JubiterBladeBTCImpl,
	virtual public QTUMTokenInterface {

public:
    JubiterBladeQTUMImpl(JUB_UINT16 deviceID) :
        JubiterBladeBTCImpl(deviceID) {
        _coin = TWCoinType::TWCoinTypeQtum;
    };
    ~JubiterBladeQTUMImpl() {};

	//QTUM functions
	virtual JUB_RV SetQRC20ETHToken(const std::string& tokenName,
		const JUB_UINT16 unitDP,
		const std::string& contractAddress) override;

    static std::shared_ptr<BTCTokenInterface> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBladeQTUMImpl>(deviceID);
    }
}; // class JubiterBladeQTUMImpl end


} // namespace token end
} // namespace jub end
