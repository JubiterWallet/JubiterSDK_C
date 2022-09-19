#pragma once
#include <memory>

#include "token/FIL/JubiterBaseFILImpl.h"
#include "token/TrezorCrypto/TrezorCryptoToken.h"
#include <string>
#include <vector>

namespace jub {
namespace token {

class TrezorCryptoFILImpl : public TrezorCryptoToken, public JubiterBaseFILImpl {
  public:
    TrezorCryptoFILImpl(JUB_UINT16 deviceID, const JUB_ENUM_CURVES curve = JUB_ENUM_CURVES::SECP256K1)
        : TrezorCryptoToken(deviceID, curve) {}
    ~TrezorCryptoFILImpl() {}

    // FIL functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV SetCoin() override;
    virtual JUB_RV GetAddress(const std::string &path, const JUB_UINT16 tag, std::string &address) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey) override;
    virtual JUB_RV SignTX(const uint64_t &nonce, const int64_t &glimit, const uint256_t &gfeeCap,
                          const uint256_t &gpremium, const std::string &to, const uint256_t &value,
                          const std::string &input, const std::string &path,
                          std::vector<uchar_vector> &vSignatureRaw) override;
}; // class TrezorCryptoFILImpl end

} // namespace token
} // namespace jub
