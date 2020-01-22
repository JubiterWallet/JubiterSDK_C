#pragma once

#include <token/BTC/JubiterBladeBTCImpl.h>

namespace jub {
namespace token {


class JubiterBladeDashImpl :
    public JubiterBladeBTCImpl {

public:
    JubiterBladeDashImpl(JUB_UINT16 deviceID) :
        JubiterBladeBTCImpl(deviceID) {};
    ~JubiterBladeDashImpl() {
		_coin = TWCoinType::TWCoinTypeDash;
	};

    static std::shared_ptr<BTCTokenInterface> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBladeDashImpl>(deviceID);
    }
}; // class JubiterBladeDashImpl end


} // namespace token end
} // namespace jub end
