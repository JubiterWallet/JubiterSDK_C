#pragma once
#include <memory>

#include "token/BTC/JubiterBladeBTCImpl.h"
#include "token/HC/JubiterBaseHCImpl.h"
#include "token/JubiterBlade/JubiterBladeToken.h"

namespace jub {
namespace token {

class JubiterBladeHCImpl : public JubiterBladeBTCImpl, public JubiterBaseHCImpl {
  public:
    JubiterBladeHCImpl(const JUB_UINT16 deviceID) : JubiterBladeBTCImpl(deviceID), JubiterBladeToken(deviceID) {
        _hashType = TWHcashSigHashAll;
    }
    ~JubiterBladeHCImpl() {}

    // HC functions
    virtual JUB_RV SelectApplet() override;
}; // class JubiterBladeHCImpl end

} // namespace token
} // namespace jub
