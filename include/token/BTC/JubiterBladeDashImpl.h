#pragma once

#include <token/BTC/JubiterBladeBTCImpl.h>
#include <token/BTC/JubiterBaseDashImpl.h>

namespace jub {
namespace token {


class JubiterBladeDashImpl :
        public JubiterBladeBTCImpl,
virtual public JubiterBaseDashImpl {

public:
    JubiterBladeDashImpl(JUB_UINT16 deviceID) :
        JubiterBladeBTCImpl(deviceID) {
        _coin = TWCoinType::TWCoinTypeDash;
    };
    ~JubiterBladeDashImpl() {};

    static std::shared_ptr<BTCTokenInterface> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBladeDashImpl>(deviceID);
    }
}; // class JubiterBladeDashImpl end


} // namespace token end
} // namespace jub end
