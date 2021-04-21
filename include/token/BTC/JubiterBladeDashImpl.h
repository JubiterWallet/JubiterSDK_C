#pragma once
#include <memory>

#include "token/BTC/JubiterBladeBTCImpl.h"
#include "token/BTC/JubiterBaseDashImpl.h"


namespace jub {
namespace token {


class JubiterBladeDashImpl :
        public JubiterBladeBTCImpl,
virtual public JubiterBaseDashImpl {
public:
    JubiterBladeDashImpl(JUB_UINT16 deviceID) :
        JubiterBladeBTCImpl(deviceID) {}
    ~JubiterBladeDashImpl() {}
}; // class JubiterBladeDashImpl end


} // namespace token end
} // namespace jub end
