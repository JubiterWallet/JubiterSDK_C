#pragma once

#include <memory>

#include "token/DOT/JubiterBladeDOTImpl.h"
#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/JubiterBlade/JubiterBladeToken.h"

namespace jub {
namespace token {

class JubiterBIODOTImpl : public JubiterBIOToken, public JubiterBladeDOTImpl {
  public:
    // for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBIODOTImpl>(deviceID);
    }

  public:
    JubiterBIODOTImpl(JUB_UINT16 deviceID)
        : JubiterBIOToken(deviceID), JubiterBladeDOTImpl(deviceID), JubiterBladeToken(deviceID) {}
    virtual ~JubiterBIODOTImpl() = default;
}; // class JubiterBIODOTImpl end

} // namespace token
} // namespace jub
