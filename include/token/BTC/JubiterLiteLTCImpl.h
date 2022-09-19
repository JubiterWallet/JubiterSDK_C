#pragma once
#include <memory>

#include "token/BTC/JubiterLiteBTCImpl.h"
#include "token/JubiterBlade/JubiterBladeToken.h"

namespace jub {
namespace token {

class JubiterLiteLTCImpl : public JubiterLiteBTCImpl {
  public:
    JubiterLiteLTCImpl(const JUB_UINT16 deviceID) : JubiterLiteBTCImpl(deviceID), JubiterBladeToken(deviceID) {
        _coin = TWCoinType::TWCoinTypeLitecoin;
    }
    ~JubiterLiteLTCImpl() {}
}; // class JubiterLiteLTCImpl end

} // namespace token
} // namespace jub
