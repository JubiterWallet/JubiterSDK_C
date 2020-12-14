#pragma once

#include "token/JubiterLite/JubiterLiteToken.h"
#include "token/JubiterLite/JubiterLiteImpl.h"
#include "token/FIL/JubiterBaseFILImpl.h"

namespace jub {
namespace token {


class JubiterLiteFILImpl :
        public JubiterLiteImpl,
virtual public JubiterBaseFILImpl {

public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterLiteFILImpl>(deviceID);
    }

    JubiterLiteFILImpl(JUB_UINT16 deviceID) :
        JubiterLiteImpl(deviceID) {}
    ~JubiterLiteFILImpl() {}

    //FIL functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV SetCoin() override;
    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) override;
    virtual JUB_RV SignTX(const uint64_t& nonce,
                          const uint64_t& gprice,
                          const uint64_t& glimit,
                          const std::string& to,
                          const uint64_t& value,
                          const std::string& input,
                          const std::string& path,
                          std::vector<JUB_BYTE>& vSignatureRaw) override;

protected:
    virtual JUB_RV _encodeRSV(const std::vector<JUB_BYTE>& vRSV, std::vector<JUB_BYTE>& signature);
}; // class JubiterLiteFILImpl end


} // namespace token end
} // namespace jub end
