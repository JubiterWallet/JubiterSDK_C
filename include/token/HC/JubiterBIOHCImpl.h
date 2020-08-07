#pragma once
#include <token/JubiterBIO/JubiterBIOToken.h>
#include <token/HC/JubiterBladeHCImpl.h>

#include <memory>

namespace jub {
namespace token {


class JubiterBIOHCImpl :
        public JubiterBIOToken,
virtual public JubiterBladeHCImpl {
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBIOHCImpl>(deviceID);
    }

public:
    JubiterBIOHCImpl(JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeHCImpl(deviceID) {}
    virtual ~JubiterBIOHCImpl() = default;
}; // class JubiterBIOHCImpl end


} // namespace token end
} // namespace jub end
