#pragma once
#include <memory>

#include "token/BTC/JubiterBaseBTCImpl.h"
#include "token/BTC/JubiterBaseDashImpl.h"
#include "token/BTC/JubiterBladeBTCImpl.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/interface/BaseToken.h"

namespace jub {
namespace token {

class JubiterBladeDashImpl : public JubiterBladeBTCImpl, public JubiterBaseDashImpl {
  public:
    JubiterBladeDashImpl(const JUB_UINT16 deviceID) : JubiterBladeBTCImpl(deviceID), JubiterBladeToken(deviceID) {}
    ~JubiterBladeDashImpl() {}
}; // class JubiterBladeDashImpl end

} // namespace token
} // namespace jub
