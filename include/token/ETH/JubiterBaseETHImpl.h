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
        _curve_name = (char*)SECP256K1_ETH_NAME;
    };

    virtual JUB_RV VerifyTx(const std::vector<JUB_BYTE>& vChainID,
                            const uchar_vector& signedRaw,
                            const TW::Data& publicKey);

protected:
    virtual JUB_RV _getPubkeyFromXpub(const std::string& xpub, TW::Data& publicKey,
                                      uint32_t hdVersionPub=TWCoinType2HDVersionPublic(TWCoinType::TWCoinTypeBitcoin),
                                      uint32_t hdVersionPrv=TWCoinType2HDVersionPrivate(TWCoinType::TWCoinTypeBitcoin));
    virtual JUB_RV _getAddress(const TW::Data publicKey, std::string& address);
}; // class JubiterBaseETHImpl end


} // namespace token end
} // namespace jub end
