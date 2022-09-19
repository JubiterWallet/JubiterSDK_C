#pragma once

#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/JubiterLite/JubiterLiteImpl.h"
#include "token/XRP/JubiterBaseXRPImpl.h"
#include <string>
#include <vector>

namespace jub {
namespace token {

class JubiterLiteXRPImpl : public JubiterLiteImpl, public JubiterBaseXRPImpl {
  public:
    JubiterLiteXRPImpl(const JUB_UINT16 deviceID) : JubiterLiteImpl(deviceID), JubiterBladeToken(deviceID) {}
    ~JubiterLiteXRPImpl() {}

    // XRP functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV GetAppletVersion(stVersion &version) override;
    virtual JUB_RV SetCoin() override;

    virtual JUB_RV GetAddress(const std::string &path, const JUB_UINT16 tag, std::string &address) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey) override;

    virtual JUB_RV SignTX(const std::vector<JUB_BYTE> &vPath, std::vector<JUB_BYTE> &vUnsignedRaw,
                          std::vector<uchar_vector> &vSignatureRaw) override;

  protected:
    virtual JUB_RV _encodeRSV(const std::vector<JUB_BYTE> &vRSV, std::vector<JUB_BYTE> &signature);
}; // class JubiterLiteXRPImpl end

} // namespace token
} // namespace jub
