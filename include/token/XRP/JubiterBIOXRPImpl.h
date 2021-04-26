#pragma once
#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/XRP/JubiterBladeXRPImpl.h"

#include <memory>

namespace jub {
namespace token {


class JubiterBIOXRPImpl :
        public JubiterBIOToken,
virtual public JubiterBladeXRPImpl {
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBIOXRPImpl>(deviceID);
    }

public:
    JubiterBIOXRPImpl(JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeXRPImpl(deviceID) {}
    virtual ~JubiterBIOXRPImpl() = default;

    virtual JUB_RV GetHDNode(const JUB_BYTE format, const std::string& path, std::string& pubkey) override;
}; // class JubiterBIOXRPImpl end


} // namespace token end
} // namespace jub end
