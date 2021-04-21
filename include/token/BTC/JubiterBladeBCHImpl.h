#pragma once
#include <memory>

#include "token/BTC/JubiterBladeBTCImpl.h"
#include "token/BTC/JubiterBaseBCHImpl.h"


namespace jub {
namespace token {


class JubiterBladeBCHImpl :
        public JubiterBladeBTCImpl,
virtual public JubiterBaseBCHImpl {
public:
    JubiterBladeBCHImpl(JUB_UINT16 deviceID) :
        JubiterBladeBTCImpl(deviceID) {
            _hashType = TWBitcoinSigHashTypeForkBCH;
	}
    ~JubiterBladeBCHImpl() {}
}; // class JubiterBladeBCHImpl end


} // namespace token end
} // namespace jub end
