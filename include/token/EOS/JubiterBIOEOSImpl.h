#pragma once
#include "token/EOS/JubiterBladeEOSImpl.h"
#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/JubiterBlade/JubiterBladeToken.h"

#include <memory>
#include <string>

namespace jub {
namespace token {

class JubiterBIOEOSImpl : public JubiterBIOToken, public JubiterBladeEOSImpl {
  public:
    JubiterBIOEOSImpl(const JUB_UINT16 deviceID)
        : JubiterBIOToken(deviceID), JubiterBladeEOSImpl(deviceID), JubiterBladeToken(deviceID) {}
    virtual ~JubiterBIOEOSImpl() = default;

    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey) override;
}; // class JubiterBIOEOSImpl end

} // namespace token
} // namespace jub
