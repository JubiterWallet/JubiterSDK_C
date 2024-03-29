#pragma once
#include <memory>

#include "token/TrezorCrypto/TrezorCryptoToken.h"
#include "token/XRP/JubiterBaseXRPImpl.h"
#include <string>
#include <vector>

namespace jub {
namespace token {

class TrezorCryptoXRPImpl : public TrezorCryptoToken, public JubiterBaseXRPImpl {
  public:
    TrezorCryptoXRPImpl(JUB_UINT16 deviceID, const JUB_ENUM_CURVES curve = JUB_ENUM_CURVES::SECP256K1)
        : TrezorCryptoToken(deviceID, curve) {}
    ~TrezorCryptoXRPImpl() {}

    // XRP functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV GetAppletVersion(stVersion &version) override;
    virtual JUB_RV SetCoin() override;

    virtual JUB_RV GetAddress(const std::string &path, const JUB_UINT16 tag, std::string &address) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey) override;

    virtual JUB_RV SignTX(const std::vector<JUB_BYTE> &vPath, std::vector<JUB_BYTE> &vUnsignedRaw,
                          std::vector<uchar_vector> &vSignatureRaw) override;
}; // class TrezorCryptoXRPImpl end

} // namespace token
} // namespace jub
