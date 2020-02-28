#pragma once
#include "token/BTC/JubiterNFCBTCImpl.h"

namespace jub {
namespace token {


class JubiterNFCLTCImpl :
    public JubiterNFCBTCImpl {

public:
    JubiterNFCLTCImpl(JUB_UINT16 deviceID) :
        JubiterNFCBTCImpl(deviceID) {
        _coin = TWCoinType::TWCoinTypeLitecoin;
    };
    ~JubiterNFCLTCImpl() {};

    static std::shared_ptr<BTCTokenInterface> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterNFCLTCImpl>(deviceID);
    }
}; // class JubiterNFCLTCImpl end


} // namespace token end
} // namespace jub end
