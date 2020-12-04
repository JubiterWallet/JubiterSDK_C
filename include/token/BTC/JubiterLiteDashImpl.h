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
    //for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterLiteDashImpl>(deviceID);
    }

public:
    JubiterLiteDashImpl(JUB_UINT16 deviceID) :
        JubiterLiteBTCImpl(deviceID) {}
    ~JubiterLiteDashImpl() {}
}; // class JubiterLiteDashImpl end


} // namespace token end
} // namespace jub end
