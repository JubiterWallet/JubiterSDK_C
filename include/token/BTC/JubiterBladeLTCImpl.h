#pragma once
#include <memory>

#include "token/BTC/JubiterBladeBTCImpl.h"


namespace jub {
namespace token {


class JubiterBladeLTCImpl :
    public JubiterBladeBTCImpl {
public:
    JubiterBladeLTCImpl(const JUB_UINT16 deviceID) :
        JubiterBladeBTCImpl(deviceID) {
            _coin = TWCoinType::TWCoinTypeLitecoin;
    }
    ~JubiterBladeLTCImpl() {}
}; // class JubiterBladeLTCImpl end


} // namespace token end
} // namespace jub end
