#pragma once
#include <memory>

#include "token/BTC/JubiterLiteBTCImpl.h"
#include "token/BTC/JubiterBaseDashImpl.h"


namespace jub {
namespace token {


class JubiterLiteDashImpl :
        public JubiterLiteBTCImpl,
virtual public JubiterBaseDashImpl {
public:
    JubiterLiteDashImpl(JUB_UINT16 deviceID) :
        JubiterLiteBTCImpl(deviceID) {}
    ~JubiterLiteDashImpl() {}
}; // class JubiterLiteDashImpl end


} // namespace token end
} // namespace jub end
