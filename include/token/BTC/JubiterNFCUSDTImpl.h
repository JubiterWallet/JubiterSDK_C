#pragma once
#include <token/BTC/JubiterNFCBTCImpl.h>

namespace jub {
namespace token {


class JubiterNFCUSDTImpl :
    public JubiterNFCBTCImpl {

public:
    JubiterNFCUSDTImpl(JUB_UINT16 deviceID) :
        JubiterNFCBTCImpl(deviceID) {};
    ~JubiterNFCUSDTImpl() {};

    static std::shared_ptr<BTCTokenInterface> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterNFCUSDTImpl>(deviceID);
    }
}; // class JubiterNFCUSDTImpl end


} // namespace token end
} // namespace jub end
