#pragma once
#include <memory>

#include "token/BTC/JubiterLiteBTCImpl.h"


namespace jub {
namespace token {


class JubiterLiteUSDTImpl :
    public JubiterLiteBTCImpl {
public:
    JubiterLiteUSDTImpl(const JUB_UINT16 deviceID) :
        JubiterLiteBTCImpl(deviceID) {}
    ~JubiterLiteUSDTImpl() {}
}; // class JubiterLiteUSDTImpl end


} // namespace token end
} // namespace jub end
