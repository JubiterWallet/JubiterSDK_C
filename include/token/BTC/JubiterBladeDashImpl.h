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
    //for Factory
    static std::shared_ptr<BaseToken> Create(const JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBladeDashImpl>(deviceID);
    }

public:
    JubiterBladeDashImpl(const JUB_UINT16 deviceID) :
        JubiterBladeBTCImpl(deviceID) {}
    ~JubiterBladeDashImpl() {}
}; // class JubiterBladeDashImpl end


} // namespace token end
} // namespace jub end
