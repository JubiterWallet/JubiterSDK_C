#pragma once

#include <token/BTC/JubiterBladeBTCImpl.h>
#include <token/BTC/JubiterBaseBCHImpl.h>

namespace jub {
namespace token {


class JubiterBladeBCHImpl :
    public JubiterBladeBTCImpl,
virtual public JubiterBaseBCHImpl {

public:
    JubiterBladeBCHImpl(JUB_UINT16 deviceID) :
        JubiterBladeBTCImpl(deviceID) {
		_coin = TWCoinType::TWCoinTypeBitcoinCash;
        _hashType = TWSignatureHashTypeAllFork;
	};
    ~JubiterBladeBCHImpl() {};

    static std::shared_ptr<BTCTokenInterface> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBladeBCHImpl>(deviceID);
    }
}; // class JubiterBladeBCHImpl end


} // namespace token end
} // namespace jub end
