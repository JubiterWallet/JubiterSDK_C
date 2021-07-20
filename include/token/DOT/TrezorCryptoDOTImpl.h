#pragma once
#include <memory>

#include "token/TrezorCrypto/TrezorCryptoToken.h"
#include "token/DOT/JubiterBaseDOTImpl.h"


namespace jub {
namespace token {


class TrezorCryptoDOTImpl :
        public TrezorCryptoToken,
virtual public JubiterBaseDOTImpl {

public:
    TrezorCryptoDOTImpl(JUB_UINT16 deviceID, JUB_ENUM_CURVES curve=JUB_ENUM_CURVES::ED25519) :
        TrezorCryptoToken(deviceID, curve) {}
    ~TrezorCryptoDOTImpl() {}

    //DOT functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV SetCoin() override;
    virtual JUB_RV SetCoinCurvePublicKeyType(JUB_ENUM_COINTYPE_DOT coinType, JUB_ENUM_CURVES curve) override;
    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) override;
    virtual JUB_RV SignTX(const std::string &path,
                          const std::string &genesisHash,
                          const std::string &blockHash,
                          const std::string &to,
                          const uint64_t& nonce,
                          const uint32_t& specVersion,
                          const uint64_t& network,
                          const uint32_t& transaction_version,
                          const uint64_t& blockNumber,
                          const std::string& value,
                          const uint64_t& eraPeriod,
                          const uint64_t& tip,
                          std::vector<JUB_BYTE>& vSignatureRaw) override;
}; // class TrezorCryptoDOTImpl end

} // namespace token end
} // namespace jub end
