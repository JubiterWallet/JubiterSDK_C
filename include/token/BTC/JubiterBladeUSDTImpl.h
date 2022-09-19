#pragma once
#include <memory>

#include "token/BTC/JubiterBladeBTCImpl.h"
#include "token/JubiterBlade/JubiterBladeToken.h"

namespace jub {
namespace token {

class JubiterBladeUSDTImpl : public JubiterBladeBTCImpl {
  public:
    JubiterBladeUSDTImpl(const JUB_UINT16 deviceID) : JubiterBladeBTCImpl(deviceID), JubiterBladeToken(deviceID) {}
    ~JubiterBladeUSDTImpl() {}
}; // class JubiterBladeUSDTImpl end

} // namespace token
} // namespace jub
