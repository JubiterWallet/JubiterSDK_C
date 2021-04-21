#pragma once

#include <memory>

#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/FIL/JubiterBladeFILImpl.h"

namespace jub {
namespace token {


class JubiterBIOFILImpl :
        public JubiterBIOToken,
virtual public JubiterBladeFILImpl {
public:
    JubiterBIOFILImpl(JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeFILImpl(deviceID) {}
    virtual ~JubiterBIOFILImpl() = default;
}; // class JubiterBIOFILImpl end


} // namespace token end
} // namespace jub end
