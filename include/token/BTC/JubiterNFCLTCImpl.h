#pragma once
#include <memory>

#include "token/BTC/JubiterNFCBTCImpl.h"


namespace jub {
namespace token {


class JubiterNFCLTCImpl :
    public JubiterNFCBTCImpl {
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterNFCLTCImpl>(deviceID);
    }

public:
    JubiterNFCLTCImpl(JUB_UINT16 deviceID) :
        JubiterNFCBTCImpl(deviceID) {
            _coin = TWCoinType::TWCoinTypeLitecoin;
    }
    ~JubiterNFCLTCImpl() {}
}; // class JubiterNFCLTCImpl end


} // namespace token end
} // namespace jub end
