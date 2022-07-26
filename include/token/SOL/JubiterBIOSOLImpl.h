#pragma once
#include <memory>

#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/SOL/JubiterBladeSOLImpl.h"


namespace jub {
namespace token {


class JubiterBIOSOLImpl :
        public JubiterBIOToken,
virtual public JubiterBladeSOLImpl {
public:
    JubiterBIOSOLImpl(JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeSOLImpl(deviceID) {}
    virtual ~JubiterBIOSOLImpl() = default;

    virtual JUB_RV SelectApplet() override;
}; // class JubiterBIOSOLImpl end


} // namespace token end
} // namespace jub end
