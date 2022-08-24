#pragma once
#include <memory>

#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/SOL/JubiterBladeSOLImpl.h"

namespace jub {
namespace token {

class JubiterBIOSOLImpl : virtual public JubiterBIOToken, virtual public JubiterBladeSOLImpl {
  public:
    JubiterBIOSOLImpl(JUB_UINT16 deviceID) : JubiterBIOToken(deviceID), JubiterBladeSOLImpl(deviceID) {}

}; // class JubiterBIOSOLImpl end

} // namespace token
} // namespace jub
