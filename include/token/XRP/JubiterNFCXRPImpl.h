#pragma once

#include <token/JubiterNFC/JubiterNFCToken.h>
#include <token/JubiterNFC/JubiterNFCImpl.h>
#include <token/XRP/JubiterBaseXRPImpl.h>

namespace jub {
namespace token {


class JubiterNFCXRPImpl :
        public JubiterNFCImpl,
virtual public JubiterBaseXRPImpl {

public:
    JubiterNFCXRPImpl(JUB_UINT16 deviceID) :
    JubiterNFCImpl(deviceID) {};

    ~JubiterNFCXRPImpl() {};

    //XRP functions
    virtual JUB_RV SelectApplet();
    virtual JUB_RV SetCoinType();
    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address);
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey);
    virtual JUB_RV SignTX(const std::vector<JUB_BYTE>& vPath,
                          std::vector<JUB_BYTE>& vUnsignedRaw,
                          std::vector<uchar_vector>& vSignatureRaw);

protected:
    virtual JUB_RV _encodeRSV(const std::vector<JUB_BYTE>& vRSV, std::vector<JUB_BYTE>& signature);
}; // class JubiterNFCXRPImpl end


} // namespace token end
} // namespace jub end
