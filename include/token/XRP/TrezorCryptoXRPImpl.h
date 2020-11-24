#pragma once
#include <memory>

#include "token/TrezorCrypto/TrezorCryptoToken.h"
#include "token/XRP/JubiterBaseXRPImpl.h"


namespace jub {
namespace token {


class TrezorCryptoXRPImpl :
        public TrezorCryptoToken,
virtual public JubiterBaseXRPImpl {

public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(const std::string& XPRVorXPUB) {
        return std::make_shared<TrezorCryptoXRPImpl>(XPRVorXPUB);
    }

    TrezorCryptoXRPImpl(const std::string& XPRVorXPUB) :
        TrezorCryptoToken(XPRVorXPUB) {}
    ~TrezorCryptoXRPImpl() {}

    //XRP functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV SetCoin() override;
    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) override;
    virtual JUB_RV SignTX(const std::vector<JUB_BYTE>& vPath,
                          std::vector<JUB_BYTE>& vUnsignedRaw,
                          std::vector<uchar_vector>& vSignatureRaw) override;
}; // class TrezorCryptoXRPImpl end


} // namespace token end
} // namespace jub end
