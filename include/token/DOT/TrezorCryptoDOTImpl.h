#pragma once
#include <memory>

#include "token/DOT/JubiterBaseDOTImpl.h"
#include "token/TrezorCrypto/TrezorCryptoToken.h"
#include <string>
#include <vector>

namespace jub {
namespace token {

class TrezorCryptoDOTImpl : public TrezorCryptoToken, public JubiterBaseDOTImpl {

  public:
    TrezorCryptoDOTImpl(JUB_UINT16 deviceID, JUB_ENUM_CURVES curve = JUB_ENUM_CURVES::ED25519)
        : TrezorCryptoToken(deviceID, curve) {}
    ~TrezorCryptoDOTImpl() {}

    // DOT functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV SetCoin() override;
    virtual JUB_RV SetCoinCurvePublicKeyType(JUB_ENUM_COINTYPE_DOT coinType, JUB_ENUM_CURVES curve) override;
    virtual JUB_RV GetAddress(const std::string &path, const JUB_UINT16 tag, std::string &address,
                              const TWCoinType &coinNet = TWCoinType::TWCoinTypeBitcoin) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string &path, std::string &pubkey,
                             const TWCoinType &coinNet = TWCoinType::TWCoinTypeBitcoin) override;
    virtual JUB_RV SignTX(const std::string &path, const std::string &genesisHash, const std::string &blockHash,
                          const uint64_t &nonce, const uint32_t &specVersion, const uint64_t &network,
                          const uint32_t &transaction_version, const uint64_t &blockNumber, const uint64_t &eraPeriod,
                          const std::string &tip, const TW::Data &call, std::vector<JUB_BYTE> &vSignatureRaw) override;
}; // class TrezorCryptoDOTImpl end

} // namespace token
} // namespace jub
