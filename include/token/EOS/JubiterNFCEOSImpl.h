#pragma once
#include <token/JubiterNFC/JubiterNFCToken.h>
#include <token/JubiterNFC/JubiterNFCImpl.h>
#include <token/EOS/JubiterBaseEOSImpl.h>

namespace jub {
namespace token {


class JubiterNFCEOSImpl :
        public JubiterNFCImpl,
virtual public JubiterBaseEOSImpl {

public:
    JubiterNFCEOSImpl(JUB_UINT16 deviceID) :
    JubiterNFCImpl(deviceID) {};

    ~JubiterNFCEOSImpl() {};

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

protected:
    virtual JUB_RV _encodeRSV(const std::vector<JUB_BYTE>& vRSV, std::vector<JUB_BYTE>& signature);
}; // class JubiterNFCEOSImpl end


} // namespace token end
} // namespace jub end
