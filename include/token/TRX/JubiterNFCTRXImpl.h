#pragma once
#include <token/JubiterNFC/JubiterNFCToken.h>
#include <token/JubiterNFC/JubiterNFCImpl.h>
#include <token/TRX/JubiterBaseTRXImpl.h>

namespace jub {
namespace token {


class JubiterNFCTRXImpl :
        public JubiterNFCImpl,
virtual public JubiterBaseTRXImpl {

public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterNFCTRXImpl>(deviceID);
    }

    JubiterNFCTRXImpl(JUB_UINT16 deviceID) :
        JubiterNFCImpl(deviceID) {}
    ~JubiterNFCTRXImpl() {}

    //TRX functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV SetCoin() override;
    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) override;
    virtual JUB_RV SignTX(const std::vector<JUB_BYTE>& vPath,
                          const std::vector<JUB_BYTE>& vRaw,
                          std::vector<uchar_vector>& vSignatureRaw) override;

protected:
    virtual JUB_RV _encodeRSV(const std::vector<JUB_BYTE>& vRSV, std::vector<JUB_BYTE>& signature);
}; // class JubiterNFCTRXImpl end


} // namespace token end
} // namespace jub end
