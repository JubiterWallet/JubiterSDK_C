#pragma once
#include <memory>

#include <token/BTC/JubiterBladeBTCImpl.h>
#include <token/HC/JubiterBaseHCImpl.h>


namespace jub {
namespace token {


class JubiterBladeHCImpl :
        public JubiterBladeBTCImpl,
virtual public JubiterBaseHCImpl {

public:
    //for Factory
    static std::shared_ptr<BaseToken> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterBladeHCImpl>(deviceID);
    }

    JubiterBladeHCImpl(JUB_UINT16 deviceID) :
        JubiterBladeBTCImpl(deviceID) {
            _hashType = TWHcashSigHashAll;
    }
    ~JubiterBladeHCImpl() {}

    //HC functions
    virtual JUB_RV SelectApplet() override;
}; // class JubiterBladeHCImpl end


} // namespace token end
} // namespace jub end
