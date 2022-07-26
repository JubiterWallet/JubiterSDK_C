#pragma once
#include <memory>

#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/SOL/JubiterBaseSOLImpl.h"


namespace jub {
namespace token {


constexpr JUB_BYTE kPKIAID_SOL[16] = {
    0xD1, 0x56, 0x00, 0x01, 0x32, 0x83, 0x00, 0x42, 0x4C, 0x44, 0x00, 0x00, 0x45, 0x54, 0x48, 0x01
};


class JubiterBladeSOLImpl :
        public JubiterBladeToken,
virtual public JubiterBaseSOLImpl {
public:
    JubiterBladeSOLImpl(JUB_UINT16 deviceID) :
        JubiterBladeToken(deviceID) {}
    ~JubiterBladeSOLImpl() {}

    //SOL functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV GetAppletVersion(stVersion& version) override;

    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) override;

protected:
}; // class JubiterBladeSOLImpl end


} // namespace token end
} // namespace jub end
