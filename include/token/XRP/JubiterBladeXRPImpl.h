#pragma once
#include <memory>

#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/XRP/JubiterBaseXRPImpl.h"


namespace jub {
namespace token {


class JubiterBladeXRPImpl :
        public JubiterBladeToken,
virtual public JubiterBaseXRPImpl {

public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBladeXRPImpl>(deviceID);
    }

    JubiterBladeXRPImpl(JUB_UINT16 deviceID) :
        JubiterBladeToken(deviceID) {}
    ~JubiterBladeXRPImpl() {}

    //XRP functions
    virtual JUB_RV SelectApplet() override;
    virtual JUB_RV SetCoin() override;
    virtual JUB_RV GetAddress(const std::string& path, const JUB_UINT16 tag, std::string& address) override;
    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) override;
    virtual JUB_RV SignTX(const std::vector<JUB_BYTE>& vPath,
                          std::vector<JUB_BYTE>& vUnsignedRaw,
                          std::vector<uchar_vector>& vSignatureRaw) override;
}; // class JubiterBladeXRPImpl end


} // namespace token end
} // namespace jub end
