#pragma once
#include <memory>

#include "token/BTC/JubiterNFCBTCImpl.h"


namespace jub {
namespace token {


class JubiterNFCUSDTImpl :
    public JubiterNFCBTCImpl {
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterNFCUSDTImpl>(deviceID);
    }

public:
    JubiterNFCUSDTImpl(JUB_UINT16 deviceID) :
        JubiterNFCBTCImpl(deviceID) {}
    ~JubiterNFCUSDTImpl() {}
}; // class JubiterNFCUSDTImpl end


} // namespace token end
} // namespace jub end
