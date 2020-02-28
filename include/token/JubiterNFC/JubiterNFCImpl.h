#pragma once
#include <token/JubiterNFC/JubiterNFCToken.h>

#include <memory>

namespace jub {
namespace token {


typedef enum {
    SINGLE = 0x00,
    DOUBLE = 0x01,
    Default  = SINGLE
} JUB_ENUM_NFC_HASHMODE;


class JubiterNFCImpl :
    public JubiterNFCToken {

public:
    JubiterNFCImpl(JUB_UINT16 deviceID) :
    JubiterNFCToken(deviceID) {
	};

    virtual ~JubiterNFCImpl() = default;

    virtual JUB_RV SelectApplet();
    //BTC functions
    virtual JUB_RV GetHDNode(const JUB_BYTE& type, const std::string& path, std::string& xpub);
    virtual JUB_RV GetAddress(const JUB_BYTE addrFmt, const JUB_ENUM_BTC_TRANS_TYPE& type, const std::string& path, const JUB_UINT16 tag, std::string& address);
    virtual JUB_RV SignTX(const std::string& path,
                          const JUB_BYTE& hashMode,
                          const JUB_BYTE& hashType,
                          const std::vector<JUB_BYTE>& vPreImageHash,
                          std::vector<JUB_BYTE>& vSignatureRaw);
}; // class JubiterNFCImpl end


} // namespace token end
} // namespace jub end
