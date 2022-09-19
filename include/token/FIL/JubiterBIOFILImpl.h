#pragma once

#include <memory>

#include "token/FIL/JubiterBladeFILImpl.h"
#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/JubiterBlade/JubiterBladeToken.h"

namespace jub {
namespace token {

class JubiterBIOFILImpl : public JubiterBIOToken, public JubiterBladeFILImpl {
  public:
    JubiterBIOFILImpl(JUB_UINT16 deviceID)
        : JubiterBIOToken(deviceID), JubiterBladeFILImpl(deviceID), JubiterBladeToken(deviceID) {}
    virtual ~JubiterBIOFILImpl() = default;
}; // class JubiterBIOFILImpl end

} // namespace token
} // namespace jub
