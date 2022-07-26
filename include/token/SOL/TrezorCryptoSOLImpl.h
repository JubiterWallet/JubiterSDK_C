#pragma once
#include <memory>

#include "token/TrezorCrypto/TrezorCryptoToken.h"
#include "token/SOL/JubiterBaseSOLImpl.h"


namespace jub {
namespace token {


class TrezorCryptoSOLImpl :
        public TrezorCryptoToken,
virtual public JubiterBaseSOLImpl {
public:
    TrezorCryptoSOLImpl(JUB_UINT16 deviceID, const JUB_ENUM_CURVES curve=JUB_ENUM_CURVES::ED25519) :
        TrezorCryptoToken(deviceID, curve) {}
    ~TrezorCryptoSOLImpl() {}

    //SOL functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV GetAppletVersion(stVersion& version) override;

    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) override;

protected:
}; // class TrezorCryptoSOLImpl end


} // namespace token end
} // namespace jub end
