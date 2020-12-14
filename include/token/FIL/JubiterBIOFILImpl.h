#pragma once

#include <memory>

#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/FIL/JubiterBladeFILImpl.h"

namespace jub {
namespace token {


class JubiterBIOFILImpl :
        public JubiterBIOToken,
virtual public JubiterBladeFILImpl {
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBIOFILImpl>(deviceID);
    }

public:
    JubiterBIOFILImpl(JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeFILImpl(deviceID) {}
    virtual ~JubiterBIOFILImpl() = default;
}; // class JubiterBIOFILImpl end


} // namespace token end
} // namespace jub end
