#pragma once
#include <memory>

#include "token/BTC/JubiterLiteBTCImpl.h"
#include "token/JubiterBlade/JubiterBladeToken.h"

namespace jub {
namespace token {

class JubiterLiteUSDTImpl : public JubiterLiteBTCImpl {
  public:
    JubiterLiteUSDTImpl(const JUB_UINT16 deviceID) : JubiterLiteBTCImpl(deviceID), JubiterBladeToken(deviceID) {}
    ~JubiterLiteUSDTImpl() {}
}; // class JubiterLiteUSDTImpl end

} // namespace token
} // namespace jub
