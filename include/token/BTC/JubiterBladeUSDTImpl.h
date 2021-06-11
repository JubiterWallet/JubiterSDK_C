#pragma once
#include <memory>

#include "token/BTC/JubiterBladeBTCImpl.h"


namespace jub {
namespace token {


class JubiterBladeUSDTImpl :
    public JubiterBladeBTCImpl {
public:
    JubiterBladeUSDTImpl(const JUB_UINT16 deviceID) :
        JubiterBladeBTCImpl(deviceID) {}
    ~JubiterBladeUSDTImpl() {}
}; // class JubiterBladeUSDTImpl end


} // namespace token end
} // namespace jub end
