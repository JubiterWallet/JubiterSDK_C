#pragma once
#include <memory>

#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/SOL/JubiterBladeSOLImpl.h"

namespace jub {
namespace token {

class JubiterBIOSOLImpl : public JubiterBIOToken, public JubiterBladeSOLImpl {
  public:
    JubiterBIOSOLImpl(JUB_UINT16 deviceID)
        : JubiterBIOToken(deviceID), JubiterBladeSOLImpl(deviceID), JubiterBladeToken(deviceID) {}

}; // class JubiterBIOSOLImpl end

} // namespace token
} // namespace jub
