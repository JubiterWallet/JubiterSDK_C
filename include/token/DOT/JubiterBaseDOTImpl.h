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
        _curve_name = (char*)ED25519_NAME;
        _publicKeyType = TWPublicKeyType::TWPublicKeyTypeED25519;
    };
protected:
    virtual JUB_RV _getPubkeyFromXpub(const std::string& xpub, TW::Data& publicKey,
                                      uint32_t hdVersionPub=TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypePolkadot),
                                      uint32_t hdVersionPrv=TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypePolkadot));
    virtual JUB_RV _getAddress(const TW::Data& publicKey, std::string& address);
}; // class JubiterBaseDOTImpl end


} // namespace token end
} // namespace jub end
