#pragma once
#include "token/BTC/JubiterBladeBTCImpl.h"

namespace jub {
namespace token {


class JubiterBladeLTCImpl :
    public JubiterBladeBTCImpl {

public:
    JubiterBladeLTCImpl(JUB_UINT16 deviceID) :
        JubiterBladeBTCImpl(deviceID) {
        _coin = TWCoinType::TWCoinTypeLitecoin;
    };
    ~JubiterBladeLTCImpl() {};

    static std::shared_ptr<BTCTokenInterface> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBladeLTCImpl>(deviceID);
    }
}; // class JubiterBladeLTCImpl end


} // namespace token end
} // namespace jub end
