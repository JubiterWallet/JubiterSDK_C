#pragma once
#include "token/EOS/JubiterBaseEOSImpl.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/JubiterLite/JubiterLiteImpl.h"
#include "token/JubiterLite/JubiterLiteToken.h"
#include <string>
#include <vector>

namespace jub {
namespace token {

class JubiterLiteEOSImpl : public JubiterLiteImpl, public JubiterBaseEOSImpl {
  public:
    JubiterLiteEOSImpl(const JUB_UINT16 deviceID) : JubiterLiteImpl(deviceID), JubiterBladeToken(deviceID) {}
    ~JubiterLiteEOSImpl() {}

    // EOS functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV SetCoin() override;
    virtual JUB_RV GetAppletVersion(stVersion &version) override;

    virtual JUB_RV GetAddress(const TW::EOS::Type &type, const std::string &path, const JUB_UINT16 tag,
                              std::string &address) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey) override;

    virtual JUB_RV SignTX(const TW::EOS::Type &type, const std::vector<JUB_BYTE> &vPath,
                          const std::vector<JUB_BYTE> &vChainId, const std::vector<JUB_BYTE> &vRaw,
                          std::vector<uchar_vector> &vSignatureRaw, const bool bWithType = false) override;

  protected:
    virtual JUB_RV _encodeRSV(const std::vector<JUB_BYTE> &vRSV, std::vector<JUB_BYTE> &signature);
}; // class JubiterLiteEOSImpl end

} // namespace token
} // namespace jub
