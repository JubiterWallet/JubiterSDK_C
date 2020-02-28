#pragma once
#include <token/JubiterNFC/JubiterNFCToken.h>
#include <token/JubiterNFC/JubiterNFCImpl.h>
#include <token/BTC/JubiterNFCBTCImpl.h>
#include <token/BTC/JubiterBaseBCHImpl.h>

namespace jub {
namespace token {


class JubiterNFCBCHImpl :
        public JubiterNFCBTCImpl,
virtual public JubiterBaseBCHImpl {

public:
    JubiterNFCBCHImpl(JUB_UINT16 deviceID) :
        JubiterNFCBTCImpl(deviceID) {
        _hashType = TWSignatureHashTypeAllFork;
	};
    ~JubiterNFCBCHImpl() {};

    static std::shared_ptr<BTCTokenInterface> Create(JUB_UINT16 deviceID) {
        return std::make_shared<JubiterNFCBCHImpl>(deviceID);
    }
}; // class JubiterNFCBCHImpl end


} // namespace token end
} // namespace jub end
