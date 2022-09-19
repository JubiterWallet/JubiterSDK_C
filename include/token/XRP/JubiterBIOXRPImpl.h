#pragma once
#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/XRP/JubiterBladeXRPImpl.h"

#include <memory>
#include <string>

namespace jub {
namespace token {

class JubiterBIOXRPImpl : public JubiterBIOToken, public JubiterBladeXRPImpl {
  public:
    JubiterBIOXRPImpl(const JUB_UINT16 deviceID)
        : JubiterBIOToken(deviceID), JubiterBladeXRPImpl(deviceID), JubiterBladeToken(deviceID) {}
    virtual ~JubiterBIOXRPImpl() = default;

    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey) override;
}; // class JubiterBIOXRPImpl end

} // namespace token
} // namespace jub
