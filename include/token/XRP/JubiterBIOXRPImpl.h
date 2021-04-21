#pragma once
#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/XRP/JubiterBladeXRPImpl.h"

#include <memory>

namespace jub {
namespace token {


class JubiterBIOXRPImpl :
        public JubiterBIOToken,
virtual public JubiterBladeXRPImpl {
public:
    JubiterBIOXRPImpl(JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeXRPImpl(deviceID) {}
    virtual ~JubiterBIOXRPImpl() = default;
}; // class JubiterBIOXRPImpl end


} // namespace token end
} // namespace jub end
