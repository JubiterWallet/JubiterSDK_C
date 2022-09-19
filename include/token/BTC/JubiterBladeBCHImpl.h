#pragma once
#include <memory>

#include "token/BTC/JubiterBaseBCHImpl.h"
#include "token/BTC/JubiterBladeBTCImpl.h"
#include "token/JubiterBlade/JubiterBladeToken.h"

namespace jub {
namespace token {

class JubiterBladeBCHImpl : public JubiterBladeBTCImpl, public JubiterBaseBCHImpl {
  public:
    JubiterBladeBCHImpl(const JUB_UINT16 deviceID) : JubiterBladeBTCImpl(deviceID), JubiterBladeToken(deviceID) {
        _hashType = TWBitcoinSigHashTypeForkBCH;
    }
    ~JubiterBladeBCHImpl() {}
}; // class JubiterBladeBCHImpl end

} // namespace token
} // namespace jub
