#pragma once
#include <memory>

#include <token/JubiterBIO/JubiterBIOToken.h>
#include <token/ETH/JubiterBladeETHImpl.h>


namespace jub {
namespace token {


class JubiterBIOETHImpl :
        public JubiterBIOToken,
virtual public JubiterBladeETHImpl {
public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBIOETHImpl>(deviceID);
    }

public:
    JubiterBIOETHImpl(JUB_UINT16 deviceID) :
        JubiterBIOToken(deviceID),
        JubiterBladeETHImpl(deviceID) {}
    virtual ~JubiterBIOETHImpl() = default;
}; // class JubiterBIOETHImpl end


} // namespace token end
} // namespace jub end
