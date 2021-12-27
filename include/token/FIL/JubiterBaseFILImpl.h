#pragma once
#include "token/interface/FILTokenInterface.hpp"
#include <TrustWalletCore/TWCoinType.h>


namespace jub {
namespace token {


class JubiterBaseFILImpl :
virtual public FILTokenInterface {
public:
    JubiterBaseFILImpl() {
        _coin = TWCoinType::TWCoinTypeFilecoin;
        _curve_name = (char*)SECP256K1_FIL_NAME;
    };

    static JUB_RV IsValidAddress(const std::string& address);
    virtual JUB_RV CheckAddress(const std::string& address) override;

protected:
    virtual JUB_RV _getPubkeyFromXpub(const std::string& xpub, TW::Data& publicKey,
                                      uint32_t hdVersionPub=TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin),
                                      uint32_t hdVersionPrv=TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin)) override;
    virtual JUB_RV _getAddress(const TW::Data& publicKey, std::string& address);
}; // class JubiterBaseFILImpl end


} // namespace token end
} // namespace jub end
