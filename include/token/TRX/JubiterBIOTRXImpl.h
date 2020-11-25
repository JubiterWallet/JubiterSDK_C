#pragma once
#include <token/JubiterBIO/JubiterBIOToken.h>
#include <token/TRX/JubiterBladeTRXImpl.h>

#include <memory>

namespace jub {
namespace token {


class JubiterBIOTRXImpl :
        public JubiterBIOToken,
virtual public JubiterBladeTRXImpl {
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBIOTRXImpl>(deviceID);
    }

public:
    JubiterBIOTRXImpl(JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeTRXImpl(deviceID) {}
    virtual ~JubiterBIOTRXImpl() = default;
}; // class JubiterBIOTRXImpl end


} // namespace token end
} // namespace jub end
