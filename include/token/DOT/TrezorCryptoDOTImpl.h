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
    //for Factory
    static std::shared_ptr<BaseToken> Create(const std::string& XPRVorXPUB)
    {
        return std::make_shared<TrezorCryptoDOTImpl>(XPRVorXPUB);
    }

    TrezorCryptoDOTImpl(const std::string& XPRVorXPUB) :
        TrezorCryptoToken(XPRVorXPUB) {}
    ~TrezorCryptoDOTImpl() {}

    //DOT functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV SetCoin() override;
    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) override;
    virtual JUB_RV SignTX(const uint64_t& nonce,
                          const uint64_t& gprice,
                          const uint64_t& glimit,
                          const std::string& to,
                          const uint64_t& value,
                          const std::string& input,
                          const std::string& path,
                          std::vector<JUB_BYTE>& vSignatureRaw) override;
}; // class TrezorCryptoDOTImpl end


} // namespace token end
} // namespace jub end
