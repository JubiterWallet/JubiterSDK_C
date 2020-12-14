#pragma once
#include <memory>

#include "token/JubiterBlade/JubiterBladeToken.h"
#include "token/FIL/JubiterBaseFILImpl.h"


namespace jub {
namespace token {


constexpr JUB_BYTE kPKIAID_FIL[16] = {
    0xD1, 0x56, 0x00, 0x01, 0x32, 0x03, 0x00, 0x42, 0x4C, 0x44, 0x00, 0x00, 0x45, 0x54, 0x49, 0x01
};


class JubiterBladeFILImpl :
        public JubiterBladeToken,
virtual public JubiterBaseFILImpl {

public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBladeFILImpl>(deviceID);
    }

    JubiterBladeFILImpl(JUB_UINT16 deviceID) :
        JubiterBladeToken(deviceID) {}
    ~JubiterBladeFILImpl() {}


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
                          std::vector<JUB_BYTE>& vRaw) override;
}; // class JubiterBladeFILImpl end


} // namespace token end
} // namespace jub end
