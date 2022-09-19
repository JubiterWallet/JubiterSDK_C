#pragma once
#include <memory>

#include "token/BTC/JubiterBaseDashImpl.h"
#include "token/BTC/JubiterLiteBTCImpl.h"
#include "token/JubiterBlade/JubiterBladeToken.h"

namespace jub {
namespace token {

class JubiterLiteDashImpl : public JubiterLiteBTCImpl, virtual public JubiterBaseDashImpl {
  public:
    JubiterLiteDashImpl(const JUB_UINT16 deviceID) : JubiterLiteBTCImpl(deviceID), JubiterBladeToken(deviceID) {}
    ~JubiterLiteDashImpl() {}
}; // class JubiterLiteDashImpl end

} // namespace token
} // namespace jub
