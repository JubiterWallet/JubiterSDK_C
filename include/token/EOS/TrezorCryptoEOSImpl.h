#pragma once

#include <token/interface/EOSTokenInterface.hpp>
#include <token/TrezorCrypto/TrezorCryptoToken.h>

namespace jub {
namespace token {


class TrezorCryptoEOSImpl :
    public TrezorCryptoToken,
    public EOSTokenInterface {

public:
    TrezorCryptoEOSImpl(const std::string& xprv) :
        TrezorCryptoToken(xprv) {};

    ~TrezorCryptoEOSImpl() {};

    //EOS functions
    virtual JUB_RV SelectApplet();
    virtual JUB_RV SetCoinType();
    virtual JUB_RV GetAddress(const TW::EOS::Type& type, const std::string& path, const JUB_UINT16 tag, std::string& address);
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey);
    virtual JUB_RV SignTX(const TW::EOS::Type& type,
                          const std::vector<JUB_BYTE>& vPath,
                          const std::vector<JUB_BYTE>& vChainId,
                          const std::vector<JUB_BYTE>& vRaw,
                          std::vector<uchar_vector>& vSignatureRaw,
                          const bool bWithType = false);
}; // class TrezorCryptoEOSImpl end


} // namespace token end
} // namespace jub end
