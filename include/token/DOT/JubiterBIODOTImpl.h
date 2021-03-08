#pragma once

#include <memory>

#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/DOT/JubiterBladeDOTImpl.h"

namespace jub {
namespace token {


class JubiterBIODOTImpl :
        public JubiterBIOToken,
virtual public JubiterBladeDOTImpl {
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBIODOTImpl>(deviceID);
    }

public:
    JubiterBIODOTImpl(JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeDOTImpl(deviceID) {}
    virtual ~JubiterBIODOTImpl() = default;
}; // class JubiterBIODOTImpl end


} // namespace token end
} // namespace jub end
