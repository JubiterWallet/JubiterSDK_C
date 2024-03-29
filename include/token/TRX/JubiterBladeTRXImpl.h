#pragma once
#include <memory>

#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/TRX/JubiterBaseTRXImpl.h"
#include <string>
#include <vector>

namespace jub {
namespace token {

class JubiterBladeTRXImpl : virtual public JubiterBladeToken, public JubiterBaseTRXImpl {
  public:
    JubiterBladeTRXImpl(const JUB_UINT16 deviceID) : JubiterBladeToken(deviceID) {}
    ~JubiterBladeTRXImpl() {}

    // TRX functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV GetAppletVersion(stVersion &version) override;
    virtual JUB_RV SetCoin() override;

    virtual JUB_RV GetAddress(const std::string &path, const JUB_UINT16 tag, std::string &address) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey) override;

    virtual JUB_RV SignTX(const std::vector<JUB_BYTE> &vPath, const std::vector<JUB_BYTE> &vRaw,
                          std::vector<uchar_vector> &vSignatureRaw) override;

    virtual JUB_RV SignBytestring(const std::vector<JUB_BYTE> &vData, const std::vector<JUB_BYTE> &vPath,
                                  std::vector<uchar_vector> &signatureRaw) override;

    virtual JUB_RV SetTRC20Token(const std::string &tokenName, const JUB_UINT16 unitDP,
                                 const std::string &contractAddress) override;
    virtual JUB_RV SetTRC721Token(const std::string &tokenName, const std::string &contractAddress) override;
}; // class JubiterBladeTRXImpl end

} // namespace token
} // namespace jub
