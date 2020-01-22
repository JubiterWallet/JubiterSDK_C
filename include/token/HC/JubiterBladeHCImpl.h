#pragma once

#include <token/interface/BTCTokenInterface.hpp>
#include <token/BTC/JubiterBladeBTCImpl.h>

namespace jub {
namespace token {


class JubiterBladeHCImpl :
    public JubiterBladeBTCImpl,
    virtual public BTCTokenInterface {

public:
    JubiterBladeHCImpl(JUB_UINT16 deviceID) :
        JubiterBladeBTCImpl(deviceID) {};

    ~JubiterBladeHCImpl() {};

    //HC functions
    virtual JUB_RV SelectApplet() override;
}; // class JubiterBladeHCImpl end


} // namespace token end
} // namespace jub end
