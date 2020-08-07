#pragma once
#include <token/JubiterBIO/JubiterBIOToken.h>
#include <token/EOS/JubiterBladeEOSImpl.h>

#include <memory>

namespace jub {
namespace token {


class JubiterBIOEOSImpl :
        public JubiterBIOToken,
virtual public JubiterBladeEOSImpl {
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBIOEOSImpl>(deviceID);
    }

public:
    JubiterBIOEOSImpl(JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeEOSImpl(deviceID) {}
    virtual ~JubiterBIOEOSImpl() = default;
}; // class JubiterBIOEOSImpl end


} // namespace token end
} // namespace jub end
