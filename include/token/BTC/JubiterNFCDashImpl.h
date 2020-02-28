#pragma once
#include <token/BTC/JubiterNFCBTCImpl.h>
#include <token/BTC/JubiterBaseDashImpl.h>

namespace jub {
namespace token {


class JubiterNFCDashImpl :
        public JubiterNFCBTCImpl,
virtual public JubiterBaseDashImpl {

public:
    JubiterNFCDashImpl(JUB_UINT16 deviceID) :
        JubiterNFCBTCImpl(deviceID) {};
    ~JubiterNFCDashImpl() {};

    static std::shared_ptr<BTCTokenInterface> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterNFCDashImpl>(deviceID);
    }
}; // class JubiterNFCDashImpl end


} // namespace token end
} // namespace jub end
