#pragma once
#include "token/JubiterBIO/JubiterBIOToken.h"
#include "token/CKB/JubiterBladeCKBImpl.h"

#include <memory>

namespace jub {
namespace token {


class JubiterBIOCKBImpl :
        public JubiterBIOToken,
virtual public JubiterBladeCKBImpl {
public:
    JubiterBIOCKBImpl(const JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeCKBImpl(deviceID) {}
    virtual ~JubiterBIOCKBImpl() = default;
}; // class JubiterBIOCKBImpl end


} // namespace token end
} // namespace jub end
