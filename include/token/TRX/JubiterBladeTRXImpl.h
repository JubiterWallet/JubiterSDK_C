#pragma once
#include <memory>

#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/TRX/JubiterBaseTRXImpl.h"


namespace jub {
namespace token {


constexpr JUB_BYTE kPKIAID_TRX[16] = {
    0xD1, 0x56, 0x00, 0x01, 0x32, 0x03, 0x00, 0x42, 0x4C, 0x44, 0x00, 0x00, 0x45, 0x54, 0x49, 0x01
};


class JubiterBladeTRXImpl :
        public JubiterBladeToken,
virtual public JubiterBaseTRXImpl {
public:
    JubiterBladeTRXImpl(const JUB_UINT16 deviceID) :
        JubiterBladeToken(deviceID) {}
    ~JubiterBladeTRXImpl() {}


    //TRX functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV GetAppletVersion(stVersion& version) override;
    virtual JUB_RV SetCoin() override;

    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) override;

    virtual JUB_RV SignTX(const std::vector<JUB_BYTE>& vPath,
                          const std::vector<JUB_BYTE>& vRaw,
                          std::vector<uchar_vector>& vSignatureRaw) override;

    virtual JUB_RV SignBytestring(const std::vector<JUB_BYTE>& vData,
                                  const std::vector<JUB_BYTE>& vPath,
                                  std::vector<uchar_vector>& signatureRaw) override;

    virtual JUB_RV SetTRC20Token(const std::string& tokenName,
                                 const JUB_UINT16 unitDP,
                                 const std::string& contractAddress) override;
    virtual JUB_RV SetTRC721Token(const std::string& tokenName,
                                  const std::string& contractAddress) override;
}; // class JubiterBladeTRXImpl end


} // namespace token end
} // namespace jub end
