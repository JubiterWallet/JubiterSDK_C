#pragma once

#include <token/JubiterBlade/JubiterBladeToken.h>
#include <token/XRP/JubiterBaseXRPImpl.h>

namespace jub {
namespace token {


class JubiterBladeXRPImpl :
        public JubiterBladeToken,
virtual public JubiterBaseXRPImpl {

public:
    JubiterBladeXRPImpl(JUB_UINT16 deviceID) :
        JubiterBladeToken(deviceID) {};

    ~JubiterBladeXRPImpl() {};

    //XRP functions
    virtual JUB_RV SelectApplet();
    virtual JUB_RV SetCoinType();
    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address);
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey);
    virtual JUB_RV SignTX(const std::vector<JUB_BYTE>& vPath,
                          std::vector<JUB_BYTE>& vUnsignedRaw,
                          std::vector<uchar_vector>& vSignatureRaw);
}; // class JubiterBladeXRPImpl end


} // namespace token end
} // namespace jub end
