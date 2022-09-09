#pragma once
#include <memory>

#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/EOS/JubiterBaseEOSImpl.h"


namespace jub {
namespace token {


constexpr JUB_BYTE kPKIAID_EOS[16] = {
    0xD1, 0x56, 0x00, 0x01, 0x32, 0x03, 0x00, 0x42, 0x4C, 0x44, 0x00, 0x00, 0x45, 0x54, 0x49, 0x01
};


class JubiterBladeEOSImpl :
        public JubiterBladeToken,
virtual public JubiterBaseEOSImpl {
public:
    JubiterBladeEOSImpl(const JUB_UINT16 deviceID) :
        JubiterBladeToken(deviceID) {}
    ~JubiterBladeEOSImpl() {}

    //EOS functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV SetCoin() override;
    virtual JUB_RV GetAppletVersion(stVersion& version) override;

    virtual JUB_RV GetAddress(const TW::EOS::Type& type, const std::string& path, const JUB_UINT16 tag, std::string& address) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) override;

    virtual JUB_RV SignTX(const TW::EOS::Type& type,
                          const std::vector<JUB_BYTE>& vPath,
                          const std::vector<JUB_BYTE>& vChainId,
                          const std::vector<JUB_BYTE>& vRaw,
                          std::vector<uchar_vector>& vSignatureRaw,
                          const bool bWithType = false) override;
}; // class JubiterBladeEOSImpl end


} // namespace token end
} // namespace jub end
