#pragma once
#include "token/interface/DOTTokenInterface.hpp"
#include <TrustWalletCore/TWCoinType.h>

namespace jub {
namespace token {


class JubiterBaseDOTImpl :
virtual public DOTTokenInterface {
public:
    JubiterBaseDOTImpl() {
        _coin = TWCoinType::TWCoinTypePolkadot;
//        _curve_name = (char*)ED25519_NAME;
//        _publicKeyType = TWPublicKeyType::TWPublicKeyTypeED25519;
    };
protected:
    virtual JUB_RV _getChainCodeFromPolkadptPath(std::string pathStr,std::string& chainCode);
    virtual JUB_RV _getSr25519KeypairFromMasterKp(const std::string kp, std::string& derivPrv, std::string& derivPub ,const std::string path, JUB_ENUM_CURVES curve);
    virtual JUB_RV _getEd25519PrvKeyFromMasterKey(const std::string prvKey, std::string& derivPrv, std::string& derivPub ,const std::string path, JUB_ENUM_CURVES curve);
    virtual JUB_RV _getAddress(const TW::Data& publicKey, std::string& address);
}; // class JubiterBaseDOTImpl end

} // namespace token end
} // namespace jub end
