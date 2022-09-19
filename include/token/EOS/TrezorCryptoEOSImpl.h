#pragma once
#include <memory>

#include "token/EOS/JubiterBaseEOSImpl.h"
#include "token/TrezorCrypto/TrezorCryptoToken.h"
#include <string>
#include <vector>

namespace jub {
namespace token {

class TrezorCryptoEOSImpl : public TrezorCryptoToken, public JubiterBaseEOSImpl {
  public:
    TrezorCryptoEOSImpl(JUB_UINT16 deviceID, const JUB_ENUM_CURVES curve = JUB_ENUM_CURVES::SECP256K1)
        : TrezorCryptoToken(deviceID, curve) {}
    ~TrezorCryptoEOSImpl() {}

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
}; // class TrezorCryptoEOSImpl end

} // namespace token
} // namespace jub
