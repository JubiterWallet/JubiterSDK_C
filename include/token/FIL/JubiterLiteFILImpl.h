#pragma once

#include "token/FIL/JubiterBaseFILImpl.h"
#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/JubiterLite/JubiterLiteImpl.h"
#include <string>
#include <vector>

namespace jub {
namespace token {

class JubiterLiteFILImpl : public JubiterLiteImpl, public JubiterBaseFILImpl {
  public:
    JubiterLiteFILImpl(JUB_UINT16 deviceID) : JubiterLiteImpl(deviceID), JubiterBladeToken(deviceID) {}
    ~JubiterLiteFILImpl() {}

    // FIL functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV SetCoin() override;
    virtual JUB_RV GetAddress(const std::string &path, const JUB_UINT16 tag, std::string &address) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey) override;
    virtual JUB_RV SignTX(const uint64_t &nonce, const int64_t &glimit, const uint256_t &gfeeCap,
                          const uint256_t &gpremium, const std::string &to, const uint256_t &value,
                          const std::string &input, const std::string &path,
                          std::vector<uchar_vector> &vSignatureRaw) override;

  protected:
    virtual JUB_RV _encodeRSV(const std::vector<JUB_BYTE> &vRSV, std::vector<JUB_BYTE> &signature);
}; // class JubiterLiteFILImpl end

} // namespace token
} // namespace jub
