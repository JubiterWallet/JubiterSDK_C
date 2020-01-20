#pragma once

#include <token/BTC/JubiterBladeBTCImpl.h>

namespace jub {
namespace token {


class JubiterBladeQTUMImpl :
    public JubiterBladeBTCImpl {

public:
    JubiterBladeQTUMImpl(JUB_UINT16 deviceID) :
        JubiterBladeBTCImpl(deviceID) {};
    ~JubiterBladeQTUMImpl() {};

    static std::shared_ptr<BTCTokenInterface> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBladeQTUMImpl>(deviceID);
    }
}; // class JubiterBladeQTUMImpl end


} // namespace token end
} // namespace jub end
