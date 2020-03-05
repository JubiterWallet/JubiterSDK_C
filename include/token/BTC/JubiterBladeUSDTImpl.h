#pragma once
#include <token/BTC/JubiterBladeBTCImpl.h>

namespace jub {
namespace token {


class JubiterBladeUSDTImpl :
    public JubiterBladeBTCImpl {

public:
    JubiterBladeUSDTImpl(JUB_UINT16 deviceID) :
        JubiterBladeBTCImpl(deviceID) {};
    ~JubiterBladeUSDTImpl() {};

    static std::shared_ptr<BTCTokenInterface> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBladeUSDTImpl>(deviceID);
    }
}; // class JubiterBladeUSDTImpl end


} // namespace token end
} // namespace jub end
