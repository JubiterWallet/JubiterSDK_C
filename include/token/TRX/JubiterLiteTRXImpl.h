#pragma once
#include "token/JubiterLite/JubiterLiteToken.h"
#include "token/JubiterLite/JubiterLiteImpl.h"
#include "token/TRX/JubiterBaseTRXImpl.h"

namespace jub {
namespace token {


class JubiterLiteTRXImpl :
        public JubiterLiteImpl,
virtual public JubiterBaseTRXImpl {

public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterLiteTRXImpl>(deviceID);
    }

    JubiterLiteTRXImpl(JUB_UINT16 deviceID) :
        JubiterLiteImpl(deviceID) {}
    ~JubiterLiteTRXImpl() {}

    //TRX functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV GetAppletVersion(stVersion& version) override;
    virtual JUB_RV SetCoin() override;
    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) override;
    virtual JUB_RV SignTX(const std::vector<JUB_BYTE>& vPath,
                          const std::vector<JUB_BYTE>& vRaw,
                          std::vector<uchar_vector>& vSignatureRaw) override;
    virtual JUB_RV SetTRC20Token(const std::string& tokenName,
                                 const JUB_UINT16 unitDP,
                                 const std::string& contractAddress) override;
    virtual JUB_RV SetTRC721Token(const std::string& tokenName,
                                  const std::string& contractAddress) override;

protected:
    virtual JUB_RV _encodeRSV(const std::vector<JUB_BYTE>& vRSV, std::vector<JUB_BYTE>& signature);
}; // class JubiterLiteTRXImpl end


} // namespace token end
} // namespace jub end
