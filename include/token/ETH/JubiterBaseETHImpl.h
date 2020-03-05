#pragma once
#include <token/interface/ETHTokenInterface.hpp>
#include <TrustWalletCore/TWCoinType.h>

namespace jub {
namespace token {


class JubiterBaseETHImpl :
virtual public ETHTokenInterface {
public:
    JubiterBaseETHImpl() {
        _coin = TWCoinType::TWCoinTypeEthereum;
    };

protected:
    virtual JUB_RV _getPubkeyFromXpub(const std::string& xpub, TW::Data& publicKey,
                                      uint32_t hdVersionPub=TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin),
                                      uint32_t hdVersionPrv=TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin));
    virtual JUB_RV _getAddress(const TW::Data publicKey, std::string& address);
}; // class JubiterBaseETHImpl end


} // namespace token end
} // namespace jub end
