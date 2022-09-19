#pragma once
#include "token/HC/JubiterBladeHCImpl.h"
#include "token/JubiterBIO/JubiterBIOToken.h"

#include <memory>

namespace jub {
namespace token {

class JubiterBIOHCImpl : public JubiterBIOToken, public JubiterBladeHCImpl {
  public:
    JubiterBIOHCImpl(const JUB_UINT16 deviceID)
        : JubiterBIOToken(deviceID), JubiterBladeHCImpl(deviceID), JubiterBladeToken(deviceID) {}
    virtual ~JubiterBIOHCImpl() = default;
}; // class JubiterBIOHCImpl end

} // namespace token
} // namespace jub
