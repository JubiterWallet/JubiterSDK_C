#pragma once
#include <token/TrezorCrypto/TrezorCryptoToken.h>
#include <token/XRP/JubiterBaseXRPImpl.h>

namespace jub {
namespace token {


class TrezorCryptoXRPImpl :
        public TrezorCryptoToken,
virtual public JubiterBaseXRPImpl {

public:
    TrezorCryptoXRPImpl(const std::string& XPRVorXPUB) :
        TrezorCryptoToken(XPRVorXPUB) {};

    ~TrezorCryptoXRPImpl() {};

    //XRP functions
    virtual JUB_RV SelectApplet();
    virtual JUB_RV SetCoinType();
    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address);
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey);
    virtual JUB_RV SignTX(const std::vector<JUB_BYTE>& vPath,
                          std::vector<JUB_BYTE>& vUnsignedRaw,
                          std::vector<uchar_vector>& vSignatureRaw);
}; // class TrezorCryptoXRPImpl end


} // namespace token end
} // namespace jub end
