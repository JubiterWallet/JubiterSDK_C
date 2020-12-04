#pragma once
#include <memory>

#include "token/BTC/JubiterLiteBTCImpl.h"


namespace jub {
namespace token {


class JubiterLiteUSDTImpl :
    public JubiterLiteBTCImpl {
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterLiteUSDTImpl>(deviceID);
    }

public:
    JubiterLiteUSDTImpl(JUB_UINT16 deviceID) :
        JubiterLiteBTCImpl(deviceID) {}
    ~JubiterLiteUSDTImpl() {}
}; // class JubiterLiteUSDTImpl end


} // namespace token end
} // namespace jub end
