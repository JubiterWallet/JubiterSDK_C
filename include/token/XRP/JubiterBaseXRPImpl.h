#pragma once
#include <token/interface/XRPTokenInterface.hpp>
#include <TrustWalletCore/TWCoinType.h>


namespace jub {
namespace token {


class JubiterBaseXRPImpl :
virtual public XRPTokenInterface {
public:
    JubiterBaseXRPImpl() {
        _coin = TWCoinType::TWCoinTypeXRP;
        _curve_name = (char*)SECP256K1_XRP_NAME;
    };

    JUB_RV SerializePreimage(const JUB_TX_XRP& tx,
                             uchar_vector& preimageRaw);

protected:
    virtual JUB_RV _getPubkeyFromXpub(const std::string& xpub, TW::Data& publicKey,
                                      uint32_t hdVersionPub=TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin),
                                      uint32_t hdVersionPrv=TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin));
    virtual JUB_RV _getAddress(const TW::Data& publicKey, std::string& address);
}; // class JubiterBaseXRPImpl end


} // namespace token end
} // namespace jub end
