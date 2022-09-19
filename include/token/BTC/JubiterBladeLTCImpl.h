#pragma once
#include <memory>

#include "token/BTC/JubiterBladeBTCImpl.h"
#include "token/JubiterBlade/JubiterBladeToken.h"

namespace jub {
namespace token {

class JubiterBladeLTCImpl : public JubiterBladeBTCImpl {
  public:
    JubiterBladeLTCImpl(const JUB_UINT16 deviceID) : JubiterBladeBTCImpl(deviceID), JubiterBladeToken(deviceID) {
        _coin = TWCoinType::TWCoinTypeLitecoin;
    }
    ~JubiterBladeLTCImpl() {}
}; // class JubiterBladeLTCImpl end

} // namespace token
} // namespace jub
