#pragma once
#include <memory>

#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/ETH/JubiterBladeETHImpl.h"


namespace jub {
namespace token {


class JubiterBIOETHImpl :
        public JubiterBIOToken,
virtual public JubiterBladeETHImpl {
public:
    JubiterBIOETHImpl(JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeETHImpl(deviceID) {}
    virtual ~JubiterBIOETHImpl() = default;
}; // class JubiterBIOETHImpl end


} // namespace token end
} // namespace jub end
